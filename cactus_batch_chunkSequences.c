#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <float.h>
#include "bioioC.h"
#include "commonC.h"

/*
 * Assistance routine for pecan2_batch.py.
 * Routine reads in fasta file progressively, writes chunks of sequence.
 */

int32_t chunkRemaining;
FILE *fileHandleForChunks;
FILE *fileHandleForBridges;
int32_t chunkSize;
int32_t overlapSize;
int32_t useCompression;

int32_t fn(char *fastaHeader, int32_t start, char *sequence, int32_t seqLength, int32_t length, FILE *fileHandle) {
	FILE *fileHandle2;
	char *tempSeqFile;
	char c;
	int32_t i;
	//Make a chunk file.
	tempSeqFile = getTempFile();
	fileHandle2 = fopen(tempSeqFile, "w");
	i = 0;
	fastaHeader = stringCopy(fastaHeader);
	while(fastaHeader[i] != '\0') {
		if(fastaHeader[i] == ' ' || fastaHeader[i] == '\t') {
			fastaHeader[i] = '\0';
			break;
		}
		i++;
	}
	fprintf(fileHandle2, ">%s|1|%i\n", fastaHeader, start);
	free(fastaHeader);
	assert(length <= chunkSize);
	assert(start >= 0);
	if(start+length > seqLength) {
		length = seqLength - start;
	}
	assert(length > 0);
	c = sequence[start+length];
	sequence[start+length] = '\0';
	fprintf(fileHandle2, "%s\n", &sequence[start]);
	sequence[start+length] = c;
	fclose(fileHandle2);

	//Do compression if asked for
	if(useCompression) {
		i = systemLocal("bzip2 %s", tempSeqFile);
		if(i != 0) {
			logInfo("Failed to compress file\n");
			return i;
		}
		i = systemLocal("mv %s.bz2 %s", tempSeqFile, tempSeqFile);
		if(i != 0) {
			logInfo("Failed to move file\n");
			return i;
		}
	}

	fprintf(fileHandle, "%s %i\n", tempSeqFile, length);
	return length;
}

int32_t fn2(int32_t i, int32_t seqLength) {
	//Update remaining portion of the chunk.
	assert(seqLength >= 0);
	i -= seqLength;
	assert(i >= 0);
	if(i == 0) {
		return chunkSize;
	}
	return i;
}

void fn3(const char *fastaHeader, const char *sequence, int32_t length) {
	int32_t j, k, l;

	if(length > 0) {
		j = fn((char *)fastaHeader, 0, (char *)sequence, length, chunkRemaining, fileHandleForChunks);
	    chunkRemaining = fn2(chunkRemaining, j);
	    while(length - j > 0) {
	    	//Make the non overlap file
			k = fn((char *)fastaHeader, j, (char *)sequence, length, chunkRemaining, fileHandleForChunks);
			chunkRemaining = fn2(chunkRemaining, k);

			//Make the overlap file
			l = j - overlapSize/2;
			if(l < 0) {
				l = 0;
			}
			fn((char *)fastaHeader, l, (char *)sequence, length, overlapSize, fileHandleForBridges);
			j += k;
	    }
	}
}

int main(int argc, char *argv[]) {
	int32_t j;
	FILE *fileHandle;
	assert(argc >= 6);
	fileHandleForChunks = fopen(argv[1], "w");
	fileHandleForBridges = fopen(argv[2], "w");
	assert(sscanf(argv[3], "%i", &chunkSize) == 1);
	assert(sscanf(argv[4], "%i", &overlapSize) == 1);
	initialiseTempFileTree(argv[5], 100, 4);
	assert(sscanf(argv[6], "%i", &useCompression) == 1);
	chunkRemaining = chunkSize;
	for(j=7; j<argc; j++) {
		fileHandle = fopen(argv[j], "r");
		fastaReadToFunction(fileHandle, fn3);
		fclose(fileHandle);
	}
	fclose(fileHandleForBridges);
	fclose(fileHandleForChunks);
	return 0;
}

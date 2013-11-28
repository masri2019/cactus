from cactus.preprocessor.preprocessorTest import *
from cactus.preprocessor.preprocessorTest import TestCase as PreprocessorTestCase
from cactus.shared.common import cactusRootPath
import xml.etree.ElementTree as ET

"""Runs cactus preprocessor using the lastz repeat mask script to show it working.
"""

class TestCase(PreprocessorTestCase):
    def testCactusPreprocessor(self):
        #Demo sequences
        sequenceNames = [ "%s.ENm001.fa" % species for species in 'human', "hedgehog" ]
        sequenceFiles = [ os.path.join(self.encodePath, self.encodeRegion, sequenceName) for sequenceName in sequenceNames ]
        #Make config file
        configFile = os.path.join(self.tempDir, "config.xml")
        rootElem =  ET.Element("preprocessor")
        #<preprocessor chunkSize="10000" proportionToSample="0.2" memory="littleMemory" preprocessorString="cactus_lastzRepeatMask.py --proportionSampled=PROPORTION_SAMPLED --minPeriod=1 --lastzOpts='--step=1 --ambiguous=iupac,100 --ungapped' IN_FILE OUT_FILE "/>
        preprocessor = ET.SubElement(rootElem, "preprocessor")
        preprocessor.attrib["chunkSize"] = "10000"
        preprocessor.attrib["proportionToSample"] = "0.2"
        preprocessor.attrib["string"] = "cactus_lastzRepeatMask.py --proportionSampled=PROPORTION_SAMPLED --minPeriod=1 --lastzOpts='--step=1 --ambiguous=iupac,100 --ungapped' IN_FILE OUT_FILE"
        fileHandle = open(configFile, "w")
        fileHandle.write(ET.tostring(preprocessor))
        fileHandle.close()
        #Run preprocessor
        command = "cactus_preprocessor.py %s %s %s --jobTree %s" % (self.tempDir, configFile, " ".join(sequenceFiles), os.path.join(self.tempDir, "jobTree"))
        system(command)
        for sequenceName, sequenceFile in zip(sequenceNames, sequenceFiles):
            #Parse sequences into dictionary
            originalSequences = getSequences(sequenceFile)
            #Load the new sequences
            processedSequences = getSequences(os.path.join(self.tempDir, sequenceName))
            #Check they are the same module masking
            self.checkSequenceSetsEqualModuloSoftMasking(originalSequences, processedSequences)
            
            #Compare the proportion of bases masked by lastz with original repeat masking
            maskedBasesOriginal = getMaskedBases(originalSequences)
            maskedBasesLastzMasked = getMaskedBases(processedSequences)
            #Total bases
            totalBases = sum([ len(i) for i in originalSequences.values() ])
            #Calculate number of hard masked bases
            totalNBases = len([ (header, i, base) for (header, i, base) in maskedBasesOriginal if base.upper() == "N" ])
            
            print " For the sequence file ", sequenceFile, \
             " the total number of sequences is ", len(originalSequences), \
             " the total number of bases ", totalBases, \
             " the number of bases originally masked was: ", len(maskedBasesOriginal),\
             " the number of bases masked after running lastz repeat masking is: ", len(maskedBasesLastzMasked), \
             " the intersection of these masked sets is: ", len(maskedBasesLastzMasked.intersection(maskedBasesOriginal)), \
             " the total number of bases that are Ns ", totalNBases
        
if __name__ == '__main__':
    unittest.main()
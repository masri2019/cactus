#include "cactusFacesTestShared.h"
/*
 * cactusFaceEndTest.c
 *
 *  Created on: 10-May-2010
 *      Author: benedictpaten
 */

static void testTearDown() {
	cactusFacesTestSharedTeardown();
}

static void testSetup() {
	cactusFacesTestSharedSetup();
}

void testFaceEnd_construct(CuTest* testCase) {
	testSetup();
	assert(testCase != NULL);
	testTearDown();
}

void testFaceEnd_getTopNode(CuTest* testCase) {
	testSetup();
	FaceEnd *faceEnd1 = face_getFaceEndForTopNode(face, topCap1);
	CuAssertTrue(testCase, faceEnd_getTopNode(faceEnd1) == cap_getPositiveOrientation(topCap1));

	FaceEnd *faceEnd2 = face_getFaceEndForTopNode(face, topCap2);
	CuAssertTrue(testCase, faceEnd_getTopNode(faceEnd2) == cap_getPositiveOrientation(topCap2));

	FaceEnd *faceEnd3 = face_getFaceEndForTopNode(face, topCap3);
	CuAssertTrue(testCase, faceEnd_getTopNode(faceEnd3) == cap_getPositiveOrientation(topCap3));

	FaceEnd *faceEnd4 = face_getFaceEndForTopNode(face, topCap4);
	CuAssertTrue(testCase, faceEnd_getTopNode(faceEnd4) == cap_getPositiveOrientation(topCap4));
	testTearDown();
}

void testFaceEnd_getFace(CuTest* testCase) {
	testSetup();
	FaceEnd *faceEnd1 = face_getFaceEndForTopNode(face, topCap1);
	CuAssertTrue(testCase, faceEnd_getFace(faceEnd1) == face);
	FaceEnd *faceEnd2 = face_getFaceEndForTopNode(face, topCap2);
	CuAssertTrue(testCase, faceEnd_getFace(faceEnd2) == face);
	testTearDown();
}

void testFaceEnd_getNumberOfBottomNodes(CuTest* testCase) {
	testSetup();
	FaceEnd *faceEnd1 = face_getFaceEndForTopNode(face, topCap1);
	CuAssertTrue(testCase, faceEnd_getNumberOfBottomNodes(faceEnd1) == 1);

	FaceEnd *faceEnd2 = face_getFaceEndForTopNode(face, topCap2);
	CuAssertTrue(testCase, faceEnd_getNumberOfBottomNodes(faceEnd2) == 1);

	FaceEnd *faceEnd3 = face_getFaceEndForTopNode(face, topCap3);
	CuAssertTrue(testCase, faceEnd_getNumberOfBottomNodes(faceEnd3) == 1);

	FaceEnd *faceEnd4 = face_getFaceEndForTopNode(face, topCap4);
	CuAssertTrue(testCase, faceEnd_getNumberOfBottomNodes(faceEnd4) == 1);
	testTearDown();
}

void testFaceEnd_bottomNodeIterator(CuTest* testCase) {
	testSetup();

	FaceEnd *faceEnd1 = face_getFaceEndForTopNode(face, topCap1);
	CuAssertTrue(testCase, faceEnd_getNumberOfBottomNodes(faceEnd1) == 1);
	FaceEnd_BottomNodeIterator *iterator = faceEnd_getBottomNodeIterator(faceEnd1);

	CuAssertTrue(testCase, faceEnd_getNextBottomNode(iterator) == cap_getPositiveOrientation(bottomCap1));
	CuAssertTrue(testCase, faceEnd_getNextBottomNode(iterator) == NULL);
	CuAssertTrue(testCase, faceEnd_getNextBottomNode(iterator) == NULL);
	FaceEnd_BottomNodeIterator *iterator2 = faceEnd_copyBottomNodeIterator(iterator);
	CuAssertTrue(testCase, faceEnd_getPreviousBottomNode(iterator) == cap_getPositiveOrientation(bottomCap1));
	CuAssertTrue(testCase, faceEnd_getPreviousBottomNode(iterator) == NULL);
	CuAssertTrue(testCase, faceEnd_getNextBottomNode(iterator) == cap_getPositiveOrientation(bottomCap1));

	//Test copied iterator
	CuAssertTrue(testCase, faceEnd_getNextBottomNode(iterator2) == NULL);
	CuAssertTrue(testCase, faceEnd_getPreviousBottomNode(iterator2) == cap_getPositiveOrientation(bottomCap1));
	CuAssertTrue(testCase, faceEnd_getPreviousBottomNode(iterator2) == NULL);

	faceEnd_destructBottomNodeIterator(iterator);
	faceEnd_destructBottomNodeIterator(iterator2);

	testTearDown();
}

CuSuite* cactusFaceEndTestSuite(void) {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, testFaceEnd_getTopNode);
	SUITE_ADD_TEST(suite, testFaceEnd_getFace);
	SUITE_ADD_TEST(suite, testFaceEnd_getNumberOfBottomNodes);
	SUITE_ADD_TEST(suite, testFaceEnd_bottomNodeIterator);
	SUITE_ADD_TEST(suite, testFaceEnd_construct);
	return suite;
}

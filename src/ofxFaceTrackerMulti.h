/*
ofxFaceTracker provides an interface to Jason Saragih's FaceTracker library.

getImagePoint()/getImageMesh() are in image space. This means that all the
points will line up with the pixel coordinates of the image you fed into
ofxFaceTracker.

getObjectPoint()/getObjectMesh() are in 3d object space. This is a product of
the mean mesh with only the expression applied. There is no rotation or
translation applied to the object space.

getMeanObjectPoint()/getMeanObjectMesh() are also in 3d object space. However,
there is no expression applied to the mesh.
*/

#pragma once

#include "ofxCv.h"

#define USE_THREADED


#ifdef USE_THREADED
#include "ofxFaceTrackerThreaded.h"
#else
#include "ofxFaceTracker.h"
#endif
//https://github.com/kylemcdonald/ofxFaceTracker/issues/24#issuecomment-145899247

using namespace cv;
using namespace ofxCv;

#ifdef USE_THREADED
class ofxFaceTrackerMulti {
#else
class ofxFaceTrackerMulti : public ofThread {
#endif
public:

	int maxNumOfFaces;
	int amtFound;


#ifdef USE_THREADED
	// vector<ofxFaceTrackerThreaded> trackers;
	ofxFaceTrackerThreaded trackers[2];
#else
	vector<ofxFaceTracker> trackers;
#endif

	cv::Mat buffer;

	void setup(int _maxFaces, int _camW, int _camH) {
		maxNumOfFaces = _maxFaces;


#ifndef USE_THREADED
		trackers.resize(maxNumOfFaces);
#endif
		for (int i = 0; i<maxNumOfFaces; i++) {
			trackers[i].setup();
		}

		buffer = Mat(_camH, _camW, CV_8UC3);
	}

	void update(cv::Mat image) {

		copy(image, buffer);

		amtFound = 0;

		for (int i = 0; i<maxNumOfFaces; i++) {

			trackers[i].update(buffer);

			if (trackers[i].getFound()) {

				// cv::Rect temp_faceRect = toCv(trackers[i].getHaarRectangle());

				cv::Rect bRect = cv::boundingRect(toCv(trackers[i].getImagePoints()));


				cv::rectangle(buffer, bRect, cvScalar(0, 0, 0), -1, 8, 0);

				amtFound++;
			}

		}




	}

	void exit() {
		for (int i = 0; i<maxNumOfFaces; i++) {
#ifdef USE_THREADED
			trackers[i].waitForThread();
#endif
		}
	}

	void draw() {


	}
	void reset(int _id);



protected:

};
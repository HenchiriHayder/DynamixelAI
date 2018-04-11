#pragma once
#include "ofThread.h"
#include "ofImage.h"
#include "ofVideoGrabber.h"
#include "ofxCvHaarFinder.h"  
#include "ofxOpenCv.h"

class VideoThread : public ofThread
{
public:
	VideoThread();
	~VideoThread();
	static bool sort_carea_compare(const ofxCvBlob & a, const ofxCvBlob & b) {
		// use opencv to calc size, then sort based on size
		float areaa = fabs(a.area);
		float areab = fabs(b.area);

		//return 0;
		return (areaa > areab);
	}

	void VideoThread::threadedFunction() {

		camWidth = 320;  // try to grab at this size.
		camHeight = 240;

		//get back a list of devices.
		vector<ofVideoDevice> devices = vidGrabber.listDevices();

		for (size_t i = 0; i < devices.size(); i++) {
			if (devices[i].bAvailable) {
				//log the device
				ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
			}
			else {
				//log the device and note it as unavailable
				ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
			}
		}

		vidGrabber.setDeviceID(0);
		vidGrabber.setDesiredFrameRate(60);
		vidGrabber.initGrabber(camWidth, camHeight);

		videoInverted.allocate(camWidth, camHeight, OF_PIXELS_RGB);
		videoTexture.allocate(videoInverted);
		ofSetVerticalSync(true);

		finder.setup("haarcascade_frontalface_default.xml");

		// start

		while (isThreadRunning()) {

			vidGrabber.update();
			if (vidGrabber.isFrameNew()) {
				lock();
				// load the image
				image.setFromPixels(vidGrabber.getPixels());
				//grayImage = image;
				finder.findHaarObjects(image);
				if (finder.blobs.size() >= 1) {
				sort(finder.blobs.begin(), finder.blobs.end(), sort_carea_compare);

				cur = finder.blobs[0].boundingRect;
				}
				//cout << "blobs Thread size = " << finder.blobs.size() << endl;
				unlock();
			}
			//cout << "isThreadRunning" << endl;
			//image.draw(10, 10);
		}

		// done
	}

	ofVideoGrabber vidGrabber; // the cam
	ofImage image;

	ofPixels videoInverted;
	ofTexture videoTexture;
	int camWidth;
	int camHeight;

	int rectX, rectY;

	ofImage grayImage;
	ofxCvHaarFinder finder;
	ofRectangle cur;
};


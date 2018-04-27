#pragma once


#include "ofThread.h"
#include "ofxCv.h"

/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).

using namespace ofxCv;
using namespace cv;

class ThreadedObject : public ofThread
{
public:

	// ofPixels pixels;
	ofVideoGrabber vidGrabber;
	int 				camWidth;
	int 				camHeight;
	int deviceID;

	ofImage cropped;

	cv::Rect biggestRect;

	float collectionDuration, collectionTimer;

	// ofImage image;
	ofxCv::ObjectFinder objectFinder;
	// cv::Mat camMat;

	/// Create a ThreadedObject and initialize the member
	/// variable in an initialization list.
	ThreadedObject() : count(0)
	{
	}


	static bool sort_carea_compare(const Rect & a, const Rect & b) {
		// use opencv to calc size, then sort based on size
		float areaa = fabs(a.area());
		float areab = fabs(b.area());

		//return 0;
		return (areaa > areab);
	}

	void setup(int _id, int _camW, int _camH, bool _useFeatures = false)
	{

		deviceID = _id;
		camWidth = _camW;
		camHeight = _camH;

		vidGrabber.listDevices();

		vidGrabber.setDeviceID(deviceID);
		//  vidGrabber.setDesiredFrameRate(60);
		vidGrabber.initGrabber(camWidth, camHeight);
		vidGrabber.setUseTexture(false);

		objectFinder.setup(ofToDataPath("haarcascade_frontalface_default.xml"));
		objectFinder.setPreset(ObjectFinder::Fast);


		cropped.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
		cropped.setUseTexture(false);

		collectionDuration = 0.2;
	}
	/// Start the thread.
	void start()
	{

		// Mutex blocking is set to true by default
		// It is rare that one would want to use startThread(false).
		startThread();
	}

	/// Signal the thread to stop.  After calling this method,
	/// isThreadRunning() will return false and the while loop will stop
	/// next time it has the chance to.
	void stop()
	{
		stopThread();
	}

	/// Our implementation of threadedFunction.
	void threadedFunction()
	{
		while (isThreadRunning())
		{
			// Attempt to lock the mutex.  If blocking is turned on,
			if (lock())
			{
				// The mutex is now locked

				vidGrabber.update();

				if (vidGrabber.isFrameNew()) {

					objectFinder.update(vidGrabber);

					if (objectFinder.size() > 0) {
						std::vector<cv::Rect> objects = objectFinder.getObjects();
						sort(objects.begin(), objects.end(), sort_carea_compare);
						biggestRect = toCv(objectFinder.getObject(0));


						/*Mat camMat = toCv(vidGrabber);

						for (int n = 0; n<objectFinder.size(); n++) {


							cv::Rect roi = toCv(objectFinder.getObject(n));

							Mat croppedCamMat(camMat, roi);
							resize(croppedCamMat, cropped);
							cropped.update();

						}*/
					}
					else {
						biggestRect = cv::Rect(0, 0, 0, 0);
					}

				}


				// Unlock the mutex.  This is only
				// called if lock() returned true above.
				unlock();

				// Sleep for 1 second.
				// sleep(1000);
			}
			else
			{
				// If we reach this else statement, it means that we could not
				// lock our mutex, and so we do not need to call unlock().
				// Calling unlock without locking will lead to problems.
				ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
			}
		}
	}

	/// This drawing function cannot be called from the thread itself because
	/// it includes OpenGL calls (ofDrawBitmapString).
	void draw()
	{
		objectFinder.draw();
	}

	// Use ofScopedLock to protect a copy of count while getting a copy.
	int getCount()
	{
		ofScopedLock lock(mutex);
		return count;
	}

protected:
	// This is a simple variable that we aim to always access from both the
	// main thread AND this threaded object.  Therefore, we need to protect it
	// with the mutex.  In the case of simple numerical variables, some
	// garuntee thread safety for small integral types, but for the sake of
	// illustration, we use an int.  This int could represent ANY complex data
	// type that needs to be protected.
	int count;

};

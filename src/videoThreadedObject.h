#pragma once


#include "ofThread.h"
#include "ofxCv.h"

/// This is a simple example of a ThreadedObject created by extending ofThread.
/// It contains data (count) that will be accessed from within and outside the
/// thread and demonstrates several of the data protection mechanisms (aka
/// mutexes).

using namespace ofxCv;
using namespace cv;

class videoThreadedObject : public ofThread
{
public:

	// ofPixels pixels;
	ofVideoGrabber vidGrabber;
	int 				camWidth;
	int 				camHeight;
	int deviceID;

	ofImage cropped;

	cv::Rect biggestRect;


	/// Create a ThreadedObject and initialize the member
	/// variable in an initialization list.
	videoThreadedObject() : count(0)
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
		vidGrabber.initGrabber(camWidth, camHeight);
		vidGrabber.setUseTexture(false);

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
				unlock();


			}
			else
			{
				ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
			}
		}
	}


	// Use ofScopedLock to protect a copy of count while getting a copy.
	int getCount()
	{
		ofScopedLock lock(mutex);
		return count;
	}

protected:

	int count;

};
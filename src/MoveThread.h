#pragma once
#include "ofThread.h"
#include "ofxDynamixel/src/ofConnexion.h"
#include "ofxDynamixel/src/ofControlTable.h"
#include "ofDynamixelServo.h"
class MoveThread : public ofThread
{
public:
	MoveThread();
	~MoveThread();

	ofDynamixelServo * dynamixels[255];
	vector< vector<int> > positionsVector;
	bool asleep = false;
	bool robotMoving = true;
	bool faceDetected = false;

	void isSleeping(bool isSleeping) {
		asleep = isSleeping;
	}

	void robotIsMoving(bool _robotMoving) {
		robotMoving = _robotMoving;
	}

	void IsFaceDetected(bool _faceDetected) {
		faceDetected = _faceDetected;
	}

	void setup(vector< vector<int> > _positionsVector, ofDynamixelServo * (&_dynamixels)[255]) {
		positionsVector.clear();
		positionsVector = _positionsVector;
		for (int i = 0; i < 255; i++) {
			ofDynamixelServo * dynServo = _dynamixels[i];
			if (dynServo) {
				dynamixels[i] = dynServo;
			}
		}
		
	}

	void setup(vector< vector<int> > _positionsVector) {
		positionsVector.clear();
		positionsVector = _positionsVector;
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


	int rangeRandomAlg2(int min, int max)
	{
		int n = max - min + 1;
		int remainder = RAND_MAX % n;
		int x;
		do
		{
			x = rand();
		} while (x >= RAND_MAX - remainder);
		return min + x % n;
	}


	void MoveThread::threadedFunction() {

		// start
		int movingSpeed = 50;
		int randomPosition;
		int servoMoteurID;
		while (isThreadRunning()) {
			Sleep(10000);
			if(!asleep && robotMoving && !faceDetected){
			randomPosition = rand() % ((positionsVector.size() - 1) - 0 + 1) + 0;
			//randomPosition = rangeRandomAlg2(0, positionsVector.size() - 1);
			cout << "\n################################ " << endl;
			cout << "randomPosition = " << randomPosition << endl;
			servoMoteurID = 0;
			for (vector<int>::iterator it = positionsVector[randomPosition].begin(); it != positionsVector[randomPosition].end(); ++it) {
				dynamixels[servoMoteurID]->getDynamixel()->move(*it, movingSpeed);
				//printf("\nposition of dynamixels[%d] = %d\n", servoMoteurID, *it);
				//Sleep(500);
				servoMoteurID++;

			}
			}
		}
	}

};


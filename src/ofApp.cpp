#include "ofApp.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include "ofxCvHaarFinder.h"  
//--------------------------------------------------------------
void ofApp::setup() {
	gui11.setDefaultWidth(300);
	myFont.load("arial.ttf", 14);

	setGui11();

	ofSetVerticalSync(true);

	camWidth = 640;	// try to grab at this size.
	camHeight = 480;

	//we can now get back a list of devices.

	int deviceId = -1;

	ofVideoGrabber temp_grabber;


	vector<ofVideoDevice> devices = temp_grabber.listDevices();

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

	//testImage.allocate(camWidth, camHeight, OF_IMAGE_COLOR);
	videoTexture.allocate(camWidth, camHeight, GL_RGB);
	//threadedObject.setup(1, camWidth, camHeight, false);

	//threadedObject.start();

	threadedObject.setup(1, camWidth, camHeight, false);

	temp_grabber.close();

	moveX = false;
	moveY = false;
	moveToReactionPositionHappy = false;
	moveToReactionPositionSurprise = false;
	returnToInitialPosition = false;
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------
	if (myXml.loadFile("myXml.xml")) {
		message = "myXml.xml loaded!";
	}
	else {
		message = "unable to load myXml.xml check data/ folder";
	}
	cout << message << endl;
	//myXml.pushTag("positions");
	cout << "getNumAttributes(type) = " << myXml.getNumTags("dynamixel") << endl;



	//myXml.pushTag("positions");
	int numberOfSavedPositions = myXml.getNumTags("positionForm");
	for (int i = 0; i < numberOfSavedPositions; i++) {
		cout << "myXml.getAttribute(positionForm, type, defaultValue, i) = " << myXml.getAttribute("positionForm", "type", "defaultValue", i) << endl;
		if (myXml.getAttribute("positionForm", "type", "", i) == "move") {
			myXml.pushTag("positionForm", i);
			std::vector<int> dynamixelsPosition;
			for (int j = 0; j < 4; j++) {
				//cout << "myXml.getAttribute(dynamixel, position,0,j) = " << myXml.getValue("dynamixel", 0, j) << endl;
				dynamixelsPosition.push_back(myXml.getAttribute("dynamixel", "position", 0, j));
			}
			positionsVectorMove.push_back(dynamixelsPosition);
		}

		if (myXml.getAttribute("positionForm", "type", "", i) == "happy") {
			myXml.pushTag("positionForm", i);
			std::vector<int> dynamixelsPosition;
			for (int j = 0; j < 4; j++) {
				//cout << "myXml.getAttribute(dynamixel, position,0,j) = " << myXml.getValue("dynamixel", 0, j) << endl;
				dynamixelsPosition.push_back(myXml.getAttribute("dynamixel", "position", 0, j));
			}
			positionsVectorHappy.push_back(dynamixelsPosition);
		}

		if (myXml.getAttribute("positionForm", "type", "", i) == "fear") {
			myXml.pushTag("positionForm", i);
			std::vector<int> dynamixelsPosition;
			for (int j = 0; j < 4; j++) {
				//cout << "myXml.getAttribute(dynamixel, position,0,j) = " << myXml.getValue("dynamixel", 0, j) << endl;
				dynamixelsPosition.push_back(myXml.getAttribute("dynamixel", "position", 0, j));
			}
			positionsVectorFear.push_back(dynamixelsPosition);
		}

		if (myXml.getAttribute("positionForm", "type", "", i) == "wakeUp") {
			myXml.pushTag("positionForm", i);
			std::vector<int> dynamixelsPosition;
			for (int j = 0; j < 4; j++) {
				//cout << "myXml.getAttribute(dynamixel, position,0,j) = " << myXml.getValue("dynamixel", 0, j) << endl;
				dynamixelsPosition.push_back(myXml.getAttribute("dynamixel", "position", 0, j));
			}
			positionsVectorWakeUp.push_back(dynamixelsPosition);
		}

		if (myXml.getAttribute("positionForm", "type", "", i) == "surprise") {
			myXml.pushTag("positionForm", i);
			std::vector<int> dynamixelsPosition;
			for (int j = 0; j < 4; j++) {
				//cout << "myXml.getAttribute(dynamixel, position,0,j) = " << myXml.getValue("dynamixel", 0, j) << endl;
				dynamixelsPosition.push_back(myXml.getAttribute("dynamixel", "position", 0, j));
			}
			positionsVectorSurprise.push_back(dynamixelsPosition);
		}

		if (myXml.getAttribute("positionForm", "type", "", i) == "sleep") {
			myXml.pushTag("positionForm", i);
			std::vector<int> dynamixelsPosition;
			for (int j = 0; j < 4; j++) {
				//cout << "myXml.getAttribute(dynamixel, position,0,j) = " << myXml.getValue("dynamixel", 0, j) << endl;
				dynamixelsPosition.push_back(myXml.getAttribute("dynamixel", "position", 0, j));
			}
			positionsVectorSleep.push_back(dynamixelsPosition);
		}

		myXml.popTag();
	}

	classifier.load("expressions");

	threadedObject.start();
	tracker.setup();
	tracker.setRescale(.5);
	time = ofGetElapsedTimef();

}

void ofApp::setGui11() {
	gui11.setup();
	gui11.setShowHeader(false);
	gui11.setPosition(350, 10);
	titleGui11.setDefaultHeight(30);
	gui11.add(titleGui11.setup("titleGui11", "            Connexion"));
	titleGui11.setShowLabelName(false);
	titleGui11.setBackgroundColor(ofColor::blueSteel);
	titleGui11.setDefaultHeight(20);
	gui11.setBorderColor(ofColor::black);
	gui11.add((new ofxGuiSpacer(5, 0, 0)));
	gui11.add(USB2Dynamixel_Ports.setup("USB2Dynamixel Ports:", ""));
	USB2Dynamixel_Ports.setBackgroundColor(ofColor(60, 60, 60));
	gui11.add((new ofxGuiSpacer(5, 0, 0)));
	gui11.add(baudrates.setup("Baudrates:", ""));
	baudrates.setBackgroundColor(ofColor(60, 60, 60));
	gui11.add((new ofxGuiSpacer(5, 0, 0)));
	gui11.add(maximumIDs.setup("Maximum IDs:", ""));
	maximumIDs.setBackgroundColor(ofColor(60, 60, 60));
	gui11.add((new ofxGuiSpacer(5, 0, 0)));
	gui11.add(scan.setup("                Scan", 0, 30));
	gui11.add((new ofxGuiSpacer(5, 0, 0)));

	scan.addListener(this, &ofApp::scanPressed);
}

void ofApp::takeFirstPositionPressed() {

}

void ofApp::scanPressed() {
	cout << ((string)USB2Dynamixel_Ports).c_str() << endl;
	cout << std::stoi((string)baudrates) << endl;
	cout << std::stoi((string)maximumIDs) << endl;
	connect(((string)USB2Dynamixel_Ports).c_str(), 1.0, std::stoi((string)baudrates), std::stoi((string)maximumIDs));

	if (taille > 0) {
		cout << "taille > 0" << endl;
		//setGui22();
		setGui33();
		setGui55();
		setGuiHH();

		int randomPositionWakeUp = rand() % ((positionsVectorWakeUp.size() - 1) - 0 + 1) + 0;
		//randomPosition = rangeRandomAlg2(0, positionsVector.size() - 1);
		cout << "\n******************************** " << endl;
		cout << "randomPositionWakeUp = " << randomPositionWakeUp << endl;

		int id = 0;
		for (vector<int>::iterator it = positionsVectorWakeUp[randomPositionWakeUp].begin(); it != positionsVectorWakeUp[randomPositionWakeUp].end(); ++it) {
			cout << "*it = " << *it << endl;
			dynamixels[id]->getDynamixel()->move(*it, 50);
			id++;

		}


		moveThread.setup(positionsVectorMove, dynamixels);
		moveThread.start();


		//trackerThread.start();
	}

}
//--------------------------------------------------------------
static bool sort_carea_compare(const ofxCvBlob & a, const ofxCvBlob & b) {
	// use opencv to calc size, then sort based on size
	float areaa = fabs(a.area);
	float areab = fabs(b.area);

	//return 0;
	return (areaa > areab);
}
//--------------------------------------------------------------
void ofApp::update() {

	//----------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------
	threadedObject.lock();
	//videoTexture.loadData(threadedObject.vidGrabber.getPixelsRef());
	testImage.setFromPixels(threadedObject.vidGrabber.getPixels());
	//biggestRectApp = threadedObject.biggestRect;
	if (tracker.update(toCv(threadedObject.vidGrabber))) {
		classifier.classify(tracker);
	}
	threadedObject.unlock();

	if (taille > 0 && robotMoving) {
		//---------------------------------------------------------------
		brightness = 0;
		for (int i = 0; i < camWidth; i++) {
			for (int j = 0; j < camHeight; j++) {
				ofColor cur = testImage.getColor(i, j);
				brightness += cur.getBrightness();
			}
		}

		//cout << "brightness = " << brightness << endl;

		if (brightness < 22000000) {
			brightnessFrameNumber++;
		}

		else {
			brightnessFrameNumber = 0;
			/*if (ofGetElapsedTimef() - time > 10 && !moveThread.isThreadRunning()) {
			moveThread.start();
			}*/
		}

		if (brightnessFrameNumber > 50 && !isSleeping) {
			cout << "sleep mode !!! " << endl;
			if (moveThread.isThreadRunning()) {
				moveThread.stop();

			}
			isSleeping = true;
			moveThread.isSleeping(isSleeping);
			//time = ofGetElapsedTimef();

			int randomPositionSleep = rand() % ((positionsVectorSleep.size() - 1) - 0 + 1) + 0;
			//randomPosition = rangeRandomAlg2(0, positionsVector.size() - 1);
			cout << "\n******************************** " << endl;
			cout << "randomPositionSleep = " << randomPositionSleep << endl;

			//Sleep(11000);
			int id = 0;
			for (vector<int>::iterator it = positionsVectorSleep[randomPositionSleep].begin(); it != positionsVectorSleep[randomPositionSleep].end(); ++it) {
				cout << "*it = " << *it << endl;
				dynamixels[id]->getDynamixel()->move(*it, 50);
				id++;
			}
			while (dynamixels[3]->getDynamixel()->getControlTable()->moving()) {}

			
		}

		//----------------------------------------------------------------------

		if (isSleeping && !moveThread.isThreadRunning() && brightnessFrameNumber == 0) {
			isSleeping = false;
			moveThread.isSleeping(isSleeping);

			int randomPositionWakeUp = rand() % ((positionsVectorWakeUp.size() - 1) - 0 + 1) + 0;
			//randomPosition = rangeRandomAlg2(0, positionsVector.size() - 1);
			cout << "\n******************************** " << endl;
			cout << "randomPositionWakeUp = " << randomPositionWakeUp << endl;

			int id = 0;
			for (vector<int>::iterator it = positionsVectorWakeUp[randomPositionWakeUp].begin(); it != positionsVectorWakeUp[randomPositionWakeUp].end(); ++it) {
				cout << "*it = " << *it << endl;
				dynamixels[id]->getDynamixel()->move(*it, 50);
				id++;

			}
			//if (!moveThread.isThreadRunning()) {
				moveThread.start();
			//}
			
		}

		if (ofGetElapsedTimef() - time > 7 && !moveThread.isThreadRunning() && brightnessFrameNumber == 0) {
			moveThread.start();
			
			faceDetected = false;
			moveThread.IsFaceDetected(faceDetected);
		}


		if (tracker.getFound()) {
			if (moveThread.isThreadRunning()) {
				moveThread.stop();
				faceDetected = true;
				moveThread.IsFaceDetected(faceDetected);
			}
			time = ofGetElapsedTimef();


			if (moveX && !dynamixels[0]->getDynamixel()->getControlTable()->moving()) {
				moveX = false;
			}
			if (moveY && !dynamixels[3]->getDynamixel()->getControlTable()->moving()) {
				moveY = false;
			}
			if (!moveX && tracker.getPosition().x < (2 * camWidth) / 5) {
				puts("+x\n");
				moveX = true;
				dynamixels[0]->getDynamixel()->move(dynamixels[0]->getCurrentPosition() + 30, 30);
			}
			if (!moveX && tracker.getPosition().x >(3 * camWidth) / 5) {
				puts("-x\n");
				moveX = true;
				dynamixels[0]->getDynamixel()->move(dynamixels[0]->getCurrentPosition() - 30, 30);
			}
			if (moveX && (unsigned)(tracker.getPosition().x - 256) < 128) {
				puts("stop moveX \n");
				moveX = false;
				dynamixels[0]->getDynamixel()->move(dynamixels[0]->getCurrentPosition(), 30);
			}


			if (!moveY && tracker.getPosition().y < (2 * camHeight) / 5) {
				puts("+y\n");
				moveY = true;
				dynamixels[3]->getDynamixel()->move(dynamixels[3]->getCurrentPosition() - 30, 30);
			}
			if (!moveY && tracker.getPosition().y >(3 * camHeight) / 5) {
				puts("-y\n");
				moveY = true;
				dynamixels[3]->getDynamixel()->move(dynamixels[3]->getCurrentPosition() + 30, 30);
			}
			if (moveY && (unsigned)(tracker.getPosition().y - 192) < 96) {
				puts("stop moveY \n");
				moveY = false;
				dynamixels[3]->getDynamixel()->move(dynamixels[3]->getCurrentPosition(), 30);
			}
		}
		else {
			smileExpressionCount = 0;
			surprisedExpressionCount = 0;
		}
	}
	if(taille > 0){
	cout << "MoveThreadIsRunning = " << moveThread.isThreadRunning() << endl;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	if (taille > 0 && robotMoving) {
		int servoMoteurID;
		int t_x = 0;
		int t_y = 0;

		ofSetHexColor(0xffffff);

		//videoTexture.draw(t_x, t_y, camWidth / 3, camHeight / 3);
		testImage.draw(t_x, t_y, camWidth / 2, camHeight / 2);

		ofPushMatrix();
		ofTranslate(t_x, t_y);
		ofScale(1 / 2.0, 1 / 2.0);
		tracker.draw();
		ofPopMatrix();


		if (tracker.getFound()) {
			int w = 100, h = 12;
			ofPushStyle();
			ofPushMatrix();
			ofTranslate(5, 10);
			int n = classifier.size();
			int primary = classifier.getPrimaryExpression();
			for (int i = 0; i < n; i++) {
				ofSetColor(i == primary ? ofColor::red : ofColor::black);
				ofDrawRectangle(0, 0, w * classifier.getProbability(i) + .5, h);
				ofSetColor(255);
				ofDrawBitmapString(classifier.getDescription(i), 5, 9);
				ofTranslate(0, h + 5);
			}
			ofPopMatrix();
			ofPopStyle();

			if (primary == 2 && classifier.getProbability(primary) + .5 > 1.4200) {
				smileExpressionCount++;
				surprisedExpressionCount = 0;
				//cout << "trackerThread.classifier.getProbability(i) + .5 = " << classifier.getProbability(primary) + .5 << endl;
			}

			if (primary == 0 && classifier.getProbability(primary) + .5 > 1.4200) {
				surprisedExpressionCount++;
				smileExpressionCount = 0;
				//cout << "trackerThread.classifier.getProbability(i) + .5 = " << classifier.getProbability(primary) + .5 << endl;
			}

			if (smileExpressionCount > 30) {
				cout << "Expression detected = Smile" << endl;
				smileExpressionCount = 0;
				//store the current position in a vector here
				moveToReactionPositionHappy = true;
				returnToInitialPosition = true;
			}

			if (surprisedExpressionCount > 25) {
				cout << "Expression detected = Surprised" << endl;
				surprisedExpressionCount = 0;
				moveToReactionPositionSurprise = true;
				returnToInitialPosition = true;
			}

			//if (tracker.getOrientation().z > 0.5  && classifier.getProbability(primary) + .5 < 1)
			if (tracker.getOrientation().z > 0.5) {
				cout << "tracker.getOrientation().z = " << tracker.getOrientation().z << endl;
				tiltedFaceFrameNumber++;
			}

			if (tiltedFaceFrameNumber > 30) {
				tiltedFaceDetected = true;
			}

		}
		else {
			smileExpressionCount = 0;
			surprisedExpressionCount = 0;
			tiltedFaceFrameNumber = 0;
		}



		int numberOfMovement = 0;
		if (moveToReactionPositionHappy) {

			dynamixelsPositionBeforeExpression.clear();
			for (int i = 0; i < 4; i++)
			{
				dynamixelsPositionBeforeExpression.push_back(dynamixels[i]->getCurrentPosition());
				//dynamixelsPosition.push_back(rand());
			}

			while (numberOfMovement < 3)
			{
				if (!dynamixels[3]->getDynamixel()->getControlTable()->moving()) {
					cout << " positionsVectorHappy[1] " << endl;
					servoMoteurID = 1;
					for (vector<int>::iterator it = std::next(positionsVectorHappy[1].begin()); it != positionsVectorHappy[1].end(); ++it) {
						cout << "*it = " << *it << endl;
						dynamixels[servoMoteurID]->getDynamixel()->move(*it, 100);
						servoMoteurID++;
					}
					while (dynamixels[3]->getDynamixel()->getControlTable()->moving())
					{

					}
				}
				if (!dynamixels[3]->getDynamixel()->getControlTable()->moving()) {
					cout << " positionsVectorHappy[2] " << endl;
					servoMoteurID = 1;
					for (vector<int>::iterator it = std::next(positionsVectorHappy[2].begin()); it != positionsVectorHappy[2].end(); ++it) {
						cout << "*it = " << *it << endl;
						dynamixels[servoMoteurID]->getDynamixel()->move(*it, 100);
						servoMoteurID++;

					}
					while (dynamixels[3]->getDynamixel()->getControlTable()->moving())
					{

					}
				}
				numberOfMovement++;
			}
			moveToReactionPositionHappy = false;
			tiltedFaceDetected = false;
		}


		if (moveToReactionPositionSurprise) {

			dynamixelsPositionBeforeExpression.clear();
			for (int i = 0; i < 4; i++)
			{
				dynamixelsPositionBeforeExpression.push_back(dynamixels[i]->getCurrentPosition());
				//dynamixelsPosition.push_back(rand());
			}
			surprisePosNumber = 0;
			for (vector< vector<int> >::iterator it = positionsVectorSurprise.begin(); it != positionsVectorSurprise.end(); ++it) {
				cout << "\n********************** " << endl;
				servoMoteurID = 0;
				if (surprisePosNumber >= 3) {
					dynamixels[0]->getDynamixel()->move(dynamixelsPositionBeforeExpression[0], 100);
					servoMoteurID = 1;
					for (vector<int>::iterator it1 = std::next(it->begin()); it1 != it->end(); ++it1) {

						//printf("\nposition of dynamixels[%d] = %d\n", servoMoteurID++, *it1);
						dynamixels[servoMoteurID]->getDynamixel()->move(*it1, 100);
						servoMoteurID++;
					}
				}
				else {
					for (vector<int>::iterator it1 = it->begin(); it1 != it->end(); ++it1) {

						//printf("\nposition of dynamixels[%d] = %d\n", servoMoteurID++, *it1);
						dynamixels[servoMoteurID]->getDynamixel()->move(*it1, 100);
						servoMoteurID++;
					}
				}
				while (dynamixels[3]->getDynamixel()->getControlTable()->moving() || dynamixels[0]->getDynamixel()->getControlTable()->moving())
				{

				}
				surprisePosNumber++;
			}
			moveToReactionPositionSurprise = false;
			tiltedFaceDetected = false;
		}

		//if (moveToReactionPosition) {
		//	puts("Move to Reaction position");
		//	dynamixelsPositionBeforeExpression.clear();

		//	for (int i = 0; i < 4; i++)
		//	{
		//		dynamixelsPositionBeforeExpression.push_back(dynamixels[i]->getCurrentPosition());
		//		//dynamixelsPosition.push_back(rand());
		//	}


		//	servoMoteurID = 0;
		//	for (vector<int>::iterator it = positionsVectorHappy[0].begin(); it != positionsVectorHappy[0].end(); ++it) {
		//		cout << "*it = " << *it << endl;
		//		dynamixels[servoMoteurID]->getDynamixel()->move(*it, 50);
		//		servoMoteurID++;

		//	}
		//	moveToReactionPosition = false;
		//}

		if (returnToInitialPosition && !dynamixels[3]->getDynamixel()->getControlTable()->moving()) {
			puts("Return to initial position");
			servoMoteurID = 0;
			for (vector<int>::iterator it = dynamixelsPositionBeforeExpression.begin(); it != dynamixelsPositionBeforeExpression.end(); ++it) {
				cout << "*it = " << *it << endl;
				dynamixels[servoMoteurID]->getDynamixel()->move(*it, 50);
				servoMoteurID++;

			}
			returnToInitialPosition = false;
		}


		if (tiltedFaceDetected) {
			cout << "tiltedFaceDetected !!!" << endl;
			tiltedFaceDetected = false;
			int dynamixel3Position = dynamixels[3]->getCurrentPosition();
			dynamixels[3]->getDynamixel()->move(188, 100);
			while (dynamixels[3]->getDynamixel()->getControlTable()->moving())
			{

			}
			dynamixels[3]->getDynamixel()->move(dynamixel3Position, 100);
			while (dynamixels[3]->getDynamixel()->getControlTable()->moving())
			{

			}
			
		}

	
		
	}
	

	if (taille > 0) {
		gui55.draw();
		guiHH.draw();
		gui33.draw();
	}

	gui11.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}


void ofApp::setled(int id) {
	if (dynamixels[id]->getDynamixel()->getControlTable()->ledState() == 0) {
		dynamixels[id]->getDynamixel()->getControlTable()->setLed(1);
	}
	else {
		dynamixels[id]->getDynamixel()->getControlTable()->setLed(0);
	}
}

void ofApp::scanDynamixel(int max)
{
	int p = 0;
	id_dynamixel.clear();
	for (int i = 0; i <= max; i++) {
		if (portConnexion->ping(i)) {
			id_dynamixel.push_back(i);
		}
	}
}



void ofApp::setGui55() {
	gui55.setup();
	gui55.setShowHeader(false);
	gui55.setPosition(720, 10);
	titleGui55.setDefaultHeight(30);
	gui55.add(titleGui55.setup("titleGui55", "             Movements"));
	titleGui55.setShowLabelName(false);
	titleGui55.setBackgroundColor(ofColor::blueSteel);
	titleGui55.setDefaultHeight(20);
	gui55.setBorderColor(ofColor::black);

	string textString = "Pour faire bouger un moteur dynamixel, ";
	textString += "Veuillez entrer d'abord l'ID de 'Dynamixel' au dessus.";
	//textString += "La vue s'adapte a chaque fois qu'un nouvel ID est selectione pour afficher les informations correspondantes";


	gui55.add((new ofxGuiSpacer(5, 0, 0)));
	gui55.add(dynID.setup("Dyamixel ID (1 - 4):", ""));
	dynID.setBackgroundColor(ofColor(60, 60, 60));

	gui55.add((new ofxGuiSpacer(5, 0, 0)));
	gui55.add(description.setup("description", "Pour faire bouger un moteur"));
	description.setShowLabelName(false);
	gui55.add(description1.setup("description", "dynamixel, Veuillez entrer d'abord"));
	description1.setShowLabelName(false);
	gui55.add(description2.setup("description", "l'ID de Dynamixel au dessus."));
	description2.setShowLabelName(false);


	gui55.add((new ofxGuiSpacer(25, 0, 0)));
	gui55.add(goalPositionSlider.setup("Angle", 140, 0, 300));

	/*
	* rotary slider
	*/
	gui55.add((new ofxGuiSpacer(5, 0, 0)));
	rotarySlider.setup("rotary");
	rotarySlider.setBorderColor(ofColor::blanchedAlmond - ofColor(50));
	angleSlider.setup(slider_param.set("Angle", 140, 0, 300), 160, 160);
	rotarySlider.add(&angleSlider);
	rotarySlider.getControl("Angle")->setFillColor(ofColor::white);
	rotarySlider.getControl("Angle")->setBackgroundColor(ofColor::blanchedAlmond - ofColor(130));
	gui55.add(&rotarySlider);

	gui55.add((new ofxGuiSpacer(5, 0, 0)));
	gui55.add(centralPosition.setup("          Central Position", 0, 30));

	gui55.add((new ofxGuiSpacer(5, 0, 0)));
	gui55.add(goalPositionText.setup("Goal position:", ""));
	goalPositionText.setBackgroundColor(ofColor(60, 60, 60));

	gui55.add((new ofxGuiSpacer(5, 0, 0)));
	gui55.add(movingSpeed.setup("Moving speed:", ""));
	movingSpeed.setBackgroundColor(ofColor(60, 60, 60));

	gui55.add((new ofxGuiSpacer(5, 0, 0)));
	gui55.add(move.setup("               Move", 0, 30));

	gui55.add((new ofxGuiSpacer(5, 0, 0)));
	gui55.add(stopMoving.setup("           Stop moving", 0, 30));

	gui55.add((new ofxGuiSpacer(5, 0, 0)));
	gui55.add(ledStatus.setup("LED on/off", false));
	gui55.add((new ofxGuiSpacer(5, 0, 0)));

	move.addListener(this, &ofApp::movePressed);
	stopMoving.addListener(this, &ofApp::stopMovingPressed);
	angleSlider.addListener(this, &ofApp::goalPositionSliderChanged);
	ledStatus.addListener(this, &ofApp::ledStatusPressed);

	//angleSlider.addListener(this, &ofApp::goalPositionSliderChanged);
}

void ofApp::movePressed() {

	cout << "\nStart Move acceleraztion : " << endl;
	cout << "vitesse value: " << (string)movingSpeed << endl;
	cout << "position value: " << (string)goalPositionText << endl;
	cout << "End Move acceleraztion \n" << endl;

	switch (std::stoi(dynID))
	{
	case 1: dynamixels[0]->move(std::stoi(movingSpeed), std::stoi(goalPositionText));
		dynamixels[0]->show();  cout << "1"; break;
	case 2: dynamixels[1]->move(std::stoi(movingSpeed), std::stoi(goalPositionText));
		dynamixels[1]->show(); cout << "2"; break;
	case 3: dynamixels[2]->move(std::stoi(movingSpeed), std::stoi(goalPositionText));
		dynamixels[2]->show(); cout << "3"; break;
	case 4: dynamixels[3]->move(std::stoi(movingSpeed), std::stoi(goalPositionText));
		dynamixels[3]->show(); cout << "4"; break;
	default:
		break;
	}
}

void ofApp::stopMovingPressed() {
	cout << "\nStop dynamixel from moving : " << endl;

	switch (std::stoi(dynID))
	{
	case 1: dynamixels[0]->move(dynamixels[0]->getCurrentPosition(), 50);
		dynamixels[0]->show();  cout << "1"; break;
	case 2: dynamixels[1]->move(dynamixels[1]->getCurrentPosition(), 50);
		dynamixels[1]->show(); cout << "2"; break;
	case 3: dynamixels[2]->move(dynamixels[2]->getCurrentPosition(), 50);
		dynamixels[2]->show(); cout << "3"; break;
	case 4: dynamixels[3]->move(dynamixels[3]->getCurrentPosition(), 50);
		dynamixels[3]->show(); cout << "4"; break;
	default:
		break;
	}
}

void ofApp::centralPositionPressed() {
	int movingSpeed = 100;
	cout << "Central position";
	switch (std::stoi(dynID))
	{
	case 1:
		movingSpeed = dynamixels[0]->getDynamixel()->getControlTable()->movingSpeed();
		dynamixels[0]->getDynamixel()->getControlTable()->setMovingSpeed(100);
		dynamixels[0]->getDynamixel()->getControlTable()->setGoalPosition(512);
		dynamixels[0]->getDynamixel()->getControlTable()->setMovingSpeed(movingSpeed);
		break;
	case 2:
		movingSpeed = dynamixels[1]->getDynamixel()->getControlTable()->movingSpeed();
		dynamixels[1]->getDynamixel()->getControlTable()->setMovingSpeed(100);
		cout << "Before Speed " << dynamixels[1]->getDynamixel()->getControlTable()->movingSpeed();
		dynamixels[1]->getDynamixel()->getControlTable()->setGoalPosition(512);
		dynamixels[1]->getDynamixel()->getControlTable()->setMovingSpeed(movingSpeed);
		cout << "After Speed " << dynamixels[1]->getDynamixel()->getControlTable()->movingSpeed();
		break;
	case 3:
		movingSpeed = dynamixels[2]->getDynamixel()->getControlTable()->movingSpeed();
		dynamixels[2]->getDynamixel()->getControlTable()->setMovingSpeed(100);
		cout << "Before Speed " << dynamixels[1]->getDynamixel()->getControlTable()->movingSpeed();
		dynamixels[2]->getDynamixel()->getControlTable()->setGoalPosition(512);
		Sleep(1000);
		/*dynamixels[2]->getDynamixel()->getControlTable()->setMovingSpeed(movingSpeed); */
		cout << "After Speed " << dynamixels[1]->getDynamixel()->getControlTable()->movingSpeed();
		break;
	case 4:
		movingSpeed = dynamixels[3]->getDynamixel()->getControlTable()->movingSpeed();
		dynamixels[3]->getDynamixel()->getControlTable()->setMovingSpeed(100);
		dynamixels[3]->getDynamixel()->getControlTable()->setGoalPosition(512);
		dynamixels[3]->getDynamixel()->getControlTable()->setMovingSpeed(movingSpeed); break;
	default:
		break;
	}
}

void ofApp::goalPositionSliderChanged(float &position) {
	cout << " Angle: " << position << endl;
	float new_position = position;

	switch (std::stoi(dynID))
	{
	case 1: dynamixels[0]->change(new_position * 3.413);
		/*dynamixels[0]->show()*/;  cout << "1"; break;
	case 2: dynamixels[1]->change(new_position * 3.413);
		/*dynamixels[1]->show()*/; cout << "2"; break;
	case 3: dynamixels[2]->change(new_position * 3.413);
		/*dynamixels[2]->show()*/; cout << "3"; break;
	case 4: dynamixels[3]->change(new_position * 3.413);
		/*dynamixels[3]->show()*/; cout << "4"; break;
	default:
		break;
	}

}

void ofApp::ledStatusPressed(bool & val) {
	cout << "LED\n" << led;
	if (cpt < 1) {
		switch (id)
		{
		case 1: setled(0); break;
		case 2: setled(1); break;
		case 3: setled(2); break;
		case 4: setled(3); break;
		default:
			break;
		}
		cpt++;
	}
	else {
		cpt = 0;
	}
}


void ofApp::setGui22() {
	gui22.setup();
	gui22.setShowHeader(false);
	gui22.setPosition(350, 340);
	titleGui22.setDefaultHeight(30);
	gui22.add(titleGui22.setup("titleGui22", "         Motor Configuration"));
	titleGui22.setShowLabelName(false);
	titleGui22.setBackgroundColor(ofColor::blueSteel);
	titleGui22.setDefaultHeight(20);
	gui22.setBorderColor(ofColor::black);
	//gui22.setDefaultTextPadding(5);

	gui22.add((new ofxGuiSpacer(5, 0, 0)));
	gui22.add(dynamixelID.setup("Dyamixel ID (1 - 4):", ""));
	dynamixelID.setBackgroundColor(ofColor(60, 60, 60));

	gui22.add((new ofxGuiSpacer(5, 0, 0)));
	gui22.add(maximumTorque.setup("Maximum torque (0 - 1023):  ", ""));
	maximumTorque.setBackgroundColor(ofColor(60, 60, 60));

	gui22.add((new ofxGuiSpacer(5, 0, 0)));
	gui22.add(angleLimit.setup("", "Angle limit (0, 300)"));
	angleLimit.setBackgroundColor(ofColor::darkSlateGray);
	angleLimit.setShowLabelName(false);

	gui22.add((new ofxGuiSpacer(5, 0, 0)));
	gui22.add(angleMin.setup("Angle minimum:  ", ""));
	angleMin.setBackgroundColor(ofColor(60, 60, 60));

	gui22.add((new ofxGuiSpacer(5, 0, 0)));
	gui22.add(angleMax.setup("Angle maximum:  ", ""));
	angleMax.setBackgroundColor(ofColor(60, 60, 60));

	gui22.add((new ofxGuiSpacer(5, 0, 0)));
	gui22.add(apply.setup("                Apply", 0, 30));
	gui22.add((new ofxGuiSpacer(5, 0, 0)));

	apply.addListener(this, &ofApp::applyPressed);
}

void ofApp::applyPressed() {
	cout << std::stoi(maximumTorque) << endl;
	cout << std::stoi(dynamixelID) << endl;
	//cout << "Données invalide\n" << endl;
	//cout << "Données invalide\n" << endl;
	if (std::stoi(maximumTorque) < 0 || std::stoi(maximumTorque) > 1023
		|| std::stoi(angleMin) < 0 || std::stoi(angleMin) > 300
		|| std::stoi(angleMax) < 0 || std::stoi(angleMax) > 300
		|| std::stoi(dynamixelID) < 1 || std::stoi(dynamixelID) > 4)
	{
		//error->setLabel("Donnée invalide !");
		cout << "Données invalide\n";
	}
	else {
		cout << "Données valide!\n";
		switch (std::stoi(dynamixelID))
		{
		case 1:
			//dynamixels[0]->getDynamixel()->getControlTable()->setReturnDelayTime(std::stoi(delai->getTextString()));
			dynamixels[0]->getDynamixel()->getControlTable()->setTorqueLimit(std::stoi(maximumTorque));
			dynamixels[0]->getDynamixel()->getControlTable()->setLowestByteOfMaxTorque(std::stoi(maximumTorque));
			dynamixels[0]->getDynamixel()->getControlTable()->setCCWAngleLimit(std::stoi(angleMax));
			dynamixels[0]->getDynamixel()->getControlTable()->setCWAngleLimit(std::stoi(angleMax));
			dynamixels[0]->getDynamixel()->setPositionMinimum(std::stoi(angleMin) * 3.413);
			dynamixels[0]->getDynamixel()->setPositionMaximum(std::stoi(angleMin) * 3.413);

			break;
		case 2:
			//dynamixels[1]->getDynamixel()->getControlTable()->setReturnDelayTime(std::stoi(delai->getTextString()));
			dynamixels[1]->getDynamixel()->getControlTable()->setTorqueLimit(std::stoi(maximumTorque));
			dynamixels[1]->getDynamixel()->getControlTable()->setLowestByteOfMaxTorque(std::stoi(maximumTorque));
			dynamixels[1]->getDynamixel()->getControlTable()->setCCWAngleLimit(std::stoi(angleMax));
			dynamixels[1]->getDynamixel()->getControlTable()->setCWAngleLimit(std::stoi(angleMax));
			dynamixels[1]->getDynamixel()->setPositionMinimum(std::stoi(angleMin) * 3.413);
			dynamixels[1]->getDynamixel()->setPositionMaximum(std::stoi(angleMax) * 3.413);

			break;
		case 3:
			//dynamixels[2]->getDynamixel()->getControlTable()->setReturnDelayTime(std::stoi(delai->getTextString()));
			dynamixels[2]->getDynamixel()->getControlTable()->setTorqueLimit(std::stoi(maximumTorque));
			dynamixels[2]->getDynamixel()->getControlTable()->setLowestByteOfMaxTorque(std::stoi(maximumTorque));
			dynamixels[2]->getDynamixel()->getControlTable()->setCCWAngleLimit(std::stoi(angleMax));
			dynamixels[2]->getDynamixel()->getControlTable()->setCWAngleLimit(std::stoi(angleMax));
			dynamixels[2]->getDynamixel()->setPositionMinimum(std::stoi(angleMin) * 3.413);
			dynamixels[2]->getDynamixel()->setPositionMaximum(std::stoi(angleMax) * 3.413);

			break;
		case 4:
			dynamixels[3]->getDynamixel()->getControlTable()->setTorqueLimit(std::stoi(maximumTorque));
			dynamixels[3]->getDynamixel()->getControlTable()->setLowestByteOfMaxTorque(std::stoi(maximumTorque));
			dynamixels[3]->getDynamixel()->getControlTable()->setCCWAngleLimit(std::stoi(angleMax));
			dynamixels[3]->getDynamixel()->getControlTable()->setCWAngleLimit(std::stoi(angleMax));
			dynamixels[3]->getDynamixel()->setPositionMinimum(std::stoi(angleMin) * 3.413);
			dynamixels[3]->getDynamixel()->setPositionMaximum(std::stoi(angleMax) * 3.413);

			break;
		default:
			break;
		}
	}
}


void ofApp::setGuiHH() {
	guiHH.setup();
	guiHH.setShowHeader(false);
	guiHH.setPosition(10, 250);
	titleGuiHH.setDefaultHeight(30);
	guiHH.add(titleGuiHH.setup("titleGuiHH", "             Control"));
	titleGuiHH.setShowLabelName(false);
	titleGuiHH.setBackgroundColor(ofColor::blueSteel);
	titleGuiHH.setDefaultHeight(20);
	guiHH.setBorderColor(ofColor::black);

	/*guiHH.add((new ofxGuiSpacer(5, 0, 0)));
	guiHH.setDefaultWidth(400);
	positionTypeMatrixParameters.add(typeMove.set("move", false));
	positionTypeMatrixParameters.add(typeWakeUp.set("wakeUp", false));
	positionTypeMatrixParameters.add(typeFear.set("fear", false));
	positionTypeMatrixParameters.add(typeHappy.set("happy", false));
	positionTypeMatrixParameters.add(typeSurprise.set("surprise", false));

	matrixPositionType.setup(positionTypeMatrixParameters);
	matrixPositionType.setShowHeader(false);
	matrixPositionType.setAlignHorizontal();
	matrixPositionType.allowMultipleActiveToggles(false);
	guiHH.add(&matrixPositionType);*/

	/*
	* matrix with only one allowed active toggle
	*/
	guiHH.add((new ofxGuiSpacer(5, 0, 0)));
	matrix_params.push_back(ofParameter<bool>("move", false));
	matrix_params.push_back(ofParameter<bool>("wakeUp", false));
	matrix_params.push_back(ofParameter<bool>("sleep", false));
	matrix_params.push_back(ofParameter<bool>("happy", false));
	matrix_params.push_back(ofParameter<bool>("surprise", false));
	//matrix_params.push_back(ofParameter<bool>("active", false));
	matrix.setup("matrix", 2);
	for (unsigned int i = 0; i < matrix_params.size(); i++) {
		matrix.add(new ofxMinimalToggle(matrix_params.at(i)));
	}
	matrix.setBorderColor(ofColor::blueSteel);
	matrix.setElementHeight(20);
	matrix.allowMultipleActiveToggles(false);
	matrix.setShowHeader(false);
	guiHH.add(&matrix);




	guiHH.add((new ofxGuiSpacer(5, 0, 0)));
	guiHH.add(savePosition.setup("            Save position", 0, 30));

	guiHH.add((new ofxGuiSpacer(5, 0, 0)));
	guiHH.add(run.setup("                Run", 0, 30));

	guiHH.add((new ofxGuiSpacer(5, 0, 0)));
	guiHH.add(stop.setup("                Stop", 0, 30));

	guiHH.add((new ofxGuiSpacer(5, 0, 0)));
	guiHH.add(takeFirstPositionH.setup("     Move to middle position", 0, 30));
	guiHH.add((new ofxGuiSpacer(5, 0, 0)));


	savePosition.addListener(this, &ofApp::savePositionPressed);
	run.addListener(this, &ofApp::runPressed);
	stop.addListener(this, &ofApp::stopPressed);
	takeFirstPositionH.addListener(this, &ofApp::takeMidlePositionPressedH);

}
void ofApp::savePositionPressed() {
	if (!robotMoving) {
		string positionType = "";
		/*if (positionTypeMatrixParameters.getBool("move")) {
		positionType = "move";
		}

		else if (positionTypeMatrixParameters.getBool("wakeUp")) {
		positionType = "wakeUp";
		}

		else if (positionTypeMatrixParameters.getBool("fear")) {
		positionType = "fear";
		}

		else if (positionTypeMatrixParameters.getBool("happy")) {
		positionType = "happy";
		}

		else if (positionTypeMatrixParameters.getBool("suprise")) {
		positionType = "surprise";
		}*/

		if (matrix_params.at(0).get()) {
			positionType = "move";
			cout << "\nmove" << endl;
		}
		else if (matrix_params.at(1).get()) {
			positionType = "wakeUp";
			cout << "\nwakeUp" << endl;
		}
		else if (matrix_params.at(2).get()) {
			positionType = "sleep";
			cout << "\nsleep" << endl;
		}
		else if (matrix_params.at(3).get()) {
			positionType = "happy";
			cout << "\nhappy" << endl;
		}

		else if (matrix_params.at(4).get()) {
			positionType = "surprise";
			cout << "\nsurprise" << endl;
		}

		cout << "\n*************************" << endl;
		cout << "* Saving a new position *" << endl;
		cout << "*************************" << endl;
		std::vector<int> dynamixelsPosition;
		for (int i = 0; i < 4; i++)
		{
			dynamixelsPosition.push_back(dynamixels[i]->getCurrentPosition());
			//dynamixelsPosition.push_back(rand());
		}
		int i = 0;
		int numDragTags = myXml.getNumTags("positionForm");
		int numTag = myXml.addTag("positionForm");
		myXml.setAttribute("positionForm", "id", numTag, numTag);
		myXml.setAttribute("positionForm", "type", positionType, numTag);
		myXml.pushTag("positionForm", numTag);
		for (vector<int>::iterator it1 = dynamixelsPosition.begin(); it1 != dynamixelsPosition.end(); ++it1) {
			int numtTagDyn = myXml.addTag("dynamixel");
			myXml.addAttribute("dynamixel", "id", i, numtTagDyn);
			myXml.addAttribute("dynamixel", "position", *it1, numtTagDyn);

			i++;
		}
		myXml.popTag();//pop position
		myXml.saveFile("myXml.xml");
		//positionsVectorMove.push_back(dynamixelsPosition);

		if (matrix_params.at(0).get()) {
			positionsVectorMove.push_back(dynamixelsPosition);
			moveThread.setup(positionsVectorMove);
			int dyn;
			for (vector< vector<int> >::iterator it = positionsVectorMove.begin(); it != positionsVectorMove.end(); ++it) {
				cout << "\n********************** " << endl;
				dyn = 0;
				for (vector<int>::iterator it1 = it->begin(); it1 != it->end(); ++it1) {

					printf("\nposition of dynamixels[%d] = %d\n", dyn++, *it1);
				}
			}
		}
		else if (matrix_params.at(1).get()) {
			positionsVectorWakeUp.push_back(dynamixelsPosition);
			int dyn;
			for (vector< vector<int> >::iterator it = positionsVectorWakeUp.begin(); it != positionsVectorWakeUp.end(); ++it) {
				cout << "\n********************** " << endl;
				dyn = 0;
				for (vector<int>::iterator it1 = it->begin(); it1 != it->end(); ++it1) {

					printf("\nposition of dynamixels[%d] = %d\n", dyn++, *it1);
				}
			}
		}
		else if (matrix_params.at(2).get()) {
			positionsVectorSleep.push_back(dynamixelsPosition);
			int dyn;
			for (vector< vector<int> >::iterator it = positionsVectorSleep.begin(); it != positionsVectorSleep.end(); ++it) {
				cout << "\n********************** " << endl;
				dyn = 0;
				for (vector<int>::iterator it1 = it->begin(); it1 != it->end(); ++it1) {

					printf("\nposition of dynamixels[%d] = %d\n", dyn++, *it1);
				}
			}
		}
		else if (matrix_params.at(3).get()) {
			positionsVectorHappy.push_back(dynamixelsPosition);
			int dyn;
			for (vector< vector<int> >::iterator it = positionsVectorHappy.begin(); it != positionsVectorHappy.end(); ++it) {
				cout << "\n********************** " << endl;
				dyn = 0;
				for (vector<int>::iterator it1 = it->begin(); it1 != it->end(); ++it1) {

					printf("\nposition of dynamixels[%d] = %d\n", dyn++, *it1);
				}
			}
		}

		else if (matrix_params.at(4).get()) {
			positionsVectorSurprise.push_back(dynamixelsPosition);
			int dyn;
			for (vector< vector<int> >::iterator it = positionsVectorSurprise.begin(); it != positionsVectorSurprise.end(); ++it) {
				cout << "\n********************** " << endl;
				dyn = 0;
				for (vector<int>::iterator it1 = it->begin(); it1 != it->end(); ++it1) {

					printf("\nposition of dynamixels[%d] = %d\n", dyn++, *it1);
				}
			}
		}



	}

	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	//cout << "\n*************************" << endl;
	//cout << "* Saving a new position *" << endl;
	//cout << "*************************" << endl;
	//std::vector<int> dynamixelsPosition;
	//for (int i = 0; i < 4; i++)
	//{
	//	dynamixelsPosition.push_back(dynamixels[i]->getCurrentPosition());
	//	//dynamixelsPosition.push_back(500);
	//}
	//positionsVectorMove.push_back(dynamixelsPosition);
	//int dyn;
	//for (vector< vector<int> >::iterator it = positionsVectorMove.begin(); it != positionsVectorMove.end(); ++it) {
	//	cout << "\n********************** " << endl;
	//	dyn = 0;
	//	for (vector<int>::iterator it1 = it->begin(); it1 != it->end(); ++it1) {
	//		printf("\nposition of dynamixels[%d] = %d\n", dyn++, *it1);
	//	}
	//}
}

void ofApp::runPressed() {
	//if (positionsVectorMove.size() > 0) {
	//	/*
	//	int movingSpeed = 50;
	//	int randomPosition;
	//	int servoMoteurID;
	//	while (robotMoving) {
	//	Sleep(10000);
	//	randomPosition = rand() % ((postionsVector.size()-1) - 0 + 1) + 0;
	//	cout << "randomPosition = " << randomPosition << endl;
	//	servoMoteurID = 0;
	//	for (vector<int>::iterator it = postionsVector[randomPosition].begin(); it != postionsVector[randomPosition].end(); ++it) {
	//	dynamixels[servoMoteurID]->getDynamixel()->move(*it, movingSpeed);
	//	Sleep(500);
	//	servoMoteurID++;
	//	}
	//	}*/
	//	cout << positionsVectorMove.size() << endl;
	//	for (auto& position : positionsVectorMove)
	//	{
	//		Sleep(3000);

	//		for (std::vector<int>::size_type i = 0; i != position.size(); i++)
	//		{
	//			dynamixels[i]->getDynamixel()->move(position[i], 50);
	//			Sleep(500);
	//		}
	//	}
	//}
	robotMoving = true;
	moveThread.robotIsMoving(robotMoving);
	if (!moveThread.isThreadRunning()) {
		moveThread.start();
	}
	cout << "\nRobot is Running " << endl;
}

void ofApp::stopPressed() {
	robotMoving = false;
	moveThread.robotIsMoving(robotMoving);
	if (moveThread.isThreadRunning()) {
		moveThread.stop();
	}

	cout << "\nRobot is stopped " << endl;
}

void ofApp::takeMidlePositionPressedH() {
	//for (int servomoteurID = 1; servomoteurID < 4; servomoteurID++) {
	dynamixels[1]->getDynamixel()->move(610, 50);
	printf("\nmoving dynamixels[1] to 512\n");
	//Sleep(3000);

	dynamixels[2]->getDynamixel()->move(610, 50);
	printf("\nmoving dynamixels[2] to 512\n");
	//Sleep(3000);

	dynamixels[3]->getDynamixel()->move(556, 50);
	printf("\nmoving dynamixels[3] to 512\n");
	//Sleep(3000);

	dynamixels[0]->getDynamixel()->move(350, 50);
	printf("\nmoving dynamixels[1] to 512\n");
	//Sleep(2000);

	//}
}


void ofApp::connect(const char * portName, float protocolVersion, int baudrate, int max) {
	//printf("ofConnexion * portConnexion = new ofConnexion(COM4, 1.0, 57142);\n");

	if (portConnexion == NULL) {
		portConnexion = new ofConnexion(portName, protocolVersion, baudrate);
		//id_dynamixel =  int[256];
		/*
		Probelem avec la creation du port, ajout un contructeur avec l'd et le portConnexion directement
		la port sera créer en dehors de la class pour plus de comprenhobilité

		*/
		bool succesConnexion = portConnexion->open();
		cout << "succesConnexion = " << succesConnexion << endl;
		//succesConnexion = portConnexion->open();
		printf("bool succesConnexion = portConnexion->open();\n");
		if (succesConnexion)
		{
			//errorScan->setLabel("Succeeded to open the port!");
			printf("Succeeded to open the port!\n");

		}
		else
		{
			//errorScan->setLabel("Failed to open the port!");
			printf("Failed to open the port!\n");
		}

		succesConnexion = portConnexion->setBaudRate(57142);
		printf("succesConnexion = portConnexion->setBaudRate(57142);\n");
		if (succesConnexion)
		{
			printf("Succeeded to change the baudrate!\n");
		}
		else
		{
			printf("Failed to change the baudrate!\n");
		}

		scanDynamixel(max);

		int s = 0;
		vector<int>::iterator v = id_dynamixel.begin();
		while (v != id_dynamixel.end()) {

			dynamixels[s++] = new ofDynamixelServo(*v, portConnexion, 0, 1023);
			v++;
			taille++;
		}

		for (int i = 0; i < taille; i++) {
			cout << dynamixels[i]->getDynamixel()->getControlTable()->modelNumber() << "\n";

		}
		dynamixels[0]->getDynamixel()->setPositionMinimum(0);
		dynamixels[0]->getDynamixel()->setPositionMaximum(1023);
		dynamixels[0]->getDynamixel()->getControlTable()->setMovingSpeed(40);

		dynamixels[1]->getDynamixel()->setPositionMinimum(25);
		dynamixels[1]->getDynamixel()->setPositionMaximum(760);
		dynamixels[1]->getDynamixel()->getControlTable()->setMovingSpeed(40);

		dynamixels[2]->getDynamixel()->setPositionMinimum(170);
		dynamixels[2]->getDynamixel()->setPositionMaximum(845);
		dynamixels[2]->getDynamixel()->getControlTable()->setMovingSpeed(40);

		dynamixels[3]->getDynamixel()->setPositionMinimum(10);
		dynamixels[3]->getDynamixel()->setPositionMaximum(760);
		dynamixels[3]->getDynamixel()->getControlTable()->setMovingSpeed(40);

		//dyna_1 = new ofDynamixelServo(1, portConnexion, 0, 1023);
		//dyna_2 = new ofDynamixelServo(2, portConnexion, 25, 760);
		//dyna_3 = new ofDynamixelServo(3, portConnexion, 170, 845);
		//dyna_4 = new ofDynamixelServo(4, portConnexion, 10, 760);
		//cout << "taille : " << taille; 
		cout << "\n";
		draw();

	}

}

void ofApp::setGui33() {
	gui33.setup();
	gui33.setShowHeader(false);
	gui33.setPosition(350, 180);
	titleGui33.setDefaultHeight(30);
	gui33.add(titleGui33.setup("titleGui33", "         Dynamixel detected"));
	titleGui33.setShowLabelName(false);
	titleGui33.setBackgroundColor(ofColor::blueSteel);
	titleGui33.setDefaultHeight(20);
	gui33.setBorderColor(ofColor::black);

	gui33.add((new ofxGuiSpacer(5, 0, 0)));
	stringstream ss0;
	ss0 << "Dynamixel ";
	ss0 << dynamixels[0]->getDynamixel()->getControlTable()->getID();
	ss0 << " [RX-";
	ss0 << dynamixels[0]->getDynamixel()->getControlTable()->modelNumber();;
	ss0 << "]";
	gui33.add(dynamixel0.setup("", ss0.str()));
	dynamixel0.setBackgroundColor(ofColor::darkSlateBlue);

	gui33.add((new ofxGuiSpacer(5, 0, 0)));
	stringstream ss1;
	ss1 << "Dynamixel ";
	ss1 << dynamixels[1]->getDynamixel()->getControlTable()->getID();
	ss1 << " [RX-";
	ss1 << dynamixels[1]->getDynamixel()->getControlTable()->modelNumber();;
	ss1 << "]";
	gui33.add(dynamixel1.setup("", ss1.str()));
	dynamixel1.setBackgroundColor(ofColor::darkSlateBlue);

	gui33.add((new ofxGuiSpacer(5, 0, 0)));
	stringstream ss2;
	ss2 << "Dynamixel ";
	ss2 << dynamixels[2]->getDynamixel()->getControlTable()->getID();
	ss2 << " [RX-";
	ss2 << dynamixels[2]->getDynamixel()->getControlTable()->modelNumber();;
	ss2 << "]";
	gui33.add(dynamixel2.setup("", ss2.str()));
	dynamixel2.setBackgroundColor(ofColor::darkSlateBlue);

	gui33.add((new ofxGuiSpacer(5, 0, 0)));
	stringstream ss3;
	ss3 << "Dynamixel ";
	ss3 << dynamixels[3]->getDynamixel()->getControlTable()->getID();
	ss3 << " [RX-";
	ss3 << dynamixels[3]->getDynamixel()->getControlTable()->modelNumber();;
	ss3 << "]";
	gui33.add(dynamixel3.setup("", ss3.str()));
	dynamixel3.setBackgroundColor(ofColor::darkSlateBlue);
	gui33.add((new ofxGuiSpacer(5, 0, 0)));
}

void ofApp::exit() {
	threadedObject.stopThread();
	threadedObject.waitForThread();
	moveThread.stop();
	moveThread.waitForThread();
}
#pragma once

#include "ofMain.h"
#include "ofxGui/src/ofxGui.h"
#include "ofxDynamixel/src/ofConnexion.h"
#include "ofxDynamixel/src/ofControlTable.h"
#include "ofDynamixelServo.h"
#include "ofxCvHaarFinder.h"  
#include "ofxInputField.h"
#include "videoThreadedObject.h"
#include "ofxGuiExtended.h"
#include "MoveThread.h"
#include "faceTrackerThread.h"
#include "ofxFaceTrackerMulti.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void connect(const char * portName, float protocolVersion, int baudrate, int max);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();



private:
	ofDynamixelServo * dyna_1;
	ofDynamixelServo * dyna_2;
	ofDynamixelServo * dyna_3;
	ofDynamixelServo * dyna_4;

	ofDynamixelServo * dynamixels[255];
	int taille = 0;

	void setled(int id);


	//ofxUILabel * error;
	//ofxUILabel * errorScan;

	int id;
	int led = 1;
	int cpt = 0;
	ofConnexion * portConnexion;

	std::vector<int> id_dynamixel;
	void scanDynamixel(int max);


	vector< vector<int> > positionsVectorMove;
	vector< vector<int> > positionsVectorFear;
	vector< vector<int> > positionsVectorHappy;
	vector< vector<int> > positionsVectorWakeUp;
	vector< vector<int> > positionsVectorSurprise;
	vector< vector<int> > positionsVectorSleep;

	bool robotMoving = true;



	ofVideoGrabber vidGrabber;
	ofPixels videoInverted;
	//ofTexture videoTexture;
	//int camWidth;
	//int camHeight;

	ofImage grayImage, colorImg;
	ofxCvHaarFinder finder;


	ofColor panelHeaderColor;
	ofColor buttonColor;

	ofxPanelExtended gui11;
	ofxLabelExtended titleGui11;
	ofxTextField USB2Dynamixel_Ports;
	ofxTextField baudrates;
	ofxTextField maximumIDs;
	ofxMinimalButton scan;
	void setGui11();
	void scanPressed();


	ofxPanelExtended gui22;
	ofxLabelExtended titleGui22;
	ofxTextField dynamixelID;
	ofxTextField maximumTorque;
	ofxLabelExtended angleLimit;
	ofxTextField angleMin;
	ofxTextField angleMax;
	ofxMinimalButton apply;
	void setGui22();
	void applyPressed();



	ofxPanelExtended gui33;
	ofxLabelExtended titleGui33;
	ofxLabelExtended dynamixel0;
	ofxLabelExtended dynamixel1;
	ofxLabelExtended dynamixel2;
	ofxLabelExtended dynamixel3;
	void setGui33();



	ofxPanelExtended gui55;
	ofxLabelExtended titleGui55;

	ofxLabelExtended description;
	ofxLabelExtended description1;
	ofxLabelExtended description2;

	ofxTextField dynID;
	ofxFloatSlider goalPositionSlider;
	ofxFloatRotarySlider angleSlider;
	ofParameter<float> slider_param;
	ofxGuiGroupExtended rotarySlider;
	ofxMinimalButton centralPosition;
	ofxTextField goalPositionText;
	ofxTextField movingSpeed;
	ofxMinimalButton move;
	ofxMinimalButton stopMoving;
	ofxToggle ledStatus;
	void setGui55();
	void movePressed();
	void centralPositionPressed();
	void goalPositionSliderChanged(float &position);
	void ledStatusPressed(bool & val);
	void stopMovingPressed();


	ofxPanelExtended guiHH;
	ofxLabelExtended titleGuiHH;
	ofxMinimalButton savePosition;
	ofxMinimalButton run;
	ofxMinimalButton stop;
	ofxMinimalButton takeFirstPositionH;

	ofxGuiMatrix matrix;
	vector<ofParameter<bool>> matrix_params;

	ofParameterGroup positionTypeMatrixParameters;
	ofParameter<bool> typeMove;
	ofParameter<bool> typeFear;
	ofParameter<bool> typeWakeUp;
	ofParameter<bool> typeHappy;
	ofParameter<bool> typeSurprise;
	ofxGuiGroupExtended matrixPositionType;
	ofxXmlSettings myXml;
	string message;
	void setGuiHH();
	void savePositionPressed();
	void runPressed();
	void stopPressed();
	void takeMidlePositionPressedH();

	ofTrueTypeFont myFont;

	ofxButton TakeFirstPostion;
	void takeFirstPositionPressed();

	ofImage img;
	ofRectangle curApp;
	MoveThread moveThread;
	float time;
	float expressionTime;
	bool moveToReactionPositionHappy, moveToReactionPositionSurprise, returnToInitialPosition;
	int smileExpressionCount = 0;
	int surprisedExpressionCount = 0;

	std::vector<int> dynamixelsPositionBeforeExpression;

	//----------------------------------------------------------
	int 				camWidth;
	int 				camHeight;

	vector<int> temp_validIDs;


	videoThreadedObject threadedObject;
	ofTexture videoTexture;
	ofImage testImage;

	cv::Rect biggestRectApp;
	bool moveX, moveY;

	faceTrackerThread trackerThread;
	ofxFaceTracker tracker;
	ExpressionClassifier classifier;
	ofxFaceTrackerMulti trackerMulti;
	int tiltedFaceFrameNumber = 0;
	bool tiltedFaceDetected = false;

	unsigned char * pix;
	unsigned int brightness = 0;
	int brightnessFrameNumber = 0;
	bool isSleeping = false;
	bool faceDetected = false;


	int faceToTrack = 0;

	bool test = false;
	//----------------------------------------------------------
};

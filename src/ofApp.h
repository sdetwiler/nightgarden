#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#include "ofxLSystemNode.h"
#include "ofxGui.h"
#include <vector>

typedef std::vector<ofxLSystemNode*> SystemVec;

class ofApp : public ofBaseApp 
{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	
	void fileButtonPressed();
	
private:
	
	ofxLSystemNode* loadSystem(char const* filename);
	
//	ofxLSystem mSystem;
	SystemVec mSystems;
	
	ofEasyCam mCamera;
	ofLight mSpotlight;
	
	ofxPanel mGui;
	
	ofxLabel mNLabel;
	ofxLabel mDeltaLabel;
	ofxLabel mMaxStepsLabel;
	ofxLabel mCurrStepsLabel;
	ofxLabel mSystemLenLabel;
	ofxLabel mLastDurationlabel;
	
	ofxButton mFileButton;
	ofxToggle mAxisButton;
	ofxToggle mEditButton;
	
	ofParameter<bool> mShowAxis;
	ofParameter<bool> mEdit;
	
	std::string mFilename;
	
	

};

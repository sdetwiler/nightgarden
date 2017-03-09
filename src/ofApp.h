#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#include "ofxLSystemNode.h"
#include "ofxGui.h"
#include "ofxBullet.h"

#include <vector>

typedef std::vector<ofxLSystemNode*> SystemVec;

class ofApp : public ofBaseApp 
{

public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void clear();


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
	void compiledFileButtonPressed();
	void compileButtonPressed();
	void clearButtonPressed();
	
private:
    
    void loadDebugIfExists();
	
	ofxLSystemNode* loadSystem(char const* filename, bool isCompiled);
	
	SystemVec mSystems;
	ofxBulletWorldRigid mWorld;
	ofxBulletBox mGround;

	
	ofEasyCam mCamera;
	ofLight mSpotlight;
	
	ofxPanel mGui;
	
	ofxButton mFileButton;
	ofxButton mCompiledFileButton;

	ofxButton mCompileButton;

	ofxButton mClearButton;

	ofxToggle mAxisButton;
	ofxToggle mEditButton;

	ofxToggle mPhysicsButton;

	
	ofParameter<bool> mShowAxis;
	ofParameter<bool> mEdit;
	ofParameter<bool> mPhysics;
	
	std::string mFilename;
	bool mCompiled;
	
	
	bool mMouseDragged;

};

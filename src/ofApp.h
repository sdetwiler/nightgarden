#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#include <vector>

typedef std::vector<ofMesh*> MeshVec;

class ofApp : public ofBaseApp{

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

private:
	void clearMeshes();
	void buildMeshes();

	void drawMeshes();
	void drawDebugHUD();

	
	void drawContours();

	int mMaxSteps;
	int mCurrSteps;
	float mLastStepTime;
	float mStepInterval;
	
	
	MeshVec mMeshes;

	
	float mLastStepDuration;
	
	
	ofEasyCam mCamera;

	bool mDrawWireframe;
	
	
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImage, grayBg, grayDiff;
	ofxCvContourFinder contourFinder;
	bool mLearnBackground;
	
	static int ImageWidth;
	static int ImageHeight;
};

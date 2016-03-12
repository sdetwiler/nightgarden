#include "ofApp.h"
#include "lsystem.h"
#include "types.h"

//--------------------------------------------------------------
void ofApp::setup()
{
//	char const* filename = "/Users/steve/projects/nightgarden/data/test.ls";
//	loadSystem(filename);

	mCamera.setPosition(0, 0, -10);
	mCamera.setTarget(ofVec3f(0,0,0));
	mCamera.enableMouseInput();
	
	mGui.setup();
	
	mGui.add(mNLabel.setup("n", ""));
	mGui.add(mDeltaLabel.setup("delta", ""));
	mGui.add(mMaxStepsLabel.setup("steps", ""));
	mGui.add(mCurrStepsLabel.setup("currSteps", ""));
	mGui.add(mSystemLenLabel.setup("system len", ""));
	mGui.add(mLastDurationlabel.setup("step duration", ""));
	
	mGui.add(mFileButton.setup("File..."));
	
	mFileButton.addListener(this, &ofApp::fileButtonPressed);
}

//--------------------------------------------------------------
void ofApp::update()
{
	mSystem.update();
	
	float n = mSystem.getLSystem().getGlobalVariable("n", 5);
	mNLabel = std::to_string(n);

	float delta = mSystem.getLSystem().getGlobalVariable("delta", 22.5);
	mDeltaLabel = std::to_string(delta);

	mMaxStepsLabel = std::to_string(mSystem.getMaxSteps());
	mCurrStepsLabel = std::to_string(mSystem.getCurrSteps());
	mSystemLenLabel = to_string(mSystem.getLSystem().getState()?mSystem.getLSystem().getState()->symbols.size():0);
	mLastDurationlabel = to_string(mSystem.getLastStepDuration()*1000) + " ms";

}


//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(0,0,0);
	ofEnableDepthTest();
	mCamera.begin();
	mSystem.draw();
	mCamera.end();
	ofDisableDepthTest();
	
	mGui.draw();
}

//--------------------------------------------------------------
void ofApp::fileButtonPressed()
{
	ofFileDialogResult res = ofSystemLoadDialog();
	
	if(res.bSuccess)
	{
		loadSystem(res.getPath().c_str());
	}
}

//--------------------------------------------------------------
void ofApp::loadSystem(char const* filename)
{
	mSystem.load(filename);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key == 'w')
	{
		mSystem.setDrawWireframe(!mSystem.getDrawWireframe());
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

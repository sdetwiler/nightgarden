#include "ofApp.h"
#include "lsystem.h"
#include "types.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	char const* filename = "/Users/steve/projects/nightgarden/data/test.ls";
	mSystem.load(filename);

	mCamera.setPosition(0, 0, -10);
	mCamera.setTarget(ofVec3f(0,0,0));
	mCamera.enableMouseInput();
}

//--------------------------------------------------------------
void ofApp::update()
{
	mSystem.update();
}


void ofApp::drawDebugHUD()
{
	float delta = mSystem.getLSystem().getGlobalVariable("delta", 22.5);
	float n = mSystem.getLSystem().getGlobalVariable("n", 5);
	
	ofSetColor(0);
	std::string s;
	s+= ("n:             " + to_string(n) + "\n");
	s+= ("delta:         " + to_string(delta) + "\n");
	s+= ("steps:         " + to_string(mSystem.getMaxSteps()) + "\n");
	s+= ("currSteps:     " + to_string(mSystem.getCurrSteps()) + "\n");
	s+= ("system len:    " + to_string(mSystem.getLSystem().getState()->symbols.size()) + "\n");
	s+= ("Step duration: " + to_string(mSystem.getLastStepDuration()*1000) + " ms");
	ofDrawBitmapString(s.c_str(), 20,20);

}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofEnableDepthTest();
	mCamera.begin();
	mSystem.draw();
	mCamera.end();
	ofDisableDepthTest();
	
	drawDebugHUD();
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

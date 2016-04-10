#include "ofApp.h"
#include "types.h"

#include "of3dUtils.h"
#include "ofRay.h"

#include "ofxBulletLSystemNode.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	mCamera.setPosition(0, 0, -10);
	mCamera.setDistance(100);
	mCamera.setTarget(ofVec3f(0,0,0));
	mCamera.enableMouseInput();
	
//	mSpotlight.setSpotlight(200.0f);
	mSpotlight.setDirectional();
	mSpotlight.setup();
	
	mGui.setup();
	
	mGui.add(mFileButton.setup("File..."));
	mFileButton.addListener(this, &ofApp::fileButtonPressed);

	mGui.add(mCompiledFileButton.setup("Compiled File..."));
	mCompiledFileButton.addListener(this, &ofApp::compiledFileButtonPressed);

	mGui.add(mCompileButton.setup("Compile..."));
	mCompileButton.addListener(this, &ofApp::compileButtonPressed);

	mPhysics = false;
	mPhysics.setName("Physics");
	mGui.add(mPhysicsButton.setup(mPhysics));

	mShowAxis = false;
	mShowAxis.setName("Axis");
	mGui.add(mAxisButton.setup(mShowAxis));

	mEdit = false;
	mEdit.setName("Edit");
	mGui.add(mEditButton.setup(mEdit));

	mGui.add(mClearButton.setup("Clear"));
	mClearButton.addListener(this, &ofApp::clearButtonPressed);
	
	mWorld.setup();
	mWorld.enableGrabbing();
	mWorld.setCamera(&mCamera);
	mWorld.setGravity( ofVec3f(0, 25., 0) );
}

//--------------------------------------------------------------
void ofApp::exit()
{
	clear();
}

//--------------------------------------------------------------
void ofApp::clear()
{
	for(SystemVec::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
	{
		ofxLSystemNode* system = *i;
		delete system;
	}
	mSystems.clear();
	
	LSystem::clearCompiledCache();
}

//--------------------------------------------------------------
void ofApp::update()
{
	mSpotlight.setPosition(mCamera.getPosition());
	mSpotlight.lookAt(mCamera.getLookAtDir());

	mWorld.update();
	
	for(SystemVec::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
	{
		ofxLSystemNode* system = *i;
		
		system->update();
	}
}


//--------------------------------------------------------------
void ofApp::draw()
{
//	ofBackground(0,0,0);
	ofEnableDepthTest();

	mSpotlight.enable();
	mCamera.begin();
	
	mWorld.drawDebug();
	
	for(SystemVec::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
	{
		ofxLSystemNode* system = *i;
		
		system->draw();
	}
	
	if(mShowAxis)
	{
		ofDrawAxis(100);
		ofDrawGrid(100, 100, false, false, true, false);
//		ofDrawRotationAxes(10, 3);
	}

	mCamera.end();
	mSpotlight.disable();
	
	ofDisableDepthTest();
	ofDisableLighting();
	mGui.draw();
}

//--------------------------------------------------------------
void ofApp::clearButtonPressed()
{
	clear();
}

//--------------------------------------------------------------
void ofApp::compileButtonPressed()
{
	string inFilename;
	string outFilename;
	ofFileDialogResult res = ofSystemLoadDialog();
	
	if(res.bSuccess)
	{
		inFilename =res.getPath();
		outFilename = res.getName() + "c";
		res = ofSystemSaveDialog(outFilename, "Save As");
		
		if(res.bSuccess)
		{
			outFilename = res.getPath();
			LSystem system;
			if(system.load(inFilename.c_str()))
			{
				system.compile(outFilename.c_str());
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::fileButtonPressed()
{
	ofFileDialogResult res = ofSystemLoadDialog("Load system", false, "data/systems/");
	
	if(res.bSuccess)
	{
		mFilename =res.getPath();
		mCompiled = false;
//		loadSystem(res.getPath().c_str());
	}
}


//--------------------------------------------------------------
void ofApp::compiledFileButtonPressed()
{
	ofFileDialogResult res = ofSystemLoadDialog();
	
	if(res.bSuccess)
	{
		mFilename =res.getPath();
		mCompiled = true;
	}
}



//--------------------------------------------------------------
ofxLSystemNode* ofApp::loadSystem(char const* filename, bool isCompiled)
{
	ofxLSystemNode* system;
	if(mPhysics)
	{
		system = new ofxBulletLSystemNode(filename, isCompiled, &mWorld);
	}
	else
	{
		system = new ofxLSystemNode(filename, isCompiled);
	}
	
	mSystems.push_back(system);
	
	return system;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key == 'w')
	{
		for(SystemVec::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
		{
			ofxLSystemNode* system = *i;
			
			system->setDrawWireframe(system->getDrawWireframe());
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	mMouseDragged = true;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mMouseDragged = false;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	if(mEdit && !mMouseDragged)
	{
		ofVec3f screenToWorld = mCamera.screenToWorld(ofVec3f(x,y,0.0));
		ofRay ray(mCamera.getPosition(),screenToWorld - mCamera.getPosition());
		bool intersection = false;
		float t = 0;
		intersection = ray.calcPlaneIntersection(ofVec3f(0,0,0), ofVec3f(0,1,0), &t);
		if (intersection)
		{
			ofxLSystemNode* system = loadSystem(mFilename.c_str(), mCompiled);
			ofVec3f p = ray.calcPosition(t);
			system->setPosition(p.x, 0, p.z);
		}
	}
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

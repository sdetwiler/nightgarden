#include "ofApp.h"
#include "lsystem.h"
#include "types.h"

int parser_test();

//--------------------------------------------------------------
void ofApp::setup(){
	parser_test();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
	VariableMap const& globals = LSystem::getInstance().getGlobalVariables();

	float delta = 22.5;
	StringVariableMap::const_iterator i;

	i = globals.variables.find(std::string("delta"));
	if(i!=globals.variables.end())
	{
		delta = i->second->value;
	}
	

	int n = 5;
	i = globals.variables.find(std::string("n"));
	if(i!=globals.variables.end())
	{
		n = i->second->value;
	}

	int w = ofGetWidth();
	int h = ofGetHeight();
	
	// Match openframeworks rendering context.
	delta*=-1;
	ofTranslate(w/2, h);
	ofRotate(180);

	
	SymbolList const* state = LSystem::getInstance().getState();
	if(state)
	{
		for(SymbolVec::const_iterator i = state->symbols.begin(); i!=state->symbols.end(); ++i)
		{
			Symbol* s = *i;
			if(s->value == "F")
			{
				ofDrawLine(0,0,0,n);
				ofTranslate(0,n);
			}
			
			else if(s->value == "[")
			{
				ofPushMatrix();
			}
			
			else if(s->value == "]")
			{
				ofPopMatrix();
			}
			else if(s->value == "+")
			{
				ofRotate(delta);
			}
			else if(s->value == "-")
			{
				ofRotate(-delta);
			}
			
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
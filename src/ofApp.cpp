#include "ofApp.h"
#include "lsystem.h"
#include "types.h"

#include <stack>

typedef std::stack<ofMatrix4x4> MatrixStack;





//--------------------------------------------------------------
void ofApp::setup(){
	char const* filename = "/Users/steve/projects/nightgarden/data/test.ls";
	if(LSystem::getInstance().load(filename))
	{
		mCurrSteps = 0;
		mMaxSteps = 4;
		
		mMaxSteps = LSystem::getInstance().getGlobalVariable("steps", 4);
		
		mLastStepTime = 0;
		mStepInterval = 0;
	}
	
	mDrawWireframe = false;
	mSpotlight.setDirectional();//20.0f);

	mCamera.setPosition(0, 0, -10);
	mCamera.setTarget(ofVec3f(0,0,0));
	mCamera.enableMouseInput();

}

//--------------------------------------------------------------
void ofApp::update()
{
	if(mCurrSteps < mMaxSteps)
	{
		float now = ofGetElapsedTimef();
		if(now-mStepInterval > mLastStepTime)
		{
			mLastStepTime = now;
			LSystem::getInstance().step();
			LSystem::getInstance().reduce();
			++mCurrSteps;
			float afterStep = ofGetElapsedTimef();
			
			mLastStepDuration = afterStep - now;

			if(mCurrSteps == mMaxSteps)
			{
				SymbolList const* state = LSystem::getInstance().getState();
				std::cout << "state:   " << state->toString() << std::endl;
			}
			
			buildMeshes();
		}
	}
}


void ofApp::drawMeshes()
{
	for(MeshVec::iterator i = mMeshes.begin(); i!=mMeshes.end(); ++i)
	{
		if(mDrawWireframe)
		{
			(*i)->drawWireframe();
		}
		else
		{
			(*i)->draw();
		}
	}
}

void ofApp::clearMeshes()
{
	for(MeshVec::iterator i = mMeshes.begin(); i!=mMeshes.end(); ++i)
	{
		delete(*i);
	}
	
	mMeshes.clear();
}


void ofApp::buildMeshes()
{
	clearMeshes();
	
	//	std::cout << "==================================" << std::endl;
	VariableMap const& globals = LSystem::getInstance().getGlobalVariables();
	
	float delta = LSystem::getInstance().getGlobalVariable("delta", 22.5);
	float n = LSystem::getInstance().getGlobalVariable("n", 5);

	
	MatrixStack matrixStack;
	ofMatrix4x4 rootMatrix;

	matrixStack.push(rootMatrix);
	ofMatrix4x4 currMatrix = matrixStack.top();
	
	SymbolList const* state = LSystem::getInstance().getState();
	if(state)
	{
		ofMesh* currMesh = nullptr;
		ofMesh* poly = nullptr;
		ofColor polyColor(0,180,0);
		
		for(SymbolVec::const_iterator i = state->symbols.begin(); i!=state->symbols.end(); ++i)
		{
			Symbol* s = *i;
			if(s->value == "F")
			{
				ofColor color(139,69,19);
				if(s->expressions && s->expressions->expressions.size() == 3)
				{
					//					std::cout << s->expressions->toString() << std::endl;
					color.r = stoi((*(s->expressions->expressions[0])).value);
					color.g = stoi((*(s->expressions->expressions[1])).value);
					color.b = stoi((*(s->expressions->expressions[2])).value);
				}
				
				// TODO Next try joining to existing mesh...
				currMesh = new ofMesh();
				float r = 1.0;
				int sides = 4;
				
				float dr = 360.0/sides;
				
				
				for(int j=0; j<sides; ++j)
				{
					ofVec4f v1(r,0,0,1);
					ofVec4f v2(r,n,0,1);
					ofMatrix4x4 rotMatrix;
					rotMatrix.glRotate(dr*j, 0, 1, 0);
					v1 = v1 * rotMatrix;
					v2 = v2 * rotMatrix;
					
					v1 = v1 * currMatrix;
					v2 = v2 * currMatrix;
					
					currMesh->addVertex(v1);
					currMesh->addColor(color);
					
					currMesh->addVertex(v2);
					currMesh->addColor(color);
				}
				currMesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
				
				
				mMeshes.push_back(currMesh);
				currMesh = nullptr;
				
				currMatrix.glTranslate(0,n,0);
			}
			
			if(s->value == "G")
			{
				currMatrix.glTranslate(0,n,0);
			}
			
			else if(s->value == ".")
			{
				if(poly)
				{
					ofVec4f v1(0,0,0,1);
					v1 = v1 * currMatrix;
					poly->addVertex(v1);
					poly->addColor(polyColor);
				}
			}
			
			else if(s->value == "f")
			{
				float d = n;
				if(s->expressions && s->expressions->expressions.size() == 1)
				{
					d = stof((*(s->expressions->expressions[0])).value);
				}

				if(poly)
				{
					ofVec4f v1(0,0,0,1);
					v1 = v1 * currMatrix;
					poly->addVertex(v1);
					poly->addColor(polyColor);
				}
				currMatrix.glTranslate(0, d, 0);
			}
			
			else if(s->value == "{")
			{
				if(poly)
				{
					std::cout << "Warning: New poly started while an existing poly was being built. Deleting existing poly.\n";
					delete poly;
				}
				
				poly = new ofMesh();

				if(s->expressions && s->expressions->expressions.size() == 3)
				{
					//					std::cout << s->expressions->toString() << std::endl;
					polyColor.r = stoi((*(s->expressions->expressions[0])).value);
					polyColor.g = stoi((*(s->expressions->expressions[1])).value);
					polyColor.b = stoi((*(s->expressions->expressions[2])).value);
				}
			}
			
			else if(s->value == "}")
			{
				if(poly)
				{
					poly->setMode(OF_PRIMITIVE_TRIANGLE_FAN);
					mMeshes.push_back(poly);
					poly = nullptr;
				}
			}
			
			else if(s->value == "[")
			{
				matrixStack.push(currMatrix);
			}

			else if(s->value == "]")
			{
				currMatrix = matrixStack.top();
				matrixStack.pop();
			}

			else if(s->value == "+")
			{
				float d = delta;
				if(s->expressions && s->expressions->expressions.size() == 1)
				{
					d = stof((*(s->expressions->expressions[0])).value);
				}
				
				currMatrix.glRotate(d, 0, 0, 1);
			}

			else if(s->value == "-")
			{
				float d = delta;
				if(s->expressions && s->expressions->expressions.size() == 1)
				{
					d = stof((*(s->expressions->expressions[0])).value);
				}
				currMatrix.glRotate(-d, 0, 0, 1);
			}

			else if(s->value == "&")
			{
				float d = delta;
				if(s->expressions && s->expressions->expressions.size() == 1)
				{
					d = stof((*(s->expressions->expressions[0])).value);
				}
				currMatrix.glRotate(-d, 1, 0, 0);
			}

			else if(s->value == "^")
			{
				float d = delta;
				if(s->expressions && s->expressions->expressions.size() == 1)
				{
					d = stof((*(s->expressions->expressions[0])).value);
				}
				currMatrix.glRotate(d, 1, 0, 0);
			}

			else if(s->value == "/")
			{
				float d = delta;
				if(s->expressions && s->expressions->expressions.size() == 1)
				{
					d = stof((*(s->expressions->expressions[0])).value);
				}
				currMatrix.glRotate(-d, 0, 1, 0);
			}

			else if(s->value == "\\")
			{
				float d = delta;
				if(s->expressions && s->expressions->expressions.size() == 1)
				{
					d = stof((*(s->expressions->expressions[0])).value);
				}
				currMatrix.glRotate(d, 0, 1, 0);
			}

			else if(s->value == "|")
			{
				currMatrix.glRotate(180, 0, 0, 1);
			}
		}
	}
}

void ofApp::drawDebugHUD()
{
	float delta = LSystem::getInstance().getGlobalVariable("delta", 22.5);
	float n = LSystem::getInstance().getGlobalVariable("n", 5);
	
	ofSetColor(0);
	std::string s;
	s+= ("n:             " + to_string(n) + "\n");
	s+= ("delta:         " + to_string(delta) + "\n");
	s+= ("steps:         " + to_string(mMaxSteps) + "\n");
	s+= ("currSteps:     " + to_string(mCurrSteps) + "\n");
	s+= ("system len:    " + to_string(LSystem::getInstance().getState()->symbols.size()) + "\n");
	s+= ("Step duration: " + to_string(mLastStepDuration*1000) + " ms");
	ofDrawBitmapString(s.c_str(), 20,20);

}

//--------------------------------------------------------------
void ofApp::draw()
{
	mSpotlight.setPosition(mCamera.getPosition());
	mSpotlight.lookAt(mCamera.getLookAtDir());

//	ofEnableLighting();
	ofEnableDepthTest();
	
	mCamera.begin();
//	mSpotlight.enable();
	
	drawMeshes();

//	mSpotlight.disable();
	mCamera.end();
	
	drawDebugHUD();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key == 'w')
	{
		mDrawWireframe = !mDrawWireframe;
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

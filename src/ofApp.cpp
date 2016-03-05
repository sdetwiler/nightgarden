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
		mMaxSteps = 14;
		mLastStepTime = 0;
		mStepInterval = 1;
	}
	
	r=0;
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
			++mCurrSteps;

			SymbolList const* state = LSystem::getInstance().getState();
//			std::cout << state->toString() << std::endl;
			
			buildMeshes();

		}
	}
	
//	r+=1;
	if(r>360)
	{
		r = 0;
	}
}


void ofApp::drawMeshes()
{
	for(MeshVec::iterator i = mMeshes.begin(); i!=mMeshes.end(); ++i)
	{
		(*i)->drawWireframe();
//		(*i)->draw();
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
	
	float tx = w/2.0;
	float ty = h*0.75;
	// Match openframeworks rendering context.
	delta*=-1;
	
	MatrixStack matrixStack;
	
	
	
	ofMatrix4x4 rm;
	rm.glTranslate(tx, ty, 0);
	rm.glRotate(180,0,0,1);
	
	rm.glRotate(r,0,1,0);
	
	matrixStack.push(rm);
	ofMatrix4x4 currMatrix = matrixStack.top();
	
	
	SymbolList const* state = LSystem::getInstance().getState();
	
	if(state)
	{
		ofMesh* currMesh = nullptr;
		ofMesh* poly = nullptr;
		ofColor polyColor(0,180,0);
		
		for(SymbolVec::const_iterator i = state->symbols.begin(); i!=state->symbols.end(); ++i)
		{
			
			//			std::cout << "Matrix Audit\n------------\nglMatrix\n" << ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) << "\nmyMatrix\n" << currMatrix << "\n\n";
			
			
			Symbol* s = *i;
			if(s->value == "F")
			{
				// Next try joining to existing mesh...
				currMesh = new ofMesh();
				
				float r = 1.0;
				int sides = 4;
				
				float dr = 360.0/sides;
				
				ofColor brown(139,69,19);
				brown = brown * ofRandom(0.5, 1.5);
				
				for(int i=0; i<sides; ++i)
				{
					ofVec4f v1(r,0,0,1);
					ofVec4f v2(r,n,0,1);
					ofMatrix4x4 rotMatrix;
					rotMatrix.glRotate(dr*i, 0, 1, 0);
					v1 = v1 * rotMatrix;
					v2 = v2 * rotMatrix;
					
					v1 = v1 * currMatrix;
					v2 = v2 * currMatrix;
					
					currMesh->addVertex(v1);
					currMesh->addColor(brown);
					
					currMesh->addVertex(v2);
					currMesh->addColor(brown);
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
					//					std::cout << v1 << std::endl;
					poly->addVertex(v1);
					poly->addColor(polyColor);
				}
			}
			
			else if(s->value == "f")
			{
				if(poly)
				{
					ofVec4f v1(0,0,0,1);
					v1 = v1 * currMatrix;
					//					std::cout << v1 << std::endl;
					poly->addVertex(v1);
					poly->addColor(polyColor);
				}
				currMatrix.glTranslate(0, n, 0);
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
				currMatrix.glRotate(delta, 0, 0, 1);
			}
			else if(s->value == "-")
			{
				currMatrix.glRotate(-delta, 0, 0, 1);
			}
			else if(s->value == "&")
			{
				currMatrix.glRotate(-delta, 1, 0, 0);
			}
			else if(s->value == "^")
			{
				currMatrix.glRotate(delta, 1, 0, 0);
			}
			else if(s->value == "/")
			{
				currMatrix.glRotate(-delta, 0, 1, 0);
			}
			else if(s->value == "\\")
			{
				currMatrix.glRotate(delta, 0, 1, 0);
			}
			else if(s->value == "|")
			{
				currMatrix.glRotate(180, 0, 0, 1);
			}
		}
	}
}


//--------------------------------------------------------------
void ofApp::draw()
{
	ofEnableDepthTest();
	drawMeshes();
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

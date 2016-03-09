//
//  ofxLSystem.cpp
//  nightgarden
//
//  Created by Stephen Detwiler on 3/8/16.
//
//

#include "ofxLSystem.h"

#include "lsystem.h"
#include "types.h"

#include <stack>

typedef std::stack<ofMatrix4x4> MatrixStack;


//--------------------------------------------------------------
ofxLSystem::ofxLSystem()
{
	
}

//--------------------------------------------------------------
ofxLSystem::ofxLSystem(char const* filename)
{
	load(filename);
}

//--------------------------------------------------------------
ofxLSystem::~ofxLSystem()
{
	clearMeshes();
}

//--------------------------------------------------------------
int ofxLSystem::getMaxSteps() const
{
	return mMaxSteps;
}

//--------------------------------------------------------------
void ofxLSystem::setMaxSteps(int v)
{
	mMaxSteps = v;
}


//--------------------------------------------------------------
int ofxLSystem::getCurrSteps() const
{
	return mCurrSteps;
}

//--------------------------------------------------------------
float ofxLSystem::getLastStepTime() const
{
	return mLastStepTime;
}

//--------------------------------------------------------------
float ofxLSystem::getStepInterval() const
{
	return mStepInterval;
}

//--------------------------------------------------------------
void ofxLSystem::setStepInterval(float v)
{
	mStepInterval = v;
}

//--------------------------------------------------------------
float ofxLSystem::getLastStepDuration() const
{
	return mLastStepDuration;
}

//--------------------------------------------------------------
bool ofxLSystem::getDrawWireframe() const
{
	return mDrawWireframe;
}

//--------------------------------------------------------------
void ofxLSystem::setDrawWireframe(bool v)
{
	mDrawWireframe = v;
}

//--------------------------------------------------------------
LSystem const& ofxLSystem::getLSystem() const
{
	return LSystem::getInstance();
}

//--------------------------------------------------------------
void ofxLSystem::load(char const* filename)
{
//	char const* filename = "/Users/steve/projects/nightgarden/data/test.ls";
	if(LSystem::getInstance().load(filename))
	{
		mCurrSteps = 0;
		mMaxSteps = 4;
		
		mMaxSteps = LSystem::getInstance().getGlobalVariable("steps", 4);
		
		mLastStepTime = 0;
		mStepInterval = 0;
	}
	
	mDrawWireframe = false;
}

//--------------------------------------------------------------
void ofxLSystem::update()
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

//--------------------------------------------------------------
void ofxLSystem::draw()
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

//--------------------------------------------------------------
void ofxLSystem::clearMeshes()
{
	for(MeshVec::iterator i = mMeshes.begin(); i!=mMeshes.end(); ++i)
	{
		delete(*i);
	}
	
	mMeshes.clear();
}

//--------------------------------------------------------------
void ofxLSystem::buildMeshes()
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

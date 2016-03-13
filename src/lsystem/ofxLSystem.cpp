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
	clear();
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
//	return mSystem;
	return LSystem::getInstance();
}

//--------------------------------------------------------------
void ofxLSystem::load(char const* filename)
{

	// FIXME
	if(LSystem::getInstance().load(filename))
	{
		mCurrSteps = 0;
		mMaxSteps = getLSystem().getGlobalVariable("steps", 4);
		
		mLastStepTime = 0;
		mStepInterval = .5;
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
			// FIXME
			LSystem::getInstance().step();
			LSystem::getInstance().reduce();
			++mCurrSteps;
			float afterStep = ofGetElapsedTimef();
			
			mLastStepDuration = afterStep - now;
			
			if(mCurrSteps == mMaxSteps)
			{
				SymbolList const* state = getLSystem().getState();
				if(state)
				{
					std::cout << "state:   " << state->toString() << std::endl;
				}
			}
			
			buildMeshes();
		}
	}
}

//--------------------------------------------------------------
void ofxLSystem::draw()
{
	for(NodeVec::iterator i = mNodes.begin(); i!=mNodes.end(); ++i)
	{
		of3dPrimitive* p = (of3dPrimitive*)(*i);
		if(mDrawWireframe)
		{
			p->drawWireframe();
			p->drawNormals(10);
		}
		else
		{
			p->draw();
		}
	}
}

//--------------------------------------------------------------
void ofxLSystem::clear()
{
	for(NodeVec::iterator i = mNodes.begin(); i!=mNodes.end(); ++i)
	{
		delete(*i);
	}
	
	mNodes.clear();

	for(MeshVec::iterator i = mMeshes.begin(); i!=mMeshes.end(); ++i)
	{
		delete(*i);
	}
	
	mMeshes.clear();
}

ofVec3f getSurfaceNormal(ofVec3f const& v1, ofVec3f const& v2, ofVec3f const& v3)
{
	ofVec3f u = v2-v1;
	ofVec3f v = v3-v1;
	return u.cross(v).normalize();
}

//--------------------------------------------------------------
void ofxLSystem::buildMeshes()
{
	clear();
	
	//	std::cout << "==================================" << std::endl;
	VariableMap const& globals = mSystem.getGlobalVariables();
	
	float delta = getLSystem().getGlobalVariable("delta", 22.5);
	float n = getLSystem().getGlobalVariable("n", 5);
	float r = getLSystem().getGlobalVariable("r", 0.25);
	

	MatrixStack matrixStack;
	ofMatrix4x4 rootMatrix;
	
	matrixStack.push(rootMatrix);
	ofMatrix4x4 currMatrix = matrixStack.top();
	
	SymbolList const* state = getLSystem().getState();
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
				int sides = 4;
				
				float dr = 360.0/sides;
				
				ofIndexType idx = 0;
				// Build faces around the center of height n.
				for(int j=0; j<sides; ++j)
				{
					ofVec4f v0(r,0,r,1);
					ofVec4f v1(r,n,r,1);
					ofVec4f v2(-r,n,r,1);
					ofVec4f v3(-r,0,r,1);
					ofMatrix4x4 rotMatrix;
					rotMatrix.glRotate(dr*j, 0, 1, 0);
					v0 = v0 * rotMatrix;
					v1 = v1 * rotMatrix;
					v2 = v2 * rotMatrix;
					v3 = v3 * rotMatrix;
					
					v0 = v0 * currMatrix;
					v1 = v1 * currMatrix;
					v2 = v2 * currMatrix;
					v3 = v3 * currMatrix;
					
					currMesh->addVertex(v0);
					currMesh->addColor(color);

					currMesh->addVertex(v1);
					currMesh->addColor(color);
					
					currMesh->addVertex(v2);
					currMesh->addColor(color);

					currMesh->addVertex(v3);
					currMesh->addColor(color);
					
					currMesh->addNormal(getSurfaceNormal(v0,v1,v3));
					currMesh->addNormal(getSurfaceNormal(v1,v2,v0));
					currMesh->addNormal(getSurfaceNormal(v2,v3,v1));
					currMesh->addNormal(getSurfaceNormal(v3,v1,v2));
					
					currMesh->addIndex(idx+0);
					currMesh->addIndex(idx+1);
					currMesh->addIndex(idx+2);

					currMesh->addIndex(idx+2);
					currMesh->addIndex(idx+3);
					currMesh->addIndex(idx+0);
					
					idx+=4;
				}
				
				currMesh->setMode(OF_PRIMITIVE_TRIANGLES);
				
				
				of3dPrimitive* prim = new of3dPrimitive(*currMesh);
				mNodes.push_back(prim);
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
					
					ofIndexType numVerts = poly->getNumVertices();
					if(numVerts == 3)
					{
						ofVec3f v0 = poly->getVertex(0);
						ofVec3f vNMinus1 = poly->getVertex(poly->getNumVertices()-2);
//						poly->addNormal(getSurfaceNormal(v0, vNMinus1, v1));
//						poly->addNormal(getSurfaceNormal(vNMinus1, v0, v1));
//						poly->addNormal(getSurfaceNormal(v1, vNMinus1, v0));
						
						poly->addIndex(0);
						poly->addIndex(1);
						poly->addIndex(2);
					}
					
					else if(numVerts > 3)
					{
						ofVec3f v0 = poly->getVertex(0);
						ofVec3f vNMinus1 = poly->getVertex(poly->getNumVertices()-2);
//						poly->addVertex(v0);
//						poly->addColor(polyColor);
//						poly->addVertex(vNMinus1);
//						poly->addColor(polyColor);

//						poly->addNormal(getSurfaceNormal(v0, vNMinus1, v1));
//						poly->addNormal(getSurfaceNormal(vNMinus1, v0, v1));

//						poly->addNormal(getSurfaceNormal(v1, v0, vNMinus1));
						
						poly->addIndex(0);
						poly->addIndex(numVerts-1);
						poly->addIndex(numVerts-2);
						
					
					}
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

					if(poly->getNumVertices() == 3)
					{
						ofVec3f v0 = poly->getVertex(0);
						ofVec3f vNMinus1 = poly->getVertex(poly->getNumVertices()-2);
						poly->addNormal(getSurfaceNormal(v0, vNMinus1, v1));
						poly->addNormal(getSurfaceNormal(vNMinus1, v0, v1));
						poly->addNormal(getSurfaceNormal(v1, v0, vNMinus1));
					}
					
					else if(poly->getNumVertices() > 3)
					{
						ofVec3f v0 = poly->getVertex(0);
						ofVec3f vNMinus1 = poly->getVertex(poly->getNumVertices()-2);
						poly->addVertex(vNMinus1);
						poly->addColor(polyColor);
						poly->addVertex(v0);
						poly->addColor(polyColor);
						
						poly->addNormal(getSurfaceNormal(v0, v1, vNMinus1));
						poly->addNormal(getSurfaceNormal(vNMinus1, v0, v1));
						poly->addNormal(getSurfaceNormal(v1, v0, vNMinus1));
						
					}

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
					of3dPrimitive* prim = new of3dPrimitive(*poly);
					mNodes.push_back(prim);

					poly->setMode(OF_PRIMITIVE_TRIANGLES);
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

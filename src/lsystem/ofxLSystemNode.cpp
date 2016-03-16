//
//  ofxLSystemNode.cpp
//  nightgarden
//
//  Created by Stephen Detwiler on 3/8/16.
//
//

#include "ofxLSystemNode.h"

#include "lsystem.h"
#include "types.h"

#include <stack>

using namespace std;


//--------------------------------------------------------------
ofxLSystemNode::ofxLSystemNode()
{
	mSystem = new ofxThreadedLSystem();
	mCurrMesh = nullptr;
}

//--------------------------------------------------------------
ofxLSystemNode::ofxLSystemNode(char const* filename)
{
	mSystem = new ofxThreadedLSystem();
	mCurrMesh = nullptr;

	load(filename);
}

//--------------------------------------------------------------
ofxLSystemNode::~ofxLSystemNode()
{
	clear();
	delete mSystem;
}

//--------------------------------------------------------------
int ofxLSystemNode::getMaxSteps() const
{
	return mMaxSteps;
}

//--------------------------------------------------------------
void ofxLSystemNode::setMaxSteps(int v)
{
	mMaxSteps = v;
}


//--------------------------------------------------------------
int ofxLSystemNode::getCurrSteps() const
{
	return mCurrSteps;
}

//--------------------------------------------------------------
float ofxLSystemNode::getLastStepTime() const
{
	return mLastStepTime;
}

//--------------------------------------------------------------
float ofxLSystemNode::getStepInterval() const
{
	return mStepInterval;
}

//--------------------------------------------------------------
void ofxLSystemNode::setStepInterval(float v)
{
	mStepInterval = v;
}

//--------------------------------------------------------------
float ofxLSystemNode::getLastStepDuration() const
{
	return mLastStepDuration;
}

//--------------------------------------------------------------
bool ofxLSystemNode::getDrawWireframe() const
{
	return mDrawWireframe;
}

//--------------------------------------------------------------
void ofxLSystemNode::setDrawWireframe(bool v)
{
	mDrawWireframe = v;
}

//--------------------------------------------------------------
ofxThreadedLSystem& ofxLSystemNode::getLSystem()
{
	return *mSystem;
}

//--------------------------------------------------------------
void ofxLSystemNode::load(char const* filename)
{
	if(mSystem->load(filename))
	{
		// Is the system configured to step through time, or over an explicit number of steps.
		float duration = getLSystem().getGlobalVariable("duration", -1);
		if(duration > 0)
		{
			mStepInterval = getLSystem().getGlobalVariable("stepInterval", 1);
			mMaxSteps = duration/mStepInterval;
		}
		else
		{
			mStepInterval = 1/30.;
			mMaxSteps = getLSystem().getGlobalVariable("steps", 4) / mStepInterval;
		}
		
		mCurrSteps = 0;
		mLastStepTime = 0;
	}
	
	mDrawWireframe = false;
}

//--------------------------------------------------------------
void ofxLSystemNode::update()
{
	if(mCurrSteps < mMaxSteps)
	{
		float now = ofGetElapsedTimef();
		if(now-mStepInterval > mLastStepTime)
		{
			mSystem->update(mStepInterval);
			if(mSystem->isUpdateAvailable())
			{
				mLastStepTime = now;
				++mCurrSteps;
				float afterStep = ofGetElapsedTimef();
				
				mLastStepDuration = afterStep - now;
				
				if(mCurrSteps == mMaxSteps)
				{
					SymbolList state;
					getLSystem().getState(&state);
					cout << "state:   " << state.toString() << endl;
				}
				
				buildMeshes();
				
			}
		}
	}
}

//--------------------------------------------------------------
void ofxLSystemNode::customDraw()
{
	for(PrimitiveVec::iterator i = mPrimitives.begin(); i!=mPrimitives.end(); ++i)
	{
		of3dPrimitive* p = *i;
		if(mDrawWireframe)
		{
//			p->drawWireframe();
			p->drawVertices();
//			p->drawNormals(10);
		}
		else
		{
			p->draw();
		}
	}
}

//--------------------------------------------------------------
void ofxLSystemNode::clear()
{
	for(PrimitiveVec::iterator i = mPrimitives.begin(); i!=mPrimitives.end(); ++i)
	{
		delete(*i);
	}
	
	mPrimitives.clear();
}

//--------------------------------------------------------------
ofVec3f getSurfaceNormal(ofVec3f const& v1, ofVec3f const& v2, ofVec3f const& v3)
{
	ofVec3f u = v2-v1;
	ofVec3f v = v3-v1;
	return u.cross(v).normalize();
}

void ofxLSystemNode::closeMesh()
{
	if(mCurrMesh)
	{
		of3dPrimitive* prim = new of3dPrimitive(*mCurrMesh);
		mPrimitives.push_back(prim);
		delete mCurrMesh;
		mCurrMesh = nullptr;
		
		cout << "closeMesh\n";
	}
}

float ofxLSystemNode::makeMeshFaces(float radius, ofColor color, ofMatrix4x4 const& currMatrix)
{
	int sides = 6;
	
	float theta = (PI*2)/sides;
	
	// Build next ring of vertices around center.
	for(int j=0; j<sides; ++j)
	{
		
		/************
		 
		 v3-------------v2
		 |              |
		 |              |
		 |              |
		 |       +      |
		 |              |
		 |              |
		 |              |
		 v0-------------v1
		 
		 ***********/
		
		float x = radius * sinf(theta*j);
		float z = radius * cosf(theta*j);
		
		ofVec4f v(x,0,z,1);
		v = v * currMatrix;
		mCurrMesh->addVertex(v);
		mCurrMesh->addColor(color);
	}
	
	// Add normals and indicies using the verticies to form new triangles.
	ofIndexType numVerts = mCurrMesh->getNumVertices();
	ofIndexType idx = (numVerts-sides);
	ofIndexType prevIdx = idx - sides;
	
	bool firstTris = (prevIdx == 0);
	
	// If this is not the first ring of verticies.
	if(idx != 0)
	{
		for(int j=0; j<sides; ++j)
		{
			
			ofIndexType i0n = (idx     + 0);
			ofIndexType i1n = (idx     + 1);
			ofIndexType i0p = (prevIdx + 0);
			ofIndexType i1p = (prevIdx + 1);
			
			// HACK
			if(i1n == numVerts)
			{
				i1n = numVerts - sides;
				i1p-=sides;
			}
			
			// New and previous verts.
			ofVec3f v0n = mCurrMesh->getVertex(i0n);
			ofVec3f v1n = mCurrMesh->getVertex(i1n);
			ofVec3f v0p = mCurrMesh->getVertex(i0p);
			ofVec3f v1p = mCurrMesh->getVertex(i1p);
			
			mCurrMesh->addNormal(getSurfaceNormal(v0n, v0p, v1n));
			mCurrMesh->addNormal(getSurfaceNormal(v1n, v1p, v0n));
			
			// If this is the first set of faces, add normals for the first ring of vertices.
			if(firstTris)
			{
				mCurrMesh->addNormal(getSurfaceNormal(v0p, v1p, v0n));
				mCurrMesh->addNormal(getSurfaceNormal(v1p, v1n, v0p));
			}
			
			mCurrMesh->addIndex(i0p);	// v0p
			mCurrMesh->addIndex(i1n);	// v1n
			mCurrMesh->addIndex(i0n);	// v0n
			
			mCurrMesh->addIndex(i1p);	// v1p
			mCurrMesh->addIndex(i1n);	// v1n
			mCurrMesh->addIndex(i0p);	// v0p
			
			++idx;
			++prevIdx;
		}
	}
}


//--------------------------------------------------------------
void ofxLSystemNode::buildMeshes()
{
	clear();
	
	float const delta = getLSystem().getGlobalVariable("delta", 22.5);
	float const n = getLSystem().getGlobalVariable("n", 5);
	float const r = getLSystem().getGlobalVariable("r", 0.25);
	
	typedef stack<ofMatrix4x4> MatrixStack;

	MatrixStack matrixStack;
	ofMatrix4x4 rootMatrix;
	
	matrixStack.push(rootMatrix);
	ofMatrix4x4 currMatrix = matrixStack.top();
	
	SymbolList state;
	getLSystem().getState(&state);
	ofMesh* poly = nullptr;
	ofColor polyColor(0,180,0);
	
	for(SymbolVec::const_iterator i = state.symbols.begin(); i!=state.symbols.end(); ++i)
	{
		Symbol* s = *i;
		if(s->value == "F")
		{
			// Scale n by the symbol's age into a local version of n.
			float ln = s->applyGrowthFunction(n);
			float lr = s->applyGrowthFunction(r);

			ofColor color(139,69,19);
			if(s->expressions && s->expressions->expressions.size() == 3)
			{
				//					cout << s->expressions->toString() << endl;
				color.r = stoi((*(s->expressions->expressions[0])).value);
				color.g = stoi((*(s->expressions->expressions[1])).value);
				color.b = stoi((*(s->expressions->expressions[2])).value);
			}

			if(mCurrMesh == nullptr)
			{
				mCurrMesh = new ofMesh();
				mCurrMesh->setMode(OF_PRIMITIVE_TRIANGLES);
				// Create initial mesh verts.
				makeMeshFaces(lr, color, currMatrix);
			}
			

			currMatrix.glTranslate(0, ln, 0);
			makeMeshFaces(lr, color, currMatrix);
		}
		
		if(s->value == "G")
		{
			currMatrix.glTranslate(0, s->applyGrowthFunction(n), 0);
		}
		
		else if(s->value == "." || s->value == "f")
		{
			if(poly)
			{
				ofVec4f v2(0,0,0,1);
				v2 = v2 * currMatrix;
				poly->addVertex(v2);
				poly->addColor(polyColor);
				
				ofIndexType numVerts = poly->getNumVertices();
				if(numVerts == 3)
				{
					ofVec3f v0 = poly->getVertex(0);
					ofVec3f v1 = poly->getVertex(numVerts-2);

					ofVec3f v1v0 = v1-v0;
					ofVec3f v2v0 = v2-v0;
					ofVec3f cp = v1v0.cross(v2v0).normalize();

//						cout << "\nnumVerts==3\nv0: (" << v0 << ")\n" << "v1: (" << v1 << ")\n" << "v2: (" << v2 << ")" << "\ncross: " << cp << endl;
					
					if(cp[2] > 0)
					{
						poly->addNormal(getSurfaceNormal(v0, v2, v1));
						poly->addNormal(getSurfaceNormal(v1, v0, v2));
						poly->addNormal(getSurfaceNormal(v2, v1, v0));
					}
					else
					{
						poly->addNormal(getSurfaceNormal(v0, v1, v2));
						poly->addNormal(getSurfaceNormal(v1, v2, v0));
						poly->addNormal(getSurfaceNormal(v2, v0, v1));
					}

					poly->addIndex(0);			// v0
					poly->addIndex(1);			// v1
					poly->addIndex(2);			// v2
				}
				
				if(numVerts > 3)
				{
					ofVec3f v0 = poly->getVertex(0);
					ofVec3f v1 = poly->getVertex(numVerts-2);

					ofVec3f v1v0 = v1-v0;
					ofVec3f v2v0 = v2-v0;
					ofVec3f cp = v1v0.cross(v2v0).normalize();
					
//						cout << "\nnumVerts==" << to_string(numVerts) << "\nv0: (" << v0 << ")\n" << "v1: (" << v1 << ")\n" << "v2: (" << v2 << ")" << "\ncross: " << cp << endl;

					if(cp[2] > 0)
					{
						poly->addNormal(getSurfaceNormal(v2, v1, v0));
					}
					else
					{
						poly->addNormal(getSurfaceNormal(v2, v0, v1));
					}

					poly->addIndex(0);			// v0
					poly->addIndex(numVerts-2);	// v1
					poly->addIndex(numVerts-1);	// v2
				}
			}
			
			if(s->value == "f")
			{
				float ln = n;

				if(s->expressions && s->expressions->expressions.size() == 1)
				{
					ln = stof((*(s->expressions->expressions[0])).value);
				}

				currMatrix.glTranslate(0, s->applyGrowthFunction(n), 0);
			}
		}
		
		
		else if(s->value == "{")
		{
			if(poly)
			{
				cout << "Warning: New poly started while an existing poly was being built. Deleting existing poly.\n";
				delete poly;
			}
			
			poly = new ofMesh();
			
			if(s->expressions && s->expressions->expressions.size() == 3)
			{
				//					cout << s->expressions->toString() << endl;
				polyColor.r = stoi((*(s->expressions->expressions[0])).value);
				polyColor.g = stoi((*(s->expressions->expressions[1])).value);
				polyColor.b = stoi((*(s->expressions->expressions[2])).value);
			}
		}
		
		else if(s->value == "}")
		{
			if(poly)
			{
				poly->setMode(OF_PRIMITIVE_TRIANGLES);
				of3dPrimitive* prim = new of3dPrimitive(*poly);
				mPrimitives.push_back(prim);

				delete poly;
				poly = nullptr;
			}
		}
		
		else if(s->value == "[")
		{
			closeMesh();
			matrixStack.push(currMatrix);
		}
		
		else if(s->value == "]")
		{
			closeMesh();
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
			currMatrix.glRotate(s->applyGrowthFunction(d), 0, 0, 1);
		}
		
		else if(s->value == "-")
		{
			float d = delta;
			if(s->expressions && s->expressions->expressions.size() == 1)
			{
				d = stof((*(s->expressions->expressions[0])).value);
			}
			currMatrix.glRotate(-s->applyGrowthFunction(d), 0, 0, 1);
		}
		
		else if(s->value == "&")
		{
			float d = delta;
			if(s->expressions && s->expressions->expressions.size() == 1)
			{
				d = stof((*(s->expressions->expressions[0])).value);
			}
			currMatrix.glRotate(-s->applyGrowthFunction(d), 1, 0, 0);
		}
		
		else if(s->value == "^")
		{
			float d = delta;
			if(s->expressions && s->expressions->expressions.size() == 1)
			{
				d = stof((*(s->expressions->expressions[0])).value);
			}
			currMatrix.glRotate(s->applyGrowthFunction(d), 1, 0, 0);
		}
		
		else if(s->value == "/")
		{
			float d = delta;
			if(s->expressions && s->expressions->expressions.size() == 1)
			{
				d = stof((*(s->expressions->expressions[0])).value);
			}
			currMatrix.glRotate(-s->applyGrowthFunction(d), 0, 1, 0);
		}
		
		else if(s->value == "\\")
		{
			float d = delta;
			if(s->expressions && s->expressions->expressions.size() == 1)
			{
				d = stof((*(s->expressions->expressions[0])).value);
			}
			currMatrix.glRotate(s->applyGrowthFunction(d), 0, 1, 0);
		}
		
		else if(s->value == "|")
		{
			currMatrix.glRotate(180, 0, 0, 1);
		}
	}
	
	closeMesh();
}

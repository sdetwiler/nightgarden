//
//  ofxLSystemNodeNode.h
//  nightgarden
//
//  Created by Stephen Detwiler on 3/8/16.
//
//

#ifndef ofxLSystemNode_h
#define ofxLSystemNode_h

#include "of3dPrimitives.h"
#include "ofxThreadedLSystem.h"

class ofxLSystemNode : public ofNode
{
public:
	ofxLSystemNode();
	ofxLSystemNode(char const* filename);
	virtual ~ofxLSystemNode();
	
	void load(char const* filename);
	void update();
	virtual void customDraw();
	
	int getMaxSteps() const;
	void setMaxSteps(int v);

	int getCurrSteps() const;
	float getLastStepTime() const;

	float getStepInterval() const;
	void setStepInterval(float v);

	float getLastStepDuration() const;

	bool getDrawWireframe() const;
	void setDrawWireframe(bool v);
	
	ofxThreadedLSystem& getLSystem();
	
private:
	int mMaxSteps;				// The maximum number of steps to take.
	int mCurrSteps;				// The current count of steps taken.
	float mLastStepTime;		// The time the last step occurred.
	float mStepInterval;		// Time to wait between steps in seconds.

	float mLastStepDuration;	// How long it took to compute the last step.

	bool mDrawWireframe;		// Should the system be drawn in wireframe or solid?

	typedef std::vector<of3dPrimitive*> PrimitiveVec;
	PrimitiveVec mPrimitives;			// The collection of primitives generated by the system.
	
	ofxThreadedLSystem* mSystem;			// The driving LSystem.
	
	void clear();
	void buildMeshes();

	void closeMesh();
	float makeMeshFaces(float radius, ofColor color, ofMatrix4x4 const& currMatrix);
	
	ofMesh* mCurrMesh;
};

#endif /* ofxLSystemNode_h */

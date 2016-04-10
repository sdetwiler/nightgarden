//
//  ofxBulletLSystemNode.h
//  nightgarden
//
//  Created by Stephen Detwiler on 4/3/16.
//
//

#ifndef ofxBulletLSystemNode_h
#define ofxBulletLSystemNode_h

#include <vector>
#include "ofxBullet.h"
#include "ofxLSystemNode.h"

class ofxBulletLSystemNode : public ofxLSystemNode
{
public:
	ofxBulletLSystemNode(ofxBulletWorldRigid* world);
	ofxBulletLSystemNode(char const* filename, bool isCompiled, ofxBulletWorldRigid* world);
		
protected:
	virtual void clear();
	
	virtual void closeMesh();
	
private:
	typedef std::vector<ofxBulletCustomShape*> BulletShapeVec;
	BulletShapeVec mBulletShapes;	// The collection of bullet shapes for when physics is emabled.
	
	ofxBulletWorldRigid* mWorld;
};

#endif /* ofxBulletLSystemNode_h */

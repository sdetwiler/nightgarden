//
//  ofxBulletLSystemNode.cpp
//  nightgarden
//
//  Created by Stephen Detwiler on 4/3/16.
//
//

#include "ofxBulletLSystemNode.h"

//--------------------------------------------------------------
ofxBulletLSystemNode::ofxBulletLSystemNode(ofxBulletWorldRigid* world)
	: ofxLSystemNode()
{
	mWorld = world;
}

//--------------------------------------------------------------
ofxBulletLSystemNode::ofxBulletLSystemNode(char const* filename, bool isCompiled, ofxBulletWorldRigid* world)
	: ofxLSystemNode(filename, isCompiled)
{
	mWorld = world;
}


void ofxBulletLSystemNode::clear()
{
	for(BulletShapeVec::iterator i=mBulletShapes.begin(); i!=mBulletShapes.end(); ++i)
	{
		ofxBulletCustomShape* shape = *i;
		shape->removeShape();
		
		delete shape;
	}
	mBulletShapes.clear();

	ofxLSystemNode::clear();
}

void ofxBulletLSystemNode::closeMesh()
{
	if(mCurrMesh)
	{
		ofxBulletCustomShape* shape = new ofxBulletCustomShape();
		ofVec3f scale(1,1,1);
		shape->addMesh(*mCurrMesh, scale, true);
		shape->create(mWorld->world);
		shape->add();
		
		mBulletShapes.push_back(shape);
		
		delete mCurrMesh;
		mCurrMesh = nullptr;
	}

}

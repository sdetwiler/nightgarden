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

//--------------------------------------------------------------
ofxBulletLSystemNode::~ofxBulletLSystemNode()
{
	clear();
}

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void ofxBulletLSystemNode::closeMesh(ofVec3f const& position)
{
	if(mCurrMesh)
	{
		float const n = getLSystem().getGlobalVariable("n", 5);

		ofxBulletCustomShape* shape = new ofxBulletCustomShape();
		ofVec3f scale(1,1,1);
		shape->create(mWorld->world, getPosition()+position);//, getOrientationQuat());
		shape->addMesh(*mCurrMesh, scale, false);
		shape->add();
		mBulletShapes.push_back(shape);
		
		
		delete mCurrMesh;
		mCurrMesh = nullptr;
	}

}

// https://forum.openframeworks.cc/t/ofcamera-screentoworld-understanding-the-meaning-of-screen-z-values/17356/2

#pragma once

#include "ofMain.h"


class ofRay {
public:
	ofRay() {}
	ofRay( const ofVec3f &aOrigin, const ofVec3f &aDirection ) : mOrigin( aOrigin ) { setDirection( aDirection ); }
	
	void            setOrigin( const ofVec3f &aOrigin ) { mOrigin = aOrigin; }
	const ofVec3f&    getOrigin() const { return mOrigin; }
	
	void setDirection( const ofVec3f &aDirection ) {
		mDirection = aDirection;
		mInvDirection = ofVec3f( 1.0f / mDirection.x, 1.0f / mDirection.y, 1.0f / mDirection.z );
		mSignX = ( mDirection.x < 0.0f ) ? 1 : 0;
		mSignY = ( mDirection.y < 0.0f ) ? 1 : 0;
		mSignZ = ( mDirection.z < 0.0f ) ? 1 : 0;
	}
	const ofVec3f&    getDirection() const { return mDirection; }
	const ofVec3f&    getInverseDirection() const { return mInvDirection; }
	
	char    getSignX() const { return mSignX; }
	char    getSignY() const { return mSignY; }
	char    getSignZ() const { return mSignZ; }
	
	ofVec3f calcPosition( float t ) const { return mOrigin + mDirection * t; }
	
	bool calcTriangleIntersection( const ofVec3f &vert0, const ofVec3f &vert1, const ofVec3f &vert2, float *result ) const;
	bool calcPlaneIntersection( const ofVec3f &origin, const ofVec3f &normal, float *result ) const;
	
	friend class ofMeshFace;
	
protected:
	ofVec3f    mOrigin;
	ofVec3f    mDirection;
	// these are helpful to certain ray intersection algorithms
	char    mSignX, mSignY, mSignZ;
	ofVec3f    mInvDirection;
};
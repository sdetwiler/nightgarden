#include "ofRay.h"

// algorithm from "Fast, Minimum Storage Ray-Triangle Intersection"
bool ofRay::calcTriangleIntersection( const ofVec3f &vert0, const ofVec3f &vert1, const ofVec3f &vert2, float *result ) const
{
	
	ofVec3f edge1, edge2, tvec, pvec, qvec;
	float det;
	float u, v;
	const float EPSILON = 0.000001f;
	
	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;
	
	pvec = getDirection().getCrossed( edge2 );
	det = edge1.dot( pvec );
	
#if 0 // we don't want to backface cull
	if ( det < EPSILON )
		return false;
	tvec = getOrigin() - vert0;
	
	u = tvec.dot( pvec );
	if ( ( u < 0.0f ) || ( u > det ) )
		return false;
	
	qvec = tvec.getCrossed(edge1);
	v = getDirection().dot( qvec );
	if ( v < 0.0f || u + v > det )
		return false;
	
	*result = edge2.dot( qvec ) / det;
	return true;
#else
	if( det > -EPSILON && det < EPSILON )
		return false;
	
	float inv_det = 1.0f / det;
	tvec = getOrigin() - vert0;
	u = tvec.dot( pvec ) * inv_det;
	if( u < 0.0f || u > 1.0f )
		return false;
	
	qvec = tvec.getCrossed(edge1);
	
	v = getDirection().dot( qvec ) * inv_det;
	if( v < 0.0f || u + v > 1.0f )
		return 0;
	
	*result = edge2.dot( qvec ) * inv_det;
	return true;
#endif
}

bool ofRay::calcPlaneIntersection( const ofVec3f &planeOrigin, const ofVec3f &planeNormal, float *result ) const
{
	float denom = planeNormal.dot(getDirection());
	
	if(denom != 0.0f){
		*result = planeNormal.dot(planeOrigin - getOrigin()) / denom;
		return true;
	}
	return false;
}

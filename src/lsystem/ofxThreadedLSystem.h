//
//  ofxThreadedLSystem.h
//  nightgarden
//
//  Created by Stephen Detwiler on 3/15/16.
//
//

#ifndef ofxThreadedLSystem_h
#define ofxThreadedLSystem_h

#include "ofThread.h"
#include "lsystem.h"

class ofxThreadedLSystem : public ofThread
{
public:
	ofxThreadedLSystem();
	virtual ~ofxThreadedLSystem();

	virtual void threadedFunction();
	
	void update(float dt);
	void getState(SymbolList* state);

	bool isUpdateAvailable();
	bool load(char const* filename);
	void clear();
	
	float getGlobalVariable(char const* name, float def=0);

private:
	LSystem mSystem;
	bool mUpdateAvailable;
	bool mUpdateRequested;
	SymbolList mState;
	float mDt;
};

#endif /* ofxThreadedLSystem_h */

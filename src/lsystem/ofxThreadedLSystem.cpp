#include "ofxThreadedLSystem.h"

ofxThreadedLSystem::ofxThreadedLSystem()
{
	mUpdateAvailable = false;
	mUpdateRequested = false;

}


ofxThreadedLSystem::~ofxThreadedLSystem()
{
	if(!isThreadRunning())
	{
		stopThread();
	}
}


void ofxThreadedLSystem::threadedFunction()
{
	while(isThreadRunning())
	{
		// dirty read...
		if(!mUpdateRequested)
		{
			yield();
		}
		else if(!mUpdateAvailable)
		{
			float dt;
			lock();
			dt = mDt;
			unlock();
			mSystem.step(dt);
			mSystem.reduce();
			lock();
			mUpdateRequested = false;
			mUpdateAvailable = true;
			unlock();
		}
	}
}

float ofxThreadedLSystem::getGlobalVariable(char const* name, float def)
{
	lock();
	float ret = mSystem.getGlobalVariable(name, def);
	unlock();
	
	return ret;
}


bool ofxThreadedLSystem::load(char const* filename)
{
	lock();
	bool ret = mSystem.load(filename);
	unlock();
	
	return ret;
}

void ofxThreadedLSystem::clear()
{
	lock();
	mSystem.clear();
	unlock();
}


void ofxThreadedLSystem::getState(SymbolList* state)
{
	lock();
	if(mUpdateAvailable)
	{
		mUpdateAvailable = false;
		mState = *mSystem.getState();
	}
	unlock();
	
	*state = mState;
}

void ofxThreadedLSystem::update(float dt)
{
	if(!isThreadRunning())
	{
		startThread();
	}
	
	lock();
	mDt = dt;
	mUpdateRequested = true;
	unlock();
}

bool ofxThreadedLSystem::isUpdateAvailable()
{
	lock();
	bool ret = mUpdateAvailable;
	unlock();
	return ret;
}


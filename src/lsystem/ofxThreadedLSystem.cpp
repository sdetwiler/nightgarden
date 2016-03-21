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
			sleep(5);
		}
		else if(!mUpdateAvailable)
		{
			float dt;
			lock();
			dt = mDt;
			unlock();
			mSystem.step(dt);
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

bool ofxThreadedLSystem::loadCompiled(char const* filename, size_t* numStates)
{
	lock();
	bool ret = mSystem.loadCompiled(filename, numStates);
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
		// TODO FIXME Memory copies again!
		mUpdateAvailable = false;
		SymbolList* t = mSystem.getDereferencedState();
		if(t)
		{
			mState = *t;
			delete t;
		}
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


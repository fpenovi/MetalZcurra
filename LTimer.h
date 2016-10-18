//
// Created by franch on 17/10/16.
//

#ifndef METALZCURRA_LTIMER_H
#define METALZCURRA_LTIMER_H


#include <SDL2/SDL_stdinc.h>

class LTimer {

public:
	LTimer();
	void start();
	void stop();
	void pause();
	void unpause();
	Uint32 getTicks();
	bool isStarted();
	bool isPaused();

private:
	Uint32 mStartTicks;
	Uint32 mPausedTicks;
	bool mPaused;
	bool mStarted;
};


#endif //METALZCURRA_LTIMER_H

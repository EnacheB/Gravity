//==============================================================================
// SDL Tutorial 1
//==============================================================================
#ifndef _CAPP_H_
    #define _CAPP_H_

#include "SDL2/SDL.h"

#include "CEvent.h"
#include "View.h"
#include "GameState.h"

#include <string>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

/**
 * Base class which holds all of the components
 */

class CApp : public CEvent {
    private:
        bool            Running;
        int simMiliSecInterval = 0;
        int gfxMiliSecInterval = 0;
		double cameraSpeed = 1.0; // temporary speed value
        GameState* gameState;
        View* view;
        std::thread *gameStateThread;

    public:
        CApp();
        ~CApp();



        int OnExecute();

    public:

        void OnEvent(SDL_Event* Event);


            void OnLButtonDown(int mX, int mY);
			void OnLButtonUp(int mX, int mY);
	void OnMouseMove(int mX,int mY,int dX,int dY,bool lDown,bool rDown,bool mDown);

			/**
			* Handles the event when a key is pressed down
			* Called once per keypress per frame
			*/
			void OnKeyDown(SDL_Keysym sym);
			void OnKeyUp(SDL_Keysym sym);
			void OnMouseWheel(bool Up, bool Down);

            void OnExit();


};

//==============================================================================

#endif

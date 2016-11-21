//==============================================================================
#include "CApp.h"

/**
 * Contains functions to handle functions
 */

//==============================================================================
void CApp::OnEvent(SDL_Event* Event) {
    CEvent::OnEvent(Event);
}


//==============================================================================
void CApp::OnExit() {
	Running = false;
}

//------------------------------------------------------------------------------
void CApp::OnLButtonDown(int mX, int mY) {
	if(view->screenCoordsAreWithinCameraView(mX,mY))
    {
        double targetAngle = atan2(mY  - WIN_H/2, mX - WIN_W/2);
        gameState->changePlayerShip([targetAngle](Ship &ship) { ship.useWeaponFire(targetAngle); });
        return;
    }


}

void CApp::OnLButtonUp(int mX, int mY) {
    if(view->screenCoordsAreWithinCameraView(mX,mY))
    {
        gameState->changePlayerShip([](Ship &ship) { ship.useWeaponNoFire(); });
        return;
    }


}

/**
 * Handles the event when a key is pressed down
 * Called once per keypress per frame
 */
void CApp::OnKeyDown(SDL_Keysym sym) {
    //.sym gives actual key pressed
    switch (sym.sym){
		case SDLK_ESCAPE:
			Running = false;
			return;
		case SDLK_EQUALS:
		case SDLK_KP_PLUS:
			view->startZoomingIn();
			return;
		case SDLK_MINUS:
		case SDLK_KP_MINUS:
			view->startZoomingOut();
			return;
        default:
            break;
		//case SDLK_
    }

//    // .scancode gives "positional" key. "s" on qwerty or "o" on dvorak will both give "s"
//    // Look at how progressive we are!!
//    using both at the same time is probably a bad idea, though...
    switch (sym.scancode){
        case SDL_SCANCODE_W:
            gameState->changePlayerShip([](Ship &ship) { ship.useEnginesForwardFull(); });
            return;
        case SDL_SCANCODE_A:
            gameState->changePlayerShip([](Ship &ship) { ship.useEnginesTurnLeft(); });
            return;
        case SDL_SCANCODE_D:
            gameState->changePlayerShip([](Ship &ship) { ship.useEnginesTurnRight(); });
            return;
        case SDL_SCANCODE_SPACE:
            gameState->changePlayerShip([](Ship &ship) { ship.useWeaponFire(); });
            return;
        default:
            return;

    }
}
void CApp::OnMouseMove(int mX,int mY,int dX,int dY,bool lDown,bool rDown,bool mDown) {
    double targetAngle = atan2(mY  - WIN_H/2, mX - WIN_W/2);
    if(lDown)
        gameState->changePlayerShip([targetAngle](Ship &ship) { ship.useWeaponFire(targetAngle); });


}

void CApp::OnKeyUp(SDL_Keysym sym) {
//    // .scancode gives "positional" key. "s" on qwerty or "o" on dvorak will both give "s"
//    // Look at how progressive we are!!
//    using both at the same time is probably a bad idea, though...
    switch (sym.sym){
		case SDLK_EQUALS:
		case SDLK_KP_PLUS:
			view->stopZooming();
			return;
		case SDLK_MINUS:
		case SDLK_KP_MINUS:
			view->stopZooming();
			return;
        default:
            break;

    }
    switch (sym.scancode){
        case SDL_SCANCODE_W:
            gameState->changePlayerShip([](Ship &ship) { ship.useEnginesForwardOff(); });
            return;
        case SDL_SCANCODE_A:
            gameState->changePlayerShip([](Ship &ship) { ship.useEnginesNoTurnLeft(); });
            return;
        case SDL_SCANCODE_D:
            gameState->changePlayerShip([](Ship &ship) { ship.useEnginesNoTurnRight(); });
            return;
        case SDL_SCANCODE_SPACE:
            gameState->changePlayerShip([](Ship &ship) { ship.useWeaponNoFire(); });
            return;
        default:
            return;

    }
}

void CApp::OnMouseWheel(bool Up, bool Down) {
	if (Up) view->zoomIn();
	if (Down) view->zoomOut();
}

//==============================================================================

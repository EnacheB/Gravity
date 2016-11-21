//==============================================================================
#include "CApp.h"

//==============================================================================
CApp::CApp(){

    gameState = new GameState();
    gameStateThread = new std::thread(std::ref(*gameState));
    view = new View(gameState, false);

    Running = true;
}

CApp::~CApp() {
    gameState->stop();
    gameStateThread->join();
    delete gameStateThread;
    delete gameState;
    delete view;
}

//------------------------------------------------------------------------------
int CApp::OnExecute() {
    Uint32 gfxPast=0;
    Uint32 now=0;

    SDL_Event Event;

    while(Running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }


        now = SDL_GetTicks();
        if(now > gfxPast+gfxMiliSecInterval)
        {
            gfxPast = now;
            view->Render();
        }
        SDL_Delay(5);

    }

    return 0;
}

//==============================================================================

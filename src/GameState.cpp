#include "GameState.h"
#include <algorithm>
#include <iostream>
#include "Entities/Crate.h"

using std::get;



GameState::GameState()
{

    startTime = std::chrono::steady_clock::now();
    StateID=1;
    Crate::seed(10);

}

/**
 * Runs the simulation and paces it so it doesn't overheat your CPU or
 * Play at different speeds depending on load
 */
void GameState::run()
{
    while(Running)
    {
        if(std::chrono::steady_clock::now() - startTime > StateID * std::chrono::microseconds(usPerTick))
            advanceState();
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

}

void GameState::stop()
{
    Running = false;
}


/**
 * Advances the physics one tick into the future
 */
void GameState::advanceState()
{

    sysMutex.lock();
    system.tickSimulation(); //Do this multiple times if necessary
    StateID++;
    sysMutex.unlock();
}

/**
 * Locking functions to ensure no weird vector reallocations happen while the state is being drawn
 */
const System* GameState::lockMostRecentSystem()
{
    sysMutex.lock();
    return &system;


}

void GameState::unlockMostRecentSystem()
{
    sysMutex.unlock();
}

/**
 * Fancy C++ stuff to make starting a physics thread look simpler
 */
void GameState::operator()()
{
    run();
}

/**
 * Takes in a lambda function and executes it on the player's Ship
 */
void GameState::changePlayerShip(std::function<void(Ship&)> func) {
    func(system.playerShip);
}


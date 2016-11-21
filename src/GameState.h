#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_


#include <vector>
#include <utility>
#include <array>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include "System.h"
#include <queue>
#include <functional>
#include <chrono>
#include <tuple>
#include "Entities/Ship.h"



/**
 * Represents a game state. Old code that should be gotten rid of completely
 */
class GameState {

	public:
        GameState();
        void run();
        void stop();
        void advanceState();
        const System* lockMostRecentSystem();
        void unlockMostRecentSystem();
        void operator()();

		void changePlayerShip(std::function<void(Ship&)> func);



    private:
        System system;
        bool Running = true;
        std::mutex sysMutex;
        int StateID;


        int usPerTick = 15000;
        std::chrono::time_point<std::chrono::steady_clock> startTime;

};

//==============================================================================

#endif

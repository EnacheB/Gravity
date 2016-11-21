
#include <iostream>
#include "cute_runner.h"
#include "ide_listener.h"
#include "cute.h"
#include "../src/System.h"
#include "../src/Entities/Ship.h"


//Checks that spawnWave makes ships
void systemSpawnsWave() {
    System mySystem;
    ASSERT_EQUAL(1,mySystem.ships.size());
    mySystem.spawnWave();
    ASSERT_GREATER(mySystem.ships.size() , 1);
}

//Checks that the system takes care of collisions
void systemDetectsCollisions(){
    System mySystem;
    mySystem.ships.emplace_back( Ship(Tuple(0,0)));
    mySystem.projectiles.emplace_back(Projectile(10,1000));
    mySystem.projectiles[0].addPosition(Tuple(100,100));
    ASSERT_EQUAL(1,mySystem.projectiles.size());
    mySystem.tickSimulation();
    ASSERT_EQUAL(0,mySystem.projectiles.size());
}

//Checks that AI ships move towards the player
void AIChasesPlayer() {
    System mySystem;
    mySystem.playerShip = Ship(Tuple(-1000,0));
    mySystem.ships.clear();
    mySystem.ships.emplace_back(Ship(Tuple(100000,0)));
    for(int i = 0; i < 100; i++)
        mySystem.tickSimulation();
    ASSERT_EQUAL(1,mySystem.ships.size());
    ASSERT_GREATER(90000,mySystem.ships[0].getPosition().x);


}

void testRunner() {
    using namespace std;
    cute::ide_listener<> lis;
    cute::runner<cute::ide_listener<> > run = cute::makeRunner(lis);
    run(systemSpawnsWave);
    run(systemDetectsCollisions);
    run(AIChasesPlayer);
}

int main() {
    testRunner();
}

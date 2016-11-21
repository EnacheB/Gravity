
#include <iostream>
#include "cute_runner.h"
#include "ide_listener.h"
#include "cute.h"
#include "../src/Entities/Ship.h"


//Checks that ships take damage when a projectile hits them
void shipTakesDamage() {
    Ship myShip = Ship(Tuple(2,2));
    ASSERT_LESS(0.9 , myShip.getShieldFraction());
    myShip.Damage(20);
    ASSERT_GREATER(0.9 , myShip.getShieldFraction());
    myShip.Damage(9001);
    ASSERT_EQUAL_DELTA(0.0, myShip.getMass(), 0.01);
}

//Checks that ships create projectiles when firing
void shipFiresWeapon() {
    Ship myShip = Ship(Tuple(2,2));
    myShip.useWeaponFire();
    ASSERT(myShip.willFire());
    ASSERT_GREATER(0.01 , myShip.getHeatFraction());
    myShip.fireWeapon();
    ASSERT_LESS(0.01 , myShip.getHeatFraction());
    ASSERT(!myShip.willFire());
}


void testRunner() {
    using namespace std;
    cute::test successfulTest = CUTE(shipTakesDamage);
    cute::ide_listener<> lis;
    cute::runner<cute::ide_listener<> > run = cute::makeRunner(lis);
    if (run(shipTakesDamage)) {
        cout << "Ship Takes Damage" << endl;
    } else {
        cout << "FAILED: Ship Takes Damage" << endl;
    }

    if (run(shipFiresWeapon)) {
        cout << "Ship Fires Weapon" << endl;
    } else {
        cout << "FAILED: Ship Fires Weapon" << endl;
    }
}

int main() {
    testRunner();
}

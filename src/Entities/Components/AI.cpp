//
// Created by geeny on 4/21/16.
//

#include <iostream>
#include "AI.h"
#include "../Ship.h"
#include "../../System.h"

/**
 * Currently makes ships chase the player and shoot at him
 */
void AI::doIntelligentThings(System *system, Ship *myShip) {
    if(system == nullptr)
        return;
    double dx = system->playerShip.getPosition().x - myShip->getPosition().x;
    double dy = system->playerShip.getPosition().y - myShip->getPosition().y;
    double targetAngle = atan2(dy, dx) - 2*PI;
    myShip->useWeaponFire(targetAngle);
    double dTheta = targetAngle - myShip->getAngle();
    while(dTheta > PI)
        dTheta -= 2*PI;
    while(dTheta < -1 * PI)
        dTheta += 2*PI;

    myShip->useEnginesTurnOff();
    if(dTheta > 0.15)
        myShip->useEnginesTurnRight();
    else if (dTheta < -0.15)
        myShip->useEnginesTurnLeft();

    if(dx * dx + dy * dy > idealTargetRange * idealTargetRange)
        myShip->useEnginesForwardFull();
    else
        myShip->useEnginesForwardOff();

}

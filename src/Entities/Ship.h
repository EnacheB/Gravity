//
// Created by geeny on 4/7/16.
//

#ifndef POLYGON_SHIP_H
#define POLYGON_SHIP_H
#define PI           3.14159265358979323846

#include "Entity.h"
#include "Projectile.h"
#include "Components/Weapon.h"
#include "Components/Cartridge.h"
#include "Components/AI.h"
#include <vector>

class System;
class AI;
/**
 * Represents a ship controllable by either the player or the AI
 */
class Ship: public Entity {

public:
    Ship(Tuple pPosition);
    Ship(Tuple pPosition, AI pBrain);
    Ship(Tuple pPosition, int pMaxHull, int pMaxShield,int pShieldRegenMod, int pMaxThrust, double pTurnSpeed);
    void tickShip(System* pSystem);
    void useEnginesForwardFull();
    void useEnginesForwardOff();
    void useEnginesTurnLeft();
    void useEnginesTurnRight();
    void useEnginesNoTurnLeft();
    void useEnginesNoTurnRight();
    void useEnginesTurnOff();
    void useWeaponFire();
    void useWeaponFire(double targetAngle);
    void useWeaponNoFire();

    bool getEnginesOn() const {return enginesOn;}
    double getAngle() const {return angle;}
    double getHullFraction() const {return hull / (double) maxHull;}
    double getShieldFraction() const {return shield / (double) maxShield;}
    double getHeatFraction() const {return weapon.getHeat() / (double) weapon.getMaxHeat();}
    Weapon getWeaponCopy() const {return weapon;}

    void setWeapon(Weapon pWeapon);
    void cannibalizeWeapon(Weapon pWeapon);

    bool willFire();
    std::vector<Projectile> fireWeapon();

    void Damage(int pDamage);


private:
    int maxHull;
    int hull;

    int maxShield;
    int shield;
    int shieldRegenMod;

    double maxThrust; //Given in Newtons

    double angle; //in Radians
    double turnSpeed; //Radians per simulation tick
    double weaponAngle;
    bool freeWeapon = false;


    //Ship State
    bool enginesOn;
    bool turningLeft;
    bool turningRight;
    bool firing;

    double dragCoefficient;

    AI brain;
    Weapon weapon;
    Cartridge cartridge;





};


#endif //POLYGON_SHIP_H

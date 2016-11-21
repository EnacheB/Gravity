//
// Created by geeny on 4/7/16.
//

#include <iostream>
#include "Ship.h"
#include "Crate.h"

Ship::Ship(Tuple pPosition) : weapon(Crate::generateWeapon()) {
    position = pPosition;
    maxHull = 100;
    maxShield = 100;
    shieldRegenMod = 15;

    collisionRange = 8000;

    maxThrust = 500;
    mass = 5000;
    turnSpeed = 0.001;


    dragCoefficient = 0.3;

    enginesOn = false;
    turningLeft = false;
    turningRight = false;
    firing = false;

    hull = maxHull;
    shield = maxShield;

    weapon.load(Crate::generateCartridge());
}

Ship::Ship(Tuple pPosition, int pMaxHull, int pMaxShield,int pShieldRegenMod, int pMaxThrust, double pTurnSpeed)
        : weapon(Crate::generateWeapon()) {

    position = pPosition;
    maxHull = pMaxHull;
    maxShield = pMaxShield;
    shieldRegenMod = pShieldRegenMod;

    collisionRange = 8000;

    maxThrust = pMaxThrust;
    mass = 5000;
    turnSpeed = pTurnSpeed;


    dragCoefficient = 0.3;

    enginesOn = false;
    turningLeft = false;
    turningRight = false;
    firing = false;

    hull = maxHull;
    shield = maxShield;

    weapon.load(Crate::generateCartridge());
}

Ship::Ship(Tuple pPosition, AI pBrain) : Ship(pPosition) {
    brain = pBrain;
}

void Ship::useEnginesForwardFull()
{
    enginesOn = true;
}

void Ship::useEnginesForwardOff()
{
    enginesOn = false;
}

void Ship::useEnginesTurnLeft()
{
    turningLeft = true;
}

void Ship::useEnginesTurnRight()
{
    turningRight = true;
}

void Ship::useEnginesNoTurnLeft()
{
    turningLeft = false;
}

void Ship::useEnginesNoTurnRight()
{
    turningRight = false;
}

void Ship::useEnginesTurnOff()
{
    turningLeft = false;
    turningRight = false;
}

void Ship::useWeaponFire() {
    firing = true;
    freeWeapon = false;
}
void Ship::useWeaponNoFire() {
    firing = false;
    freeWeapon = false;
}

/**
 * Applies engine forces and drag to the ship
 */
void Ship::tickShip(System* pSystem) {
    brain.doIntelligentThings(pSystem, this);

    if (enginesOn) {
        acceleration.x += (maxThrust * cos(angle)) / mass;
        acceleration.y += (maxThrust * sin(angle)) / mass;
    }

    if (fabs(velocity.x) > 0.01 || fabs(velocity.y) > 0.01) {
        double velocitySquared = velocity.x * velocity.x + velocity.y * velocity.y;
        double dragForce = 0.1 * velocitySquared * dragCoefficient;
        double velAngle = atan2(velocity.y, velocity.x);
        acceleration.x -= (dragForce * cos(velAngle)) / mass;
        acceleration.y -= (dragForce * sin(velAngle)) / mass;
    }

    if(turningLeft)
        angle -= turnSpeed * timeScale;
    if(turningRight)
        angle += turnSpeed * timeScale;
    weapon.cool();
    if(angle >   2*PI)
        angle -= 2*PI;
    if(angle <  -2*PI)
        angle += 2*PI;
    if(shield < maxShield && lifeTime % shieldRegenMod == 0)
        shield++;
}

void Ship::Damage(int pDamage) {
    shield -= pDamage;
    if(shield < 0)
    {
        hull += shield;
        shield = 0;
    }
    if(hull < 0)
    {
        hull = 0;
        Destroy();
    }
}



/**
 * Checks if a ship wants to fire its weapon
 */
 bool Ship::willFire() {
     return firing && weapon.canFire();

 }

/**
 * Constructs the projectiles fired from a gun
 */
std::vector<Projectile> Ship::fireWeapon() {
    if(!freeWeapon)
        weaponAngle = angle;

    auto projectiles =  weapon.fire(collisionRange, weaponAngle);
    for(auto &projectile : projectiles) {
        projectile.addVelocity(velocity);
        projectile.addPosition(position);
    }

    return projectiles;
}

/**
 * Sets a ship's weapon
 */
void Ship::setWeapon(Weapon pWeapon) {
    weapon = pWeapon;
    weapon.load(Crate::generateCartridge());
}

void Ship::useWeaponFire(double targetAngle) {
    firing = true;
    freeWeapon = true;
    weaponAngle = targetAngle;

}

void Ship::cannibalizeWeapon(Weapon pWeapon) {
    weapon.cannibalize(pWeapon);

}

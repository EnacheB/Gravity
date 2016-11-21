//
// Created by geeny on 4/13/16.
//

#include "Weapon.h"
#include <iostream>

/**
 * Constructor that determines its properties
 */
Weapon::Weapon(double pSpreadAngle, int pProjectilesPerShot, int pHeatPerShot, int pMaxHeat,
               int pCooldownRate, int pTicksPerShot) {

    spread = pSpreadAngle;
    updateAngleDistribution();

    projectilesPerShot = pProjectilesPerShot;
    heatPerShot = pHeatPerShot;

    heat = 0;
    cooldownRate = pCooldownRate;
    maxHeat = pMaxHeat;

    ticksPerShot = pTicksPerShot;
    ticksToNextShot = 0;
}

/**
 * Links a projectile type to a weapon
 */
void Weapon::load(Cartridge pCartridge) {
    cartridge = pCartridge;
}

/**
 * Checks if heat and firerate constraints are satisfied;
 */
bool Weapon::canFire() {
    if(ticksToNextShot == 0 && heat == 0)
        return true;
    return ticksToNextShot == 0 &&
            heat + projectilesPerShot * cartridge.getHeatPerProjectile() + heatPerShot < maxHeat;
}

/**
 * Called every tick to keep track of firerate and heat
 */
void Weapon::cool() {
    if(heat > 0)
        heat -= cooldownRate;
    else
        heat = 0;
    if(ticksToNextShot > 0)
        ticksToNextShot--;
}

/**
 * Constructs the projectiles that would shot from the gun. Does not include the
 * Velocity and position of the firing ship
 */
std::vector<Projectile> Weapon::fire(double additionalSpawnOffset, double angle) {
    std::vector<Projectile> projectiles;
    for(int i = 0; i < projectilesPerShot; i++) {
        auto projectile = cartridge.generateProjectile(additionalSpawnOffset, angle + angleOffsetMaker());
        projectiles.push_back(projectile);

    }
    heat += projectilesPerShot * cartridge.getHeatPerProjectile() * spreadHeatMod;
    ticksToNextShot = ticksPerShot;
    return projectiles;

}

/**
 * Copy constructor
 */
Weapon::Weapon(const Weapon &weapon) {
    spread = weapon.spread;
    updateAngleDistribution();

    projectilesPerShot = weapon.projectilesPerShot;
    heatPerShot = weapon.heatPerShot;

    heat = 0;
    cooldownRate = weapon.cooldownRate;
    maxHeat = weapon.maxHeat;

    ticksPerShot = weapon.ticksPerShot;
    ticksToNextShot = 0;

}

void Weapon::cannibalize(Weapon otherWeapon) {
    /*
    spread = spread + otherWeapon.spread/2;
    projectilesPerShot = std::max(projectilesPerShot, otherWeapon.projectilesPerShot);
    heatPerShot = std::min(heatPerShot, otherWeapon.heatPerShot);
    cooldownRate = std::max(otherWeapon.cooldownRate, cooldownRate);
    maxHeat = std::max(otherWeapon.maxHeat, maxHeat);
    ticksPerShot = std::min( otherWeapon.ticksPerShot, ticksPerShot);
     */
    if(otherWeapon.projectilesPerShot > projectilesPerShot)
        projectilesPerShot = otherWeapon.projectilesPerShot;
    else
        projectilesPerShot++;
    spread = spread + otherWeapon.spread/2;
    heatPerShot = std::min(heatPerShot, otherWeapon.heatPerShot);
    if(otherWeapon.cooldownRate > 1)
        cooldownRate++;
    maxHeat += otherWeapon.maxHeat;
    ticksPerShot = std::min( otherWeapon.ticksPerShot, ticksPerShot);
}

void Weapon::updateAngleDistribution() {
    distribution = std::uniform_real_distribution<double>(-spread/2,spread/2);
    angleOffsetMaker = std::bind ( distribution, generator );
    spreadHeatMod = 0.01 +  (1- 0.01)/pow((1 + 1/exp(80*spread)),0.05);



}

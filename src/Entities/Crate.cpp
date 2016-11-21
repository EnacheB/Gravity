//
// Created by geeny on 4/14/16.
//

#include <iostream>
#include "Crate.h"

/**
 * randommly generates a cartridge
 */
Cartridge Crate::generateCartridge() {
    int damage = (2*normal_dist(generator)) + 10;
    int damageRange = 3000 * normal_dist(generator) + 15000;
    double velocity = 10 * normal_dist(generator) + 150;
    return Cartridge(damage, damageRange, velocity);
}

/**
 * randomly generates a weapon
 */
Weapon Crate::generateWeapon() {
    int heatPerShot = 10;

    int cooldownRate = poisson_dist(generator) + 1;
    int maxHeat = 20 * normal_dist(generator) + 100;

    int ticksPerShot = 5 * normal_dist(generator) + 20;
    double spreadAngle = fabs(normal_dist(generator)/2);//gamma_dist(generator) ;
    int projectilesPerShot = spreadAngle * 5 + 1;
    if(projectilesPerShot <= 0)
        projectilesPerShot = 1;
    return Weapon(spreadAngle,projectilesPerShot,heatPerShot,maxHeat,cooldownRate,ticksPerShot);
}

/**
 * Seeds the random number generator
 */
void Crate::seed(unsigned long pSeed) {
    generator.seed(pSeed);

}

std::normal_distribution<double> Crate::normal_dist = std::normal_distribution<double>(0.0,1.0);
std::poisson_distribution<int> Crate::poisson_dist = std::poisson_distribution<int>(0.2);
std::gamma_distribution<double> Crate::gamma_dist = std::gamma_distribution<double>(1,0.4);
std::default_random_engine Crate::generator = std::default_random_engine();


Weapon Crate::getContainedWeapon() { return weapon; }

/**
 * Constructor that sets its location and bounty
 */
Crate::Crate(Tuple pPosition, Weapon pWeapon) {
    position = pPosition;
    weapon = pWeapon;
    mass = 1.0;
    collisionRange = 15000;
}

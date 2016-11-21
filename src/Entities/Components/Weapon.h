//
// Created by geeny on 4/13/16.
//

#ifndef POLYGON_WEAPON_H
#define POLYGON_WEAPON_H


#include "Cartridge.h"
#include <vector>
#include <random>
#include <functional>
#include "../Projectile.h"

/**
 * Represents a weapon on a ship
 */
class Weapon {
public:
    Weapon():Weapon(0.0,1,10,100,1,15) {}
    Weapon(double pSpreadAngle, int pProjectilesPerShot, int pHeatPerShot, int pMaxHeat,
                   int pCooldownRate, int pTicksPerShot);
    Weapon( const Weapon &wep);

    void cannibalize(Weapon otherWeapon);


    void load(Cartridge pCartridge);
    bool canFire();
    void cool();
    std::vector<Projectile> fire(double additionalSpawnOffset, double angle);

    int getHeat() const {return heat;}
    int getMaxHeat() const {return maxHeat;}

private:

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;
    std::function<double()> angleOffsetMaker;

    void updateAngleDistribution();

    double spread;
    double spreadHeatMod;
    int projectilesPerShot;
    int heatPerShot;
    Cartridge cartridge;

    int heat;
    int cooldownRate;
    int maxHeat;

    int ticksPerShot;
    int ticksToNextShot;
};


#endif //POLYGON_WEAPON_H

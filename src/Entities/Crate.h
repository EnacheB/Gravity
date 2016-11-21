//
// Created by geeny on 4/14/16.
//

#ifndef POLYGON_CRATE_H
#define POLYGON_CRATE_H


#include "Entity.h"
#include "Components/Weapon.h"

/**
 * Represents a object that can be picked up
 * Also contains the random generation methods
 * For weapons and cartridges
 */
class Crate: public Entity {
    static std::default_random_engine generator;
    static std::normal_distribution<double> normal_dist;
    static std::poisson_distribution<int> poisson_dist;
    static std::gamma_distribution<double> gamma_dist;

    Weapon weapon;
public:
    Crate(Tuple pPosition, Weapon pWeapon);
    static Cartridge generateCartridge();

    static Weapon generateWeapon();

    static void seed(unsigned long pSeed);

    Weapon getContainedWeapon();

};


#endif //POLYGON_CRATE_H

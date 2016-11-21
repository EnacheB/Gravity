//
// Created by geeny on 4/13/16.
//

#ifndef POLYGON_CARTRIDGE_H
#define POLYGON_CARTRIDGE_H


#include "../Projectile.h"

/**
 * Represents a factory for a certain bullet type
 */
class Cartridge {
public:

    Cartridge(int pDamage, int pDamageRange, double pVelocity);
    Cartridge(): Cartridge(10,10000,100) {}

    Projectile generateProjectile( double pAdditionalOffset, double pAngle);

    int getHeatPerProjectile();


private:
    int heatPerProjectile;
    int damage;
    int damageRange;
    double velocity;


};


#endif //POLYGON_CARTRIDGE_H

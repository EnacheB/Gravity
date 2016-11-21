//
// Created by geeny on 4/13/16.
//

#include "Cartridge.h"
#include <cmath>

int Cartridge::getHeatPerProjectile() {
    return heatPerProjectile;
}

/**
 * Makes a a projectile in the direction of angle, offset by additionalOffset in distance
 */
Projectile Cartridge::generateProjectile(double additionalOffset, double pAngle) {
    additionalOffset += additionalOffset* 0.1 + damageRange;
    auto projectile = Projectile(damage,damageRange );
    double angleCos = cos(pAngle);
    double angleSin = sin(pAngle);
    Tuple tVelocity(angleCos * velocity, angleSin * velocity);
    projectile.addVelocity(tVelocity);
    Tuple tPosition(angleCos * additionalOffset, angleSin * additionalOffset);
    projectile.addPosition(tPosition);
    return projectile;
}

/**
 * Constructor that determines the properties of constructed bullets
 */
Cartridge::Cartridge(int pDamage, int pDamageRange, double pVelocity) {
    damage = pDamage;
    damageRange = pDamageRange;
    velocity = pVelocity;
    heatPerProjectile = damage * damageRange* velocity/(10000*100);
}



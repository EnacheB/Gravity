//
// Created by geeny on 4/8/16.
//

#include "Projectile.h"
#include "Entity.h"

/**
 * Projectile constructs itself based on properties of its shooter
 */
Projectile::Projectile(int pDamage, double pDamageRange)
{
    damage = pDamage;
    collisionRange = pDamageRange;
    mass = 1.0;

}

void Projectile::addVelocity(Tuple pVelocity) {
    velocity.x += pVelocity.x;
    velocity.y += pVelocity.y;
}

void Projectile::addPosition(Tuple pPosition) {
    position.x += pPosition.x;
    position.y += pPosition.y;
}

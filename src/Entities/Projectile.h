//
// Created by geeny on 4/8/16.
//

#ifndef POLYGON_PROJECTILE_H
#define POLYGON_PROJECTILE_H

#include "Entity.h"

/**
 * Represents some object that does damage when it collides with a Ship
 */
class Projectile: public Entity {

    int damage;


public:
    Projectile(int pDamage, double pDamageRange);
    void addVelocity(Tuple velocity);
    void addPosition(Tuple position);


    int getDamage() const {return damage;}

};


#endif //POLYGON_PROJECTILE_H

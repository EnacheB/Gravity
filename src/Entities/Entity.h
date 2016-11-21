//
// Created by geeny on 4/7/16.
//

#ifndef POLYGON_ENTITY_H
#define POLYGON_ENTITY_H


#include "../Utilities/Tuple.h"

/**
 * Represents some moving physical Entity in the world
 */
class Entity {
protected:
    double mass;
    Tuple position;
    Tuple velocity;
    Tuple acceleration;

    Tuple oldAcceleration; //Used for physics

    static const int timeScale = 50;
    long int lifeTime = 0;

    double collisionRange;
public:

    void updatePosition();
    void updateVelocity();

    Tuple getPosition() const {return position;}
    double getMass() const {return mass;}
    double getCollisionRange() const {return collisionRange;}
    void Destroy() {mass = 0.0;}


};


#endif //POLYGON_ENTITY_H

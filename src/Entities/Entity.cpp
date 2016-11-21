

#include "Entity.h"

void Entity::updatePosition() {
    position.x = position.x + velocity.x * timeScale + acceleration.x * timeScale * timeScale / 2;
    position.y = position.y + velocity.y * timeScale + acceleration.y * timeScale * timeScale / 2;


    oldAcceleration = acceleration;
    acceleration = Tuple(0,0);
    lifeTime++;
}

void Entity::updateVelocity() {
    velocity.x = velocity.x + timeScale * (acceleration.x + oldAcceleration.x)/2;
    velocity.y = velocity.y + timeScale * (acceleration.y + oldAcceleration.y)/2;
}

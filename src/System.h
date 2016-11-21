#ifndef _SYSTEM_H_
    #define _SYSTEM_H_


#include <vector>
#include <utility>
#include "Entities/Ship.h"
#include "Entities/Crate.h"


/**
 * Represents the physical ships and projectiles in play
 */
class System {

	public:
    System();
    void tickSimulation();
    void spawnWave();
    double getKillRadius() const { return killRadius;}



    std::vector<Ship> ships;
    std::vector<Projectile> projectiles;
    std::vector<Projectile> AIprojectiles;
    std::vector<Crate> crates;
    Ship playerShip;

private:
    void applyForces();
    void updatePositions();
    void updateVelocities();
    void collideAndClean();
    void fireProjectiles();
    void Collide(Ship& S, Projectile& P);
    void cleanProjectiles(std::vector<Projectile> &projs);
    void CollectCrates(Ship& ship, Crate& crate);

    double killRadius = 1e6;
    int wave = 0;

};

//==============================================================================

#endif

#include "System.h"
#include "Entities/Projectile.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>


System::System() : playerShip(Tuple(0,0)) {
    spawnWave();
}

void System::spawnWave() {
    if(wave == 10) {
        std::cout << "Game completed" << std::endl;
    }

    wave++;
    double dAngle = 2 * M_PI / wave;
    double angle = 0.001;

     std::default_random_engine generator;
    generator.seed(wave);
    auto distribution = std::uniform_int_distribution<int>(0,1000);
    auto angleDist = std::uniform_real_distribution<double>(0.0015,0.005);
    std::function<double()> randGen = std::bind ( distribution, generator );

    while(angle < 2*M_PI) {
        ships.emplace_back(Tuple(cos(angle) * killRadius/2,sin(angle) * killRadius / 2),
                           randGen() / 5+10,randGen() /20 + 10, randGen()/70 + 15,randGen()/7 + 150
                           ,angleDist(generator)   );
        angle += dAngle;
    }
    if(wave > 3)
        for(auto &ship : ships)
            ship.cannibalizeWeapon(Crate::generateWeapon());
    if(wave > 5)
        for(int i = wave; i > 5; i--)
            ships[4].cannibalizeWeapon(Crate::generateWeapon());



}

void System::updatePositions()
{
    playerShip.updatePosition();
    for(auto &ship : ships)
        ship.updatePosition();
    for(auto &projectile : projectiles)
        projectile.updatePosition();
    for(auto &projectile : AIprojectiles)
        projectile.updatePosition();
}

/**
 * Handles collisions between objects and removes them if they have zero mass
 */
void System::collideAndClean()
{
    for(auto &proj : projectiles) {
        Collide(playerShip, proj);
        for (auto &ship: ships) {
            Collide(ship, proj);
        }
    }
    for(auto &proj : AIprojectiles) {
        Collide(playerShip, proj);
    }

    cleanProjectiles(AIprojectiles);
    cleanProjectiles(projectiles);

    //Damages ships that are out of bounds and deletes those that have died
    auto shipEnd = std::remove_if(ships.begin(),    ships.end(),  [this](Ship &B){
                if(B.getPosition().x * B.getPosition().x + B.getPosition().y * B.getPosition().y
                   > this->killRadius * this->killRadius)
                    B.Damage(1);
            return B.getMass() == 0.0;});
    if(playerShip.getPosition().x * playerShip.getPosition().x + playerShip.getPosition().y * playerShip.getPosition().y
                   > this->killRadius * this->killRadius)
                    playerShip.Damage(1);
    for(auto shipIter = shipEnd; shipIter < ships.end();shipIter++)
    {
        crates.emplace_back(shipIter->getPosition(), shipIter->getWeaponCopy());

    }
    ships.erase(shipEnd  ,  ships.end());
    if(ships.size() == 0)
        spawnWave();

    for(auto &crate : crates)
    {
        CollectCrates(playerShip,crate);
        for(auto &ship: ships)
        {
            CollectCrates(ship,crate);
        }
    }

     auto crateEnd = std::remove_if(crates.begin(),    crates.end(),  [this](const Crate &B){
            return B.getMass() == 0.0;});
    crates.erase(crateEnd    ,        crates.end());



}

void System::cleanProjectiles(std::vector<Projectile> &projs) {



    //Deletes projectiles that are out of bounds or have hit a target
    auto projEnd = std::remove_if(projs.begin(),    projs.end(),  [this](const Projectile &B){
        if(B.getPosition().x * B.getPosition().x + B.getPosition().y * B.getPosition().y > this->killRadius * this->killRadius)
            return true;
        return B.getMass() == 0.0;});
    projs.erase(projEnd    ,        projs.end());


}


void System::updateVelocities()
{
    playerShip.updateVelocity();
    for(auto &ship : ships)
        ship.updateVelocity();
    for(auto &projectile : projectiles)
        projectile.updateVelocity();
    for(auto &projectile : AIprojectiles)
        projectile.updateVelocity();

}

/**
 * Applies things like engine acceleration, drag, etc.
 */
void System::applyForces()
{
    playerShip.tickShip(nullptr);
    for(auto &ship : ships)
        ship.tickShip(this);

}


/**
 * Orchestrates physics for each tick
 */
void System::tickSimulation() {


    updatePositions();
    collideAndClean();
    fireProjectiles();

    applyForces();
    updateVelocities();

}



void System::fireProjectiles() {

    if(playerShip.willFire())
    {
        auto newProjs = playerShip.fireWeapon();
        projectiles.insert(projectiles.end(), newProjs.begin(), newProjs.end());
    }
    for(auto &ship : ships)
        if(ship.willFire())
        {
            auto newProjs = ship.fireWeapon();
            AIprojectiles.insert(AIprojectiles.end(), newProjs.begin(), newProjs.end());
        }
}


/**
 * Handles collision logic between a ship and a projectile
 */
void System::Collide(Ship& ship, Projectile& proj) {
    if(proj.getMass() == 0 || ship.getMass() == 0) return;

    double dx = proj.getPosition().x-ship.getPosition().x;
    double dy = proj.getPosition().y-ship.getPosition().y;
    double separationSquared = dx*dx+dy*dy;
    //May be inaccurate with verlet
    double totalDamageRange = proj.getCollisionRange() + ship.getCollisionRange();
    if(separationSquared < (totalDamageRange * totalDamageRange))
    {
        ship.Damage(proj.getDamage());
        proj.Destroy();
    }

}


/**
 * Checks if a ship and a crate can interact. If they do, give the ship the picked up weapon
 */
void System::CollectCrates(Ship &ship, Crate &crate) {
    if( ship.getMass() == 0) return;

    double dx = crate.getPosition().x-ship.getPosition().x;
    double dy = crate.getPosition().y-ship.getPosition().y;
    double separationSquared = dx*dx+dy*dy;
    //May be inaccurate with verlet
    double totalDamageRange = ship.getCollisionRange() + crate.getCollisionRange();
    //std::cout << totalDamageRange * totalDamageRange << " " << separationSquared << std::endl;
    if(separationSquared < totalDamageRange * totalDamageRange)
    {
        ship.cannibalizeWeapon(crate.getContainedWeapon());
        crate.Destroy();
    }


}

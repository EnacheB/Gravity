//
// Created by geeny on 4/21/16.
//

#ifndef POLYGON_AI_H
#define POLYGON_AI_H


class Ship;
class System;


/**
 * Class that is a brain for a ship
 * Can hold relevant data, and different functions
 * Which can be changed using parameters or inheritance
 */
class AI {

private:
    double idealTargetRange = 100000;
public:
    void doIntelligentThings(System* system, Ship* ship);


};


#endif //POLYGON_AI_H

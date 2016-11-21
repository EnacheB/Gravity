//
// Created by geeny on 4/7/16.
//

#ifndef POLYGON_TUPLE_H
#define POLYGON_TUPLE_H


/**
 * Represents a tuple of doubles, mostly used for x,y physics stuff
 */
struct Tuple {
    Tuple(double pX, double pY) : x(pX) , y(pY) {}
    Tuple() : x(0.0), y(0.0) {}
    double x;
    double y;
};


#endif //POLYGON_TUPLE_H

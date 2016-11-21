//
// Created by geeny on 4/8/16.
//

#ifndef POLYGON_COLOR_H
#define POLYGON_COLOR_H


#include <SDL_stdinc.h>

/**
 * Represents an RGB color, for graphics purposes
 */
struct Color {
    Color(Uint8 pr, Uint8 pg, Uint8 pb) : r(pr), g(pg), b(pb) {}
    Color() : r(0), g(0), b(0) {}
    Uint8 r;
    Uint8 g;
    Uint8 b;
};


#endif //POLYGON_COLOR_H

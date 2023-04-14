#ifndef GRID_H
#define GRID_H

#include <core/containers/array.h>

class Grid
{
public:
    enum {
        EMPTY = 999,
        FILLED,
        ERROR
    };

    Grid();
    Grid(unsigned int width_, unsigned int height_);
    unsigned int &At(unsigned int i1, unsigned int i2);

    unsigned int width, height;
    Array<unsigned int> cells;
private:
    unsigned int error;
};

#endif // GRID_H

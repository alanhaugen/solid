#ifndef CELL_H
#define CELL_H

#include <core/containers/array.h>

class Cell
{
public:
    enum {
        EMPTY = 999,
        FILLED,
        ERROR
    };

    Cell();
    Cell(unsigned int width_, unsigned int height_);
    unsigned int &At(unsigned int i1, unsigned int i2);

    unsigned int width, height;
    Array<unsigned int> cells;
private:
    unsigned int error;
};

#endif // CELL_H

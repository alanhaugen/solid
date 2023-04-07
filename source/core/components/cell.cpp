#include "cell.h"

Cell::Cell()
{
    width = 0;
    height = 0;
    error = ERROR;
}

Cell::Cell(unsigned int width_, unsigned int height_)
{
    width = width_;
    height = height_;
    error = ERROR;

    cells.Resize(width * height);

    for (unsigned int i = 0; i < cells.Size(); i++)
    {
        cells[i] = EMPTY;
    }
}

unsigned int &Cell::At(unsigned int i1, unsigned int i2)
{
    if (i1 >= width || i2 >= height)
    {
        return error;
    }

    return cells[i1 * width + i2];
}

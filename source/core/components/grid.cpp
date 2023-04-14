#include "grid.h"

Grid::Grid()
{
    width = 0;
    height = 0;
    error = ERROR;
}

Grid::Grid(unsigned int width_, unsigned int height_)
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

unsigned int &Grid::At(unsigned int i1, unsigned int i2)
{
    if (i1 >= width || i2 >= height)
    {
        return error;
    }

    return cells[i1 * width + i2];
}

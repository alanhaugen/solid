#ifndef DRAWABLE_COMPONENT_H
#define DRAWABLE_COMPONENT_H

#include "core/components/component.h"

class Drawable : public Component
{
public:
    IDrawable* drawable;
};

#endif // DRAWABLE_COMPONENT_H

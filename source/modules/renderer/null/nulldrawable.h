#ifndef NULLDRAWABLE_H
#define NULLDRAWABLE_H

#include "modules/renderer/drawable.h"

class NullDrawable : public IDrawable
{
public:
    NullDrawable();

    void Hide();

    void Show();
};

#endif // NULLDRAWABLE_H

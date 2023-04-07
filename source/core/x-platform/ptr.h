#ifndef PTR_H
#define PTR_H

//! Inspired by https://docs.opencv.org/3.4/d0/de7/structcv_1_1Ptr.html#details

#include <stddef.h>

template<typename T>
class Ptr
{
private:
    T *object;

public:
    Ptr()
    {
        object = NULL;
    };

    Ptr(T *object_)
    {
        object = object_;
    };

    ~Ptr()
    {
        delete object;
    };
};

#endif // PTR_H

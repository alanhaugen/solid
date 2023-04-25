#ifndef PTR_H
#define PTR_H

//! Inspired by https://docs.opencv.org/3.4/d0/de7/structcv_1_1Ptr.html#details

#include <stddef.h>

template<typename T>
class Ptr
{
private:
    bool empty;
    unsigned int count;
    T *other;

public:
    T *object;

    Ptr()
    {
        object = NULL;
        other  = NULL;
        empty  = true;
        count  = 0;
    };

    Ptr(T *object_)
    {
        object = object_;
        other  = NULL;
        empty  = false;
        count  = 0;
    };

    Ptr(const T *object_)
    {
        object = object_;
        other  = NULL;
        empty  = false;
        count  = 0;
    };

    Ptr operator=(const Ptr &o)
    {
        other = o.object;
        count++;

        return *this;
    }

    ~Ptr()
    {
        if (count == 0)
        {
            delete object;
            object = NULL;
            empty  = true;
        }
        else
        {
            count--;

            if (count < 0)
            {
                delete object;
                object = NULL;
                empty  = true;
            }
        }
    };

    bool isEmpty()
    {
        return empty;
    }

    void Swap(Ptr &o)
    {
        T *temp = object;
        object = o.object;
        o.object = temp;
    }
};

#endif // PTR_H

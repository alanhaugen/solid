#ifndef PTR_H
#define PTR_H

//! Inspired by https://docs.opencv.org/3.4/d0/de7/structcv_1_1Ptr.html#details

#include <stddef.h>

template<typename T>
class Ptr
{
private:
    bool empty;

    struct Data
    {
        T object;
        Ptr *ptrWithDeleteResponsibility;

        Data(T object_, Ptr *ptr_)
        {
            object = object_;
            ptrWithDeleteResponsibility = ptr_;
        }
        ~Data()
        {
            delete object;
            object = NULL;
        }
    };

public:
    Data *data;

    Ptr()
    {
        data  = NULL;
        empty = true;
    };

    Ptr(T object_)
    {
        data  = new Data(object_, this);
        empty = false;
    };

    Ptr operator=(const Ptr &lhs)
    {
        Ptr *rhs = this;

        // Guard self assignment
        if (rhs == &lhs)
            return *this;

        // Take responsibility for object
        data = lhs.data;
        data->ptrWithDeleteResponsibility = rhs;
        empty = false;

        return *this;
    }

    ~Ptr()
    {
        if (this == data->ptrWithDeleteResponsibility)
        {
            delete data;
            data  = NULL;
            empty = true;
        }
    };

    bool isEmpty()
    {
        return empty;
    }

    void Swap(Ptr &o)
    {
        /*T *temp  = object;
        bool cpy = o.isCopy;
        object   = o.object;
        o.object = temp;
        o.isCopy = isCopy;
        isCopy   = cpy;*/
    }
};

#endif // PTR_H

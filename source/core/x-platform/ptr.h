#ifndef PTR_H
#define PTR_H

//! Inspired by https://docs.opencv.org/3.4/d0/de7/structcv_1_1Ptr.html#details

#include <stddef.h>

template<typename T>
class Ptr
{
private:
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
    bool empty;

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

    T operator*()
    {
        return data->object;
    }

    void ChangeResponsibility(Ptr *ptr_) const
    {
        if (data != NULL)
        {
            data->ptrWithDeleteResponsibility = ptr_;
        }
    }

    ~Ptr() // I
    {
        if (this == data->ptrWithDeleteResponsibility)
        {
            delete data;
            data  = NULL;
            empty = true;
        }
    };

    Ptr(const Ptr &other) // II
    {
        data = other.data;
        empty = false;
    }

    Ptr operator=(const Ptr &rhs) // III
    {
        Ptr *lhs = this;

        // Guard self assignment
        if (lhs == &rhs)
            return *this;

        // Take responsibility for object
        data = rhs.data;
        data->ptrWithDeleteResponsibility = lhs;
        empty = false;

        return *this;
    }

    bool isEmpty()
    {
        return empty;
    }

    void Swap(Ptr &o)
    {
        T *temp  = data;
        data     = o.object;
        o.object = temp;

        bool tempEmpty = empty;
        empty = o.empty;
        o.empty = tempEmpty;
    }
};

#endif // PTR_H

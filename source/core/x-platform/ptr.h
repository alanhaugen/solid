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
            if (object != NULL)
            {
                delete object;
                object = NULL;
            }
        }
    };

    bool isLocked;

public:
    Data *data;
    bool empty;

    Ptr()
    {
        data  = NULL;
        empty = true;
        isLocked = false;
    };

    Ptr(T object_)
    {
        data  = new Data(object_, this);
        empty = false;
        isLocked = false;
    };

    T operator*()
    {
        return data->object;
    }

    void ChangeResponsibility(Ptr *ptr_) const
    {
        if (data != NULL && isLocked == false)
        {
            data->ptrWithDeleteResponsibility = ptr_;
        }
    }

    ~Ptr() // I
    {
        if (this == data->ptrWithDeleteResponsibility) // TODO:  FIXME!!! MAJOR MEMORY LEAK!!1
        {
            if (data != NULL)
            {
                delete data;
                data  = NULL;
                empty = true;
            }
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
        isLocked = rhs.isLocked;
        ChangeResponsibility(lhs);
        empty = false;

        return *this;
    }

    bool isEmpty()
    {
        return empty;
    }

    bool Lock()
    {
        isLocked = true;
    }

    bool UnLock()
    {
        isLocked = false;
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

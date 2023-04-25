#ifndef PTR_H
#define PTR_H

//! Inspired by https://docs.opencv.org/3.4/d0/de7/structcv_1_1Ptr.html#details

#include <stddef.h>

template<typename T>
class Ptr
{
private:
    bool empty;

public:
    struct Data
    {
        T *object;
        bool isCopied;

        Data(T *object_)
        {
            object = object_;
            isCopied = false;
        }
        ~Data()
        {
            delete object;
            object = NULL;
        }
    };

    bool isCopy;
    Data *data;
    T *object;

    Ptr()
    {
        data   = NULL;
        object = NULL;
        isCopy = false;
        empty  = true;
    };

    Ptr(T *object_)
    {
        data   = new Data(object_);
        object = data->object;
        isCopy = false;
        empty  = false;
    };

    Ptr(const T *object_)
    {
        data   = new Data(object_);
        object = data->object;
        isCopy = false;
        empty  = false;
    };

    Ptr operator=(const Ptr &o)
    {
        isCopy = true;
        data = o.data;
        data->isCopied = true;
        object = data->object;
        return *this;
    }

    ~Ptr()
    {
        if (data->isCopied == false)
        {
            delete data;
            data  = NULL;
            empty = true;
        }
        else if (isCopy == true && empty == false)
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

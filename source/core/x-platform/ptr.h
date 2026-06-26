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
        int refCount;

        Data(T object_, Ptr *ptr_)
            : object(object_), ptrWithDeleteResponsibility(ptr_), refCount(1)
        {}
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

    ~Ptr()
    {
        if (data != NULL)
        {
            if (isLocked == false)
            {
                data->refCount--;
                if (data->refCount == 0 && this == data->ptrWithDeleteResponsibility)
                {
                    delete data;
                    data  = NULL;
                    empty = true;
                }
            }
        }
    };

    Ptr(const Ptr &other)
    {
        data = other.data;
        empty = !other.empty;
        isLocked = other.isLocked;
        if (data != NULL)
        {
            data->refCount++;
        }
    }

    Ptr operator=(const Ptr &rhs)
    {
        if (this != &rhs)
        {
            if (data != NULL)
            {
                if (isLocked == false)
                {
                    data->refCount--;
                    if (data->refCount == 0 && this == data->ptrWithDeleteResponsibility)
                    {
                        delete data;
                        data  = NULL;
                        empty = true;
                    }
                }
            }

            data = rhs.data;
            empty = !rhs.empty;
            isLocked = rhs.isLocked;
            if (data != NULL)
            {
                data->refCount++;
            }
        }

        return *this;
    }

    bool isEmpty()
    {
        return empty;
    }

    void Lock()
    {
        isLocked = true;
    }

    void UnLock()
    {
        isLocked = false;
    }

    void Swap(Ptr &o)
    {
        Data* temp = data;
        data = o.data;
        o.data = temp;

        bool tempEmpty = empty;
        empty = o.empty;
        o.empty = tempEmpty;

        bool tempLocked = isLocked;
        isLocked = o.isLocked;
        o.isLocked = tempLocked;
    }
};

#endif // PTR_H

#ifndef STACK_H
#define STACK_H

#include "linkedlist.h"

/*!
 * \brief The Stack container (LIFO structure)
 */
template <class Datatype>
class Stack : public LinkedList<Datatype> {
public:
    void Push(Datatype data)
    {
        Append(data);
    }

    Datatype Pop()
    {
        if (Empty())
        {
            return NULL;
        }

        Datatype data = lastNode_->element_;
        Remove(lastNode_);

        return data;
    }

    Datatype Top()
    {
        return lastNode_->element_;
    }

    int Size()
    {
        return count_;
    }

    int Empty() const
    {
        return count_ == 0;
    }
};

#endif

#ifndef QUEUE_H
#define QUEUE_H

#include "linkedlist.h"

/*!
 * \brief The Queue container (FIFO structure)
 */
template <class Datatype>
class Queue : public LinkedList<Datatype>
{
public:
    void Enqueue(Datatype data) {
        Append(data);
    }

    Datatype Dequeue() {
        Datatype data = firstNode_->element_;
        Remove(firstNode_);

        return data;
    }

    Datatype Front() {
        return firstNode_->element_;
    }

    int Count() {
        return count_;
    }
};

#endif

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
    void Enqueue(Datatype data)
    {
        LinkedList<Datatype>::Append(data);
    }

    Datatype Dequeue()
    {
        Datatype data = LinkedList<Datatype>::firstNode->element_;
        Remove(LinkedList<Datatype>::firstNode);

        return data;
    }

    Datatype Front()
    {
        return LinkedList<Datatype>::firstNode->element_;
    }
};

#endif

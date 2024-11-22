#ifndef STACK_H
#define STACK_H

#include "linkedlist.h"

/*!
 * \brief The Stack container (LIFO structure)
 */

template <class Datatype>
class Stack : public LinkedList<Datatype>
{
public:
    void Push(Datatype data)
    {
        LinkedList<Datatype>::Append(data);
    }

    Datatype Pop()
    {
        /*if (LinkedList<Datatype>::Empty())
        {
            return NULL;
        }*/

        Datatype data = LinkedList<Datatype>::lastNode->element;
        LinkedList<Datatype>::Remove(LinkedList<Datatype>::lastNode);

        return data;
    }

    Datatype Top()
    {
        return LinkedList<Datatype>::lastNode->element;
    }
};

#endif

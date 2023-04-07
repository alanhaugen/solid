#ifndef STACK_H
#define STACK_H

#include "linkedlist.h"

/*!
 * \brief The Stack container (FILO structure)
 */
template <class Datatype>
class Stack : public LinkedList<Datatype> {
public:
    void Push(Datatype data) {
        Append(data);
    }

    Datatype Pop() {
        Datatype data = lastNode_->element_;
        Remove(lastNode_);

        return data;
    }

    Datatype Top() {
        return lastNode_->element_;
    }

    int Count() {
        return count_;
    }
};

#endif

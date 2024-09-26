#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <cstddef>

/*!
 * \brief The Linked List container
 *
 * Iterators: Iterator, Begin, End
 * Modifiers: Append, RemoveAt, Clear
 *
 * Example usage:
 *
 * LinkedList<Sprite*> entities;
 *
 * LinkedList<Sprite*>::Iterator iterator(entities.Begin());
 *
 * for(; iterator != entities.End(); ++iterator) {
 *     (*iterator)->draw();
 * }
 */

template <class Datatype>
class LinkedList
{
private:
    class LinkedNode
    {
    private:
        LinkedNode *nextElement;
        LinkedNode *prevElement;

    public:
        Datatype element;

        LinkedNode(Datatype element)
        {
            this->element = element;

            nextElement = NULL;
            prevElement = NULL;
        }

        inline void SetNext(LinkedNode *node) { nextElement = node; }
        inline LinkedNode *GetNext() { return nextElement; }
        inline void SetPrev(LinkedNode *node) { prevElement = node; }
        inline LinkedNode *GetPrev() { return prevElement; }
    };

protected:
    LinkedNode *firstNode;
    LinkedNode *lastNode;

    unsigned int count;

    void Append(LinkedNode *node)
    {
        if (!firstNode)
        {
            firstNode = node;
            lastNode = node;
        }
        else
        {
            lastNode->SetNext(node); // Update next link of old last
            node->SetPrev(lastNode); // Point to old last
            lastNode = node; // Make this last
        }

        count++;
        lastNode = node;
    }

    void Remove(LinkedNode *node)
    {
        LinkedNode *nextNode = node->GetNext();
        LinkedNode *prevNode = node->GetPrev();

        if (prevNode != NULL)
        {
            prevNode->SetNext(nextNode);
        }

        if (nextNode != NULL)
        {
            nextNode->SetPrev(prevNode);
        }
        else
        {
            lastNode = prevNode;  // removing last node, set new last node.
        }

        if (prevNode == NULL)
        {
            firstNode = nextNode;
        }

        count--;
        delete node;
    }

public:
    LinkedList()
    {
        count = 0;
        firstNode = NULL;
        lastNode  = NULL;
    }

    ~LinkedList()
    {
        while (firstNode)
        {
            RemoveAt(0);
        }
    }

    // Iterators
    class Iterator
    {
    private:
        LinkedNode *curNode;
    public:
        Iterator(LinkedNode *node)
        {
            curNode = node;
        }

        inline Datatype *Item() { return &curNode->element; }

        inline Iterator &operator++() { curNode = curNode->GetNext(); return *this; }
        inline Iterator &operator--() { curNode = curNode->GetPrev(); return *this; }

        inline Datatype &operator*() { return curNode->element; } // Danger! Make sure to return a reference or we will cause copy constructor to be invoked on Datatype!
        inline Datatype *operator->() { return &curNode->element; } // Why overload arrow operator?

        bool operator==(const Iterator &a) const
        {
            return curNode == a.curNode;
        }

        bool operator!=(const Iterator &a) const
        {
            return curNode != a.curNode;
        }
    };

    Iterator Begin()
    {
        return firstNode;
    }

    Iterator End()
    {
        return lastNode->GetNext();
    }

    // Helper
    bool Empty()
    {
        return firstNode == NULL;
    }

    int Size() const
    {
        return count;
    }

    // Modifiers
    void Append(Datatype val)
    {
        // FIXME: Check if this fails and log error
        Append(new LinkedNode(val));
    }

    void RemoveAt(int index)
    {
        LinkedNode *curNode = firstNode;

        for(int k = 0; k < index; ++k)
        {
            curNode = curNode->GetNext();
        }

        Remove(curNode);
    }

    void Clear()
    {
        while (firstNode)
        {
            RemoveAt(0);
        }

        count = 0;
    }
};

#endif // LINKED_LIST_H

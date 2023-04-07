#ifndef HASH_H
#define HASH_H

#include <cstddef>
#include "array.h"
#include "linkedlist.h"
#include "core/x-platform/string.h"

#define DEFAULT_HASH_SIZE 31

/*!
 * \brief The Hash Map container
 *
 * example:
 *
 *   Hash<String, unsigned int> controls;
 *   controls.Insert(controlName, key);
 *
 * From Data Structures For Game Programmers, page 220:
 * TIP:
 * Hash Tables
 * It is actually more efficient to make your hash table sizes prime numbers. I
 * can’t really explain why this is without going into a whole discussion about discrete
 * mathematics, but you should generally try to find a prime number above
 * the desired number of items you store in the table. For example, if I wanted to
 * store 10 items in a hash table, I would make it 11 cells large. However, I use 10
 * cells throughout this chapter because it makes the hashing functions easier to
 * explain. I will go more in depth on this topic later on in this chapter.
 */
template <class Keytype, class Datatype>
class Hash
{
private:
    struct HashNode
    {
        Keytype key_;
        Datatype data_;
    };

protected:
    unsigned int size_;
    unsigned int count_;

    Array <LinkedList <HashNode> > table_;
    unsigned long int (*hashFunction_)(Keytype);

public:
    static unsigned long int StringHash(String string)
    {
        unsigned long int hash = 0;
        for (unsigned int i = 0; i < string.Length(); i++)
        {
            hash += ((i + 1) * string[i]);
        }
        return hash;
    }

    static unsigned long int NumberHash(Keytype i)
    {
        return i;
    }

    Hash(const unsigned int size, unsigned long int (*hashFunction)(Keytype))
        : table_(size)
    {
        size_ = size;
        count_ = 0;

        hashFunction_ = hashFunction;
    }

    ~Hash()
    {
    }

    void Insert(Keytype key, Datatype data)
    {
        HashNode entry;
        entry.key_  = key;
        entry.data_ = data;

        const unsigned int index = hashFunction_(key) % size_;

        table_[index].Append(entry);

        count_++;
    }

    HashNode *Find(Keytype key)
    {
        const unsigned int index = hashFunction_(key) % size_;

        typename LinkedList<HashNode>::Iterator iterator(table_[index].Begin());

        if (iterator != NULL)
        {
            for (; iterator != table_[index].End(); ++iterator)
            {
                if ((*iterator).key_ == key)
                {
                    return iterator.Item();
                }
            }
        }

        return NULL;
    }

    bool Remove(Keytype key)
    {
        unsigned int index = hashFunction_(key) % size_;

        typename LinkedList <HashNode>::Iterator iterator(table_[index].Begin());

        for (; iterator != table_[index].End(); ++iterator)
        {
            if ((*iterator).key_ == key)
            {
                table_[index].Remove((*iterator));
                count_--;

                return true;
            }
        }

        return false;
    }

    int Count() const
    {
        return count_;
    }

    //! Element access
    inline HashNode *operator[](Keytype key) { return Find(key); }
};

#endif // HASH_H

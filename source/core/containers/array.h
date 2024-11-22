#ifndef ARRAY_H
#define ARRAY_H

#include <cstddef>
#include <assert.h>

/*!
 * \brief The Array container
 *
 * Check out Data Structures and Algorithms for Game Programmers by André LaMothe, page 40 (sorry for not using STL)
 * https://cdn.preterhuman.net/texts/math/Data_Structure_And_Algorithms/Data%20Structure%20For%20Game%20Programers%20-%20Ron%20Penton.pdf
 *
 * WARNING: An array of pointers must manually get deleted on deconstruct
 *
 * Example:
 *
 *   void example()
 *   {
 *
 *     Array<int> array(10);
 *
 *     array[0] = 0;
 *     array[1] = 0;
 *     array[3] = 0;
 *
 *     // Insert value between cells 0 and 1
 *     array.Insert(7, 1);
 *
 *     // Automatically deleted by destructor when array goes out of scope
 *
 *   }
 */

template <class Datatype>
class Array
{
public:
    Datatype *array;

private:
    unsigned size;

protected:
    void Init()
    {
        size = 0;
        array = NULL;
    }

    void Copy(const Datatype *fromArray, Datatype *toArray, const int elements)
    {
        for (int i = 0; i < elements; i++)
        {
            toArray[i] = fromArray[i];
        }
    }

public:
    Array()
    {
        Init();
    }

    Array(const unsigned _size)
    {
        Init();
        Resize(_size);
    }

    ~Array()
    {
        if (size != 0)
        {
            delete[] array;
        }
    }

    // Copy constructor
    Array(const Array &originalArray)
    {
        Init();
        Resize(originalArray.size);
        for(unsigned i=0; i<originalArray.Size(); i++)
        {
            array[i] = originalArray[i];
        }
    }

    //Array& operator=(const Array& other) = delete; // Get compile error if anyone tries to assign one array to another

    unsigned Size() const
    {
        return size;
    }

    unsigned Empty() const
    {
        return size == 0;
    }

    void Resize(const unsigned newSize)
    {
        // Keep track of old array to copy over data and delete it
        Datatype *oldArray = array;
        array = 0x0;

        unsigned min;

        if (newSize < size)
            min = newSize;
        else
            min = size;

        // Let's make a new, bigger/smaller array
        array = new Datatype[newSize];

        // Copy over data from old array
        Copy(oldArray, array, min);

        delete[] oldArray;

        size = newSize;
    }

    // Modifiers
    void Insert(Datatype item, const unsigned index)
    {
        // Performance Warning / TODO
        // This will require N allocations for each Add/push_back
        // its better to keep allocation capacity and array size separate,
        // and do fewer allocations by increasing array size geometrically,
        // for example by a factor of 2 for each time we run out of capacity.
        unsigned requiredSize = index + 1;

        if (requiredSize > size)
        {
            Resize(requiredSize);
        }

        for (unsigned i = size - 1; i > index; i--)
        {
            array[i] = array[i - 1];
        }

        array[index] = item;
    }

    void Add(Datatype item)
    {
        Insert(item, size);
    }

    void RemoveAt(const unsigned i)
    {
        // Swaps i with element at end of array and shrinks array by 1
        // Any client code depending on indices of Array should be careful
        //assert( size > 0);
        array[i] = array[size-1];
        Resize(size - 1);
    }

    void Clear()
    {
        Resize(0);
    }

    // Non const, when data is to be mutated in Array
    inline Datatype& operator[](const int i)
    {
        return array[i];
    }

    // Const, for when lhs is also const
    inline Datatype& operator[](const int i) const
    {
        return array[i];
    }

    inline operator Datatype*()
    {
        return array;
    }

    inline void operator+=(Array<Datatype> *other)
    {
        Resize(size + other->size);

        for (unsigned i = other->size; i < size; i++)
        {
            array[i] = other[i];
        }
    }

    inline void operator+=(Array<Datatype> other)
    {
        //Resize(size + other.size);

        for (unsigned i = 0; i < other.Size(); i++)
        {
            Add(other[i]);
        }
    }
};

#endif // ARRAY_H

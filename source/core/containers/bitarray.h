#ifndef BITARRAY_H
#define BITARRAY_H

#include <cstddef>

/*!
 * \brief The Bit-Array container
 */
class BitArray {
protected:
    unsigned long int *array_;
    int size_;
    int bitSize_;

public:
    BitArray(const unsigned int size) {
        bitSize_ = sizeof(size) * 8;
        Resize(size);
    }

    ~BitArray() {
        if (array_) {
            delete[] array_;
        }

        array_ = NULL;
    }

    void Resize(const unsigned int size) {
        unsigned long int *newVector = NULL;

        if (size % bitSize_ == 0) {
            size = size / bitSize_;
        }
        else {
            size = (size / bitSize_) + 1;
        }

        newVector = new unsigned long int[size];

        if (newVector == NULL) {
            // FIXME: Log error
            return;
        }

        int min;

        if (size < size_) {
            min = size;
        }
        else {
            min = size_;
        }

        int index;

        for (index = 0; index < min; index++) {
            newVector[index] = array_[index];
        }

        size_ = size;

        if (array_ != NULL) {
            delete[] array_;
        }

        array_ = newVector;
    }

    void Set(int index, bool value) {
        int cell = index / bitSize_;
        int bit = index % bitSize_;

        if (value) {
           array_[cell] = (array_[cell] | (1 << bit));
        }
        else {
           array_[cell] = (array_[cell] & (~(1 << bit)));
        }
    }

    void SetAll() {
        int index;

        for (index = 0; index < size_; index++) {
            array_[index] = 1;
        }
    }

    void ClearAt() {
        int index;

        for (index = 0; index < size_; index++) {
            array_[index] = 0;
        }
    }

    bool operator[](int index) {
        int cell = index / bitSize_;
        int bit = index % bitSize_;

        return (array_[cell] & (1 << bit)) >> bit;
    }
};

#endif // BITARRAY_H

#ifndef MEMORY_H
#define MEMORY_H

#include <new>
#include "core/x-platform/locator.h"

#ifdef __MACH__ // clang
void *operator new(size_t mem, const char *filename, int line) _THROW_BAD_ALLOC;
void *operator new[](size_t mem) _THROW_BAD_ALLOC;
void operator delete(void *ptr) _NOEXCEPT;
void operator delete(void *ptr, const char *filename, int line) _NOEXCEPT;
void operator delete[](void *ptr) _NOEXCEPT;
#elif GCC
void *operator new(size_t mem, const char *filename, int line);
void *operator new[](size_t mem) _GLIBCXX_THROW (std::bad_alloc);
void operator delete(void *ptr) _GLIBCXX_USE_NOEXCEPT;
void operator delete[](void *ptr) _GLIBCXX_USE_NOEXCEPT;

#ifdef HEAP
// Thanks to https://web.archive.org/web/20110128171515/http://www.swedishcoding.com/
class LinearAllocator
{
public:
    LinearAllocator()
    {
        buffer = malloc(HEAP);
        bufferSize = HEAP;
        currentOffset = 0;
    }
    void* Alloc(size_t size)
    {
       void* pMemToReturn = buffer + currentOffset;
       currentOffset += size;
       return pMemToReturn;
    }
    void Free(void* pMem)
    {
       // We can't easily free memory in this type of allocator.
       // Therefore we just ignore this... or you could assert.
    }
private:
    size_t bufferSize;
    size_t currentOffset;
    void* buffer;
};
#endif

#endif

#endif // MEMORY_H

#include "memory.h"

#ifdef __MACH__ // clang

void *operator new(size_t mem, const char *filename, int line) _THROW_BAD_ALLOC
{
//    printf("Loading %lo %s %i \n", mem, filename, line);
    return malloc(mem);
}

void *operator new(size_t mem) _THROW_BAD_ALLOC
{
//    printf("Loading %lo \n", mem);
    return malloc(mem);
}

void *operator new[](size_t mem) _THROW_BAD_ALLOC
{
//    printf("Loading %lo \n", mem);
    return malloc(mem);
}

void operator delete(void *ptr) _NOEXCEPT
{
//    printf("Freeing memory %lu \n", reinterpret_cast<std::uintptr_t>(ptr));
    free(ptr);
}

void operator delete(void *ptr, const char *filename, int line) _NOEXCEPT
{
//    printf("Freeing %s line %i \n", filename, line);
    free(ptr);
}

void operator delete[](void *ptr) _NOEXCEPT
{
//    printf("Freeing memory %lu \n", reinterpret_cast<std::uintptr_t>(ptr));
    free(ptr);
}

#elif GCC

#ifdef HEAP

LinearAllocator linearAllocator;

void *operator new(size_t mem, const char *filename, int line)
{
    return linearAllocator.Alloc(mem);
}

void *operator new(size_t mem)
{
    return linearAllocator.Alloc(mem);
}

void *operator new[](size_t mem) _GLIBCXX_THROW (std::bad_alloc)
{
    return linearAllocator.Alloc(mem);
}

void operator delete(void *ptr) _GLIBCXX_USE_NOEXCEPT
{
    linearAllocator.Free(ptr);
}

void operator delete(void *ptr, const char *filename, int line)
{
    linearAllocator.Free(ptr);
}

void operator delete[](void *ptr) _GLIBCXX_USE_NOEXCEPT
{
    linearAllocator.Free(ptr);
}

#else

void *operator new(size_t mem, const char *filename, int line)
{
    return malloc(mem);
}

void *operator new(size_t mem)
{
    return malloc(mem);
}

void *operator new[](size_t mem) _GLIBCXX_THROW (std::bad_alloc)
{
    return malloc(mem);
}

void operator delete(void *ptr) _GLIBCXX_USE_NOEXCEPT
{
    free(ptr);
}

void operator delete(void *ptr, const char *filename, int line)
{
    free(ptr);
}

void operator delete[](void *ptr) _GLIBCXX_USE_NOEXCEPT
{
    free(ptr);
}

#endif

#endif

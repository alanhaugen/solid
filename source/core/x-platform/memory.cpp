#include "memory.h"

#ifdef __MACH__ // clang

void *operator new(size_t mem, const char *filename, int line) _THROW_BAD_ALLOC
{
    Log("Loading " + String(mem) + " B file: " + filename + " line: " + String(line));
    return malloc(mem);
}

void *operator new(size_t mem) _THROW_BAD_ALLOC
{
    return malloc(mem);
}

void *operator new[](size_t mem) _THROW_BAD_ALLOC
{
    return malloc(mem);
}

void operator delete(void *ptr) _NOEXCEPT
{
    free(ptr);
}

void operator delete(void *ptr, const char *filename, int line) _NOEXCEPT
{
    //Log("Freeing " + "x B file: " + filename + " line: " + String(line));
    free(ptr);
}

void operator delete[](void *ptr) _NOEXCEPT
{
    free(ptr);
}

#elif GCC

void *operator new(size_t mem, const char *filename, int line)
{
    Log( (String("Loading ") + String(mem) + String(" B file: ") + filename + String(" line: ") + String(line)).ToChar() );
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
    Log( ( String("Freeing ") + "x B file: " + filename + " line: " + String(line)).ToChar() );
    free(ptr);
}

void operator delete[](void *ptr) _GLIBCXX_USE_NOEXCEPT
{
    free(ptr);
}

#endif

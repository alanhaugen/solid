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
#endif

#endif // MEMORY_H

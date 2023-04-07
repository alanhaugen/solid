#ifndef COMPUTEDINCLUDE_H
#define COMPUTEDINCLUDE_H

// This lets you include a macro from a Makefile, which otherwise will not be seen as a string
// https://gcc.gnu.org/onlinedocs/cpp/Computed-Includes.html

#define STRINGIZE(text) #text

#define COMPUTED_INCLUDE(X) STRINGIZE(X)

#endif // COMPUTEDINCLUDE_H

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef int S32;
typedef unsigned long U64;
typedef long long S64;

// The UNUSED macro is only to be used in special circumstances, such as
// unit-testing were an assert will be compiled out in a release build
// or a variable / object is only declared to prove that it can be done
#define UNUSED(x) (void)(x)

#endif // TYPEDEFS_H

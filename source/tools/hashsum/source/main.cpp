#include <core/x-platform/string.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "guid.h"

unsigned long int StringHash(String string) {
    unsigned long int hash = 0;

    unsigned int i;

    for (i = 0; i < string.Length(); i++) {
        hash += ((i + 1) * string[i]);
    }

    return hash;
}
/*
std::string Guid() {
    auto myGuid = generator.newGuid();
    std::string stream;
    stream << myGuid;
    auto guidString = stream.str();
    return guidString;
}
*/
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Hashsum\nUsage: hashsum <filename>");
        return -1;
    }

    printf("%lu", StringHash(argv[1]));
    //std::cout << Guid();

    return 0;
}

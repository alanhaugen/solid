#include "url.h"

URL::URL()
    : String()
{
}

URL::URL(String path)
    : String(StringHash(path))
{
    raw = String(path);
}

String URL::StringHash(String string)
{
    unsigned long int hash = 0;

    unsigned int i;

    for (i = 0; i < string.Length(); i++) {
        hash += ((i + 1) * string[i]);
    }

    String returnString = String(hash);

    return returnString;
}

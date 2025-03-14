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
    return string;
}

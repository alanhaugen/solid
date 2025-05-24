#ifndef URL_H
#define URL_H

#include "core/x-platform/string.h"

/*!
 * \brief The URL class
 * Automatic look-up of file paths from the filesystem
 * The build system converts file paths to a guid
 */

class URL : public String
{
public:
    URL();
    URL(String path);

    String raw;

private:
    String StringHash(String string); // TODO: Look up a guid instead
};

#endif // URL_H

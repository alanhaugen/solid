#ifndef PARSERJSON_H
#define PARSERJSON_H

/*! Parser json class
 *
 *  There are two implementations, one for CAFE and one for the rest.
 *  Note that the API (which is in f. ex. ParserJSMN) isn't very good.
 *
 *  A better API would give the user an object instead.
 *
 *  A better API could look like this:
 *
 *  objs = AddObject(String name)
 *
 *  if (objs)
 *  {
 *      x = objs[0]->getNumber("x")
 *      y = objs[0]->getNumber("y")
 *      z = objs[0]->getNumber("z")
 *  }
*/

#ifdef CAFE
#include "core/platforms/cafe/parsernlib.h"
class ParserJSON : public ParserNlib
#else
#include "core/x-platform/parserjsmn.h"
class ParserJSON : public ParserJSMN
#endif
{
public:
    ParserJSON(const char *json, int tokenSize = 1024);
};

#endif // PARSERJSON_H

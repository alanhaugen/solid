#ifndef PARSERJSMN_H
#define PARSERJSMN_H

//#define JSMN_HEADER
//#include "3rdparty/jsmn.h"
#include "core/x-platform/string.h"
#include "core/x-platform/locator.h"

class ParserJSMN
{
public:
    ParserJSMN();
    ~ParserJSMN();

    void AddObject(String label);
    String GetNextObject();

    double GetNumber(String label);
    String GetString(String label);

    bool isInArray();

    bool Init(const char *json, int tokenSize);

private:
    int FindToken(int beginSearch, int dataType, String label);

//    jsmntok_t *tokens;
//    jsmn_parser p;

    String jsonText;

    int r;

    int seekPoint;
    int objectPoint;

    bool secondPass;

    int level;

    Array<String> objects;
};

#endif // PARSERJSMN_H

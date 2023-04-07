#include "parserjsmn.h"

ParserJSMN::ParserJSMN()
{
}

bool ParserJSMN::Init(const char *json, int tokenSize)
{
    // Initialize JSMN
    /*tokens = new jsmntok_t[tokenSize];
    jsmn_init(&p);

    // Read json file
    IFile *jsonFile = Locator::filesystem->Open(URL(json), PLAIN_TEXT);

    jsonText = jsonFile->Read();

    // Try parsing file
    r = jsmn_parse(&p, jsonText.ToChar(), jsonFile->Size(), tokens, tokenSize);

    delete jsonFile;

    // Log error if parsing failed
    if (r < 0)
    {
        LogError("Failed to parse JSON: " + String(r) + " File: " + json);
        return false;
    }

    // Assume the top-level element is an object
    if (r < 1 || tokens[0].type != JSMN_OBJECT)
    {
        LogError("Object expected");
        return false;
    }

    // Start search from beginning
    seekPoint = 1;

    // Start object hierarchy at level minus one (TODO: Remove magic number)
    level = -1;

    // To enable out of order parameters objects are searched twice
    secondPass = false;*/

    return true;
}

ParserJSMN::~ParserJSMN()
{
    //delete[] tokens;
}

void ParserJSMN::AddObject(String label)
{
    objects.Add(label);
}

String ParserJSMN::GetNextObject()
{
    // Loop over all keys of the root object, start where it left off last
    /*for (int i = seekPoint; i < r; i++)
    {
        String jsonString = jsonText.SubString(tokens[i].start, tokens[i].end);

        if (tokens[i].type == JSMN_STRING)
        {
            for (unsigned int j = 0; j < objects.Size(); j++)
            {
                if (objects[j] == jsonString)
                {
                    seekPoint = i + 1;
                    objectPoint = seekPoint;
                    return objects[j];
                }
            }
        }
    }*/

    return String("Empty");
}

int ParserJSMN::FindToken(int beginSearch, int dataType, String label)
{
    /*bool bail = false;

    // Loop over all keys of the object, start where it left off last
    for (int i = beginSearch; i < r && bail == false; i++)
    {
        String jsonString = jsonText.SubString(tokens[i].start, tokens[i].end);

        // Make sure we have not gone past the end of a current object
        if (tokens[i].type == JSMN_STRING)
        {
            for (unsigned int j = 0; j < objects.Size(); j++)
            {
                if (objects[j] == jsonString)
                {
                    bail = true;
                }
            }
        }

        // Search for numbers
        if (tokens[i].type == dataType && jsonString == label)
        {
            seekPoint = i;
            return i;
        }
    }

    // Seek back to beginning of the object and search again
    if (secondPass == false)
    {
        secondPass = true;
        return FindToken(objectPoint, dataType, label);
    }

    secondPass = false;*/

    return -1;
}

double ParserJSMN::GetNumber(String label)
{
    /*int i = FindToken(seekPoint, JSMN_STRING, label);

    if (i != -1)
    {
        return atof(jsonText.SubString(tokens[i+1].start, tokens[i+1].end).ToChar());
    }

    LogWarning("Failed to find number: " + label);*/

    return 0;
}

String ParserJSMN::GetString(String label)
{
    /*int i = FindToken(seekPoint, JSMN_STRING, label);

    if (i != -1)
    {
        return jsonText.SubString(tokens[i+1].start, tokens[i+1].end);
    }

    LogWarning("Failed to find string: " + label);*/

    return String("Empty");
}

bool ParserJSMN::isInArray()
{
    /*if (level == -1)
    {
        level = tokens[seekPoint].size;
    }

    if (level >= 1)
    {
        level--;
        return true;
    }*/

    return false;
}

#include "core/x-platform/string.h"

#include <assert.h>
#include <stdio.h> // vsnprintf
#include <string.h> // strlen, strcpy, strncmp
#include <stdarg.h> // va_list, va_start, va_arg, va_end
#include <ctype.h>  // isspace

String::~String()
{
    if (text)
    {
        delete[] text;
    }
    text = NULL;
}

String::String()
{
    text = new char[1];
    text[0] = '\0';
    assert( strlen(text) == 0 );
}

String::String(const String &other)
{
    assert (this != &other);  // protect against invalid self-assignment
    int len = other.Length() + 1;
    text = new char[len];
    // DST SRC LEN
    strncpy(text, other.ToChar(), len);
    assert( text[len-1] == '\0' );
}

String& String::operator=(const String& other)
{
    if (this != &other) // protect against invalid self-assignment
    {
        int len = other.Length() + 1;
        delete[] text;
        text = new char[len];
        // DST SRC LEN
        strncpy(text, other.ToChar(), len);
        //assert( text[len-1] == '\0' );
    }
    // by convention, always return *this
    return *this;
}

static char* fromCharBuffer(const char* in)
{
    size_t len = strlen(in) + 1;
    char* text = new char[len];
    // DST SRC CHARS
    strncpy(text, in, len);
    assert( text[len-1] == '\0' );
    return text;
}

String::String(const char *in)
{
    text = fromCharBuffer(in);
}

void String::operator=(const char* in)
{
    assert( text );
    delete[] text;
    text = fromCharBuffer(in);
}

static int bufferSize(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vsnprintf(NULL, 0, format, args);
    va_end(args);
    return result + 1; // safe byte for \0
}

String::String(int number)
{
    // Get the number of characters needed to represent number
    const char *format = "%d";

    int len = bufferSize(format, number);
    assert(len);
    text = new char[len+1]; // Allocate that + 1 for nullterminator
    sprintf(text, format, number);
    assert(text[len-1] == '\0');
}

String::String(unsigned long number)
{
    // Get the number of characters needed to represent number
    const char *format = "%d";

    int len = bufferSize(format, number);
    assert(len);
    text = new char[len+1]; // Allocate that + 1 for nullterminator
    sprintf(text, format, number);
    assert(text[len-1] == '\0');
}

String::String(float number) {
    // Get the number of characters needed to represent number
    const char *format = "%f";

    int len = bufferSize(format, number);
    assert(len);
    text = new char[len+1]; // Allocate that + 1 for nullterminator
    sprintf(text, format, number);
    assert(text[len-1] == '\0');
}

String::String(unsigned int number)
{
    // Get the number of characters needed to represent number
    const char *format = "%d";

    int len = bufferSize(format, number);
    assert(len);
    text = new char[len+1]; // Allocate that + 1 for nullterminator
    sprintf(text, format, number);
    assert(text[len-1] == '\0');
}

bool String::operator ==(const char* other) const
{
    return !strncmp(text, other, Length());
}

bool String::operator !=(const char* other) const
{
    return strncmp(text, other, Length());
}

bool String::operator ==(String& other) const
{
    if (other.Length() != Length())
    {
        return false;
    }

    return !strncmp(text, other.ToChar(), Length());
}

bool String::operator ==(const String& other) const
{
    return !strncmp(text, other.ToChar(), Length());
}

bool String::operator !=(String& other) const
{
    return strncmp(text, other.ToChar(), Length());
}

char String::At(unsigned int index) const
{
    return text[index];
}

char String::operator[](unsigned int index) const
{
    return text[index];
}

void String::Append(const String& other)
{
    assert(text != NULL);
    int newLength = Length() + other.Length();
    int oldLength = Length();

    // Copy Old String
    char* temp = new char[oldLength + 1];
    for(int i=0; i<oldLength; i++) { temp[i] = text[i]; }
    temp[oldLength] = '\0';

    delete[] text;
    text = new char[newLength+1];
    assert(text != NULL);

    int otherLen = other.Length();
    for(int i=0; i<oldLength; i++) { text[i] = temp[i]; }
    delete[] temp;

    const char* otherString = other.ToChar();
    for(int i=0; i<otherLen; i++) { text[oldLength + i] = otherString[i]; }
    text[newLength] = '\0';
}

unsigned int String::Length() const
{
    return strlen(text);
}

bool String::StartsWith(const String& searchValue, unsigned int start) const
{
    if (start >= Length() ) return false; // the startoffset is beyond our string
    unsigned otherlen = searchValue.Length();
    unsigned len = Length();
    if ( (otherlen) > len) return false; // The other string is longer than our searchee

    for(unsigned i=0; i<otherlen; i++)
    {
        if ( text[i+start] != searchValue[i] ) return false; // Give up early
    }
    // If we got this far, the searchValue must be a substring
    return true;
}

const char *String::ToChar() const
{
    return text;
}

int String::ToInt() const
{
    // Thanks to https://blog.udemy.com/c-string-to-int/
    int i, len;

    int result = 0;

    len = strlen(text);

    for(i=0; i<len; i++)
    {
        // Substract '0' to convert to int, do it for every decimal place
        result = result * 10 + (text[i] - '0');
    }

    return result;
}

Array<String> String::ToWords() const
{
    Array<String> words;

    for (unsigned int i = 0; i < Length(); i++)
    {
        int j = IndexOf(" ", i);

        if (j == -1 || i == unsigned (j))
        {
            break;
        }

        words.Add(SubString(i, j));
        i = j;
    }

    return words;
}

int String::IndexOf(String string, unsigned int searchStart) const
{
    for (unsigned int i = searchStart; i < Length(); i++)
    {
        if (text[i] == string.At(0))
        {
            bool isFound = true;

            for (unsigned int j = 0; j < string.Length(); j++)
            {
                if (string.At(j) != text[i + j])
                {
                    isFound = false;
                }
            }

            if (isFound)
            {
                return i;
            }
        }
    }

    return -1;
}

String& String::TrimLeft()
{
    assert(text);
    if (!*text)
        return *this; // handle empty string

    char *p = text, *q = text;
    while (isspace(*q))            ++q; // find first nonspace
    while (*q)                     *p++ = *q++; // move string back
    *p = '\0';
    return *this;
}

String& String::TrimRight()
{
    char *ptr;
    assert(text);
    if (!*text)
        return *this;      // handle empty string

    for (ptr = text + strlen(text) - 1; // Start from back of string
         (ptr >= text) && isspace(*ptr); // while were still in the string, and the char is whitespace
         --ptr); // move backwards
    ptr[1] = '\0';
    return *this;
}

String& String::Trim()
{
    TrimLeft();
    TrimRight();
    return *this;
}

String String::SubString(int from, int to) const
{
    /*assert(from);
    assert(to);
    assert( (to-from) > 0 );
    assert( from < static_cast<int>(this->Length()) );
    assert( to <= static_cast<int>(this->Length()) );*/
    int distance = to - from;

    // Copy this
    char* temp = new char[distance + 1];
    char* ch = temp;
    for(int i=from; i<=to; i++) { *ch = text[i]; ++ch; }
    temp[distance] = '\0';

    String ret(temp);
    delete[] temp;
    return ret;
}

String& String::ToLowerCase()
{
    int i=0;
    while (text[i])
    {
        text[i] = tolower(text[i]);
        i++;
    }
    return *this;
}

String& String::ToUpperCase()
{
    int i=0;
    while (text[i])
    {
        text[i] = toupper(text[i]);
        i++;
    }
    return *this;
}

String operator+(String a, String b)
{
    a.Append(b);
    return a;
}

static
int string_find_last(const char* s, char c, size_t* p)
{
    const char* e;

    e = s + strlen(s);
    while (e > s)
    {
        e--;

        if (*e == c)
        {
            *p = (size_t)(e - s);
            return 1;
        }
    }

    return 0;
}

String String::GetFileExtension() const
{
    size_t sep;
    int foundDot = string_find_last(text, '.', &sep);
    assert(foundDot);
    return SubString( sep, Length() );
}

String operator+(const char* cstr, String str)
{
    String a(cstr);
    a.Append(str);
    return a;
}

String operator+(String str, const char* cstr)
{
    str.Append(String(cstr));
    return str;
}

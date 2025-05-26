#ifndef STRING_H
#define STRING_H

#include "core/containers/array.h"

/*!
 * \brief The String class
 * Has UTF-16 support and automatic internationalization
 */
class String
{
protected:
    char* text;

private:
    //template<typename Type>
    //void SetNumber(Type number, const char *format);

public:
    explicit String();
    String(const String& other);
    String& operator=(const String& other);
    String(const char *str);
    void operator=(const char* str);
    operator const char*() { return text; } // WARNING: implicit cast to const char*
//    operator char*() { return text; } // implicit cast to char*
    String(int number);
    String(unsigned long number);
    String(unsigned int number);
    String(float number);
    ~String();

    bool operator==(const char* other) const; // Allow us to do String("mystring") == "a c string"
    bool operator!=(const char* other) const; // Allow us to do String("mystring") == "a c string"
    bool operator==(String& other) const;
    bool operator==(const String& other) const;
    bool operator!=(String& other) const;
    char operator[](unsigned int index) const;
    char At(unsigned int index) const;
    //operator const char*() const; // testing implicit conversion to const char* as alternative to char *ToChar() (so you can shoot yourself in the foot with conversions!)
    // We already got a bug where compiler decides to implicitly convert a class String to const char* on input to operator+ (we got 3 operator+s here and want both)

    /*!
     * \brief Append concatenates a string to the string
     * \param other is the string to be appended
     */
    void Append(const String &other);

    /*!
     * \brief Length how many characters there is in the string
     * \return length of the string
     */
    unsigned int Length() const;

    /*!
     * \brief StartsWith checks wether string begins with specified characters
     * \param searchValue
     * \param start
     * \return
     */
    bool StartsWith(const String &searchValue, unsigned int start = 0) const;

    /*!
     * \brief ToLowerCase changes each character in string to all lower-case letters
     */
    String &ToLowerCase();

    /*!
     * \brief ToUpperCase changes each character in string to all upper-case letters
     */
    String &ToUpperCase();

    /*!
     * \brief ToChar attempts to return a 8-bit-per-character character array
     * \return string as char array
     */
    const char *ToChar() const;

    /*!
     * \brief ToInt attempts to return a C int based on the string
     * \return int
     */
    int ToInt() const;

    /*!
     * \brief ToWords
     * \return Array of words
     */
    Array<String> ToWords() const;

    /*!
     * \brief IndexOf
     * \return int
     */
    int IndexOf(String string, unsigned int searchStart = 0) const;

    String& TrimLeft();
    String& TrimRight();
    /*!
     * \brief Trim removes whitespace from both sides of the string
     */
    String& Trim();
    /*!
     * \brief Substring create a new string [from..to]
     */
    String SubString(int from, int to) const;

    /*!
     * \brief Append concatenates a string to the string
     * \param other is the string to be appended
     */
    bool Empty() const { return Length() == 0; };

    String GetFileExtension() const;

};

String operator+(String a, String b);
String operator+(const char* cstr, String str);
String operator+(String str, const char* cstr);

#endif // STRING_H

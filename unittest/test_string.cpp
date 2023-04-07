#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "../source/3rdparty/catch2/catch.hpp"
#include "../source/core/containers/array.h"
//#include "../source/core/containers/bitarray.h"
#include "../source/core/containers/hash.h"
#include "../source/core/containers/linkedlist.h"
//#include "../source/core/containers/queue.h"
//#include "../source/core/containers/stack.h"
#include "../source/core/x-platform/string.h"
#include "../source/core/x-platform/typedefs.h"
#include "../source/core/x-platform/random.h"

#include <math.h>

TEST_CASE( "Types", "[types]" ) {
    REQUIRE( sizeof(U8) == 1 );
    REQUIRE( sizeof(U16) == 2 );
    REQUIRE( sizeof(U32) == 4 );
    REQUIRE( sizeof(U64) == 8 );
}

TEST_CASE( "Strings", "[String]") {
    String mystring;

    String cat("Cat");
    String dog("Dog");
    // Test that concat works
    String catdog = cat + dog;
    REQUIRE( strcmp( catdog.ToChar(), "CatDog") == 0 );

    // Test copy ctor
    String copycat( cat );
    REQUIRE( strcmp( copycat.ToChar(), "Cat") == 0 );

    // Test copy assign
    String copyassign = cat;
    REQUIRE( strcmp( copyassign.ToChar(), "Cat") == 0 );

    REQUIRE( strcmp( String(42).ToChar(), "42") == 0 );

    // Test conversion to upper and lower
    REQUIRE( strcmp( String("MiXeD CaSeS123!").ToUpperCase().ToChar(), "MIXED CASES123!") == 0 );
    REQUIRE( strcmp( String("MiXeD CaSeS123!").ToLowerCase().ToChar(), "mixed cases123!") == 0 );

    // Test isSubstring functionality
    REQUIRE( String("AnotherTest").StartsWith("AnotherT", 0) );
    REQUIRE( String("AnotherTest").StartsWith("Tes", strlen("Another") ) );
    REQUIRE( !String("AnotherTest").StartsWith("Bar", 0 ) );
    REQUIRE( !String("AnotherTest").StartsWith("Another", 12 ) );

    // Test trimming
    REQUIRE( strcmp( String("  has some whitespace ").Trim().ToChar(), "has some whitespace") == 0 );
    REQUIRE( strcmp( String("  has some whitespace  ").TrimLeft().ToChar(), "has some whitespace  ") == 0 );
    REQUIRE( strcmp( String("  has some whitespace ").TrimRight().ToChar(), "  has some whitespace") == 0 );

    REQUIRE( strcmp( String("  has some whitespace ").Trim().ToChar(), "has some whitespace") == 0 );
    REQUIRE( strcmp( String("  has some whitespace  ").TrimLeft().ToChar(), "has some whitespace  ") == 0 );
    REQUIRE( strcmp( String("  has some whitespace ").TrimRight().ToChar(), "  has some whitespace") == 0 );
    REQUIRE( strcmp( String("").Trim().ToChar(),      ""   ) == 0 );// Trivial
    REQUIRE( strcmp( String("   ").Trim().ToChar(),   ""   ) == 0 );// Trivial
    REQUIRE( strcmp( String("\t").Trim().ToChar(),    ""   ) == 0 );// Trivial
    REQUIRE( strcmp( String("a").Trim().ToChar(),     "a"  ) == 0 );// NOP
    REQUIRE( strcmp( String("abc").Trim().ToChar(),   "abc") == 0 );// NOP
    REQUIRE( strcmp( String("  a").Trim().ToChar(),   "a"  ) == 0 );// Leading
    REQUIRE( strcmp( String("  a c").Trim().ToChar(), "a c") == 0 );// Leading
    REQUIRE( strcmp( String("a  ").Trim().ToChar(),   "a"  ) == 0 );// Trailing
    REQUIRE( strcmp( String("a c  ").Trim().ToChar(), "a c") == 0 );// Trailing
    REQUIRE( strcmp( String(" a ").Trim().ToChar(),   "a"  ) == 0 );// Both
    REQUIRE( strcmp( String(" a c ").Trim().ToChar(), "a c") == 0 );// Both
}

TEST_CASE( "ArrayU32", "[Array]") {
    Array<U32> arr;
    for(int i=0; i<1024; i++)
    {
        arr.Add(i);
    }
    for(int i=0; i<1024; i++)
    {
        REQUIRE( arr[i] == i );
    }
    REQUIRE(arr.Size() == 1024);
    arr.Clear();
    REQUIRE(arr.Size() == 0);
    arr.Add(1);
    arr.Add(2);
    arr.Add(3);
    arr.RemoveAt(1);
    REQUIRE( arr.Size() == 2 );
    REQUIRE( arr[0] == 1 );
    REQUIRE( arr[1] == 3 );
}

TEST_CASE( "ArrayString", "[Array]") {
    Array<String> arr;
    for(int i=0; i<1024; i++)
    {
        arr.Add( String(i) );
    }
    for(int i=0; i<1024; i++)
    {
        String str(i);
        REQUIRE( arr[i] == str );
    }
    REQUIRE(arr.Size() == 1024);
    arr.Clear();
    REQUIRE(arr.Size() == 0);

    arr.Add(String("there"));
    arr.Add(String("can"));
    arr.Add(String("only"));
    arr.Add(String("be"));
    arr.Add(String("one"));
    REQUIRE( arr.Size() == 5 );
    // Do we really want the Array to re-arrange the order of our items like this ?
    // Example:
    // 0: there can only be one
    // 1: one can only be
    // 2: be can only
    // 3: only can
    arr.RemoveAt(0);
    arr.RemoveAt(0);
    arr.RemoveAt(0);
    REQUIRE( arr.Size() == 2 );
    REQUIRE( arr[0] == "only" );
    REQUIRE( arr[1] == "can" );
}

TEST_CASE( "LinkedListU32", "[List]") {
    LinkedList<U32> list;
    for(int i=0; i<1024; i++)
    {
        list.Append(i);
    }
    REQUIRE( list.Count() == 1024 );
    REQUIRE( !list.Empty() );
    U32 num = 0;

    LinkedList<U32>::Iterator it(list.Begin());
    for(; it != list.End(); ++it) {
        REQUIRE( *it == num );
        num++;
    }
    list.Clear();
    REQUIRE( list.Count() == 0 );

    list.Append(1);
    list.Append(2);
    list.Append(3);
    list.RemoveAt(0);
    REQUIRE( *list.Begin() == 2 );
    REQUIRE( *(++list.Begin()) == 3 );

    // Test that we are allowed to make const references
    const U32 &const_intref = *list.Begin();
    UNUSED(const_intref);
    // Test that we can modify the contents of list via a reference
    U32 &intref = *list.Begin();
    intref = 42;
    REQUIRE( *list.Begin() == intref );
}

TEST_CASE( "LinkedListString", "[List]") {
    LinkedList<String> list;
    for(int i=0; i<1024; i++)
    {
        list.Append( String(i) );
    }
    REQUIRE( list.Count() == 1024 );
    REQUIRE( !list.Empty() );
    U32 num = 0;

    LinkedList<String>::Iterator it(list.Begin());
    for(; it != list.End(); ++it) {
        REQUIRE( *it == String(num) );
        num++;
    }
    list.Clear();
    REQUIRE( list.Count() == 0 );

    list.Append( String(1) );
    list.Append( String(2) );
    list.Append( String(3) );
    list.RemoveAt(0);
    REQUIRE( *list.Begin() == "2" );
    REQUIRE( *(++list.Begin()) == "3" );

    // Test that we are allowed to make const references
    const String &const_ref = *list.Begin();
    UNUSED(const_ref);
    // Test that we can modify the contents of list via a reference
    String &ref = *list.Begin();
    ref = String(42);
    REQUIRE( *list.Begin() == ref );
}

TEST_CASE( "HashU32toU32", "[420]") {
    #define TEST_HASH_SIZE 1
    Hash<U32, U32> map(TEST_HASH_SIZE, Hash<U32, U32>::NumberHash);
    #undef TEST_HASH_SIZE
    for(U32 i=0; i<100; i++)
    {
        map.Insert(i, i * 1000);
    }
    REQUIRE( map.Count() == 100 );

    for(U32 i=0; i<100; i++)
    {
        REQUIRE( map.Find(i)->data_ == (i*1000u) );
    }
    //map.Remove(50u); // Not allowed!?
}

TEST_CASE( "HashStringtoU32", "[420]") {
    #define TEST_HASH_SIZE 1
    Hash<String, U32> map(DEFAULT_HASH_SIZE, Hash<String, U32>::StringHash);
    #undef TEST_HASH_SIZE
    for(U32 i=0; i<100; i++)
    {
        map.Insert( String(i), i * 1000 );
    }
    REQUIRE( map.Count() == 100 );

    for(U32 i=0; i<100; i++)
    {
        REQUIRE( map.Find(String(i))->data_ == i*1000u );
    }
    //map.Remove( String(50u) ); // Not allowed!?
    // U32 val = *map.operator [](String(50u)); // Doesnt compile
}

TEST_CASE( "Random" , "[rnd]" ) {
    int seed = 0;
    Random r(seed);
    REQUIRE( r.GetRandomSeed() == 0);
    for(int i=0; i<1000; i++)
    {
        int val = r.RandomRange(-42, 42);
        REQUIRE( val >= -42 );
        REQUIRE( val <=  42 );
    }


    static const int N = 1024*10;
    int storage[N];

    int sum = 0;
    for(int i=0; i<N; i++)
    {
        storage[i] = r.RandomRange(0, 100);
        sum += storage[i];
    }
    double average = sum / double(N);

    //int storage_deviation[N];
    double sum_deviation = 0.0;
    for(int i=0; i<N; i++)
    {
        sum_deviation += pow(storage[i] - average, 2.0);
    }
    double variation = sum_deviation / N;

    int unique_numbers = 0;
    for(int i=0; i<N; i++)
    {
        for(int j=i+1; j<N; j++)
        {
            if (storage[i] != storage[j]) {
                unique_numbers++;
                break;
            }
        }
    }
    REQUIRE( unique_numbers/double(N) > .9 ); // at least 90% were unique. This is a stupid test, but it proves there is some randomness...
    printf("Random generator unique_numbers %d of %d total. Percentage unique: %f\n", unique_numbers, N, unique_numbers/double(N));
    printf("Random generator avg = %f\n", average);
    printf("Random generator variation = %f\n", variation);
    printf("Random generator stdev = %f\n", sqrt(variation) );
}

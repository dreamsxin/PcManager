/* -------------------------------------------------------------------------- *
   WinUnit - Maria Blees (maria.blees@microsoft.com)

   Version: 1.2.0909.1
 * -------------------------------------------------------------------------- */

#pragma once

// Compiler version check:
// WinUnit supports Visual C++ compiler version 9 and subsequent versions
#if _MSC_VER < 1500
#pragma message("Compiler not supported. WinUnit requires at least Microsoft Visual C++ compiler version 9")
#endif

/// @file WinUnit.h
///
/// This file contains macros and helper functions to make it easier to write
/// tests. 
/// See @ref TestConstruction and @ref asserts for more information.
///

/// @defgroup TestConstruction Test Construction
/// These are the basic macros you will use for creating tests and doing 
/// setup/teardown.
/// 
/// @par Example:
/// @code 
///
/// // Variables local to this file
/// //
/// namespace
/// {
///   int s_myNumber = 0;
/// }
/// 
/// FIXTURE(MyFixture);     // declares a fixture
///
/// SETUP(MyFixture)
/// {
///     // Do some stuff here that you want done before a test starts. It can
///     // include accessing static variables.  An example would be creating
///     // a file (and then deleting it in TEARDOWN).
///     s_myNumber = 5;
/// }
///
/// TEARDOWN(MyFixture)
/// {
///     // Undo whatever you did in the SETUP.  
///     s_myNumber = 0;
/// }
///
/// // This test uses a fixture (the second argument).  Setup and Teardown will 
/// // be run before and after the body of the test, respectively.
/// BEGIN_TESTF(MyTestThatUsesAFixture, MyFixture)
/// {
///     // Do some tests.  You can assume s_MyNumber is 5.
///     WIN_ASSERT_EQUAL(5, s_myNumber);
/// }
/// END_TESTF
///
/// BEGIN_TEST(MyTestThatDoesNotUseFixtures)
/// {
///     // Do some tests.  Do not rely on any variables set in the setup or 
///     // teardown.
/// }
/// END_TEST
///
/// @endcode
///

///
/// @defgroup asserts Assert Macros and Functions
/// These are the "asserts" you will use to verify conditions which, if not true,
/// indicate a test failure.
///
/// The WIN_ASSERT_* macros call similarly named Assert::* functions. While the 
/// Assert::* functions can be called without using the macros, the macros add
/// value by including the file and line number and sometimes a string depiction
/// of the expression(s) passed in.
///
/// The WIN_ASSERT_* macros all allow for an optional printf-style format string and 
/// parameters to be tacked on to the condition part of the assert.  The 
/// Assert::* functions require at least one message string.
///
/// @remarks 
/// The error messages sometimes include the expression that was 
/// passed in as a string.  If the type of the expression does not have a ToString
/// implementation, [OBJECT] will be displayed.
///

#include <windows.h>
#include <tchar.h>
#include <wchar.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>	// for _countof
#include <locale>   // for isprint

// This is the max buffer length for the message assigned to an assert exception.
#define MAX_ASSERT_MESSAGE_LENGTH 1024

// This is the max buffer length for a single trace string.
#define MAX_TRACE_MESSAGE_LENGTH 1024

// This is the max buffer length for the string form of a single expression
// (used when displaying the expression as part of an error message)
#define MAX_EXPRESSION_STRING     256

// This is the max buffer length to hold an error string from GetLastError.
// (80 was suggested in the sample code for the FormatMessage function, but was
// too small for at least one error message.)
#define MAX_SYSERROR_STRING       120

#define EMIT_ERROR(...)                                                            \
    ::_snwprintf_s(__winUnit_buffer, __winUnit_cchBuffer, _TRUNCATE, __VA_ARGS__); \
    return false;

#define BEGIN_TEST_FUNC(x)                   extern "C" __declspec(dllexport) bool __cdecl TEST_##x(wchar_t* __winUnit_buffer, size_t __winUnit_cchBuffer) {
#define BEGIN_FIXTURE_BLOCK(fixtureName)     try { FIXTURE_##fixtureName __fixture_##fixtureName;
#define BEGIN_TEST_BLOCK                     try 
#define END_TEST_BLOCK                                              \
        catch(WinUnit::AssertException& e)                          \
        {                                                           \
            EMIT_ERROR(L"%s", e.Message());                         \
        }                                                           \
        catch(...)                                                  \
        {                                                           \
            EMIT_ERROR(L"%s(%d): Exception thrown.",                \
                    __WFILE__, __LINE__);                           \
        }                                                           \

#define END_FIXTURE_BLOCK                                           \
    }                                                               \
    catch(WinUnit::AssertException& e)                              \
    {                                                               \
        EMIT_ERROR(e.Message());                                    \
    }                                                               \
    catch(...)                                                      \
    {                                                               \
        EMIT_ERROR(L"%s(%d): Exception thrown in fixture.",         \
            __WFILE__, __LINE__);                                   \
    }                                                               \

#define END_TEST_FUNC                       return true; }

/// @ingroup TestConstruction
/// This macro indicates the beginning of a test.  It writes the first part of
/// a function named "x" ensuring you use the proper function prototype, and 
/// that the function name is exported (thereby readable by WinUnit.exe). 
/// Additionally it includes a try, which is closed in a catch by the END_TEST
/// macro. 
#define BEGIN_TEST(x)   BEGIN_TEST_FUNC(x) BEGIN_TEST_BLOCK

/// @ingroup TestConstruction
/// This macro indicates the end of a test.  It's necessary because it includes
/// a catch block.  Since these tests use C++ exceptions to 
/// indicate failure, and when thrown, they are crossing a DLL boundary, they 
/// end up being treated as an unhandled exception, and the stack doesn't get 
/// unwound.  This results in unexpected behavior if you were counting on C++
/// scoping effects.  The solution is to put a try/catch block around every 
/// exported function, and just do a throw in the catch block.
#define END_TEST        END_TEST_BLOCK     END_TEST_FUNC

/// @ingroup TestConstruction
/// This macro indicates the beginning of a test that uses a fixture.  It needs
/// an extra try/catch to handle the case where Teardown (i.e. the destructor
/// of the "fixture object") throws exceptions (see documentation for 
/// terminate() function for why this could be a problem) 
#define BEGIN_TESTF(x, f) BEGIN_TEST_FUNC(x) BEGIN_FIXTURE_BLOCK(f) BEGIN_TEST_BLOCK

/// @ingroup TestConstruction
/// This macro indicates the end of a test that uses a fixture. 
#define END_TESTF         END_TEST_BLOCK     END_FIXTURE_BLOCK      END_TEST_FUNC

namespace WinUnit
{
    /// Implement this function for your type if you would like it to show up
    /// in assert messages as something more specific than [OBJECT].
    /// @return Pointer to buffer passed in, for easier inline use.
    template<class T>
    inline const TCHAR* ToString(
        const T& object,                ///< Object that needs a string representation
        TCHAR buffer[],                 ///< Buffer to hold the returned string
        size_t bufferSize);             ///< Size of buffer in TCHARs
}

// The macros and functions in this file are designed to work whether or not
// _UNICODE is defined.  The following are helper macros that give certain
// strings the right character width.
//
#define WINUNIT_WIDEN2(x) L ## x
#define WINUNIT_WIDEN(x) WINUNIT_WIDEN2(x)

#ifndef __WFILE__
#define __WFILE__ WINUNIT_WIDEN(__FILE__)
#else
#pragma message("WinUnit.h: __WFILE__ already defined")
#endif

#ifndef __WFUNCTION__
#define __WFUNCTION__ WINUNIT_WIDEN(__FUNCTION__)
#else
#pragma message("WinUnit.h: __WFUNCTION__ already defined")
#endif

//
#ifdef _UNICODE
#define TSTRING(x) WINUNIT_WIDEN(#x)
#define __TFILE__ __WFILE__
#define __TFUNCTION__ __WFUNCTION__
#else
#define TSTRING(x) #x
#define __TFILE__ __FILE__
#define __TFUNCTION__ __FUNCTION__
#endif

#pragma region WIN_ASSERT_* macros

/// The operator== is used to compare expected and actual; the test has failed
/// if they are shown to be not equal.
/// @param expected The expected value.
/// @param actual   The actual value, to be compared with expected.
/// @param ...      An optional printf-style format string and arguments.
/// @remarks 
/// If there is no operator== for the two types, a compiler error will occur.  
/// You can write your own operator== for custom types.
/// See http://msdn2.microsoft.com/en-us/library/09ka8bxx(VS.80).aspx for 
/// arithmetic conversions that occur when using a binary operator such as ==.
/// @ingroup asserts
#define WIN_ASSERT_EQUAL(expected, actual, ...) WinUnit::Assert::AreEqual(TSTRING(expected), expected, TSTRING(actual), actual, __TFILE__, __LINE__, __VA_ARGS__)

/// The operator!= is used to compare notExpected and actual; the test has
/// failed if they are shown to be equal.
/// @param notExpected  The value not expected.
/// @param actual       The actual value, to be compared with notExpected.
/// @param ...          An optional printf-style format string and arguments.
/// @remarks 
/// If there is no operator!= for the two types, a compiler error will occur.  
/// You can write your own operator!= for custom types.
/// See http://msdn2.microsoft.com/en-us/library/09ka8bxx(VS.80).aspx for 
/// arithmetic conversions that occur when using a binary operator such as !=.
/// @ingroup asserts
#define WIN_ASSERT_NOT_EQUAL(notExpected, actual, ...) WinUnit::Assert::AreNotEqual(TSTRING(notExpected), notExpected, TSTRING(actual), actual, __TFILE__, __LINE__, __VA_ARGS__)

/// A string-compare function (wcscmp or strcmp) is used to do a case-sensitive
/// comparison of the two given strings (up to the first null character); the 
/// test has failed if they are not equal.
/// @param expected     The expected string.
/// @param actual       The actual string, to be compared with expected.
/// @param ..           An optional printf-style format string and arguments.
/// @remarks
/// Either both strings must be wide-character, or both strings must be ANSI.
/// The format string + arguments at the end are expected to be of character 
/// type TCHAR (i.e. Unicode if _UNICODE is defined; ANSI otherwise), regardless
/// of which character type expected and actual are.
/// @ingroup asserts
#define WIN_ASSERT_STRING_EQUAL(expected, actual, ...) WinUnit::Assert::StringEqual(expected, actual, __TFILE__, __LINE__, __VA_ARGS__)

/// The test fails if zeroExpression does not evaluate to 0.  
/// @param zeroExpression   The expression that is expected to evaluate to zero.
/// @param ...              An optional printf-style format string and arguments.
/// @ingroup asserts
#define WIN_ASSERT_ZERO(zeroExpression, ...) WinUnit::Assert::IsZero(TSTRING(zeroExpression), zeroExpression, __TFILE__, __LINE__, __VA_ARGS__)

/// The test fails if nonzeroExpression evaluates to 0.
/// @param nonzeroExpression  The expression that is expected to evaluate to a non-zero value.
/// @param ...                An optional printf-style format string and arguments.
/// @ingroup asserts
#define WIN_ASSERT_NOT_ZERO(nonzeroExpression, ...) WinUnit::Assert::IsNotZero(TSTRING(nonzeroExpression), nonzeroExpression, __TFILE__, __LINE__, __VA_ARGS__)

/// The test fails if nullExpression is not NULL.  
/// @param nullExpression   The expression that is expected to evaluate to NULL.
/// @param ...              An optional printf-style format string and arguments.
/// @remarks
/// A compiler error will occur if nullExpression does not evaluate to a 
/// pointer type.
/// @ingroup asserts
#define WIN_ASSERT_NULL(nullExpression, ...) WinUnit::Assert::IsNull(TSTRING(nullExpression), nullExpression, __TFILE__, __LINE__, __VA_ARGS__)

/// The test fails if notNullExpression evaluates to NULL.
/// @param notNullExpression  The expression that is expected to evaluate to a non-NULL value.
/// @param ...                An optional printf-style format string and arguments.
/// @remarks
/// A compiler error will occur if notNullExpression does not evaluate to a 
/// pointer type.
/// @ingroup asserts
#define WIN_ASSERT_NOT_NULL(notNullExpression, ...) WinUnit::Assert::IsNotNull(TSTRING(notNullExpression), notNullExpression, __TFILE__, __LINE__, __VA_ARGS__)

/// @def WIN_ASSERT_FAIL(message, ...)
/// This "test" always fails, displaying the given message.
/// @param message  A printf-style format string (required).
/// @param ...      Optional parameters for the format string.
/// @ingroup asserts
#define WIN_ASSERT_FAIL(message, ...) WinUnit::Assert::Fail(__TFILE__, __LINE__, message, __VA_ARGS__)

/// The test fails if trueExpression does not evaluate to true.
/// @param trueExpression   Expression expected to be true.
/// @param ...              An optional printf-style format string and arguments.
/// @ingroup asserts
#define WIN_ASSERT_TRUE(trueExpression, ...) WinUnit::Assert::IsTrue(TSTRING(trueExpression), (trueExpression ? true : false), __TFILE__, __LINE__, __VA_ARGS__)

/// The test fails if falseExpression evaluates to true.
/// @param falseExpression   Expression expected to be false.
/// @param ...               An optional printf-style format string and arguments.
/// @ingroup asserts
#define WIN_ASSERT_FALSE(falseExpression, ...) WinUnit::Assert::IsFalse(TSTRING(falseExpression), (falseExpression ? true : false), __TFILE__, __LINE__, __VA_ARGS__)

/// This assert is for testing for success of Windows API functions that require
/// GetLastError() to be called for more information.  Because the return 
/// value indicating success is not always the same for system functions, the
/// value passed in should be an expression that evaluates to true if successful.
///     Example: 
///       WIN_ASSERT_WINAPI_SUCCESS(0 != ::DeleteFile(_T("Foo")));
/// The string associated with the GetLastError() code is included in the 
/// exception message.
/// @param trueExpression       Expression that should be true.
/// @param ...                  An optional printf-style format string and arguments.
/// @ingroup asserts
#define WIN_ASSERT_WINAPI_SUCCESS(trueExpression, ...) WinUnit::Assert::WinapiSucceeded(TSTRING(trueExpression), (trueExpression ? true : false), __TFILE__, __LINE__, __VA_ARGS__)

/// The test fails if expression does not throw an exception of type exceptionType.
/// @param expression       Expression expected to throw exception.
/// @param exceptionType    The exception type expected to be thrown.
/// @param ...              An optional printf-style format string and arguments
/// @remarks
/// The exceptionType parameter is simply the name of the exception type--
/// the macro uses it in a catch block.
/// @ingroup asserts
#define WIN_ASSERT_THROWS(expression, exceptionType, ...)                       \
{                                                                               \
    bool __thrown = false;                                                      \
    try { (expression); }                                                       \
    catch(exceptionType&) { __thrown = true; }                                  \
    WinUnit::Assert::ThrowsException(                                           \
       TSTRING(expression), TSTRING(exceptionType),                             \
       __thrown, __TFILE__, __LINE__, __VA_ARGS__);                             \
}

#pragma endregion

#pragma region Setup/Teardown helpers

/// This macro indicates a fixture of name x.
/// @remarks Although you can put more than one fixture in one .cpp file it is
/// a good idea to only use one.  Also, it is recommended that you use the fixture
/// in all tests in the file, for consistency.  If you declare a fixture and use
/// it anywhere, you will need to also define SETUP and 
/// TEARDOWN for the fixture. (@ingroup TestConstruction)
#define FIXTURE(x)                  \
class FIXTURE_##x                   \
{                                   \
public:                             \
    void Setup();                   \
    void Teardown();                \
public:                             \
    FIXTURE_##x() { Setup(); }      \
    ~FIXTURE_##x() { Teardown(); }  \
};

/// This macro starts the setup function for fixture x.  It's used like:
/// @code
///    SETUP(x)
///    {
///       // Do stuff for setup (you can access file-static variables)
///       // This will be called before every test that uses fixture x.
///    }
/// @endcode
/// (@ingroup TestConstruction)
#define SETUP(x) void FIXTURE_##x::Setup()

/// This macro starts the teardown function for fixture x.  It's used like:
/// @code
///    TEARDOWN(x)
///    {
///       // Do stuff that undoes whatever you did in setup.
///       // This will be called after every test that uses fixture x.
///    }
/// @endcode
/// (@ingroup TestConstruction)
#define TEARDOWN(x) void FIXTURE_##x::Teardown()

#pragma endregion

#pragma region AssertException and friends

namespace WinUnit
{
    /// AssertException is a base class for wide-character and ANSI versions, 
    /// used when _UNICODE is defined or not defined, respectively.  The "Message"
    /// method is wide-character-only, so the ANSI version has to account for this.
    /// You do not need to use this class if you are not implementing your own
    /// asserts--use the WIN_ASSERT* macros instead (see @ref asserts).
    /// @remarks If you're implementing your own "asserts", you should use 
    /// the derived AssertExceptionT instead (actually a preprocessor define
    /// replaced by AssertExceptionW in Unicode and AssertExceptionA in ANSI).
    /// You can think of it as declared like this:
    /// @code
    ///   class AssertExceptionT : public AssertException
    ///   {
    ///   public:
    ///        // Append a formatted message to the exception's Message string.  "message"
    ///        // is a printf-style format string; "..." and "args" are two ways of passing
    ///        // in the variable-length argument list.
    ///        void AppendMessage(const TCHAR* message, ...);
    ///        void AppendMessage(const TCHAR* message, va_list args);
    ///        virtual const wchar_t* Message();
    ///   };
    /// @endcode
    class AssertException
    {
    public:
        /// Returns the message associated with this exception.
        virtual const wchar_t* Message() = 0;
    public:
        virtual ~AssertException() = 0 {}
    };
}

// AssertExceptionT is used within the Assert::* methods, all of which 
// use strings of type TCHAR*.
#ifdef _UNICODE
#define AssertExceptionT AssertExceptionW<MAX_ASSERT_MESSAGE_LENGTH>
#else
#define AssertExceptionT AssertExceptionA<MAX_ASSERT_MESSAGE_LENGTH>
#endif

#ifndef EXCLUDE_FROM_DOCUMENTATION
// (The details about the implementations of these classes don't need to be part
// of the documentation on how to use this file.)
namespace WinUnit
{
    // The derived classes of AssertException are templated with a buffer size
    // (size in characters) to make them easier to test.
    //
    // AssertExceptionW is the version with the wide-character interface.
    template<size_t size=MAX_ASSERT_MESSAGE_LENGTH>
    class AssertExceptionW : public AssertException
    {
    private:
        size_t _messageLength;    // The length of the message as filled in so far (starts at 0)
        const size_t _cchBuffer;  // The size of the buffer in characters (same as "size")
        wchar_t _buffer[size];    // The wide-character buffer that holds the message

    public:
        AssertExceptionW();

        // Append a formatted message to the exception's Message string.  "message"
        // is a printf-style format string; "..." and "args" are two ways of passing
        // in the variable-length argument list.
        void AppendMessage(const wchar_t* message, ...);
        void AppendMessage(const wchar_t* message, va_list args);
    public:
        virtual const wchar_t* Message();

    protected:
        // This property is used for clarity when manipulating the buffers; it 
        // returns a pointer to where additional text should be appended.
        __declspec(property(get=GetAvailableBufferStart))
        wchar_t* AvailableBufferStart;
        wchar_t* GetAvailableBufferStart() { return _buffer + _messageLength; }

        // This property is used for clarity when manipulating the buffers; it
        // returns the length left in the buffer, to be used when appending.
        __declspec(property(get=GetAvailableBufferLength))
        size_t AvailableBufferLength;
        size_t GetAvailableBufferLength() { return _cchBuffer - _messageLength; }

        // This property encapsulates the _messageLength member, for symmetry
        // with the other properties.
        __declspec(property(get=GetMessageLength, put=SetMessageLength))
        size_t MessageLength;
        size_t GetMessageLength() { return _messageLength; }
        void SetMessageLength(size_t messageLength) { _messageLength = messageLength; }

    private:
        AssertExceptionW& operator=(AssertExceptionW&);
    };

    // This one extends AssertExceptionW to deal with ANSI strings.
    template<size_t size=MAX_ASSERT_MESSAGE_LENGTH>
    class AssertExceptionA : public AssertExceptionW<size>
    {
    public:
        void AppendMessage(const char* message, ...);
        void AppendMessage(const char* message, va_list args);
    private:
        AssertExceptionA& operator=(AssertExceptionA&);
    };
}
#endif // EXCLUDE_FROM_DOCUMENTATION

#pragma endregion

#pragma region Assert class definition

namespace WinUnit
{
    /// This is the class (with only static methods) which sort of mimics
    /// .NET's Assert class. You do not need to use this class directly--
    /// use the WIN_ASSERT* macros instead (see @ref asserts).  You can use this 
    /// class as an example for your own assert implementations.
    class Assert
    {
    public:
        // AreEqual
        template <class T, class U>
        static void AreEqual(
            const TCHAR* expectedExpressionString, const T& expectedExpressionValue,
            const TCHAR* actualExpressionString, const U& actualExpressionValue,
            const TCHAR* fileName, int lineNumber,
            const TCHAR* message = NULL, ...);

        // AreNotEqual
        template <class T, class U>
        static void AreNotEqual(
            const TCHAR* notExpectedExpressionString, const T& notExpectedExpressionValue,
            const TCHAR* actualExpressionString, const U& actualExpressionValue,
            const TCHAR* fileName, int lineNumber, 
            const TCHAR* message = NULL, ...);

        // StringEqual
        static void StringEqual(
            const wchar_t* expected, const wchar_t* actual,
            const TCHAR* fileName, int lineNumber,
            const TCHAR* message = NULL, ...);

        static void StringEqual(
            const char* expected, const char* actual,
            const TCHAR* fileName, int lineNumber,
            const TCHAR* message = NULL, ...);

        // IsZero
        template<class T>
        static void IsZero(const TCHAR* zeroExpressionString, const T& zeroExpression,
            const TCHAR* fileName, int lineNumber,
            const TCHAR* message = NULL, ...);

        // IsNotZero
        template<class T>
        static void IsNotZero(const TCHAR* nonzeroExpressionString, const T& nonzeroExpression,
            const TCHAR* fileName, int lineNumber, 
            const TCHAR* message = NULL, ...);

        // IsNull
        template<class T>
        static void IsNull(const TCHAR* expressionString, T* nullExpression,
            const TCHAR* fileName, int lineNumber,
            const TCHAR* message = NULL, ...);

        // IsNotNull
        template<class T>
        static void IsNotNull(const TCHAR* notNullExpressionString, T* notNullExpression,
            const TCHAR* fileName, int lineNumber, 
            const TCHAR* message = NULL, ...);

        // Fail
        static void Fail(const TCHAR* fileName, int lineNumber, 
            const TCHAR* message = NULL, ...);

        // IsTrue
        static void IsTrue(const TCHAR* expressionString, bool trueExpression,
            const TCHAR* fileName, int lineNumber, 
            const TCHAR* message = NULL, ...);

        // IsFalse
        static void IsFalse(const TCHAR* expressionString, bool falseExpression,
            const TCHAR* fileName, int lineNumber, 
            const TCHAR* message = NULL, ...);

        // WinapiSucceeded
        static void WinapiSucceeded(const TCHAR* expressionString, 
            bool success, 
            const TCHAR* fileName, int lineNumber, 
            const TCHAR* message = NULL, ...);

        // ThrowsException
        static void ThrowsException(
            const TCHAR* expressionString, 
            const TCHAR* exceptionType, 
            bool exceptionWasThrown, 
            const TCHAR* fileName, int lineNumber,
            const TCHAR* message = NULL, ...);

    private:
        ~Assert();
    };
}
#pragma endregion

#pragma region Assert implementation

namespace WinUnit
{
// AreEqual
template <class T, class U>
inline void Assert::AreEqual(
    const TCHAR* expectedExpressionString, const T& expectedExpressionValue,
    const TCHAR* actualExpressionString, const U& actualExpressionValue,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    // Unfortunately if a numeric literal was passed in as one of the values
    // and an unsigned number as the other, you'll get a signed/unsigned mismatch,
    // as numeric literal integers always template-match to int.  To get around
    // this, postfix numeric literals with 'U' so they will match as unsigned.
    if (expectedExpressionValue == actualExpressionValue) { return; }

    TCHAR buffer1[MAX_EXPRESSION_STRING] = _T("");
    TCHAR buffer2[MAX_EXPRESSION_STRING] = _T("");

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_EQUAL failed. Expected: <%s> (\"%s\"); Actual: <%s> (\"%s\"). "), 
        fileName,
        lineNumber,
        ToString(expectedExpressionValue, buffer1, ARRAYSIZE(buffer1)),
        expectedExpressionString,
        ToString(actualExpressionValue, buffer2, ARRAYSIZE(buffer2)),
        actualExpressionString);

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// AreNotEqual
template <class T, class U>
inline void Assert::AreNotEqual(
    const TCHAR* notExpectedExpressionString, const T& notExpectedExpressionValue,
    const TCHAR* actualExpressionString, const U& actualExpressionValue,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    // Unfortunately if a numeric literal was passed in as one of the values
    // and an unsigned number as the other, you'll get a signed/unsigned mismatch,
    // as numeric literal integers always template-match to int.  To get around
    // this, postfix numeric literals with 'U' so they will match as unsigned.
    if (notExpectedExpressionValue != actualExpressionValue) { return; }

    TCHAR buffer1[MAX_EXPRESSION_STRING] = _T("");

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_NOT_EQUAL failed. \"%s\" and \"%s\" were expected different, but were same (<%s>). "), 
        fileName,
        lineNumber,
        notExpectedExpressionString,
        actualExpressionString,
        ToString(notExpectedExpressionValue, buffer1, ARRAYSIZE(buffer1)));

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// StringEqual
inline void Assert::StringEqual(
    const wchar_t* expected, const wchar_t* actual,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    if (::wcscmp(expected, actual) == 0) { return; }

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_STRING_EQUAL failed. "), 
        fileName, lineNumber);
#ifdef _UNICODE
    wchar_t* formatString = L"Expected: <\"%s\">; Actual: <\"%s\">. ";
#else
    char* formatString = "Expected: <\"%S\">; Actual: <\"%S\">. ";
#endif
    exception.AppendMessage(formatString, expected, actual); 

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

inline void Assert::StringEqual(
    const char* expected, const char* actual,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    if (::strcmp(expected, actual) == 0) { return; }

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_STRING_EQUAL failed. "), 
        fileName, lineNumber);
#ifdef _UNICODE
    wchar_t* formatString = L"Expected: <\"%S\">; Actual: <\"%S\">. ";
#else
    char* formatString = "Expected: <\"%s\">; Actual: <\"%s\">. ";
#endif
    exception.AppendMessage(formatString, expected, actual); 

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// IsZero
template<class T>
inline void Assert::IsZero(const TCHAR* zeroExpressionString, const T& zeroExpression,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    if (zeroExpression == 0) { return; }

    TCHAR buffer[MAX_EXPRESSION_STRING] = _T("");

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_ZERO failed: \"%s\" expected to be zero, but was <%s>. "),
        fileName, lineNumber,
        zeroExpressionString,
        ToString(zeroExpression, buffer, ARRAYSIZE(buffer)));

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// IsNotZero
template<class T>
inline void Assert::IsNotZero(
    const TCHAR* nonzeroExpressionString, const T& nonzeroExpression,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    if (nonzeroExpression != 0) { return; }

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_NOT_ZERO failed: \"%s\". "),
        fileName, lineNumber,
        nonzeroExpressionString);

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// IsNull
template<class T>
inline void Assert::IsNull(const TCHAR* expressionString, T* nullExpression,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    if (nullExpression == NULL) { return; }

    TCHAR buffer[MAX_EXPRESSION_STRING] = _T("");

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_NULL failed: \"%s\" expected to be NULL, but was <%s>. "),
        fileName, lineNumber,
        expressionString,
        ToString(nullExpression, buffer, _countof(buffer)));

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// IsNotNull
template<class T>
inline void Assert::IsNotNull(
    const TCHAR* notNullExpressionString, T* notNullExpression,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    if (notNullExpression != NULL) { return; }

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_NOT_NULL failed: \"%s\". "),
        fileName, lineNumber,
        notNullExpressionString);

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// Fail
inline void Assert::Fail(const TCHAR* fileName, int lineNumber, 
                         const TCHAR* message, ...)
{
    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_FAIL failed. "),
        fileName,
        lineNumber);

    va_list args;
    va_start(args, message);
    exception.AppendMessage(message, args);
    va_end(args);

    throw exception;
}

// IsTrue
inline void Assert::IsTrue(
    const TCHAR* expressionString, bool expression,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message, ...)
{
    if (expression) { return; }

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_TRUE failed: \"%s\". "), fileName, lineNumber, expressionString);

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// IsFalse
inline void Assert::IsFalse(const TCHAR* expressionString, bool falseExpression,
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    if (!falseExpression) { return; }

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_FALSE failed: \"%s\". "), fileName, lineNumber, expressionString);

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// WinapiSucceeded
inline void Assert::WinapiSucceeded(
        const TCHAR* expressionString, 
        bool success, 
        const TCHAR* fileName, int lineNumber, 
        const TCHAR* message /* NULL */, ...)
{
    if (success) { return; }

    // Get error string associated with GetLastError().
    DWORD error = ::GetLastError();
    TCHAR errorBuffer[MAX_SYSERROR_STRING] = _T("");

    DWORD charsWritten = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        errorBuffer,
        MAX_SYSERROR_STRING, NULL );

    // These error strings tend to end with \r\n
    if (charsWritten > 0 && 
        errorBuffer[charsWritten - 1] == _T('\n') &&
        errorBuffer[charsWritten - 2] == _T('\r'))
    {
        errorBuffer[charsWritten - 2] = _T('\0');
    }

    AssertExceptionT exception;
    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_WINAPI_SUCCESS failed: \"%s\". (%s) "), 
        fileName, lineNumber, expressionString, errorBuffer);

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}

// ThrowsException
inline void Assert::ThrowsException(
    const TCHAR* expressionString, 
    const TCHAR* exceptionType, 
    bool exceptionWasThrown, 
    const TCHAR* fileName, int lineNumber,
    const TCHAR* message /* NULL */, ...)
{
    if (exceptionWasThrown) { return; }

    AssertExceptionT exception;

    exception.AppendMessage(_T("%s(%d): error : WIN_ASSERT_THROWS failed: Exception %s not thrown by \"%s\". "),
        fileName, lineNumber,
        exceptionType, expressionString);

    if (message)
    {
        va_list args;
        va_start(args, message);
        exception.AppendMessage(message, args);
        va_end(args);
    }

    throw exception;
}
}

#pragma endregion

#pragma region AssertExceptionT implementation

#ifndef EXCLUDE_FROM_DOCUMENTATION
namespace WinUnit
{
template<size_t size>
inline AssertExceptionW<size>::AssertExceptionW()
        :
_messageLength(0),
_cchBuffer(size)
{
    wmemset(_buffer, L'\0', size);
}

template<size_t size>
inline const wchar_t* AssertExceptionW<size>::Message()
{
    return _buffer;
}

template<size_t size>
inline void AssertExceptionW<size>::AppendMessage(const wchar_t* message, ...)
{
    va_list args;
    va_start(args, message);

    this->AppendMessage(message, args);

    va_end(args);
}

template<size_t size>
inline void AssertExceptionW<size>::AppendMessage(const wchar_t* message, va_list args)
{
    if (this->AvailableBufferLength == 0) { return; }

    int cchWritten = _vsnwprintf_s(
        this->AvailableBufferStart,  
        this->AvailableBufferLength, 
        _TRUNCATE,                   
        message,                        
        args);

    // If _vsnwprintf_s returns -1, it means a truncation occurred.  Therefore
    // the buffer is now completely full--so set MessageLength appropriately.
    // Note that it's not set to "size" since MessageLength does not include 
    // the null terminator.
    if (cchWritten == -1)
    {
        this->MessageLength = size - 1;
    }
    else
    {
        this->MessageLength += cchWritten;
    }
}

template<size_t size>
inline void AssertExceptionA<size>::AppendMessage(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    this->AppendMessage(message, args);
    va_end(args);
}

template<size_t size>
inline void AssertExceptionA<size>::AppendMessage(const char* message, va_list args)
{
    // For the ANSI versions of AppendMessage, we first print the formatted
    // message into an ANSI buffer, then use MultiByteToWideChar to copy it
    // (append it) into the wide character buffer.
    //
    // (Avoiding "min" macro for NOMINMAX compatibility.)
    const int bufferSize = (size < INT_MAX) ? size : INT_MAX;
    char bufferA[bufferSize] = "";

    int cchWritten = ::vsnprintf_s(
        bufferA, bufferSize, _TRUNCATE, message, args);

    int availableBufferLength = (int)(this->AvailableBufferLength);
    int bytesToWrite =
        ((cchWritten == -1 || cchWritten > availableBufferLength - 1) ? 
        availableBufferLength - 1 :
        cchWritten);

    int cchConverted = ::MultiByteToWideChar(CP_ACP, 0, 
        bufferA, bytesToWrite, 
        this->AvailableBufferStart, availableBufferLength);

    // If it was 0, nothing was copied
    if (cchConverted != 0)
    {
        this->MessageLength += cchConverted;
    }
}
}
#endif // EXCLUDE_FROM_DOCUMENTATION
#pragma endregion

#pragma region ToString implementation

namespace WinUnit
{
// Objects of an unknown type are shown as [OBJECT].  If you see this 
// in your error message and want something clearer, you can implement
// ToString for your type.
template<class T>
inline const TCHAR* ToString(const T& /* object */, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%s"), _T("[OBJECT]"));
    return buffer;
}

// Pointers are represented by their hex value.  
template<class T>
inline const TCHAR* ToString(T* object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("0x%0*p"), sizeof(void*) * 2, (void*)object);
    return buffer;
}

// Define BOOL_NOT_DEFINED if you get a compiler error with this one.
#ifndef BOOL_NOT_DEFINED
template<>
inline const TCHAR* ToString(const bool& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%s"), (object ? _T("true") : _T("false")));
    return buffer;
}
#endif

template<>
inline const TCHAR* ToString(const signed char& object, TCHAR buffer[], size_t bufferSize)
{
    bool isPrintable = (int)object >= 0 && (int)object <= 0xFF && isprint((int)object);
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("'%lc' [%i]"), 
        isPrintable ? object : '.', object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const unsigned char& object, TCHAR buffer[], size_t bufferSize)
{
    bool isPrintable = (int)object >= 0 && (int)object <= 0xFF && isprint((int)object);
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("'%lc' [0x%02X]"), 
        isPrintable ? object : '.', object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const char& object, TCHAR buffer[], size_t bufferSize)
{
    bool isPrintable = (int)object >= 0 && (int)object <= 0xFF && isprint((int)object);
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("'%lc' [%i]"), 
        isPrintable ? object : '.', object);
    return buffer;
}

#ifdef _NATIVE_WCHAR_T_DEFINED
template<>
inline const TCHAR* ToString(const wchar_t& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("'%wc' [0x%04X]"), 
        iswprint(object) ? object : L'.', object);
    return buffer;
}
#endif

template<>
inline const TCHAR* ToString(const __int16& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%ld"), object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const unsigned __int16& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%lu [0x%04lX]"), object, object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const __int32& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%d"), object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const unsigned __int32& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%u [0x%08X]"), object, object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const long& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%d"), object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const unsigned long& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%u [0x%0*X]"), object, sizeof(object) * 2, object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const __int64& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%I64i"), object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const unsigned __int64& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%I64u [0x%0*I64X]"), object, sizeof(object) * 2, object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const float& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%G"), object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const double& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%G"), object);
    return buffer;
}

template<>
inline const TCHAR* ToString(const long double& object, TCHAR buffer[], size_t bufferSize)
{
    ::_sntprintf_s(buffer, bufferSize, _TRUNCATE, _T("%G"), object);
    return buffer;
}

}
#pragma endregion

#pragma region Trace

/// @defgroup Trace Trace
/// @{

/// The WIN_TRACE macro should be used for informational messages that are not 
/// test errors.
/// @param x        A printf-style format string.
/// @param ...      Arguments to the format string, if any.
/// @remarks 
/// This function calls OutputDebugString.  When tests are run, the output
/// sent to OutputDebugString is captured and sent to whichever loggers are 
/// active.  Either wide-character or ANSI strings are allowed.
#define WIN_TRACE(x, ...) WinUnit::Trace(x, __VA_ARGS__)

/// @}

namespace WinUnit
{
    void Trace(const wchar_t* formatString, ...);
    void Trace(const char* formatString, ...);

    /// Wide-character version of function called by WIN_TRACE macro.
    inline void Trace(const wchar_t* formatString, ...)
    {
        wchar_t buffer[MAX_TRACE_MESSAGE_LENGTH] = L"";
        va_list args;
        va_start(args, formatString);

        ::_vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE,
            formatString, args);
        
        va_end(args);
        OutputDebugStringW(buffer);
    }

    /// ANSI version of function called by WIN_TRACE macro.
    inline void Trace(const char* formatString, ...)
    {
        char buffer[MAX_TRACE_MESSAGE_LENGTH] = "";
        va_list args;
        va_start(args, formatString);

        _vsnprintf_s(buffer, _countof(buffer), _TRUNCATE,
            formatString, args);
        
        va_end(args);
        OutputDebugStringA(buffer);
    }
}

#pragma endregion

#pragma region Environment

namespace WinUnit
{
    /// This class assists in retrieving environment variables.  It is useful if
    /// environment variables were passed in on the command line of WinUnit.exe.
    class Environment
    {
    public:
        static bool GetVariable(const TCHAR* variableName, 
            TCHAR buffer[], DWORD bufferSize,
            DWORD* pSizeNeeded = NULL);
    private:
        Environment();
    };

    /// Gets value of environment variable named variableName.  
    /// @return true if found and buffer was big enough to hold it; otherwise false.
    inline bool Environment::GetVariable(
         const TCHAR* variableName,         ///< Case-insensitive name of variable to be looked up
         TCHAR buffer[],                    ///< Buffer to receive value of variable
         DWORD bufferSize,                  ///< Size of buffer, in TCHARs
         DWORD* pSizeNeeded /* NULL */ )    ///< [out] Buffer size that was needed, if too 
                                            ///< small (zero if variable not found)
    {
        DWORD charCount = GetEnvironmentVariable(variableName, buffer, bufferSize);
        if (pSizeNeeded != NULL)
        {
            *pSizeNeeded = charCount;
        }
        return (charCount > 0 && charCount < bufferSize);
    }
}

#pragma endregion
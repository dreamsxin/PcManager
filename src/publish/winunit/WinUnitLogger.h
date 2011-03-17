/* -------------------------------------------------------------------------- *
   WinUnit - Maria Blees (maria.blees@microsoft.com)
 * -------------------------------------------------------------------------- */

/// @file WinUnitLogger.h
///
/// This file contains function prototypes for functions to be implemented by
/// a custom logger DLL for WinUnit.

#ifndef WINUNIT_LOGGER_FUNCTION
#define WINUNIT_LOGGER_FUNCTION(returntype, func) extern "C" __declspec(dllexport) returntype __cdecl func
#endif

/// @defgroup CustomLoggerFunctions Implementing a Custom Logger
/// To implement a custom logger, create a DLL that implements and exports one or more of
/// the following functions.  A very simple example would be a logger that writes
/// out the results the end of a test run, to stdout if no failures, and to 
/// stderr if there were failures.
/// @remarks The functions must be exported by the exact names shown.  This can
/// be accomplished by using a .def file or by using the WINUNIT_LOGGER_FUNCTION
/// macro to prepend 'extern "C" __declspec(dllexport)'.
///
/// @par Example:
/// (See @ref GettingStarted for how to build MyTests.dll.)
///
/// @code 
/// // ResultsLogger.cpp
/// // Build with: cl [/I includePath] /LD ResultsLogger.cpp 
/// // Run with: WinUnit -q -l ResultsLogger.dll MyTests.dll
///
/// #include "WinUnitLogger.h"
/// #include <stdio.h>
/// 
/// WINUNIT_LOGGER_FUNCTION(void, OutputFinalResults)(
///    unsigned int succeeded, 
///    unsigned int totalRun)
/// {
///     if (succeeded < totalRun)
///     {
///         fprintf(stderr, "There were %d failures out of %d tests run.\n", totalRun - succeeded, totalRun);
///     }
///     else
///     {
///         printf("There were no failures out of %d tests run.\n", totalRun);
///     }
/// }
///
/// @endcode
///

/// @{

/// Initialize the logger with any required data.
/// @remarks The initialization string can be any string, and should be parsed 
/// appropriately in this function.  It is the string passed in on the command
/// line after the name of this DLL and a colon.  Consider outputting usage
/// information to stderr if an invalid string is passed in (or NULL).
WINUNIT_LOGGER_FUNCTION(void, Initialize)(
    const wchar_t* initializationString  ///< String for initializing this logger.
    );

/// Outputs the full path and just the name of the executable tests
/// are being run from, before running the tests.
WINUNIT_LOGGER_FUNCTION(void, OutputTestExecutableNamePreTest)(
    const wchar_t* fullPath,            ///< Full path to the executable whose tests are to be run.
    const wchar_t* nameOfExecutable     ///< Name of executable.
);

/// Outputs the full path and just the name of the executable tests
/// are being run from, after running the tests.
WINUNIT_LOGGER_FUNCTION(void, OutputTestExecutableNamePostTest)(
    const wchar_t* fullPath,         ///< Full path to the executable whose tests were run.
    const wchar_t* nameOfExecutable, ///< Name of executable.
    unsigned int succeeded,          ///< Number of tests succeeded.
    unsigned int totalRun            ///< Number of tests run.
);

/// Outputs the name of the test about to be run.
/// @remarks You can expect that before each test is run, this function will be
/// called, followed by any traces or errors that came from the test, followed
/// by OutputTestNamePostTest.
WINUNIT_LOGGER_FUNCTION(void, OutputTestNamePreTest)(
    const char* testName   ///< Name of test about to be run.
);

/// Outputs the name of the test after it has been run, and whether 
/// or not it passed.
/// @remarks The only reason this would not be called after a test would be if
/// an unrecoverable error had occurred during the test and the application was
/// being shut down.
WINUNIT_LOGGER_FUNCTION(void, OutputTestNamePostTest)(
    const char* testName,   ///< Name of test that was just run.
    bool passed             ///< True if test passed.
);

/// Output a trace message.  This comes from any OutputDebugString
/// calls that have been made in the test executable.
/// @remarks Messages are expected to contain newlines where appropriate.
/// Message may be a continuation of a previous message.
WINUNIT_LOGGER_FUNCTION(void, OutputTrace)(
    const wchar_t* message ///< Informational message to be output.
);

/// Output a message from a failed test.
/// @remarks This will occur after OutputTestNamePreTest has been called but before
/// OutputTestNamePostTest, for the test in question.  You can expect that the 
/// message will end with a period, but not a newline.
WINUNIT_LOGGER_FUNCTION(void, OutputTestError)(
    const wchar_t* message    ///< Message indicating details about failure.
);

/// Output an error message that describes an unexpected occurrence in the 
/// program (e.g. file not found).
/// @remarks The line passed in will be a complete error message, but does not
/// include a newline.
WINUNIT_LOGGER_FUNCTION(void, OutputProgramErrorLine)(
    const wchar_t* message
    );

/// Output final results from all tests run.
/// @remarks This can be treated as an "Uninitialize" function.  You can expect
/// it will be the last function to be called, and cleanup can be done here.
WINUNIT_LOGGER_FUNCTION(void, OutputFinalResults)(
    unsigned int succeeded,  ///< Total number of tests that succeeded.
    unsigned int totalRun    ///< Total number of tests that were run.
);

/// Set verbosity level.
/// @remarks This can be ignored or stored and used to decide how to handle
/// output sent to the other (output) functions.  This is set by the user on
/// the command line.  The default is 1.
WINUNIT_LOGGER_FUNCTION(void, SetVerbosity)(
    unsigned int verbosity
);

/// @}

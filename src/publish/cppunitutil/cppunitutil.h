/**
* @file    cppunitutil.h
* @brief   ...
* @author  zhangrui
* @date    2008-11-14  18:34
*/

#ifndef CPPUNITUTIL_H
#define CPPUNITUTIL_H

#pragma warning(push)
#pragma warning(disable: 4996)  // 'function': was declared deprecated

#include <atlstr.h>

#include "cppunit/TestRunner.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestResultCollector.h"

#include "cppunit/BriefTestProgressListener.h"

#include "cppunit/CompilerOutputter.h"
#include "cppunit/XmlOutputter.h"

#include "cppunit/TestFixture.h"

#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/extensions/HelperMacros.h"

#ifdef _DLL
#   ifdef _DEBUG
#       pragma comment(lib, "libcppunit-mdd.lib")
#   else
#       pragma comment(lib, "libcppunit-md.lib")
#   endif
#else
#   ifdef _DEBUG
#       pragma comment(lib, "libcppunit-mtd.lib")
#   else
#       pragma comment(lib, "libcppunit-mt.lib")
#   endif
#endif

inline int CppUnitMain(LPCWSTR lpTestTarget, LPCSTR lpTestSuite = "All Tests")
{
    // Create the event manager and test controller
    CPPUNIT_NS::TestResult controller; 


    // Add a listener that collects test result
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);


    // Add a listener that print the name of each test before running it.
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener(&progress);


    // Add the top suite to the test runner
    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry(lpTestSuite).makeTest());
    runner.run(controller);


    // Print test in a compiler compatible format.
    CPPUNIT_NS::CompilerOutputter compilerOutputter(&result, CPPUNIT_NS::stdCOut());
    compilerOutputter.write(); 


    // Print test to xml
    CString strTestPath;
    GetModuleFileName(NULL, strTestPath.GetBuffer(BUFSIZ), BUFSIZ);
    strTestPath.Append(lpTestTarget);
    strTestPath.Append(L".unittest.xml");
    CPPUNIT_NS::OFileStream xmlFile((LPCSTR) CW2A(strTestPath));
    CPPUNIT_NS::XmlOutputter xmlOutputter(&result, xmlFile, "utf-8");
    xmlOutputter.write(); 


    return result.wasSuccessful() ? 0 : 1;
}

inline int CppUnitMainQuiet(LPCWSTR lpTestTarget)
{
    // Create the event manager and test controller
    CPPUNIT_NS::TestResult controller; 


    // Add a listener that collects test result
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);


    // Add the top suite to the test runner
    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);


    // Print test to xml
    CString strTestPath;
    GetModuleFileName(NULL, strTestPath.GetBuffer(BUFSIZ), BUFSIZ);
    strTestPath.Append(lpTestTarget);
    strTestPath.Append(L".unittest.xml");
    CPPUNIT_NS::OFileStream  xmlFile((LPCSTR) CW2A(strTestPath));
    CPPUNIT_NS::XmlOutputter xmlOutputter(&result, xmlFile, "utf-8");
    xmlOutputter.write(); 


    return result.wasSuccessful() ? 0 : 1;
}




CPPUNIT_NS_BEGIN

#define CPPUNIT_ASSERT_WSTR_EQUAL(expected,actual)              \
  ( CPPUNIT_NS::assertEquals<LPCWSTR>((expected),               \
                              (actual),                         \
                              CPPUNIT_SOURCELINE(),             \
                              "" ) )

#define CPPUNIT_ASSERT_STR_EQUAL(expected,actual)               \
  ( CPPUNIT_NS::assertEquals<LPCSTR>((expected),                \
                              (actual),                         \
                              CPPUNIT_SOURCELINE(),             \
                              "" ) )

#define CPPUNIT_ASSERT_WSTR_EQUAL_MESSAGE(msg,expected,actual)  \
  ( CPPUNIT_NS::assertEquals<LPCWSTR>((expected),               \
                              (actual),                         \
                              CPPUNIT_SOURCELINE(),             \
                              msg ) )

#define CPPUNIT_ASSERT_STR_EQUAL_MESSAGE(msg,expected,actual)   \
  ( CPPUNIT_NS::assertEquals<LPCSTR>((expected),                \
                              (actual),                         \
                              CPPUNIT_SOURCELINE(),             \
                              msg ) )

#define CPPUNIT_ASSERT_MEM_EQUAL(expected,actual,len)           \
    ( CPPUNIT_NS::assertEquals<CPPUNIT_NS::MEMTYPE>(            \
    (CPPUNIT_NS::MEMTYPE(expected, len)),                       \
    (CPPUNIT_NS::MEMTYPE(actual, len)),                         \
    CPPUNIT_SOURCELINE(),                                       \
    "" ) )


template <>
struct assertion_traits<LPCWSTR>
{  
    static bool equal(LPCWSTR x, LPCWSTR y)
    {
        if (!x && !y)
            return TRUE;

        if (!x || !y)
            return FALSE;

        return 0 == wcscmp(x, y);
    }

    static std::string toString(LPCWSTR x)
    {
        if (!x)
            return "";

        return (LPCSTR)CW2A(x, CP_UTF8);
    }
};

template <>
struct assertion_traits<LPCSTR>
{  
    static bool equal(LPCSTR x, LPCSTR y)
    {
        if (!x && !y)
            return TRUE;

        if (!x || !y)
            return FALSE;

        return 0 == strcmp(x, y);
    }

    static std::string toString(LPCSTR x)
    {
        return x ? x : "";
    }
};



template <>
struct assertion_traits<ULONGLONG>
{  
    static bool equal( ULONGLONG x, ULONGLONG y )
    {
        return x == y;
    }

    static std::string toString( ULONGLONG x )
    {
        char buffer[128];
        sprintf(buffer, "%I", x); 
        return buffer;
    }
};





struct MEMTYPE
{
    MEMTYPE(const void* pMem, size_t nLen):
        m_pMem(pMem),
        m_nLen(nLen)
    {
    }

    const void* m_pMem;
    size_t      m_nLen;   
};

template <>
struct assertion_traits<MEMTYPE>
{  
    static bool equal( MEMTYPE x, MEMTYPE y )
    {
        if (x.m_nLen != y.m_nLen)
            return FALSE;

        return 0 == memcmp(x.m_pMem, y.m_pMem, x.m_nLen);
    }

    static std::string toString( MEMTYPE x )
    {
        if (!x.m_pMem)
            return "NULL";

        CStringA strHex;
        LPCSTR   pMem = (LPCSTR)x.m_pMem;
        for (size_t i = 0; i < x.m_nLen; ++i)
        {
            strHex.AppendFormat("%02x", (BYTE)pMem[i]);
        }

        return (LPCSTR)strHex;
    }
};




#pragma warning(pop)

CPPUNIT_NS_END

#endif//CPPUNITUTIL_H
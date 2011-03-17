/**
* @file    bkxdx_example.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-10 10:38
*/

#ifndef BKXDX_EXAMPLE_H
#define BKXDX_EXAMPLE_H

#include "bkxdx.h"
#include "bkxdx_cursor_bkjson.h"    // use bkjson implements
#include "bkxdx_exchange_c.h"       // support c type exchange
#include "bkxdx_exchange_atl.h"     // support ATL class exchange
#include "bkxdx_exchange_stl.h"     // support STL class exchange


struct TestCType
{
    signed   int        m_int_max;
    signed   int        m_int_min;
    unsigned int        m_uint_max;

    signed   long       m_long_max;
    signed   long       m_long_min;
    unsigned long       m_ulong_max;

    signed   short      m_short_max;
    signed   short      m_short_min;
    unsigned short      m_ushort_max;

    char                m_char_max;
    char                m_char_min;
    signed char         m_schar_max;
    signed char         m_schar_min;
    unsigned char       m_uchar_max;

    void Reset()
    {
        memset(this, 0, sizeof(*this));
    }
};

XDX_CLASS_BEGIN(TestCType)
    XDX_MEMBER_OBJECT(L"int_max",       m_int_max)
    XDX_MEMBER_OBJECT(L"int_min",       m_int_min)
    XDX_MEMBER_OBJECT(L"uint_max",      m_uint_max)

    XDX_MEMBER_OBJECT(L"long_max",      m_long_max)
    XDX_MEMBER_OBJECT(L"long_min",      m_long_min)
    XDX_MEMBER_OBJECT(L"ulong_max",     m_ulong_max)

    XDX_MEMBER_OBJECT(L"short_max",     m_short_max)
    XDX_MEMBER_OBJECT(L"short_min",     m_short_min)
    XDX_MEMBER_OBJECT(L"ushort_max",    m_ushort_max)

    XDX_MEMBER_OBJECT(L"char_max",      m_char_max)
    XDX_MEMBER_OBJECT(L"char_min",      m_char_min)
    XDX_MEMBER_OBJECT(L"schar_max",     m_schar_max)
    XDX_MEMBER_OBJECT(L"schar_min",     m_schar_min)
    XDX_MEMBER_OBJECT(L"uchar_max",     m_uchar_max)
XDX_CLASS_END




struct TestExtraCType
{
    signed   __int64    m_int64_max;
    signed   __int64    m_int64_min;
    unsigned __int64    m_uint64_max;

    void Reset()
    {
        memset(this, 0, sizeof(*this));
    }
};

XDX_CLASS_BEGIN(TestExtraCType)
    XDX_MEMBER_OBJECT(L"int64_max",     m_int64_max)
    XDX_MEMBER_OBJECT(L"int64_min",     m_int64_min)
    XDX_MEMBER_OBJECT(L"uint64_max",    m_uint64_max)
XDX_CLASS_END






struct TestATLClass
{
    ATL::CStringW                               m_atl_stringw;
    ATL::CStringA                               m_atl_stringa;
    ATL::CAtlArray<CStringW>                    m_atl_array;
    ATL::CAtlArray<CStringW>                    m_atl_array_empty;
    ATL::CAtlList<CStringW>                     m_atl_list;
    ATL::CAtlList<CStringW>                     m_atl_list_empty;
    ATL::CAtlMap<CStringW, CStringW>            m_atl_map;
    ATL::CAtlMap<CStringW, CStringW>            m_atl_map_empty;
    ATL::CRBMap<CStringW, CStringW>             m_atl_rbmap;
    ATL::CRBMap<CStringW, CStringW>             m_atl_rbmap_empty;
    ATL::CRBMultiMap<CStringW, CStringW>        m_atl_rbmultimap;
    ATL::CRBMultiMap<CStringW, CStringW>        m_atl_rbmultimap_empty;

    ATL::CAtlMap<CStringW, CStringW>            m_atl_map_as_dictionary;
    ATL::CAtlMap<CStringW, CStringW>            m_atl_map_as_dictionary_empty;
    ATL::CRBMap<CStringW, CStringW>             m_atl_rbmap_as_dictionary;
    ATL::CRBMap<CStringW, CStringW>             m_atl_rbmap_as_dictionary_empty;

    void Reset()
    {
        m_atl_stringw.Empty();
        m_atl_stringa.Empty();
        m_atl_array.RemoveAll();
        m_atl_array_empty.RemoveAll();
        m_atl_list.RemoveAll();
        m_atl_list_empty.RemoveAll();
        m_atl_map.RemoveAll();
        m_atl_map_empty.RemoveAll();
        m_atl_rbmap.RemoveAll();
        m_atl_rbmap_empty.RemoveAll();
        m_atl_rbmultimap.RemoveAll();
        m_atl_rbmultimap_empty.RemoveAll();

        m_atl_map_as_dictionary.RemoveAll();
        m_atl_map_as_dictionary_empty.RemoveAll();
        m_atl_rbmap_as_dictionary.RemoveAll();
        m_atl_rbmap_as_dictionary_empty.RemoveAll();
    }
};

XDX_CLASS_BEGIN(TestATLClass)
    XDX_MEMBER_OBJECT(L"atl_stringw",                       m_atl_stringw)
    XDX_MEMBER_OBJECT(L"atl_stringa",                       m_atl_stringa)
    XDX_MEMBER_OBJECT(L"atl_array",                         m_atl_array)
    XDX_MEMBER_OBJECT(L"atl_array_empty",                   m_atl_array_empty)
    XDX_MEMBER_OBJECT(L"atl_list",                          m_atl_list)
    XDX_MEMBER_OBJECT(L"atl_list_empty",                    m_atl_list_empty)
    XDX_MEMBER_OBJECT(L"atl_map",                           m_atl_map)
    XDX_MEMBER_OBJECT(L"atl_map_empty",                     m_atl_map_empty)
    XDX_MEMBER_OBJECT(L"atl_rbmap",                         m_atl_rbmap)
    XDX_MEMBER_OBJECT(L"atl_rbmap_empty",                   m_atl_rbmap_empty)
    XDX_MEMBER_OBJECT(L"atl_rbmultimap",                    m_atl_rbmultimap)
    XDX_MEMBER_OBJECT(L"atl_rbmultimap_empty",              m_atl_rbmultimap_empty)
    
    XDX_MEMBER_DICTIONARY(L"atl_map_as_dictionary",         m_atl_map_as_dictionary)
    XDX_MEMBER_DICTIONARY(L"atl_map_as_dictionary_empty",   m_atl_map_as_dictionary_empty)
    XDX_MEMBER_DICTIONARY(L"atl_rbmap_as_dictionary",       m_atl_rbmap_as_dictionary)
    XDX_MEMBER_DICTIONARY(L"atl_rbmap_as_dictionary_empty", m_atl_rbmap_as_dictionary_empty)
XDX_CLASS_END


#ifndef _ATL_MIN_CRT
struct TestSTLClass
{
    std::wstring                                m_stl_stringw;
    std::string                                 m_stl_stringa;
    std::pair<std::wstring, std::wstring>       m_stl_pair;
    std::vector<std::wstring>                   m_stl_vector;
    std::vector<std::wstring>                   m_stl_vector_empty;
    std::list<std::wstring>                     m_stl_list;
    std::list<std::wstring>                     m_stl_list_empty;
    std::deque<std::wstring>                    m_stl_deque;
    std::deque<std::wstring>                    m_stl_deque_empty;
    std::set<std::wstring>                      m_stl_set;
    std::set<std::wstring>                      m_stl_set_empty;
    std::map<std::wstring, std::wstring>        m_stl_map;
    std::map<std::wstring, std::wstring>        m_stl_map_empty;
    std::multiset<std::wstring>                 m_stl_multiset;
    std::multiset<std::wstring>                 m_stl_multiset_empty;
    std::multimap<std::wstring, std::wstring>   m_stl_multimap;
    std::multimap<std::wstring, std::wstring>   m_stl_multimap_empty;

    std::map<std::wstring, std::wstring>        m_stl_map_as_dictionary;
    std::map<std::wstring, std::wstring>        m_stl_map_as_dictionary_empty;

    void Reset()
    {
        m_stl_stringw.clear();
        m_stl_stringa.clear();
        m_stl_pair.first.clear();
        m_stl_pair.second.clear();
        m_stl_vector.clear();
        m_stl_vector_empty.clear();
        m_stl_list.clear();
        m_stl_list_empty.clear();
        m_stl_deque.clear();
        m_stl_deque_empty.clear();
        m_stl_set.clear();
        m_stl_set_empty.clear();
        m_stl_map.clear();
        m_stl_map_empty.clear();
        m_stl_multiset.clear();
        m_stl_multiset_empty.clear();
        m_stl_multimap.clear();
        m_stl_multimap_empty.clear();

        m_stl_map_as_dictionary.clear();
        m_stl_map_as_dictionary_empty.clear();
    }
};

XDX_CLASS_BEGIN(TestSTLClass)
    XDX_MEMBER_OBJECT(L"stl_stringw",                       m_stl_stringw)
    XDX_MEMBER_OBJECT(L"stl_stringa",                       m_stl_stringa)
    XDX_MEMBER_OBJECT(L"stl_pair",                          m_stl_pair)
    XDX_MEMBER_OBJECT(L"stl_vector",                        m_stl_vector)
    XDX_MEMBER_OBJECT(L"stl_vector_empty",                  m_stl_vector_empty)
    XDX_MEMBER_OBJECT(L"stl_list",                          m_stl_list)
    XDX_MEMBER_OBJECT(L"stl_list_empty",                    m_stl_list_empty)
    XDX_MEMBER_OBJECT(L"stl_deque",                         m_stl_deque)
    XDX_MEMBER_OBJECT(L"stl_deque_empty",                   m_stl_deque_empty)
    XDX_MEMBER_OBJECT(L"stl_set",                           m_stl_set)
    XDX_MEMBER_OBJECT(L"stl_set_empty",                     m_stl_set_empty)
    XDX_MEMBER_OBJECT(L"stl_map",                           m_stl_map)
    XDX_MEMBER_OBJECT(L"stl_map_empty",                     m_stl_map_empty)
    XDX_MEMBER_OBJECT(L"stl_multiset",                      m_stl_multiset)
    XDX_MEMBER_OBJECT(L"stl_multiset_empty",                m_stl_multiset_empty)
    XDX_MEMBER_OBJECT(L"stl_multimap",                      m_stl_multimap)
    XDX_MEMBER_OBJECT(L"stl_multimap_empty",                m_stl_multimap_empty)

    XDX_MEMBER_OBJECT(L"stl_map_as_dictionary",             m_stl_map_as_dictionary)
    XDX_MEMBER_DICTIONARY(L"stl_map_as_dictionary_empty",   m_stl_map_as_dictionary_empty)
XDX_CLASS_END
#endif//_ATL_MIN_CRT







struct TestObject
{
    TestCType       m_test_c_type;
    TestExtraCType  m_test_extra_c_type;
    TestATLClass    m_test_atl_class;
#ifndef _ATL_MIN_CRT
    TestSTLClass    m_test_stl_class;
#endif//_ATL_MIN_CRT

    void Reset()
    {
        m_test_c_type.Reset();
        m_test_extra_c_type.Reset();
        m_test_atl_class.Reset();
    }
};

XDX_CLASS_BEGIN(TestObject)
    XDX_MEMBER_OBJECT(L"c_type",        m_test_c_type)
    XDX_MEMBER_OBJECT(L"extra_c_type",  m_test_extra_c_type)
    XDX_MEMBER_OBJECT(L"atl_class",     m_test_atl_class)
#ifndef _ATL_MIN_CRT
    XDX_MEMBER_OBJECT(L"stl_class",     m_test_stl_class)
#endif//_ATL_MIN_CRT
XDX_CLASS_END






#define TEST_WIDE_STRING   L"~!@#$%^&*()_+`-=[{]}\\|;:'\",<.>/?"
#define TEST_ANSI_STRING    "~!@#$%^&*()_+`-=[{]}\\|;:'\",<.>/?"






inline int TestBkXDXExample(BSTR* pbstr)
{
    TestObject              data;
    BkXDX::CBKJsonCursor    cursor;

    //////////////////////////////////////////////////////////////////////////
    data.m_test_c_type.m_int_max    = INT_MAX;
    data.m_test_c_type.m_int_min    = INT_MIN;
    data.m_test_c_type.m_uint_max   = UINT_MAX;

    data.m_test_c_type.m_long_max   = LONG_MAX;
    data.m_test_c_type.m_long_min   = LONG_MIN;
    data.m_test_c_type.m_ulong_max  = ULONG_MAX;

    data.m_test_c_type.m_short_max  = SHRT_MAX;
    data.m_test_c_type.m_short_min  = SHRT_MIN;
    data.m_test_c_type.m_ushort_max = USHRT_MAX;

    data.m_test_c_type.m_char_max   = CHAR_MAX;
    data.m_test_c_type.m_char_min   = CHAR_MIN;
    data.m_test_c_type.m_schar_max  = SCHAR_MAX;
    data.m_test_c_type.m_schar_min  = SCHAR_MIN;
    data.m_test_c_type.m_uchar_max  = UCHAR_MAX;


    //////////////////////////////////////////////////////////////////////////
    data.m_test_extra_c_type.m_int64_max    = LLONG_MAX;
    data.m_test_extra_c_type.m_int64_min    = LLONG_MIN;
    data.m_test_extra_c_type.m_uint64_max   = ULLONG_MAX;


    //////////////////////////////////////////////////////////////////////////
    data.m_test_atl_class.m_atl_stringw     = TEST_WIDE_STRING;
    data.m_test_atl_class.m_atl_stringa     = TEST_ANSI_STRING;
    data.m_test_atl_class.m_atl_array.Add(L"123");
    data.m_test_atl_class.m_atl_array.Add(L"456");
    data.m_test_atl_class.m_atl_array_empty.RemoveAll();
    data.m_test_atl_class.m_atl_list.AddTail(L"234");
    data.m_test_atl_class.m_atl_list.AddTail(L"567");
    data.m_test_atl_class.m_atl_list_empty.RemoveAll();
    data.m_test_atl_class.m_atl_map[L"1"] = L"2";
    data.m_test_atl_class.m_atl_map[L"3"] = L"4";
    data.m_test_atl_class.m_atl_map_empty.RemoveAll();
    data.m_test_atl_class.m_atl_rbmap.SetAt(L"5", L"6");
    data.m_test_atl_class.m_atl_rbmap.SetAt(L"7", L"8");
    data.m_test_atl_class.m_atl_rbmap_empty.RemoveAll();
    data.m_test_atl_class.m_atl_rbmultimap.Insert(L"9", L"10");
    data.m_test_atl_class.m_atl_rbmultimap.Insert(L"9", L"11");
    data.m_test_atl_class.m_atl_rbmultimap_empty.RemoveAll();

    data.m_test_atl_class.m_atl_map_as_dictionary[L"1"] = L"2";
    data.m_test_atl_class.m_atl_map_as_dictionary[L"3"] = L"4";
    data.m_test_atl_class.m_atl_map_as_dictionary_empty.RemoveAll();
    data.m_test_atl_class.m_atl_rbmap_as_dictionary.SetAt(L"5", L"6");
    data.m_test_atl_class.m_atl_rbmap_as_dictionary.SetAt(L"7", L"8");
    data.m_test_atl_class.m_atl_rbmap_as_dictionary_empty.RemoveAll();



    //////////////////////////////////////////////////////////////////////////
#ifndef _ATL_MIN_CRT
    data.m_test_stl_class.m_stl_stringw     = TEST_WIDE_STRING;
    data.m_test_stl_class.m_stl_stringa     = TEST_ANSI_STRING;
    data.m_test_stl_class.m_stl_pair.first  = L"first";
    data.m_test_stl_class.m_stl_pair.second = L"second";
    data.m_test_stl_class.m_stl_vector.push_back(L"123");
    data.m_test_stl_class.m_stl_vector.push_back(L"456");
    data.m_test_stl_class.m_stl_vector_empty.clear();
    data.m_test_stl_class.m_stl_list.push_back(L"123");
    data.m_test_stl_class.m_stl_list.push_back(L"456");
    data.m_test_stl_class.m_stl_list_empty.clear();
    data.m_test_stl_class.m_stl_deque.push_back(L"123");
    data.m_test_stl_class.m_stl_deque.push_back(L"456");
    data.m_test_stl_class.m_stl_deque_empty.clear();
    data.m_test_stl_class.m_stl_set.insert(L"123");
    data.m_test_stl_class.m_stl_set.insert(L"456");
    data.m_test_stl_class.m_stl_set_empty.clear();
    data.m_test_stl_class.m_stl_map.insert(std::make_pair(std::wstring(L"1"), std::wstring(L"2")));
    data.m_test_stl_class.m_stl_map.insert(std::make_pair(std::wstring(L"3"), std::wstring(L"4")));
    data.m_test_stl_class.m_stl_map_empty.clear();
    data.m_test_stl_class.m_stl_multiset.insert(L"123");
    data.m_test_stl_class.m_stl_multiset.insert(L"456");
    data.m_test_stl_class.m_stl_multiset_empty.clear();
    data.m_test_stl_class.m_stl_multimap.insert(std::make_pair(std::wstring(L"1"), std::wstring(L"2")));
    data.m_test_stl_class.m_stl_multimap.insert(std::make_pair(std::wstring(L"3"), std::wstring(L"4")));
    data.m_test_stl_class.m_stl_multimap_empty.clear();

    data.m_test_stl_class.m_stl_map_as_dictionary.insert(std::make_pair(std::wstring(L"1"), std::wstring(L"2")));
    data.m_test_stl_class.m_stl_map_as_dictionary.insert(std::make_pair(std::wstring(L"3"), std::wstring(L"4")));
    data.m_test_stl_class.m_stl_map_as_dictionary_empty.clear();
#endif//_ATL_MIN_CRT





    cursor.WriteData(data);

    if (pbstr) *pbstr = ::SysAllocString(cursor.m_doc.Dump(TRUE));
    //wprintf(L"%s", cursor.m_doc.Dump(TRUE));
    data.Reset();

    cursor.ReadData(data);






    //////////////////////////////////////////////////////////////////////////
    assert(data.m_test_c_type.m_int_max    == INT_MAX);
    assert(data.m_test_c_type.m_int_min    == INT_MIN);
    assert(data.m_test_c_type.m_uint_max   == UINT_MAX);

    assert(data.m_test_c_type.m_long_max   == LONG_MAX);
    assert(data.m_test_c_type.m_long_min   == LONG_MIN);
    assert(data.m_test_c_type.m_ulong_max  == ULONG_MAX);

    assert(data.m_test_c_type.m_short_max  == SHRT_MAX);
    assert(data.m_test_c_type.m_short_min  == SHRT_MIN);
    assert(data.m_test_c_type.m_ushort_max == USHRT_MAX);

    assert(data.m_test_c_type.m_char_max   == CHAR_MAX);
    assert(data.m_test_c_type.m_char_min   == CHAR_MIN);
    assert(data.m_test_c_type.m_schar_max  == SCHAR_MAX);
    assert(data.m_test_c_type.m_schar_min  == SCHAR_MIN);
    assert(data.m_test_c_type.m_uchar_max  == UCHAR_MAX);


    //////////////////////////////////////////////////////////////////////////
    assert(data.m_test_extra_c_type.m_int64_max    == LLONG_MAX);
    assert(data.m_test_extra_c_type.m_int64_min    == LLONG_MIN);
    assert(data.m_test_extra_c_type.m_uint64_max   == ULLONG_MAX);


    //////////////////////////////////////////////////////////////////////////
    assert(0 == data.m_test_atl_class.m_atl_stringw.Compare(TEST_WIDE_STRING));
    assert(0 == data.m_test_atl_class.m_atl_stringa.Compare(TEST_ANSI_STRING));
    assert(2 == data.m_test_atl_class.m_atl_array.GetCount());
    assert(0 == data.m_test_atl_class.m_atl_array_empty.GetCount());
    assert(2 == data.m_test_atl_class.m_atl_list.GetCount());
    assert(0 == data.m_test_atl_class.m_atl_list_empty.GetCount());
    assert(2 == data.m_test_atl_class.m_atl_map.GetCount());
    assert(0 == data.m_test_atl_class.m_atl_map[L"1"].Compare(L"2"));
    assert(0 == data.m_test_atl_class.m_atl_map[L"3"].Compare(L"4"));
    assert(0 == data.m_test_atl_class.m_atl_map_empty.GetCount());
    assert(2 == data.m_test_atl_class.m_atl_rbmap.GetCount());
    assert(0 == data.m_test_atl_class.m_atl_rbmap_empty.GetCount());
    assert(2 == data.m_test_atl_class.m_atl_rbmultimap.GetCount());
    assert(0 == data.m_test_atl_class.m_atl_rbmultimap_empty.GetCount());

    assert(2 == data.m_test_atl_class.m_atl_map_as_dictionary.GetCount());
    assert(0 == data.m_test_atl_class.m_atl_map_as_dictionary[L"1"].Compare(L"2"));
    assert(0 == data.m_test_atl_class.m_atl_map_as_dictionary[L"3"].Compare(L"4"));
    assert(0 == data.m_test_atl_class.m_atl_map_as_dictionary_empty.GetCount());
    assert(2 == data.m_test_atl_class.m_atl_rbmap_as_dictionary.GetCount());
    assert(0 == data.m_test_atl_class.m_atl_rbmap_as_dictionary.Lookup(L"5")->m_value.Compare(L"6"));
    assert(0 == data.m_test_atl_class.m_atl_rbmap_as_dictionary.Lookup(L"7")->m_value.Compare(L"8"));
    assert(0 == data.m_test_atl_class.m_atl_rbmap_as_dictionary_empty.GetCount());


    //////////////////////////////////////////////////////////////////////////
#ifndef _ATL_MIN_CRT
    assert(0 == data.m_test_stl_class.m_stl_stringw.compare(TEST_WIDE_STRING));
    assert(0 == data.m_test_stl_class.m_stl_stringa.compare(TEST_ANSI_STRING));
    assert(0 == data.m_test_stl_class.m_stl_pair.first.compare(L"first"));
    assert(0 == data.m_test_stl_class.m_stl_pair.second.compare(L"second"));
    assert(2 == data.m_test_stl_class.m_stl_vector.size());
    assert(0 == data.m_test_stl_class.m_stl_vector_empty.size());
    assert(2 == data.m_test_stl_class.m_stl_list.size());
    assert(0 == data.m_test_stl_class.m_stl_list_empty.size());
    assert(2 == data.m_test_stl_class.m_stl_deque.size());
    assert(0 == data.m_test_stl_class.m_stl_deque_empty.size());
    assert(2 == data.m_test_stl_class.m_stl_set.size());
    assert(0 == data.m_test_stl_class.m_stl_set_empty.size());
    assert(2 == data.m_test_stl_class.m_stl_map.size());
    assert(0 == data.m_test_stl_class.m_stl_map[L"1"].compare(L"2"));
    assert(0 == data.m_test_stl_class.m_stl_map[L"3"].compare(L"4"));
    assert(0 == data.m_test_stl_class.m_stl_map_empty.size());
    assert(2 == data.m_test_stl_class.m_stl_multiset.size());
    assert(0 == data.m_test_stl_class.m_stl_multiset_empty.size());
    assert(2 == data.m_test_stl_class.m_stl_multimap.size());
    assert(0 == data.m_test_stl_class.m_stl_multimap_empty.size());

    assert(2 == data.m_test_stl_class.m_stl_map_as_dictionary.size());
    assert(0 == data.m_test_stl_class.m_stl_map_as_dictionary[L"1"].compare(L"2"));
    assert(0 == data.m_test_stl_class.m_stl_map_as_dictionary[L"3"].compare(L"4"));
    assert(0 == data.m_test_stl_class.m_stl_map_as_dictionary_empty.size());
#endif//_ATL_MIN_CRT



	return 0;
}

#endif//BKXDX_EXAMPLE_H
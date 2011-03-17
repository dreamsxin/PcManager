/**
* @file    bkxdx_exchange_stl.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-28 17:40
*/

#ifndef BKXDX_EXCHANGE_STL_H
#define BKXDX_EXCHANGE_STL_H

#ifndef _ATL_MIN_CRT

#include <utility>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include "bkxdx_base.h"
#include "bkxdx_error.h"
#include "bkxdx_macro.h"

NS_BKXDX_BEGIN

//////////////////////////////////////////////////////////////////////////
// std::string<char, ...>
template<class T_Cursor, class T_Traits, class T_Alloc>
inline void ObjectExchange(T_Cursor& cursor, std::basic_string<char, T_Traits, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.SetLexValue((LPCWSTR)CA2W(value.c_str(), cursor.GetCodePage()));
    }
    else
    {
        value = CW2A(cursor.GetLexValue(), cursor.GetCodePage());
    }
}

//////////////////////////////////////////////////////////////////////////
// std::string<wchar_t, ...>
template<class T_Cursor, class T_Traits, class T_Alloc>
inline void ObjectExchange(T_Cursor& cursor, std::basic_string<wchar_t, T_Traits, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.SetLexValue((LPCWSTR)value.c_str());
    }
    else
    {
        value = cursor.GetLexValue();
    }
}






//////////////////////////////////////////////////////////////////////////
// std::pair<...>
template<class T_Cursor, class T_Key, class T_Value>
inline void ObjectExchange(T_Cursor& cursor, std::pair<T_Key, T_Value>& value)
{
    if (cursor.IsWriteMode())
    {
        // transform {key => value} as [key, value]
        cursor.BeginArray();

        // key
        cursor.AppendArrayElem();
        cursor.AccessObject(value.first);

        // array
        cursor.AppendArrayElem();
        cursor.AccessObject(value.second);

        cursor.EndArray();
    }
    else
    {
        cursor.BeginArray();

        // key
        if (cursor.IsEndOfArrayElem())
        {
            cursor.RaiseError(ERROR_BKXDX_DATA_STRUCTURE_MISMATCH);
            return;
        }
        cursor.AccessObject(value.first);

        // value
        if (!cursor.FindNextArrayElem())
        {
            cursor.RaiseError(ERROR_BKXDX_DATA_STRUCTURE_MISMATCH);
            return;
        }
        cursor.AccessObject(value.second);

        cursor.EndArray();
    }
}

template<class T_Cursor, class T_Key, class T_Value>
inline void ObjectExchange_Dictionary(T_Cursor& cursor, std::pair<T_Key, T_Value>& value)
{
    if (cursor.IsWriteMode())
    {
        T_Cursor::LexCast lexcast;
        lexcast.InitAsLexCast(cursor);
        lexcast.SetValue(value.first);

        cursor.BeginChild(lexcast.GetLexValue());
        cursor.AccessObject(value.second);
        cursor.EndChild();
    }
    else
    {
        T_Cursor::LexCast lexcast;
        lexcast.InitAsLexCast(cursor);
        lexcast.SetLexValue(cursor.GetName());
        lexcast.GetValue(value.first);

        cursor.BeginChild(cursor.GetName());
        cursor.AccessObject(value.second);
        cursor.EndChild();
    }
}


template<class T_Cursor, class T_Key, class T_Value>
inline void ObjectExchange(T_Cursor& cursor, std::pair<const T_Key, T_Value>& value)
{
    ObjectExchange(cursor, reinterpret_cast<std::pair<T_Key, T_Value>&>(value));
}

template<class T_Cursor, class T_Key, class T_Value>
inline void ObjectExchange_Dictionary(T_Cursor& cursor, std::pair<const T_Key, T_Value>& value)
{
    ObjectExchange_Dictionary(cursor, reinterpret_cast<std::pair<T_Key, T_Value>&>(value));
}






//////////////////////////////////////////////////////////////////////////
// std::vector<...>
template<class T_Cursor, class T_Elem, class T_Alloc>
inline void ObjectExchange(T_Cursor& cursor, std::vector<T_Elem, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        for (size_t i = 0; i < value.size(); ++i)
        {
            cursor.AppendArrayElem();
            cursor.AccessObject(value[i]);
        }
        cursor.EndArray();
    }
    else
    {
        value.clear();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            value.push_back(T_Elem());
            cursor.AccessObject(value.back());
        }
        cursor.EndArray();
    }
}




//////////////////////////////////////////////////////////////////////////
// std::list<...>
template<class T_Cursor, class T_Elem, class T_Alloc>
inline void ObjectExchange(T_Cursor& cursor, std::list<T_Elem, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        std::list<T_Elem, T_Alloc>::iterator it = value.begin();
        for (NULL; it != value.end(); ++it)
        {
            cursor.AppendArrayElem();
            cursor.AccessObject(*it);
        }
        cursor.EndArray();
    }
    else
    {
        value.clear();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            value.push_back(T_Elem());
            cursor.AccessObject(value.back());
        }
        cursor.EndArray();
    }
}




//////////////////////////////////////////////////////////////////////////
// std::deque<...>
template<class T_Cursor, class T_Elem, class T_Alloc>
inline void ObjectExchange(T_Cursor& cursor, std::deque<T_Elem, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        std::deque<T_Elem, T_Alloc>::iterator it = value.begin();
        for (NULL; it != value.end(); ++it)
        {
            cursor.AppendArrayElem();
            cursor.AccessObject(*it);
        }
        cursor.EndArray();
    }
    else
    {
        value.clear();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            value.push_back(T_Elem());
            cursor.AccessObject(value.back());
        }
        cursor.EndArray();
    }
}





//////////////////////////////////////////////////////////////////////////
// std::set<...>
template<class T_Cursor, class T_Key, class T_Pri, class T_Alloc>
inline void ObjectExchange(T_Cursor& cursor, std::set<T_Key, T_Pri, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        std::set<T_Key, T_Pri, T_Alloc>::iterator it = value.begin();
        for (NULL; it != value.end(); ++it)
        {
            cursor.AppendArrayElem();
            cursor.AccessObject(*it);
        }
        cursor.EndArray();
    }
    else
    {
        value.clear();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            T_Key elem_key;
            cursor.AccessObject(elem_key);
            value.insert(elem_key);
        }
        cursor.EndArray();
    }
}




//////////////////////////////////////////////////////////////////////////
// std::multiset<...>
template<class T_Cursor, class T_Key, class T_Pri, class T_Alloc>
inline void ObjectExchange(T_Cursor& cursor, std::multiset<T_Key, T_Pri, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        std::multiset<T_Key, T_Pri, T_Alloc>::iterator it = value.begin();
        for (NULL; it != value.end(); ++it)
        {
            cursor.AppendArrayElem();
            cursor.AccessObject(*it);
        }
        cursor.EndArray();
    }
    else
    {
        value.clear();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            T_Key elem_key;
            cursor.AccessObject(elem_key);
            value.insert(elem_key);
        }
        cursor.EndArray();
    }
}





//////////////////////////////////////////////////////////////////////////
// std::map<...>
template<class T_Cursor, class T_Key, class T_Value, class T_Pri, class T_Alloc>
inline void ObjectExchange(T_Cursor& cursor, std::map<T_Key, T_Value, T_Pri, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        std::map<T_Key, T_Value, T_Pri, T_Alloc>::iterator it = value.begin();
        for (NULL; it != value.end(); ++it)
        {
            cursor.AppendArrayElem();
            cursor.AccessObject(*it);
        }
        cursor.EndArray();
    }
    else
    {
        value.clear();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            std::pair<T_Key, T_Value> elem_pair;
            cursor.AccessObject(elem_pair);
            value.insert(elem_pair);
        }
        cursor.EndArray();
    }
}

template<class T_Cursor, class T_Key, class T_Value, class T_Pri, class T_Alloc>
inline void ObjectExchange_Dictionary(T_Cursor& cursor, std::map<T_Key, T_Value, T_Pri, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        std::map<T_Key, T_Value, T_Pri, T_Alloc>::iterator it = value.begin();
        for (NULL; it != value.end(); ++it)
        {
            cursor.AppendArrayElem();
            CDictionaryHelper::AccessObject(cursor, *it);
        }
        cursor.EndArray();
    }
    else
    {
        value.clear();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            std::pair<T_Key, T_Value> elem_pair;
            CDictionaryHelper::AccessObject(cursor, elem_pair);
            value.insert(elem_pair);
        }
        cursor.EndArray();
    }
}






//////////////////////////////////////////////////////////////////////////
// std::multimap<...>
template<class T_Cursor, class T_Key, class T_Value, class T_Pri, class T_Alloc>
inline void ObjectExchange(T_Cursor& cursor, std::multimap<T_Key, T_Value, T_Pri, T_Alloc>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        std::multimap<T_Key, T_Value, T_Pri, T_Alloc>::iterator it = value.begin();
        for (NULL; it != value.end(); ++it)
        {
            cursor.AppendArrayElem();
            cursor.AccessObject(*it);
        }
        cursor.EndArray();
    }
    else
    {
        value.clear();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            std::pair<T_Key, T_Value> elem_pair;
            cursor.AccessObject(elem_pair);
            value.insert(elem_pair);
        }
        cursor.EndArray();
    }
}






NS_BKXDX_END

#endif//_ATL_MIN_CRT

#endif//BKXDX_EXCHANGE_STL_H
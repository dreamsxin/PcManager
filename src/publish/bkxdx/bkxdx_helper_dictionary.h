/**
* @file    bkxdx_helper_dictionary.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-28 11:13
*/

#ifndef BKXDX_HELPER_DICTIONARY_H
#define BKXDX_HELPER_DICTIONARY_H

#include "bkxdx_base.h"

NS_BKXDX_BEGIN

template <class T_ValueClass>
class CDictionaryAccessor
{
public:
    CDictionaryAccessor(T_ValueClass& value_object)
        : m_value(&value_object)
    {
    }

    T_ValueClass* m_value;
};


class CDictionaryHelper
{
public:
    template <class T_Cursor, class T_ValueClass>
    static void AccessObject(T_Cursor& cursor, T_ValueClass& value)
    {
        CDictionaryAccessor<T_ValueClass> accessor(value);
        cursor.AccessObject(accessor);
    }
};


// 将 ObjectExchange约定 重定向到 ObjectExchange_Dictionary约定
template<class T_Cursor, class T_ValueClass>
inline void ObjectExchange(T_Cursor& cursor, CDictionaryAccessor<T_ValueClass>& value)
{
    ObjectExchange_Dictionary(cursor, *(value.m_value));
}


NS_BKXDX_END

#endif//BKXDX_HELPER_DICTIONARY_H
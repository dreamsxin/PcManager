/**
* @file    bkxdx_exchange_atl.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-20 18:01
*/

#ifndef BKXDX_EXCHANGE_ATL_H
#define BKXDX_EXCHANGE_ATL_H

#include <atlstr.h>
#include <atlcoll.h>
#include "bkxdx_base.h"
#include "bkxdx_error.h"
#include "bkxdx_macro.h"

NS_BKXDX_BEGIN

//////////////////////////////////////////////////////////////////////////
// ATL::CStringT<char, ...>
template<class T_Cursor, class T_Traits>
inline void ObjectExchange(T_Cursor& cursor, ATL::CStringT<char, T_Traits>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.SetLexValue((LPCWSTR)CA2W(value, cursor.GetCodePage()));
    }
    else
    {
        value = CW2A(cursor.GetLexValue(), cursor.GetCodePage());
    }
}

//////////////////////////////////////////////////////////////////////////
// ATL::CStringT<wchar_t, ...>
template<class T_Cursor, class T_Traits>
inline void ObjectExchange(T_Cursor& cursor, ATL::CStringT<wchar_t, T_Traits>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.SetLexValue((LPCWSTR)value);
    }
    else
    {
        value = cursor.GetLexValue();
    }
}





//////////////////////////////////////////////////////////////////////////
// ATL::CAtlArray<...>
template<class T_Cursor, class T_Elem, class T_Traits>
inline void ObjectExchange(T_Cursor& cursor, ATL::CAtlArray<T_Elem, T_Traits>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        for (size_t i = 0; i < value.GetCount(); ++i)
        {
            cursor.AppendArrayElem();
            cursor.AccessObject(value[i]);
        }
        cursor.EndArray();
    }
    else
    {
        value.RemoveAll();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            value.Add();
            cursor.AccessObject(value[value.GetCount() - 1]);
        }
        cursor.EndArray();
    }
}






//////////////////////////////////////////////////////////////////////////
// ATL::CAtlList<...>
template<class T_Cursor, class T_Elem, class T_Traits>
inline void ObjectExchange(T_Cursor& cursor, ATL::CAtlList<T_Elem, T_Traits>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        POSITION pos = value.GetHeadPosition();
        for (NULL; pos; value.GetNext(pos))
        {
            cursor.AppendArrayElem();
            cursor.AccessObject(value.GetAt(pos));
        }
        cursor.EndArray();
    }
    else
    {
        value.RemoveAll();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            POSITION pos = value.AddTail();
            cursor.AccessObject(value.GetAt(pos));
        }
        cursor.EndArray();
    }
}





//////////////////////////////////////////////////////////////////////////
// ATL::CAtlMap<...>
template<class T_Cursor, class T_Key, class T_Value, class T_KeyTraits, class T_ValueTraits>
inline void ObjectExchange(T_Cursor& cursor, ATL::CAtlMap<T_Key, T_Value, T_KeyTraits, T_ValueTraits>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        POSITION pos = value.GetStartPosition();
        for (NULL; pos; value.GetNext(pos))
        {
            cursor.AppendArrayElem();

            // transform {key => value} as [key, value]
            cursor.BeginArray();

            // key
            cursor.AppendArrayElem();
            cursor.AccessObject((T_Key&)value.GetKeyAt(pos));

            // array
            cursor.AppendArrayElem();
            cursor.AccessObject(value.GetValueAt(pos));

            cursor.EndArray();
        }
        cursor.EndArray();
    }
    else
    {
        value.RemoveAll();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            T_Key   elem_key;
            T_Value elem_value;

            cursor.BeginArray();

            // key
            if (cursor.IsEndOfArrayElem())
            {
                cursor.RaiseError(ERROR_BKXDX_DATA_STRUCTURE_MISMATCH);
                return;
            }
            cursor.AccessObject(elem_key);

            // value
            if (!cursor.FindNextArrayElem())
            {
                cursor.RaiseError(ERROR_BKXDX_DATA_STRUCTURE_MISMATCH);
                return;
            }
            cursor.AccessObject(elem_value);

            cursor.EndArray();

            value.SetAt(elem_key, elem_value);
        }
        cursor.EndArray();
    }
}










//////////////////////////////////////////////////////////////////////////
// ATL::CAtlRBMap<...>
template<class T_Cursor, class T_Key, class T_Value, class T_KeyTraits, class T_ValueTraits>
inline void ObjectExchange(T_Cursor& cursor, ATL::CRBMap<T_Key, T_Value, T_KeyTraits, T_ValueTraits>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        POSITION pos = value.GetHeadPosition();
        for (NULL; pos; value.GetNext(pos))
        {
            cursor.AppendArrayElem();

            // transform {key => value} as [key, value]
            cursor.BeginArray();

            // key
            cursor.AppendArrayElem();
            cursor.AccessObject((T_Key&)value.GetKeyAt(pos));

            // array
            cursor.AppendArrayElem();
            cursor.AccessObject(value.GetValueAt(pos));

            cursor.EndArray();
        }
        cursor.EndArray();
    }
    else
    {
        value.RemoveAll();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            T_Key   elem_key;
            T_Value elem_value;

            cursor.BeginArray();

            // key
            if (cursor.IsEndOfArrayElem())
            {
                cursor.RaiseError(ERROR_BKXDX_DATA_STRUCTURE_MISMATCH);
                return;
            }
            cursor.AccessObject(elem_key);

            // value
            if (!cursor.FindNextArrayElem())
            {
                cursor.RaiseError(ERROR_BKXDX_DATA_STRUCTURE_MISMATCH);
                return;
            }
            cursor.AccessObject(elem_value);

            cursor.EndArray();

            value.SetAt(elem_key, elem_value);
        }
        cursor.EndArray();
    }
}







//////////////////////////////////////////////////////////////////////////
// ATL::CAtlRBMultiMap<...>
template<class T_Cursor, class T_Key, class T_Value, class T_KeyTraits, class T_ValueTraits>
inline void ObjectExchange(T_Cursor& cursor, ATL::CRBMultiMap<T_Key, T_Value, T_KeyTraits, T_ValueTraits>& value)
{
    if (cursor.IsWriteMode())
    {
        cursor.BeginArray();
        POSITION pos = value.GetHeadPosition();
        for (NULL; pos; value.GetNext(pos))
        {
            cursor.AppendArrayElem();

            // transform {key => value} as [key, value]
            cursor.BeginArray();

            // key
            cursor.AppendArrayElem();
            cursor.AccessObject((T_Key&)value.GetKeyAt(pos));

            // array
            cursor.AppendArrayElem();
            cursor.AccessObject(value.GetValueAt(pos));

            cursor.EndArray();
        }
        cursor.EndArray();
    }
    else
    {
        value.RemoveAll();

        cursor.BeginArray();
        for (NULL; !cursor.IsEndOfArrayElem(); cursor.FindNextArrayElem())
        {
            T_Key   elem_key;
            T_Value elem_value;

            cursor.BeginArray();

            // key
            if (cursor.IsEndOfArrayElem())
            {
                cursor.RaiseError(ERROR_BKXDX_DATA_STRUCTURE_MISMATCH);
                return;
            }
            cursor.AccessObject(elem_key);

            // value
            if (!cursor.FindNextArrayElem())
            {
                cursor.RaiseError(ERROR_BKXDX_DATA_STRUCTURE_MISMATCH);
                return;
            }
            cursor.AccessObject(elem_value);

            cursor.EndArray();

            value.Insert(elem_key, elem_value);
        }
        cursor.EndArray();
    }
}









//////////////////////////////////////////////////////////////////////////
// ObjectExchange_Dictionary


//////////////////////////////////////////////////////////////////////////
// ATL::CAtlMap<...>
template<class T_Cursor, class T_Key, class T_Value, class T_KeyTraits, class T_ValueTraits>
inline void ObjectExchange_Dictionary(T_Cursor& cursor, ATL::CAtlMap<T_Key, T_Value, T_KeyTraits, T_ValueTraits>& value)
{
    if (cursor.IsWriteMode())
    {
        POSITION pos = value.GetStartPosition();
        for (NULL; pos; value.GetNext(pos))
        {
            T_Key elem_key = value.GetKeyAt(pos);

            T_Cursor::LexCast lexcast;
            lexcast.InitAsLexCast(cursor);
            lexcast.SetValue(elem_key);

            cursor.BeginChild(lexcast.GetLexValue());
            cursor.AccessObject(value.GetValueAt(pos));
            cursor.EndChild();

        }
    }
    else
    {
        value.RemoveAll();

        cursor.BeginChildren();
        for (NULL; !cursor.IsEndOfChildren(); cursor.FindNextChild())
        {
            T_Key   elem_key;
            T_Value elem_value;

            T_Cursor::LexCast lexcast;
            lexcast.InitAsLexCast(cursor);
            lexcast.SetLexValue(cursor.GetName());
            lexcast.GetValue(elem_key);

            cursor.AccessObject(elem_value);

            value.SetAt(elem_key, elem_value);
        }
        cursor.EndChildren();
    }
}



//////////////////////////////////////////////////////////////////////////
// ATL::CRBMap<...>
template<class T_Cursor, class T_Key, class T_Value, class T_KeyTraits, class T_ValueTraits>
inline void ObjectExchange_Dictionary(T_Cursor& cursor, ATL::CRBMap<T_Key, T_Value, T_KeyTraits, T_ValueTraits>& value)
{
    if (cursor.IsWriteMode())
    {
        POSITION pos = value.GetHeadPosition();
        for (NULL; pos; value.GetNext(pos))
        {
            T_Key elem_key = value.GetKeyAt(pos);

            T_Cursor::LexCast lexcast;
            lexcast.InitAsLexCast(cursor);
            lexcast.SetValue(elem_key);

            cursor.BeginChild(lexcast.GetLexValue());
            cursor.AccessObject(value.GetValueAt(pos));
            cursor.EndChild();

        }
    }
    else
    {
        value.RemoveAll();

        cursor.BeginChildren();
        for (NULL; !cursor.IsEndOfChildren(); cursor.FindNextChild())
        {
            T_Key   elem_key;
            T_Value elem_value;

            T_Cursor::LexCast lexcast;
            lexcast.InitAsLexCast(cursor);
            lexcast.SetLexValue(cursor.GetName());
            lexcast.GetValue(elem_key);

            cursor.AccessObject(elem_value);

            value.SetAt(elem_key, elem_value);
        }
        cursor.EndChildren();
    }
}









NS_BKXDX_END


#endif//BKXDX_EXCHANGE_ATL_H
/**
* @file    bkxdx_lexvalue.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-20 00:22
*/

#ifndef BKXDX_LEXVALUE_H
#define BKXDX_LEXVALUE_H

#include <wchar.h>
#include <atlstr.h>
#include "bkxdx_base.h"
#include "bkxdx_macro.h"

NS_BKXDX_BEGIN

class CLexValue
{
public:
    void    SetLexValue(LPCWSTR lpszValue);
    LPCWSTR GetLexValue();



    template <class T_Value>
    void SetValue(const T_Value& value) {NotSupportedByCLexValue(value);}
    template <class T_Value>
    void GetValue(T_Value& value) {NotSupportedByCLexValue(value);}



    void SetValue(const signed int& value);
    void GetValue(signed int& value);

    void SetValue(const unsigned int& value);
    void GetValue(unsigned int& value);
    
    void SetValue(const signed long& value);
    void GetValue(signed long& value);
    
    void SetValue(const unsigned long& value);
    void GetValue(unsigned long& value);

    void SetValue(const signed short& value);
    void GetValue(signed short& value);

    void SetValue(const unsigned short& value);
    void GetValue(unsigned short& value);




    void SetValue(const signed __int64& value);
    void GetValue(signed __int64& value);

    void SetValue(const unsigned __int64& value);
    void GetValue(unsigned __int64& value);




    void SetValue(const char& value);
    void GetValue(char& value);

    void SetValue(const signed char& value);
    void GetValue(signed char& value);

    void SetValue(const unsigned char& value);
    void GetValue(unsigned char& value);

protected:
    CString m_strValue;
};


inline void CLexValue::SetLexValue(LPCWSTR lpszValue)
{
    m_strValue = lpszValue;
}

inline LPCWSTR CLexValue::GetLexValue()
{
    return m_strValue;
}



//////////////////////////////////////////////////////////////////////////
// signed int
inline void CLexValue::SetValue(const signed int& value)
{
    m_strValue.Format(L"%d", value);
}

inline void CLexValue::GetValue(signed int& value)
{
    value = ::StrToInt(m_strValue);
}



//////////////////////////////////////////////////////////////////////////
// unsigned int
inline void CLexValue::SetValue(const unsigned int& value)
{
    m_strValue.Format(L"%u", value);
}

inline void CLexValue::GetValue(unsigned int& value)
{
    value = ::StrToInt(m_strValue);
}



//////////////////////////////////////////////////////////////////////////
// signed long
inline void CLexValue::SetValue(const signed long& value)
{
    m_strValue.Format(L"%ld", value);
}

inline void CLexValue::GetValue(signed long& value)
{
    value = ::StrToInt(m_strValue);
}




//////////////////////////////////////////////////////////////////////////
// unsigned long
inline void CLexValue::SetValue(const unsigned long& value)
{
    m_strValue.Format(L"%lu", value);
}

inline void CLexValue::GetValue(unsigned long& value)
{
    value = ::StrToInt(m_strValue);
}




//////////////////////////////////////////////////////////////////////////
// signed short
inline void CLexValue::SetValue(const signed short& value)
{
    m_strValue.Format(L"%hd", value);
}

inline void CLexValue::GetValue(signed short& value)
{
    value = ::StrToInt(m_strValue);
}




//////////////////////////////////////////////////////////////////////////
// unsigned short
inline void CLexValue::SetValue(const unsigned short& value)
{
    m_strValue.Format(L"%hu", value);
}

inline void CLexValue::GetValue(unsigned short& value)
{
    value = ::StrToInt(m_strValue);
}







//////////////////////////////////////////////////////////////////////////
// signed __int64
inline void CLexValue::SetValue(const signed __int64& value)
{
    m_strValue.Format(L"%I64d", value);
}

inline void CLexValue::GetValue(signed __int64& value)
{
#ifdef _ATL_MIN_CRT
    if (!::StrToInt64Ex(m_strValue, STIF_DEFAULT, &value))
        value = 0;
#else
    value = _wcstoi64(m_strValue, NULL, 10);
#endif
}




//////////////////////////////////////////////////////////////////////////
// unsigned __int64
inline void CLexValue::SetValue(const unsigned __int64& value)
{
    m_strValue.Format(L"%I64u", value);
}

inline void CLexValue::GetValue(unsigned __int64& value)
{
#ifdef _ATL_MIN_CRT
    if (!::StrToInt64Ex(m_strValue, STIF_DEFAULT, (LONGLONG*)&value))
        value = 0;
#else
    value = _wcstoui64(m_strValue, NULL, 10);
#endif
}





//////////////////////////////////////////////////////////////////////////
// char
inline void CLexValue::SetValue(const char& value)
{
    m_strValue.Format(L"%hd", value);
}

inline void CLexValue::GetValue(char& value)
{
    value = ::StrToInt(m_strValue);
}






//////////////////////////////////////////////////////////////////////////
// signed char
inline void CLexValue::SetValue(const signed char& value)
{
    m_strValue.Format(L"%hd", value);
}

inline void CLexValue::GetValue(signed char& value)
{
    value = ::StrToInt(m_strValue);
}




//////////////////////////////////////////////////////////////////////////
// unsigned char
inline void CLexValue::SetValue(const unsigned char& value)
{
    m_strValue.Format(L"%hu", value);
}

inline void CLexValue::GetValue(unsigned char& value)
{
    value = ::StrToInt(m_strValue);
}











template <class T_LexClass, class T_Value>
inline void LexSetValue(CLexValue& lex, const T_Value& value)
{
    lex.SetValue(value);
}

template <class T_LexClass, class T_Value>
inline void LexGetValue(CLexValue& lex, T_Value& value)
{
    lex.GetValue(value);
}







NS_BKXDX_END

#endif//BKXDX_LEXVALUE_H
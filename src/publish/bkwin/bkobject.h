//////////////////////////////////////////////////////////////////////////
//   File Name: BkObject.h
// Description: BkObject Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#if !defined(TINYXML_INCLUDED)
    #error Please include tinyxml.h first!
#endif

#include <bkres/bkfontpool.h>
#include <bkres/bkresutil.h>

//////////////////////////////////////////////////////////////////////////

// BkObject Class Name Declaration
#define BKOBJ_DECLARE_CLASS_NAME(theclass, classname)   \
public:                                                 \
    static theclass* CheckAndNew(LPCSTR lpszName)       \
    {                                                   \
        if (strcmp(GetClassName(), lpszName)  == 0)     \
            return new theclass;                        \
        else                                            \
            return NULL;                                \
    }                                                   \
                                                        \
    static LPCSTR GetClassName()                        \
    {                                                   \
        return classname;                               \
    }                                                   \
                                                        \
    virtual LPCSTR GetObjectClass()                     \
    {                                                   \
        return classname;                               \
    }                                                   \
                                                        \
    virtual BOOL IsClass(LPCSTR lpszName)               \
    {                                                   \
        return strcmp(GetClassName(), lpszName)  == 0;  \
    }                                                   \

//////////////////////////////////////////////////////////////////////////
// Xml Attributes Declaration Map

// Attribute Declaration
#define BKWIN_DECLARE_ATTRIBUTES_BEGIN()                            \
public:                                                             \
    virtual HRESULT SetAttribute(                                   \
        CStringA strAttribName,                                     \
        CStringA strValue,                                          \
        BOOL     bLoading)                                          \
    {                                                               \
        HRESULT hRet = __super::SetAttribute(                       \
            strAttribName,                                          \
            strValue,                                               \
            bLoading                                                \
            );                                                      \
        if (SUCCEEDED(hRet))                                        \
            return hRet;                                            \

#define BKWIN_DECLARE_ATTRIBUTES_END()                              \
            return E_FAIL;                                          \
                                                                    \
        return hRet;                                                \
    }                                                               \

#define BKWIN_CHAIN_ATTRIBUTE(varname, allredraw)                   \
        if (SUCCEEDED(hRet = varname.SetAttribute(strAttribName, strValue, bLoading)))   \
        {                                                           \
            return hRet;                                            \
        }                                                           \
        else                                                        \

// Custom Attribute
// HRESULT OnAttributeChange(CStringA& strValue, BOOL bLoading)
#define BKWIN_CUSTOM_ATTRIBUTE(attribname, func)                    \
        if (attribname == strAttribName)                            \
        {                                                           \
            hRet = func(strValue, bLoading);                        \
        }                                                           \
        else                                                        \

// Int = %d StringA
#define BKWIN_INT_ATTRIBUTE(attribname, varname, allredraw)         \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = ::StrToIntA(strValue);                        \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// UInt = %u StringA
#define BKWIN_UINT_ATTRIBUTE(attribname, varname, allredraw)        \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = (UINT)::StrToIntA(strValue);                  \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// DWORD = %u StringA
#define BKWIN_DWORD_ATTRIBUTE(attribname, varname, allredraw)       \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = (DWORD)::StrToIntA(strValue);                 \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// StringA = StringA
#define BKWIN_STRING_ATTRIBUTE(attribname, varname, allredraw)      \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = strValue;                                     \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// StringT = StringA
#define BKWIN_TSTRING_ATTRIBUTE(attribname, varname, allredraw)     \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = CA2T(strValue, CP_UTF8);                      \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// DWORD = %X StringA
#define BKWIN_HEX_ATTRIBUTE(attribname, varname, allredraw)         \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = CBkObject::HexStringToULong(strValue);        \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// COLORREF = %08X StringA
#define BKWIN_COLOR_ATTRIBUTE(attribname, varname, allredraw)       \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = CBkObject::HexStringToColor(strValue);        \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// HFONT = %04X StringA
#define BKWIN_FONT_ATTRIBUTE(attribname, varname, allredraw)        \
        if (attribname == strAttribName)                            \
        {                                                           \
            DWORD dwValue = CBkObject::HexStringToULong(strValue);  \
            varname = BkFontPool::GetFont(LOWORD(dwValue));         \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// Value In {String1 : Value1, String2 : Value2 ...}
#define BKWIN_ENUM_ATTRIBUTE(attribname, vartype, allredraw)        \
        if (attribname == strAttribName)                            \
        {                                                           \
            vartype varTemp;                                        \
                                                                    \
            hRet = allredraw ? S_OK : S_FALSE;                      \

#define BKWIN_ENUM_VALUE(enumstring, enumvalue)                     \
            if (strValue == enumstring)                             \
                varTemp = enumvalue;                                \
            else                                                    \

#define BKWIN_ENUM_END(varname)                                     \
                return E_FAIL;                                      \
                                                                    \
            varname = varTemp;                                      \
        }                                                           \
        else                                                        \

// BkStyle From StringA Key
#define BKWIN_STYLE_ATTRIBUTE(attribname, varname, allredraw)       \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = BkStyle::GetStyle(strValue);                  \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

// BkSkin From StringA Key
#define BKWIN_SKIN_ATTRIBUTE(attribname, varname, allredraw)        \
        if (attribname == strAttribName)                            \
        {                                                           \
            varname = BkSkin::GetSkin(strValue);                    \
            hRet = allredraw ? S_OK : S_FALSE;                      \
        }                                                           \
        else                                                        \

class CBkObject
{
public:
    CBkObject()
    {
    }

    virtual ~CBkObject()
    {
    }

    virtual BOOL IsClass(LPCSTR lpszName) = 0;
    virtual LPCSTR GetObjectClass() = 0;

    virtual BOOL Load(TiXmlElement* pXmlElem)
    {
        for (TiXmlAttribute *pAttrib = pXmlElem->FirstAttribute(); NULL != pAttrib; pAttrib = pAttrib->Next())
        {
            SetAttribute(pAttrib->Name(), pAttrib->Value(), TRUE);
        }

        return TRUE;
    }

    virtual HRESULT SetAttribute(CStringA strAttribName, CStringA strValue, BOOL bLoading)
    {
        return E_FAIL;
    }

protected:
    static ULONG HexStringToULong(LPCSTR lpszValue, int nSize = -1)
    {
        LPCSTR pchValue = lpszValue;
        ULONG ulValue = 0;

        while (*pchValue && nSize != 0)
        {
            ulValue <<= 4;

            if ('a' <= *pchValue && 'f' >= *pchValue)
                ulValue |= (*pchValue - 'a' + 10);
            else if ('A' <= *pchValue && 'F' >= *pchValue)
                ulValue |= (*pchValue - 'A' + 10);
            else if ('0' <= *pchValue && '9' >= *pchValue)
                ulValue |= (*pchValue - '0');
            else
                return 0;

            ++ pchValue;
            -- nSize;
        }

        return ulValue;
    }

    static COLORREF HexStringToColor(LPCSTR lpszValue)
    {
        return RGB(
            HexStringToULong(lpszValue, 2), 
            HexStringToULong(lpszValue + 2, 2), 
            HexStringToULong(lpszValue + 4, 2)
            );
    }
};
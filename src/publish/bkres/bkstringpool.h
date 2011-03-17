//////////////////////////////////////////////////////////////////////////
//   File Name: bkstringpool.h
// Description: String Pool
//     Creator: Zhang Xiaoxuan
//     Version: 2009.5.13 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlcoll.h>
#include "bkresutil.h"

#if !defined(TINYXML_INCLUDED)
    #error Please include tinyxml.h first!
#endif

class BkString
{
protected:

    typedef CAtlMap<UINT, CString> _TypeStringResPool;

public:
    BkString()
    {
    }
    virtual ~BkString()
    {
        m_mapString.RemoveAll();
    }

    static LPCTSTR Get(UINT uResID)
    {
        const _TypeStringResPool::CPair* pPair = _Instance()->m_mapString.Lookup(uResID);
        if (!pPair)
        {
            BKRES_ASSERT(FALSE, L"Failed loading string %u", uResID);

            return NULL;
        }

        return pPair->m_value;
    }

    static BOOL Load(UINT uResID)
    {
        CStringA strXml;
        BOOL bRet = FALSE;
        
        bRet = BkResManager::LoadResource(uResID, strXml);
        if (!bRet)
            return FALSE;

        TiXmlDocument xmlDoc;

        xmlDoc.Parse(strXml, NULL, TIXML_ENCODING_UTF8);

        if (xmlDoc.Error())
            return FALSE;

        LPCSTR lpszStringID = NULL;
        UINT uStringID = 0;

        TiXmlElement *pXmlStringRootElem = xmlDoc.RootElement();

        if (!pXmlStringRootElem)
            return FALSE;

        if (strcmp(pXmlStringRootElem->Value(), "string") != 0)
            return FALSE;

        for (TiXmlElement* pXmlChild = pXmlStringRootElem->FirstChildElement("s"); NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement("s"))
        {
            lpszStringID = pXmlChild->Attribute("id");
            if (!lpszStringID)
                continue;

            uStringID = (UINT)(ULONG)::StrToIntA(lpszStringID);

            {
                _Instance()->m_mapString[uStringID] = CA2T(pXmlChild->GetText(), CP_UTF8);
            }
        }

        return TRUE;
    }

    static size_t GetCount()
    {
        return _Instance()->m_mapString.GetCount();
    }

protected:

    _TypeStringResPool m_mapString;

    static BkString* ms_pInstance;

    static BkString* _Instance()
    {
        if (!ms_pInstance)
            ms_pInstance = new BkString;
        return ms_pInstance;
    }
};

class CBkStringArray
{
public:
    CBkStringArray(UINT arrStringID[])
    {
        if (m_arrStringID.IsEmpty())
        {
            size_t nCount = sizeof(arrStringID) / sizeof(UINT);
            m_arrStringID.SetCount(nCount);

            for (size_t i = 0; i < nCount; i ++)
                m_arrStringID[i] = arrStringID[i];
        }
    }

    LPCTSTR operator[](size_t nIndex)
    {
        if (nIndex >= m_arrStringID.GetCount())
            return L"";

        return BkString::Get(m_arrStringID[nIndex]);
    }

    CAtlArray<UINT> m_arrStringID;
};

__declspec(selectany) BkString* BkString::ms_pInstance = NULL;
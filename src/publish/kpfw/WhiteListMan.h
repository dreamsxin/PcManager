#pragma once

#include <map>
#include "PublicEncrypt.h"
#include "shlobj.h"
#include "string"

typedef std::map<std::wstring, int> ITEMMAP;

class CWhiteListMan:public CPublicEncrypt
{
public:

    CWhiteListMan(const std::wstring& wstrFilePath):CPublicEncrypt(wstrFilePath)
    {
        m_bNeedWrite = FALSE;
        Decrypt();
    }

    ~CWhiteListMan()
    {
        if (m_bNeedWrite)
        {
            Encrypt();
        }
    }

    BOOL    IsInWhiteList(const std::wstring& wstrUrl)
    {
        if (m_ItemsMap.empty() || wstrUrl.empty())
        {
            return FALSE;
        }

        ITEMMAP::const_iterator iter;

        iter = m_ItemsMap.find(wstrUrl);

        if (iter != m_ItemsMap.end())
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    BOOL    GetAllItems(ITEMMAP& itemMap)
    {
        itemMap.clear();
        itemMap = m_ItemsMap;
        return TRUE;
    }

    BOOL    InsertItem(const std::wstring& wstrItem)
    {
        if (wstrItem.empty())
        {
            return FALSE;
        }

        std::pair<ITEMMAP::iterator, bool> InsertResult;

        InsertResult = m_ItemsMap.insert(ITEMMAP::value_type(wstrItem, wstrItem.length()));

        if (InsertResult.second)
        {
            m_bNeedWrite = TRUE;
        }

        return TRUE;
    }

    BOOL    DeleteItem(const std::wstring& wstrItem)
    {
        if (wstrItem.empty() || m_ItemsMap.empty())
        {
            return FALSE;
        }

        ITEMMAP::iterator iter;

        iter = m_ItemsMap.find(wstrItem);

        if (m_ItemsMap.end() == iter)
        {
            return FALSE;
        }

        m_ItemsMap.erase(iter);

        m_bNeedWrite = TRUE;

        return TRUE;
    }

    BOOL    DeleteAllItems()
    {
        m_ItemsMap.clear();
        m_bNeedWrite = TRUE;
        return TRUE;
    }

    BOOL    ReplaceAllItems(const ITEMMAP& itemMap)
    {
        m_ItemsMap.clear();
        m_ItemsMap = itemMap;
        m_bNeedWrite = TRUE;
        return TRUE;
    }

private:
    BOOL    GetEncryptBufInfo(PBYTE& pbyEncryptBuf, long& lEncryptBufSize)
    {
        int nSize = 0;
        ITEMMAP::const_iterator iter;
        PBYTE pbyBuf = NULL;
        union
        {
            PBYTE   pbyUnionBuf;
            PINT    pnUnionBuf;
        };

        pbyUnionBuf = NULL;

        if (m_ItemsMap.empty())
        {
            return FALSE;
        }

        for (iter = m_ItemsMap.begin(); iter != m_ItemsMap.end(); iter++)
        {
            nSize += sizeof(int);
            nSize += iter->second * sizeof(WCHAR);
        }

        pbyBuf = new BYTE[nSize];

        if (NULL == pbyBuf)
        {
            return FALSE;
        }

        ZeroMemory(pbyBuf, nSize);

        pbyUnionBuf = pbyBuf;

        for (iter = m_ItemsMap.begin(); iter != m_ItemsMap.end(); iter++)
        {
            *pnUnionBuf = iter->second;
            pnUnionBuf++;
            memcpy(pbyUnionBuf, iter->first.c_str(), iter->first.length() * sizeof(WCHAR));
            pbyUnionBuf += iter->first.length() * sizeof(WCHAR);
        }

        pbyEncryptBuf = pbyBuf;
        lEncryptBufSize = nSize;

        return TRUE;
    }

    BOOL    ParseDecryptData(PBYTE pbyDecryptBuf, long lDecryptBufSize)
    {
        int nItemLen = 0;
        std::wstring wstrItemStr;
        
        union
        {
            PBYTE   pbyBuf;
            PINT    pnBuf;
        };

        PBYTE pbyEndBuf;

        if (NULL == pbyDecryptBuf || lDecryptBufSize <= 0)
        {
            return FALSE;
        }

        pbyBuf = pbyDecryptBuf;
        pbyEndBuf = pbyBuf + lDecryptBufSize;

        while(pbyBuf < pbyEndBuf)
        {
            if (!GetItemLen(nItemLen, pnBuf, pbyEndBuf))
            {
                break;
            }

            if (!GetItemStr(nItemLen, wstrItemStr, pbyBuf, pbyEndBuf))
            {
                break;
            }

            m_ItemsMap.insert(ITEMMAP::value_type(wstrItemStr, nItemLen));

        }
        return TRUE;
    }

    BOOL    GetItemLen(int& nLen, PINT& pn, PBYTE const pbyEnd)
    {
        if (NULL == pn || NULL == pbyEnd)
        {
            return FALSE;
        }

        if ((PBYTE)(pn + 1) >= pbyEnd)
        {
            return FALSE;
        }

        nLen = *pn;

        if (nLen <= 0)
        {
            return FALSE;
        }

        pn += 1;

        return TRUE;
    }

    BOOL GetItemStr(int nLen, std::wstring& wstrItemStr, PBYTE& pbyBuf, PBYTE const pbyEnd)
    {
        if (NULL == pbyBuf || NULL == pbyEnd || nLen <= 0)
        {
            return FALSE;
        }

        if (pbyBuf + sizeof(WCHAR) * nLen > pbyEnd)
        {
            return FALSE;
        }

        wstrItemStr.assign((PWCHAR)pbyBuf, nLen);

        pbyBuf += sizeof(WCHAR) * nLen;

        return TRUE;
    }

private:
   ITEMMAP  m_ItemsMap;
   BOOL     m_bNeedWrite;
};
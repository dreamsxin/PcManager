/**
* @file    winregvaluefind.h
* @brief   ...
* @author  bbcallen
* @date    2009-04-23  14:16
*/

#ifndef WINREGVALUEFIND_H
#define WINREGVALUEFIND_H

#include <assert.h>
#include <atlstr.h>
#include <atlbase.h>
#include "winmodbase.h"
#include "winregkey.h"

NS_WINMOD_BEGIN

class CWinRegValueFind
{
public:
    enum {WIN_REG_VALUE_NAME_MAX_BUF_SIZE = 16384};

    CWinRegValueFind();
    virtual ~CWinRegValueFind();

    operator HKEY() const throw();

    inline CWinRegKey& GetRegKey()
    {
        return m_hKeyEnum;
    }


    void    Reset();

    BOOL    FindFirstValueName(HKEY hKeyParent, LPCWSTR lpszKeyName, REGSAM samDesired = KEY_QUERY_VALUE);

    BOOL    FindNextValueName();

    BOOL    IsEndOfFind() const;

    LPCWSTR GetValueName() const;

    HRESULT GetStringValue(CString& strValue, DWORD dwMaxLen = WINMOD_REASONABL_VALUE_LEN);


private:
    // Disabled
    CWinRegValueFind(const CWinRegValueFind&);
    CWinRegValueFind& operator=(const CWinRegValueFind&);

public:
    CWinRegKey  m_hKeyEnum;

protected:

    BOOL        DoEnumRegValue();

    DWORD       m_dwType;
    LPWSTR      m_lpszeValueName;
    CString     m_strLongValueNameBuffer;
    WCHAR       m_szValueNameBuffer[BUFSIZ];
    DWORD       m_dwEnumIndex;
};


inline CWinRegValueFind::CWinRegValueFind()
{
    Reset();
}

inline CWinRegValueFind::~CWinRegValueFind()
{
}

inline void CWinRegValueFind::Reset()
{
    m_hKeyEnum.Close();

    m_dwType = REG_NONE;
    m_lpszeValueName = NULL;
    m_strLongValueNameBuffer.Empty();
    m_szValueNameBuffer[0] = L'\0';
    m_dwEnumIndex = 0;
}

inline CWinRegValueFind::operator HKEY() const throw()
{
    return m_hKeyEnum;
}

inline BOOL CWinRegValueFind::IsEndOfFind() const
{
    return NULL == m_hKeyEnum;
}

inline LPCWSTR CWinRegValueFind::GetValueName() const
{
    assert(!IsEndOfFind());
    return m_lpszeValueName;
}

NS_WINMOD_END

#endif//WINREGVALUEFIND_H

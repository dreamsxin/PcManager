//////////////////////////////////////////////////////////////////////////
//  Class Name: BKFontPool
// Description: Font Pool
//     Creator: ZhangXiaoxuan
//     Version: 2009.4.22 - 1.0 - Change stl::map to CAtlMap
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlcoll.h>

#define BKF_BOLD        0x0004U
#define BKF_UNDERLINE   0x0002U
#define BKF_ITALIC      0x0001U

#define BKF_MAKEKEY(bold, underline, italic, adding) \
    (WORD)((adding << 8) \
    | (bold ? BKF_BOLD : 0) \
    | (underline ? BKF_UNDERLINE : 0) \
    | (italic ? BKF_ITALIC : 0))

#define BKF_ISBOLD(key)         ((key & BKF_BOLD) == BKF_BOLD)
#define BKF_ISUNDERLINE(key)    ((key & BKF_UNDERLINE) == BKF_UNDERLINE)
#define BKF_ISITALIC(key)       ((key & BKF_ITALIC) == BKF_ITALIC)
#define BKF_GETADDING(key)      (key >> 8)

#define BKF_DEFAULTFONT         (BKF_MAKEKEY(FALSE, FALSE, FALSE, 0))
#define BKF_BOLDFONT            (BKF_MAKEKEY(TRUE, FALSE, FALSE, 0))

#define BKF_GetFont(key)        BkFontPool::GetFont(key)

class BkFontPool
{
protected:

    typedef CAtlMap<WORD, HFONT> _TypeFontPool;

public:
    BkFontPool()
        : m_strFaceName(_T("Tahoma"))
        , m_lFontSize(-11)
    {
        m_mapFont[BKF_DEFAULTFONT] = _GetDefaultGUIFont();
    }
    virtual ~BkFontPool()
    {
        HFONT hFont = NULL;

        POSITION pos = m_mapFont.GetStartPosition();

        while (pos != NULL) 
        {
            hFont = m_mapFont.GetNextValue(pos);
            if (hFont)
                ::DeleteObject(hFont);
        }

        m_mapFont.RemoveAll();
    }

    static HFONT GetFont(WORD uKey)
    {
        _TypeFontPool::CPair* pPairRet = _Instance()->m_mapFont.Lookup(uKey);
        HFONT hftRet = NULL;

        if (NULL == pPairRet)
        {
            hftRet = _Instance()->_CreateNewFont(
                BKF_ISBOLD(uKey), BKF_ISUNDERLINE(uKey), BKF_ISITALIC(uKey), BKF_GETADDING(uKey)
                );
            if (hftRet)
                _Instance()->m_mapFont[uKey] = hftRet;
        }
        else
            hftRet = pPairRet->m_value;

        return hftRet;
    }

    static HFONT GetFont(BOOL bBold, BOOL bUnderline, BOOL bItalic, char chAdding = 0)
    {
        return GetFont(BKF_MAKEKEY(bBold, bUnderline, bItalic, chAdding));
    }

    static void SetDefaultFont(LPCTSTR lpszFaceName, LONG lSize)
    {
        _Instance()->m_strFaceName = lpszFaceName;
        _Instance()->m_lFontSize = lSize;

        HFONT hftOld = _Instance()->m_mapFont[BKF_DEFAULTFONT];
        _Instance()->m_mapFont[BKF_DEFAULTFONT] = _Instance()->_GetDefaultGUIFont();
        ::DeleteObject(hftOld);
    }

    static size_t GetCount()
    {
        return _Instance()->m_mapFont.GetCount();
    }

protected:

    LOGFONT m_lfDefault;
    _TypeFontPool m_mapFont;
    CString m_strFaceName;
    LONG m_lFontSize;

    static BkFontPool* ms_pInstance;

    static BkFontPool* _Instance()
    {
        if (!ms_pInstance)
            ms_pInstance = new BkFontPool;
        return ms_pInstance;
    }

    HFONT _GetDefaultGUIFont()
    {
        DWORD dwSysVer = ::GetVersion();

        ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &m_lfDefault);

        m_lfDefault.lfHeight = _GetFontAbsHeight(m_lFontSize);
        _tcscpy(m_lfDefault.lfFaceName, m_strFaceName);

        m_lfDefault.lfQuality = ANTIALIASED_QUALITY;

        return ::CreateFontIndirect(&m_lfDefault);
    }

    HFONT _CreateNewFont(BOOL bBold, BOOL bUnderline, BOOL bItalic, char chAdding)
    {
        LOGFONT lfNew;

        memcpy(&lfNew, &m_lfDefault, sizeof(LOGFONT));

        lfNew.lfWeight      = (bBold ? FW_BOLD : FW_NORMAL);
        lfNew.lfUnderline   = (FALSE != bUnderline);
        lfNew.lfItalic      = (FALSE != bItalic);

        lfNew.lfHeight = _GetFontAbsHeight(lfNew.lfHeight - chAdding);

        return ::CreateFontIndirect(&lfNew);
    }

    inline LONG _GetFontAbsHeight(LONG lSize)
    {
        return lSize;
        //         HDC hDC = ::GetDC(NULL);
        //         LONG lHeight = chSize;
        // 
        //         lHeight *= 96;
        //         lHeight /= ::GetDeviceCaps(hDC, LOGPIXELSY);
        // 
        //         ::ReleaseDC(NULL, hDC);
        // 
        // 
        //         return lHeight;
    }
};

__declspec(selectany) BkFontPool* BkFontPool::ms_pInstance = NULL;
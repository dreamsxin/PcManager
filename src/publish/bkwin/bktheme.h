//////////////////////////////////////////////////////////////////////////
//  Class Name: BkTheme
// Description: Windows Theme(XP later)
//     Creator: ZhangXiaoxuan
//     Version: 2009.5.12 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlcoll.h>
#include <Uxtheme.h>
#include <tmschema.h>

class BkWinThemeFunc
{
    typedef HTHEME (WINAPI *FnOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
    typedef HRESULT (WINAPI *FnCloseThemeData)(HTHEME hTheme);
    typedef HRESULT (WINAPI *FnDrawThemeBackground)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect);
    typedef HRESULT (WINAPI *FnSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);

public:
    BkWinThemeFunc()
        : m_bThemeValid(FALSE)
        , m_pfnOpenThemeData(NULL)
        , m_pfnCloseThemeData(NULL)
        , m_pfnDrawThemeBackground(NULL)
        , m_pfnSetWindowTheme(NULL)
    {
        HMODULE hModTheme = ::GetModuleHandle(_T("uxtheme.dll"));

        if (!hModTheme)
            return;

        m_bThemeValid = TRUE;
        m_pfnOpenThemeData          = (FnOpenThemeData)::GetProcAddress(hModTheme, "OpenThemeData");
        m_pfnCloseThemeData         = (FnCloseThemeData)::GetProcAddress(hModTheme, "CloseThemeData");
        m_pfnDrawThemeBackground    = (FnDrawThemeBackground)::GetProcAddress(hModTheme, "DrawThemeBackground");
        m_pfnSetWindowTheme         = (FnSetWindowTheme)::GetProcAddress(hModTheme, "SetWindowTheme");
    }

    static BOOL IsValid()
    {
        return _Instance()->m_bThemeValid;
    }

    static HTHEME OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
    {
        if (_Instance()->m_pfnOpenThemeData)
        {
            return _Instance()->m_pfnOpenThemeData(hwnd, pszClassList);
        }
        else
            return NULL;
    }

    static HRESULT CloseThemeData(HTHEME hTheme)
    {
        if (_Instance()->m_pfnCloseThemeData)
        {
            return _Instance()->m_pfnCloseThemeData(hTheme);
        }
        else
            return E_NOTIMPL;
    }

    static HRESULT DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect)
    {
        if (_Instance()->m_pfnDrawThemeBackground)
        {
            return _Instance()->m_pfnDrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
        }
        else
            return E_NOTIMPL;
    }

    static HRESULT SetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList)
    {
        if (_Instance()->m_pfnSetWindowTheme)
        {
            return _Instance()->m_pfnSetWindowTheme(hwnd, pszSubAppName, pszSubIdList);
        }
        else
            return E_NOTIMPL;
    }

    enum
    {
        BkThemeNameButton = 0, 
        BkThemeNameEnd, 
    };

    static LPCWSTR ThemeName(int nID)
    {
        static const LPCWSTR Names[] =
        {
            L"BUTTON",
            L"",
        };

        static int NamesCount = sizeof(Names) / sizeof(LPCWSTR) - 1;

        if (nID >= 0 && nID < NamesCount)
            return Names[BkThemeNameButton];
        else
            return NULL;
    }

private:

    BOOL m_bThemeValid;

    FnOpenThemeData         m_pfnOpenThemeData;
    FnCloseThemeData        m_pfnCloseThemeData;
    FnDrawThemeBackground   m_pfnDrawThemeBackground;
    FnSetWindowTheme        m_pfnSetWindowTheme;

    static BkWinThemeFunc* ms_pInstance;

    static BkWinThemeFunc* _Instance()
    {
        if (!ms_pInstance)
            ms_pInstance = new BkWinThemeFunc;
        return ms_pInstance;
    }

//     static BkWinThemeFunc& _Instance()
//     {
//         static BkWinThemeFunc s_obj;
// 
//         return s_obj;
//     }
};

__declspec(selectany) BkWinThemeFunc* BkWinThemeFunc::ms_pInstance = NULL;

template<int t_nThemeId, int t_partid>
class CBkWinTheme
{
public:
    CBkWinTheme(HWND hWnd = NULL)
        : m_hTheme(NULL)
    {
        if (hWnd)
            OpenTheme(hWnd);
    }

    ~CBkWinTheme()
    {
        BkWinThemeFunc::CloseThemeData(m_hTheme);
    }

    BOOL IsValid()
    {
        return (NULL != m_hTheme);
    }

    BOOL OpenTheme(HWND hWnd)
    {
        if (m_hTheme)
            return FALSE;

        m_hTheme = BkWinThemeFunc::OpenThemeData(NULL, BkWinThemeFunc::ThemeName(t_nThemeId));

        if (m_hTheme)
            return TRUE;

        return FALSE;
    }

    void DrawBackground(HDC hdc, int iStateId, const RECT *pRect)
    {
        BkWinThemeFunc::DrawThemeBackground(m_hTheme, hdc, t_partid, iStateId, pRect, NULL);
    }

protected:

    HTHEME m_hTheme;
};

typedef CBkWinTheme<BkWinThemeFunc::BkThemeNameButton, BP_CHECKBOX> CBkCheckBoxTheme;
typedef CBkWinTheme<BkWinThemeFunc::BkThemeNameButton, BP_RADIOBUTTON> CBkRadioBoxTheme;
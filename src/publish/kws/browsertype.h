#ifndef BROWSERTYPE_H
#define BROWSERTYPE_H

#include "shlwapi.h"

namespace BrowserType
{
    enum BROWSERTYPE
    {
        BT_UNKNOWN = 1,
        BT_IE,
        BT_MAXTHON2,
        BT_THEWORLD,
        BT_CHROME,
        BT_FIREFOX,
        BT_360SE,
        BT_SOGOU,
        BT_TT,
        BT_OPERA,
        BT_SAAYAA,
        BT_SAFARI,
        BT_TANGO3,
        BT_IQ,
        BT_GB
    };

    inline BROWSERTYPE GetBrowserType(const std::wstring& wstrBrowserPath)
    {
        BROWSERTYPE browserType = BT_UNKNOWN;
        const wchar_t *pwszFileName = NULL;

        if ( wstrBrowserPath.empty() )
        {
            return BT_UNKNOWN;
        }

        pwszFileName = PathFindFileNameW(wstrBrowserPath.c_str());

        if (wstrBrowserPath.c_str() == pwszFileName)
        {
            return BT_UNKNOWN;
        }

        //BEGIN
        if (_wcsicmp(pwszFileName, L"iexplore.exe") == 0)
        {
            browserType = BT_IE;
        }
        else if (_wcsicmp(pwszFileName, L"maxthon.exe") == 0)
        {
            browserType = BT_MAXTHON2;
        }
        else if (_wcsicmp(pwszFileName, L"theworld.exe") == 0)
        {
            browserType = BT_THEWORLD;
        }
        else if (_wcsicmp(pwszFileName, L"chrome.exe") == 0)
        {
            browserType = BT_CHROME;
        }
        else if (_wcsicmp(pwszFileName, L"firefox.exe") == 0)
        {
            browserType = BT_FIREFOX;
        }
        else if (_wcsicmp(pwszFileName, L"360se.exe") == 0)
        {
            browserType = BT_360SE;
        }
        else if (_wcsicmp(pwszFileName, L"sogouexplorer.exe") == 0)
        {
            browserType = BT_SOGOU;
        }
        else if (_wcsicmp(pwszFileName, L"ttraveler.exe") == 0)
        {
            browserType = BT_TT;
        }
        else if (_wcsicmp(pwszFileName, L"opera.exe") == 0)
        {
            browserType = BT_OPERA;
        }
        else if (_wcsicmp(pwszFileName, L"saayaa.exe") == 0)
        {
            browserType = BT_SAAYAA;
        }
        else if (_wcsicmp(pwszFileName, L"safari.exe") == 0)
        {
            browserType = BT_SAFARI;
        }
        else if (_wcsicmp(pwszFileName, L"tango3.exe") == 0)
        {
            browserType = BT_TANGO3;
        }
        else if (_wcsicmp(pwszFileName, L"myiq.exe") == 0)
        {
            browserType = BT_IQ;
        }
        else if (_wcsicmp(pwszFileName, L"greenbrowser.exe") == 0)
        {
            browserType = BT_GB;
        }
        else
        {
            browserType = BT_UNKNOWN;
        }

        return browserType;

    }

    inline bool IsIECore(BROWSERTYPE browserType)
    {
        bool bRetVal = false;

        switch (browserType)
        {
        case	BT_IE:
        case	BT_MAXTHON2:
        case	BT_THEWORLD:
        case	BT_360SE:
        case	BT_SOGOU:
        case	BT_SAAYAA:
        case	BT_TANGO3:
        case	BT_IQ:
        case    BT_TT:
            {
                bRetVal = true;
                break;
            }
        default:
            {
                bRetVal = false;
                break;
            }
        }

        return bRetVal;

    }
}
#endif
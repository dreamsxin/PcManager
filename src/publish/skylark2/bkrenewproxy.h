/**
* @file    bkrenewproxy.h
* @brief   ...
* @author  bbcallen
* @date    2010-03-12 15:22
*/

#ifndef BKRENEWPROXY_H
#define BKRENEWPROXY_H

#include <atlbase.h>
#include <atlpath.h>
#include "common\utility.h"
#include "skylark2\skylarkbase.h"

NS_SKYLARK_BEGIN

inline HRESULT BkRenewProxySettingsFromReg()
{
    CPath pathScanIni;
    HRESULT hr = CAppPath::Instance().GetLeidianCfgPath(pathScanIni.m_strPath);
    if (FAILED(hr))
        return hr;

    pathScanIni.Append(L"scansetting.ini");


    WCHAR szProxyServer[1024];
    WCHAR szProxyByPass[1024];
    DWORD dwProxyEnable = 0;
    CRegKey hRegKey;
    LONG lRet = hRegKey.Open(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
        KEY_READ);
    if (ERROR_SUCCESS == lRet)
    {
        memset(szProxyServer, 0, sizeof(szProxyServer));
        ULONG uSize = _countof(szProxyServer);
        lRet = hRegKey.QueryStringValue(L"ProxyServer", szProxyServer, &uSize);
        if (ERROR_SUCCESS == lRet)
        {
            szProxyServer[_countof(szProxyServer) - 1] = L'\0';
        }
        else
        {
            szProxyServer[0] = L'\0';
        }



        uSize = _countof(szProxyServer);
        lRet = hRegKey.QueryStringValue(L"ProxyByPass", szProxyByPass, &uSize);
        if (ERROR_SUCCESS == lRet)
        {
            szProxyByPass[_countof(szProxyByPass) - 1] = L'\0';
        }
        else
        {
            szProxyByPass[0] = L'\0';
        }



        lRet = hRegKey.QueryDWORDValue(L"ProxyEnable", dwProxyEnable);
        if (ERROR_SUCCESS == lRet)
        {
            NULL;
        }
        else
        {
            dwProxyEnable = 0;
        }
    }
    else
    {
        szProxyServer[0] = L'\0';
        szProxyByPass[0] = L'\0';
        dwProxyEnable    = 0;
    }


    CString strProxyEnable;
    strProxyEnable.Format(L"%lu", dwProxyEnable);


    ::WritePrivateProfileString(
        L"Proxy",
        L"ProxyServer",
        szProxyServer,
        pathScanIni.m_strPath);

    ::WritePrivateProfileString(
        L"Proxy",
        L"ProxyByPass",
        szProxyByPass,
        pathScanIni.m_strPath);

    ::WritePrivateProfileString(
        L"Proxy",
        L"ProxyEnable",
        strProxyEnable,
        pathScanIni.m_strPath);


    return S_OK;
}

NS_SKYLARK_END

#endif//BKRENEWPROXY_H
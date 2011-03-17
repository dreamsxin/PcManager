/////////////////////////////////////////////////////////////////
//
//	Filename: 	winhttpdownloader.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2010-11-06  11:54
//	Comment:	
//
///////////////////////////////////////////////////////////////

#pragma once

#include "winhttpdownloaderbase.h"
#include "winhttpapi.h"
#include "winmod/proxyinfo.h"
//#include   <string>
#include "kispublic/kcalloem.h"
#include "bksafe/bksafeconfig.h"

NS_WINMOD_BEGIN

class CWinHttpDownloader
{
public:
    CWinHttpDownloader(void);
    ~CWinHttpDownloader(void);

    operator HINTERNET() const;

    HRESULT Open(
        LPCTSTR lpszAgent       = NULL,
        DWORD   dwAccessType    = INTERNET_OPEN_TYPE_PRECONFIG, 
        LPCTSTR lpszProxy       = NULL, 
        LPCTSTR lpszProxyBypass = NULL, 
        DWORD   dwFlags         = 0);

    void Close();

    HRESULT HttpDownload(
        /* [in ] */ LPCWSTR                     lpszLocalFilePath,
        /* [in ] */ IInetHttpDownloadProgress*  piCallback,
        /* [in ] */ LPCTSTR                     lpszServerName,
        /* [in ] */ int                         nServerPort,
        /* [in ] */ LPCTSTR                     lpObject,
        /* [in ] */ DWORD                       dwTimeout,
        /* [out] */ DWORD*                      pdwStatusCode       = NULL,
        /* [in ] */ LPCTSTR                     lpszSpecHostName    = NULL, 
        /* [in ] */ BOOL                        bAcceptGZip         = FALSE);

	// 增加带来支持 [1/20/2011 zhangbaoliang]
	void SetUpLiveProxyInfo(BKSAFE_PROXY_INFO::UPLIVE_PROXY_INFO proxyInfo);

	//////////////////////////////////////////////////////////////////////////
	//
	//使用dwProxyType置顶代理类型
	void UseBKSafeProxyInfo(DWORD dwProxyType = PROXY_TYPE::PROXY_TYPE_ALL);

private:
    HINTERNET   m_h;

	BKSAFE_PROXY_INFO::UPLIVE_PROXY_INFO	m_upliveProxyInfo;
	BOOL				_ProxyInfoValid();

};

NS_WINMOD_END
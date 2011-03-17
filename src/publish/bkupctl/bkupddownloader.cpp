//#include "stdafx.h"

#include "stdafx_bkupclt.h"
#include <string>
#include "../kispublic/kcalloem.h"
#include "../kispublic/kopksafereg.h"

#include "bkupclterr.h"
#include "bkupddownloader.h"
#include "bkurl.h"
#include "winhttp/WinHttpApi.h"





// 下载器http Agent名
#define BKUPD_HTTP_AGENT_NAME      L"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)"

// TCP连接超时
#define BKUPD_TCP_CONNECT_TIMEOUT  20000
// TCP接收超时
#define BKUPD_TCP_RECEIVE_TIMEOUT  20000

// 网络传输buffer大小
#define BKUPD_NETWORK_BUFFER_SIZE  1024

#define BKUPD_REQUEST_HEADER_KEEPALIVE                 _T("Keep-Alive: 5")
#define BKUPD_REQUEST_HEADER_CONNECTION_KEEPALIVE      _T("Connection: Keep-Alive")
#define BKUPD_REQUEST_HEADER_CONNECTION_CLOSE          _T("Connection: Close")



class CWinHttpApi_ : public WinHttpApi
{
public:
	CWinHttpApi_() {
		Initialize();
	}

	~CWinHttpApi_() {
		Uninitialize();
	}
};

static CWinHttpApi_ __winHttp;
static WinHttpApi* GetWinHttp()
{
	return &__winHttp;
}


HRESULT CBkUpdDownloader::Download(
								   LPCTSTR lpszUrl, 
								   IBkUpdWebFile *piWebFile, 
								   IBkDownloadCallback* piCallback
								   )
{
	HRESULT hResult = E_FAIL;
	BOOL bRet = FALSE;
	HINTERNET hNet = NULL, hConnect = NULL, hRequest = NULL;
	DWORD dwSize = 0;
	DWORD dwHttpRetCode = 0, dwTotalSize = 0, dwDownloadSize = 0;
	BYTE pbyBuffer[BKUPD_NETWORK_BUFFER_SIZE];
	CBkUrl url;

	bRet = url.SetUrl(lpszUrl);
	if (!bRet)
		goto Exit0;

	if (TRUE == _ProxyInfoValid())
	{//使用代理
		TCHAR pszHost[MAX_PATH] = {0};
		_sntprintf_s(pszHost, sizeof(pszHost), L"%s:%s", m_upliveProxyInfo.m_strProxyHost, m_upliveProxyInfo.m_strProxyPort);
		hNet = __winHttp.OpenSession(BKUPD_HTTP_AGENT_NAME, pszHost);
	}
	else
	{//不适用代理
		hNet = __winHttp.OpenSession(BKUPD_HTTP_AGENT_NAME);
	}
	
	if (NULL == hNet)
		goto Exit0;

	bRet = __winHttp.SetTimeouts(hNet, BKUPD_TCP_CONNECT_TIMEOUT, BKUPD_TCP_CONNECT_TIMEOUT, BKUPD_TCP_RECEIVE_TIMEOUT, BKUPD_TCP_RECEIVE_TIMEOUT);
	// 	if ( !bRet )
	// 		goto Exit0;


	hConnect = __winHttp.Connect(hNet, url.GetHostName(), url.GetPort());
	if (NULL == hConnect)
		goto Exit0;



	hRequest = __winHttp.OpenRequest(hConnect, NULL, url.GetUrlPath(), 0);
	if (NULL == hRequest)
		goto Exit0;

	if (m_bConnectionKeepAlive)
	{
		__winHttp.AddRequestHeaders(hRequest, BKUPD_REQUEST_HEADER_KEEPALIVE);
		__winHttp.AddRequestHeaders(hRequest, BKUPD_REQUEST_HEADER_CONNECTION_KEEPALIVE);
	}
	else
	{
		__winHttp.AddRequestHeaders(hRequest, BKUPD_REQUEST_HEADER_CONNECTION_CLOSE);
	}

	// 设置代理信息 [1/11/2011 zhangbaoliang]
	if (TRUE == _ProxyInfoValid())
	{
		WINHTTP_PROXY_INFO proxyInfo = { 0 };
		memset(&proxyInfo, 0, sizeof(proxyInfo));

		proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
		TCHAR szProxy[MAX_PATH] = L"";
		_sntprintf_s(szProxy, sizeof(szProxy), L"%s:%s", m_upliveProxyInfo.m_strProxyHost, m_upliveProxyInfo.m_strProxyPort);
		proxyInfo.lpszProxy = szProxy;

		// 注意设置代理失败没有退出 [1/11/2011 zhangbaoliang]
		if (TRUE == __winHttp.UseWinHttp())
		{//wininet和winhttp设置代理方式不一样，所哟需要判断
			if (FALSE == __winHttp.WinHttpSetOption(hNet, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo)))
			{
				goto Exit0;
			}
		}

		if (FALSE == m_upliveProxyInfo.m_strProxyUserName.IsEmpty() &&
			0	  != m_upliveProxyInfo.nVerify)
		{
			// 因为wininet和winhttp的代理设置参数不完全兼容，所哟在设置用户名和密码的时候需要判断一下 [1/12/2011 zhangbaoliang]
			DWORD dwOption = WINHTTP_OPTION_PROXY_USERNAME;
			if (FALSE == __winHttp.UseWinHttp())
				dwOption = INTERNET_OPTION_PROXY_USERNAME;

			if (FALSE == __winHttp.WinHttpSetOption(hRequest,  dwOption, (LPVOID)m_upliveProxyInfo.m_strProxyUserName.GetBuffer(-1), m_upliveProxyInfo.m_strProxyUserName.GetLength()))
			{
				m_upliveProxyInfo.m_strProxyUserName.ReleaseBuffer(-1);
				goto Exit0;
			}
			m_upliveProxyInfo.m_strProxyUserName.ReleaseBuffer(-1);

			dwOption = WINHTTP_OPTION_PROXY_PASSWORD;
			if (FALSE == __winHttp.UseWinHttp())
				dwOption = INTERNET_OPTION_PROXY_PASSWORD;

			if (FALSE == m_upliveProxyInfo.m_strProxyPassWord.IsEmpty())
			{
				if (FALSE == __winHttp.WinHttpSetOption(hRequest, dwOption, (LPVOID)m_upliveProxyInfo.m_strProxyPassWord.GetBuffer(-1), m_upliveProxyInfo.m_strProxyPassWord.GetLength()))
				{
					m_upliveProxyInfo.m_strProxyPassWord.ReleaseBuffer(-1);
					goto Exit0;
				}
			}
			m_upliveProxyInfo.m_strProxyPassWord.ReleaseBuffer(-1);
		}
	}
	// end set proxy [1/11/2011 zhangbaoliang]


	bRet = __winHttp.SendRequest(hRequest, NULL, 0);
	if (!bRet)
	{
		goto Exit0;
	}

	bRet = __winHttp.EndRequest(hRequest);
	if ( !bRet )
		goto Exit0;

	dwSize = sizeof(DWORD);
	bRet = __winHttp.QueryInfo(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, (CHAR*)&dwHttpRetCode, &dwSize);
	if (!bRet)
		goto Exit0;

	if (HTTP_STATUS_OK != dwHttpRetCode)
	{
		hResult = BKERR_MAKE_HTTPERROR(dwHttpRetCode);
		goto Exit0;
	}

	dwSize = sizeof(DWORD);
	bRet = __winHttp.QueryInfo(hRequest, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, (CHAR*)&dwTotalSize, &dwSize);
	if (!bRet)
		goto Exit0;

	if (piWebFile)
	{
		hResult = piWebFile->SetSize(dwTotalSize);
		if ( FAILED( hResult ) )
		{
			goto Exit0;
		}

		piWebFile->SetPos(0);
	}

	do 
	{
		bRet = __winHttp.ReadData(hRequest, pbyBuffer, BKUPD_NETWORK_BUFFER_SIZE, &dwSize);
		if (!bRet)
		{
			hResult = HRESULT_FROM_WIN32( GetLastError());
			goto Exit0;
		}

		if (0 == dwSize)
			break;

		if (dwDownloadSize + dwSize > dwTotalSize)
			dwSize = dwTotalSize - dwDownloadSize;

		if (piWebFile)
			piWebFile->Write(pbyBuffer, dwSize);

		dwDownloadSize += dwSize;

		if ( piCallback )
		{
			if ( !piCallback->DownloadProcess( dwTotalSize, dwDownloadSize ) )
			{
				hResult = E_BKUPCTL_INTERRUPT;
				break;;
			}
		}

	} while (dwDownloadSize < dwTotalSize);

	//hResult = S_OK;

Exit0:

	if (E_FAIL == hResult)
		hResult = BKERR_FROM_LASTERROR();

	if (hRequest)
	{
		__winHttp.CloseInternetHandle(hRequest);
		hRequest = NULL;
	}

	if (hConnect)
	{
		__winHttp.CloseInternetHandle(hConnect);
		hConnect = NULL;
	}

	if (hNet)
	{
		__winHttp.CloseInternetHandle(hNet);
		hNet = NULL;
	}

	if (piWebFile)
		piWebFile->Close();

	return hResult;
}

void CBkUpdDownloader::SetUpLiveProxyInfo(BKSAFE_PROXY_INFO::UPLIVE_PROXY_INFO proxyInfo)
{
	m_upliveProxyInfo = proxyInfo;
}

void CBkUpdDownloader::UseBKSafeProxyInfo(DWORD dwProxyType)
{
	//设置代理
	BKSAFE_PROXY_INFO::UPLIVE_PROXY_INFO proxyInfo;
	BKSafeConfig::GetProxyInfo(proxyInfo.m_strProxyHost, proxyInfo.m_strProxyPort, proxyInfo.m_strProxyUserName, proxyInfo.m_strProxyPassWord, proxyInfo.nProxyType, proxyInfo.nVerify);
	if (FALSE == proxyInfo.m_strProxyHost.IsEmpty())
	{//如果代理服务器有效并且是HTTP代理就设置代理服务器
		BOOL bFlag = FALSE;
		if (dwProxyType == PROXY_TYPE::PROXY_TYPE_ALL)
		{
			bFlag = TRUE;
		}
		else if (dwProxyType == proxyInfo.nProxyType)
		{
			bFlag = TRUE;
		}

		if (TRUE == bFlag)
		{
			proxyInfo.bProxyValid = TRUE;
			SetUpLiveProxyInfo(proxyInfo);
		}
	}

}

BOOL CBkUpdDownloader::_ProxyInfoValid()
{
	if (TRUE == m_upliveProxyInfo.bProxyValid &&
		FALSE == m_upliveProxyInfo.m_strProxyHost.IsEmpty() &&
		FALSE == m_upliveProxyInfo.m_strProxyPort.IsEmpty())
	{
		return TRUE;
	}

	return FALSE;
}
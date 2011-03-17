#include "StdAfx.h"
#include "winhttpdownloader.h"

// 下载器http Agent名
#define BKUPD_HTTP_AGENT_NAME      L"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)"

// 网络传输buffer大小
#define BKUPD_NETWORK_BUFFER_SIZE  1024

using namespace WinMod;

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

CWinHttpDownloader::CWinHttpDownloader(void)
{
    m_h = NULL;
}

CWinHttpDownloader::~CWinHttpDownloader(void)
{
    Close();
}


CWinHttpDownloader::operator HINTERNET() const
{
    return m_h;
}

HRESULT CWinHttpDownloader::Open(LPCTSTR lpszAgent /* = NULL */, 
                         DWORD dwAccessType /* = INTERNET_OPEN_TYPE_PRECONFIG */, 
                         LPCTSTR lpszProxy /* = NULL */,
                         LPCTSTR lpszProxyBypass /* = NULL */,
                         DWORD dwFlags /* = 0 */
                         )
{
    Close();
 //   m_h = __winHttp.OpenSession(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwAccessType);

	if (TRUE == _ProxyInfoValid())
	{//使用代理
		TCHAR pszHost[MAX_PATH] = {0};
		_sntprintf_s(pszHost, sizeof(pszHost), L"%s:%s", m_upliveProxyInfo.m_strProxyHost, m_upliveProxyInfo.m_strProxyPort);
		m_h = __winHttp.OpenSession(lpszProxy, pszHost);
	}
	else
	{//不适用代理
		m_h = __winHttp.OpenSession(lpszAgent, dwAccessType, lpszProxy, lpszProxyBypass, dwAccessType);
	}


    if (NULL == m_h)
        return E_FAIL;

    return S_OK;
}

void CWinHttpDownloader::Close()
{
    if (m_h != NULL)
    {
        __winHttp.CloseInternetHandle(m_h);
        m_h = NULL;
    }
}

HRESULT CWinHttpDownloader::HttpDownload(
    /* [in ] */ LPCWSTR                     lpszLocalFilePath,
    /* [in ] */ IInetHttpDownloadProgress*  piCallback,
    /* [in ] */ LPCTSTR                     lpszServerName,
    /* [in ] */ int                         nServerPort,
    /* [in ] */ LPCTSTR                     lpObject,
    /* [in ] */ DWORD                       dwTimeout,
    /* [out] */ DWORD*                      pdwStatusCode,
    /* [in ] */ LPCTSTR                     lpszSpecHostName, 
    /* [in ] */ BOOL                        bAcceptGZi
    )
{

    if (!lpszLocalFilePath ||!lpszServerName ||!lpObject|| !m_h)
        return E_POINTER;

    HRESULT hResult = E_FAIL;
    BOOL bRet = FALSE;
    HINTERNET hNet = NULL, hConnect = NULL, hRequest = NULL;
    DWORD dwSize = 0;
    DWORD dwHttpRetCode = 0, dwTotalSize = 0, dwDownloadSize = 0;
    BYTE pbyBuffer[BKUPD_NETWORK_BUFFER_SIZE];
    CAtlFile m_hFile;

    hConnect = __winHttp.Connect(m_h, lpszServerName, nServerPort);
    if (NULL == hConnect)
        goto Exit0;

    hRequest = __winHttp.OpenRequest(hConnect, NULL, lpObject, 0);
    if (NULL == hRequest)
        goto Exit0;

    bRet = __winHttp.SetTimeouts(hRequest, dwTimeout, dwTimeout, dwTimeout, dwTimeout);
    if ( !bRet )
        goto Exit0;

	// 设置代理信息 [1/20/2011 zhangbaoliang]
	if (TRUE == _ProxyInfoValid())
	{
		WINHTTP_PROXY_INFO proxyInfo = { 0 };
		memset(&proxyInfo, 0, sizeof(proxyInfo));

		proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
		TCHAR szProxy[MAX_PATH] = L"";
		_sntprintf_s(szProxy, sizeof(szProxy), L"%s:%s", m_upliveProxyInfo.m_strProxyHost, m_upliveProxyInfo.m_strProxyPort);
		proxyInfo.lpszProxy = szProxy;

		// 注意设置代理失败没有退出 [1/11/2011 zhangbaoliang]
		if (TRUE == __winHttp.IsUseWinHttp())
		{//wininet和winhttp设置代理方式不一样，所哟需要判断
			if (FALSE == __winHttp.WinHttpSetOption(m_h, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo)))
			{
				goto Exit0;
			}
		}

		if (FALSE == m_upliveProxyInfo.m_strProxyUserName.IsEmpty() &&
			0	  != m_upliveProxyInfo.nVerify)
		{
			// 因为wininet和winhttp的代理设置参数不完全兼容，所哟在设置用户名和密码的时候需要判断一下 [1/12/2011 zhangbaoliang]
			DWORD dwOption = WINHTTP_OPTION_PROXY_USERNAME;
			if (FALSE == __winHttp.IsUseWinHttp())
				dwOption = INTERNET_OPTION_PROXY_USERNAME;

			if (FALSE == __winHttp.WinHttpSetOption(hRequest,  dwOption, (LPVOID)m_upliveProxyInfo.m_strProxyUserName.GetBuffer(-1), m_upliveProxyInfo.m_strProxyUserName.GetLength()))
			{
				m_upliveProxyInfo.m_strProxyUserName.ReleaseBuffer(-1);
				goto Exit0;
			}
			m_upliveProxyInfo.m_strProxyUserName.ReleaseBuffer(-1);

			dwOption = WINHTTP_OPTION_PROXY_PASSWORD;
			if (FALSE == __winHttp.IsUseWinHttp())
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
	// end set proxy [1/20/2011 zhangbaoliang]

    bRet = __winHttp.SendRequest(hRequest, NULL, 0);
    if (!bRet)
        goto Exit0;

    bRet = __winHttp.PrepareReceiveResponse(hRequest);
    if ( !bRet )
        goto Exit0;

    dwSize = sizeof(DWORD);
    bRet = __winHttp.QueryInfo(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, (CHAR*)&dwHttpRetCode, &dwSize);
    if (!bRet)
        goto Exit0;

    if (pdwStatusCode)
        *pdwStatusCode = dwHttpRetCode;

    if (HTTP_STATUS_OK != dwHttpRetCode)
    {
        hResult = MAKE_WINMOD_HTTP_ERROR(dwHttpRetCode);
        goto Exit0;
    }

    dwSize = sizeof(DWORD);
    bRet = __winHttp.QueryInfo(hRequest, WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER, (CHAR*)&dwTotalSize, &dwSize);
    if (!bRet)
        goto Exit0;

    hResult = m_hFile.Create(lpszLocalFilePath,
        GENERIC_WRITE,
        FILE_SHARE_DELETE,
        CREATE_ALWAYS);
    if (FAILED(hResult))
    {
        goto Exit0;
    }
    
    hResult = m_hFile.SetSize(dwTotalSize);
    if ( FAILED( hResult ) )
    {
        goto Exit0;
    }

    m_hFile.Seek(0, FILE_BEGIN);
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

        if ( piCallback )
        {
            if ( FAILED(hResult = piCallback->OnReceiveData(
                dwTotalSize, 
                dwDownloadSize,
                dwSize,
                pbyBuffer
                ))
                )
            {
                break;;
            }
        }

        m_hFile.Write(pbyBuffer, dwSize);

        dwDownloadSize += dwSize;

    } while (dwDownloadSize < dwTotalSize);


    hResult = (dwDownloadSize == dwTotalSize) ? S_OK : E_FAIL;

Exit0:
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

    if (FAILED(hResult))
        DeleteFileW(lpszLocalFilePath);

    return hResult;
}


// 一下是新增加的代理支持 [1/20/2011 zhangbaoliang]

void CWinHttpDownloader::SetUpLiveProxyInfo(BKSAFE_PROXY_INFO::UPLIVE_PROXY_INFO proxyInfo)
{
	m_upliveProxyInfo = proxyInfo;
}

void CWinHttpDownloader::UseBKSafeProxyInfo(DWORD dwProxyType)
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

BOOL CWinHttpDownloader::_ProxyInfoValid()
{
	if (TRUE == m_upliveProxyInfo.bProxyValid &&
		FALSE == m_upliveProxyInfo.m_strProxyHost.IsEmpty() &&
		FALSE == m_upliveProxyInfo.m_strProxyPort.IsEmpty())
	{
		return TRUE;
	}

	return FALSE;
}
#pragma once

#include <WinInet.h>

#pragma comment(lib, "wininet.lib")

#include <atlfile.h>

#pragma warning(push)
#pragma warning(disable: 4819)
#include <atlsecurity.h>
#pragma warning(pop)

// 下载器http Agent名
#define VKSDL_HTTP_AGENT_NAME      L"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)"

// TCP连接超时
#define VKSDL_TCP_CONNECT_TIMEOUT  10000
// TCP接收超时
#define VKSDL_TCP_RECEIVE_TIMEOUT  5000

// 网络传输buffer大小
#define VKSDL_NETWORK_BUFFER_SIZE  1024

#define VKSDL_REQUEST_HEADER_CONNECTION_CLOSE          _T("Connection: Close")

interface IBkMiniDownloaderCallback
{
    virtual BOOL OnProgress(DWORD dwTotalSize, DWORD dwDownloadedSize) = 0;
};

class CBkMiniDownloader
{
public:

    class _UrlCracker
    {
    public:
        _UrlCracker()
            : m_bIsValidUrl(FALSE)
        {

        }
        ~_UrlCracker()
        {

        }

        BOOL SetUrl(LPCTSTR lpszUrl)
        {
            URL_COMPONENTS urlcomp = {sizeof(URL_COMPONENTS)};
            BOOL bRet = FALSE;

            urlcomp.lpszHostName = m_strHostName.GetBuffer(MAX_PATH + 1);
            urlcomp.dwHostNameLength = MAX_PATH;
            urlcomp.lpszUrlPath = m_strUrlPath.GetBuffer(MAX_PATH + 1);
            urlcomp.dwUrlPathLength = MAX_PATH;

            bRet = ::InternetCrackUrl(lpszUrl, 0, ICU_ESCAPE, &urlcomp);

            m_strHostName.ReleaseBuffer();
            m_strUrlPath.ReleaseBuffer();

            m_usPort = urlcomp.nPort;

            m_bIsValidUrl = bRet;

            m_strFullUrl = lpszUrl;

            return m_bIsValidUrl;
        }

        LPCTSTR GetHostName(void)
        {
            if (!m_bIsValidUrl)
                return NULL;

            return m_strHostName;
        }

        LPCTSTR GetUrlPath(void)
        {
            if (!m_bIsValidUrl)
                return NULL;

            return m_strUrlPath;
        }

        INTERNET_PORT GetPort(void)
        {
            if (!m_bIsValidUrl)
                return NULL;

            return m_usPort;
        }

        LPCTSTR GetFullUrl(void)
        {
            if (!m_bIsValidUrl)
                return NULL;

            return m_strFullUrl;
        }

    private:
        BOOL m_bIsValidUrl;
        CString m_strFullUrl;
        CString m_strHostName;
        CString m_strUrlPath;
        INTERNET_PORT m_usPort;
    };

    CBkMiniDownloader()
    {
    }

    HRESULT Download(LPCTSTR lpszUrl, CAtlTemporaryFile &tmpFile, IBkMiniDownloaderCallback *piCallback = NULL)
    {
        HRESULT hResult = E_FAIL;
        BOOL bRet = FALSE;
        HINTERNET hNet = NULL, hConnect = NULL, hRequest = NULL;
        DWORD dwConnectTimeout = 0, dwSize = 0;
        DWORD dwHttpRetCode = 0, dwTotalSize = 0, dwDownloadSize = 0;
        BYTE pbyBuffer[VKSDL_NETWORK_BUFFER_SIZE];
        _UrlCracker url;

        bRet = url.SetUrl(lpszUrl);
        if (!bRet)
            goto Exit0;

        hNet = ::InternetOpen(VKSDL_HTTP_AGENT_NAME, INTERNET_OPEN_TYPE_PRECONFIG, _T(""), _T(""), 0);
        if (NULL == hNet)
            goto Exit0;

        dwConnectTimeout = VKSDL_TCP_CONNECT_TIMEOUT;
        bRet = ::InternetSetOption(hNet, INTERNET_OPTION_CONNECT_TIMEOUT, &dwConnectTimeout, sizeof(DWORD));
        if (!bRet)
            goto Exit0;

        dwConnectTimeout = VKSDL_TCP_RECEIVE_TIMEOUT;
        bRet = ::InternetSetOption(hNet, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwConnectTimeout, sizeof(DWORD));
        if (!bRet)
            goto Exit0;

        hConnect = ::InternetConnect(
            hNet, url.GetHostName(), url.GetPort(), 
            L"", L"", INTERNET_SERVICE_HTTP, 0, 0
            );
        if (NULL == hConnect)
            goto Exit0;

        hRequest = ::HttpOpenRequest(
            hConnect, NULL, url.GetUrlPath(), NULL, NULL, NULL, 
            INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0
            );
        if (NULL == hRequest)
            goto Exit0;

        ::HttpAddRequestHeaders(
            hRequest, 
            VKSDL_REQUEST_HEADER_CONNECTION_CLOSE, 
            (DWORD)_tcslen(VKSDL_REQUEST_HEADER_CONNECTION_CLOSE), 
            HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE
            );

        bRet = ::HttpSendRequest(hRequest, NULL, 0, NULL, 0);
        if (!bRet)
            goto Exit0;

        dwSize = sizeof(DWORD);
        bRet = ::HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwHttpRetCode, &dwSize, NULL);
        if (!bRet)
            goto Exit0;

        if (HTTP_STATUS_OK != dwHttpRetCode)
        {
            hResult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_HTTP, dwHttpRetCode);
            goto Exit0;
        }

        dwSize = sizeof(DWORD);
        bRet = ::HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwTotalSize, &dwSize, NULL);
        if (!bRet)
            goto Exit0;

        tmpFile.SetSize(dwTotalSize);
        tmpFile.Seek(0, FILE_BEGIN);

        if (piCallback)
        {
            bRet = piCallback->OnProgress(dwTotalSize, dwDownloadSize);
            if (!bRet)
            {
                hResult = E_ABORT;
                goto Exit0;
            }
        }

        do 
        {
            bRet = ::InternetReadFile(hRequest, pbyBuffer, VKSDL_NETWORK_BUFFER_SIZE, &dwSize);
            if (!bRet)
                goto Exit0;

            if (0 == dwSize)
                break;

            if (dwDownloadSize + dwSize > dwTotalSize)
                dwSize = dwTotalSize - dwDownloadSize;

            tmpFile.Write(pbyBuffer, dwSize);

            dwDownloadSize += dwSize;

            if (piCallback)
            {
                bRet = piCallback->OnProgress(dwTotalSize, dwDownloadSize);
                if (!bRet)
                {
                    hResult = E_ABORT;
                    goto Exit0;
                }
            }

        } while (dwDownloadSize < dwTotalSize);

        hResult = S_OK;

Exit0:

        if (E_FAIL == hResult)
            hResult = AtlHresultFromLastError();

        if (hRequest)
        {
            ::InternetCloseHandle(hRequest);
            hRequest = NULL;
        }

        if (hConnect)
        {
            ::InternetCloseHandle(hConnect);
            hConnect = NULL;
        }

        if (hNet)
        {
            ::InternetCloseHandle(hNet);
            hNet = NULL;
        }

        return hResult;
    }
};
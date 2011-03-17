/**
* @file    winmodinetconnection.h
* @brief   ...
* @author  bbcallen
* @date    2009-08-07 15:15
*/

#ifndef WINMODINETCONNECTION_H
#define WINMODINETCONNECTION_H

#include "winmodinethandle.h"

NS_WINMOD_BEGIN

class CInetConnection: public CInetHandle
{
public:
    CInetConnection();
    virtual ~CInetConnection();

    HRESULT SetConnectTimeOut(DWORD  dwMilliSeconds);
    HRESULT GetConnectTimeOut(DWORD& dwMilliSeconds);

    HRESULT SetReceiveTimeOut(DWORD  dwMilliSeconds);
    HRESULT GetReceiveTimeOut(DWORD& dwMilliSeconds);

    HRESULT SetSendTimeOut(DWORD  dwMilliSeconds);
    HRESULT GetSendTimeOut(DWORD& dwMilliSeconds);

    // 未实现的SetOption/GetOption
    //
    // INTERNET_OPTION_BYPASS_EDITED_ENTRY      IE5
    // INTERNET_OPTION_CACHE_TIMESTAMPS 
    // INTERNET_OPTION_CALLBACK
    // INTERNET_OPTION_CLIENT_CERT_CONTEXT 
    // INTERNET_OPTION_CODEPAGE                 IE7
    // INTERNET_OPTION_CODEPAGE_PATH            IE7
    // INTERNET_OPTION_CODEPAGE_EXTRA           IE7
    // INTERNET_OPTION_CONNECT_RETRIES          对同一个HOST的多个IP,每个IP尝试一次
    // INTERNET_OPTION_CONNECTED_STATE
    // INTERNET_OPTION_CONTEXT_VALUE
    // INTERNET_OPTION_DATAFILE_NAME 
    // INTERNET_OPTION_DATAFILE_EXT 
    // INTERNET_OPTION_DIAGNOSTIC_SOCKET_INFO 
    // INTERNET_OPTION_DIGEST_AUTH_UNLOAD 
    // INTERNET_OPTION_END_BROWSER_SESSION 
    // INTERNET_OPTION_ERROR_MASK 
    // INTERNET_OPTION_EXTENDED_ERROR 
    // INTERNET_OPTION_FROM_CACHE_TIMEOUT 
    // INTERNET_OPTION_HANDLE_TYPE 
    // INTERNET_OPTION_HTTP_DECODING 
    // INTERNET_OPTION_HTTP_VERSION 
    // INTERNET_OPTION_IDN                      IE7
    // INTERNET_OPTION_IGNORE_OFFLINE           IE5
    // INTERNET_OPTION_MAX_CONNS_PER_1_0_SERVER IE5
    // INTERNET_OPTION_MAX_CONNS_PER_SERVER     IE5
    // INTERNET_OPTION_PARENT_HANDLE
    // INTERNET_OPTION_PASSWORD 
    // INTERNET_OPTION_PER_CONNECTION_OPTION 
    // INTERNET_OPTION_PROXY 
    // INTERNET_OPTION_PROXY_PASSWORD 
    // INTERNET_OPTION_PROXY_USERNAME 
    // INTERNET_OPTION_READ_BUFFER_SIZE 
    // INTERNET_OPTION_REFRESH 
    // INTERNET_OPTION_REQUEST_FLAGS 
    // INTERNET_OPTION_REQUEST_PRIORITY 
    // INTERNET_OPTION_RESET_URLCACHE_SESSION 
    // INTERNET_OPTION_SECONDARY_CACHE_KEY 
    // INTERNET_OPTION_SECURITY_CERTIFICATE 
    // INTERNET_OPTION_SECURITY_CERTIFICATE_STRUCT 
    // INTERNET_OPTION_SECURITY_FLAGS 
    // INTERNET_OPTION_SECURITY_KEY_BITNESS 
    // INTERNET_OPTION_SETTINGS_CHANGED 
    // INTERNET_OPTION_URL 
    // INTERNET_OPTION_USER_AGENT 
    // INTERNET_OPTION_USERNAME 
    // INTERNET_OPTION_VERSION 
    // INTERNET_OPTION_WRITE_BUFFER_SIZE 

private:
    // denied
    CInetConnection(CInetConnection& h);
    explicit CInetConnection(HANDLE h);
    CInetConnection& operator=(CInetConnection& h);

};




inline CInetConnection::CInetConnection()
{
}

inline CInetConnection::~CInetConnection()
{
}

inline HRESULT CInetConnection::SetConnectTimeOut(DWORD  dwMilliSeconds)
{
    return DoSetOptionDWORD(INTERNET_OPTION_CONNECT_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CInetConnection::GetConnectTimeOut(DWORD& dwMilliSeconds)
{
    return DoGetOptionDWORD(INTERNET_OPTION_CONNECT_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CInetConnection::SetReceiveTimeOut(DWORD  dwMilliSeconds)
{
    return DoSetOptionDWORD(INTERNET_OPTION_RECEIVE_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CInetConnection::GetReceiveTimeOut(DWORD& dwMilliSeconds)
{
    return DoGetOptionDWORD(INTERNET_OPTION_RECEIVE_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CInetConnection::SetSendTimeOut(DWORD  dwMilliSeconds)
{
    return DoSetOptionDWORD(INTERNET_OPTION_SEND_TIMEOUT, dwMilliSeconds);
}

inline HRESULT CInetConnection::GetSendTimeOut(DWORD& dwMilliSeconds)
{
    return DoGetOptionDWORD(INTERNET_OPTION_SEND_TIMEOUT, dwMilliSeconds);
}

NS_WINMOD_END

#endif//WINMODINETCONNECTION_H
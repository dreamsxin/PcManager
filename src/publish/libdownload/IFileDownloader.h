/***************************************************************
 * @date:   2007-10-26
 * @author: BrucePeng
 * @brief:  Define the interface for file downloader
 *          Now, Only support HTTP protocol

 * @modify: 2007-11-16 Add interface "IMiniFileDownloader"
 *          2007-11-22 Add a lot of comments
 *          2007-12-10 Remove interface "IMiniFileDownloader"
 */

#ifndef I_FILE_Downloader_H_
#define I_FILE_Downloader_H_

#include <Unknwn.h>

class IDownloadCallback;

interface __declspec(uuid("0BBF42C3-0873-48ea-8E89-3929A9E5B6FD"))
IFileDownloader : public IUnknown
{
    /**
     * @brief Tell Downloader how to use proxy
     *
     * @param[IN]   nProxyMethod    The method of using proxy
     *                              see: ENUM_PROXY_METHOD
     * @note
     * It's optional.
     * 
     * If the Downloader is not be told, the default will be used
     * The default: enumPROXY_METHOD_DIRECT, it means, no proxy
     *
     * If enumPROXY_METHOD_CUSTOM is set, you must call function
     * "SetProxyInfo" to tell Downloader the proxy information
     *
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall SetProxyMethod(
        IN  int     nProxyMethod
    ) = 0;

    /**
     * @brief Tell Downloader the custom proxy information
     *
     * @param[IN]   nProxyType      The type of proxy used
     *                              see: ENUM_PROXY_TYPE
     * @param[IN]   pszProxyAddr    The address of proxy server
     * @param[IN]   usProxyPort     The port of proxy server
     * @param[IN]   pszUsername     The username for validating
     *                              Default: NULL
     * @param[IN]   pszPassword     The password for validating
     *                              Default: NULL
     *
     * @note
     * It's optional. 
     *
     * Attention please:!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     * If Set failed, The old proxy information will lost.
     * It's will be set to default value
     *
     * The password must not be null If user name is not null
     *
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall SetProxyInfo(
        IN  int             nProxyType,
        IN  const char*     pszProxyAddr,
        IN  unsigned short  usProxyPort,
        IN  const char*     pszUsername     = NULL,
        IN  const char*     pszPassword     = NULL
    ) = 0;

    /**
     * @brief Tell Downloader the callback interface
     *
     * The callback interface will be used for 
     * telling client the state of downloading
     *
     * @param[IN]   pCallback   The callback interface
     *
     * @note
     * It's optional. 
     * If the Downloader is not be told, the default will be used.
     * The default: NULL, never callback
     *
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall SetCallback(
        IN  IDownloadCallback* pCallback
    )= 0;

    /**
     * @brief Tell Downloader the "connect timeout" (seconds)
     * The timeout will be used while connecting to the server
     *
     * @param[IN}   nTimeout    The timeout
     * 
     * @note
     * It's optional. 
     * If the Downloader is not be told, the default will be used.
     * The default: 10 seconds
     *
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall SetConnectTimeout(
        IN  unsigned long   ulTimeout
    ) = 0;

    /**
     * @brief Tell Downloader the "send timeout" (seconds)
     * The timeout will be used while sending data
     *
     * @param[IN}   nTimeout    The timeout
     * 
     * @note
     * It's optional. 
     * If the Downloader is not be told, the default will be used.
     * The default: 10 seconds
     *
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall SetSendTimeout(
        IN  unsigned long   ulTimeout
    ) = 0;

        /**
     * @brief Tell Downloader the "receive timeout" (seconds)
     * The timeout will be used while receiving data
     *
     * @param[IN}   nTimeout    The timeout
     * 
     * @note
     * It's optional. 
     * If the Downloader is not be told, the default will be used.
     * The default: 10 seconds
     *
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall SetRecvTimeout(
        IN  unsigned long     ulTimeout
    ) = 0;

    /**
     * @brief Tell Downloader how many threads to use
     *
     * @param[IN]   nNumber     The number of thread
     *
     * @note
     * It's optional. 
     * If the Downloader is not be told, the default will be used.
     * The default: 1 threads(single)
     * 
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall SetThreadNumber(
        IN  int             nNumber
    ) = 0;

    /**
     * @brief Tell Downloader the min size of a block in bytes
     *
     * @param[IN]   ulSize      The min size
     *
     * @note
     * It's optional.
     * If the Downloader is not be told, the default will be used.
     * The default: 100K
     *
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall SetMinBlockSize(
        IN  unsigned long   ulSize
    ) = 0;

    /**
     * @brief Tell Download the time of trying when download file failed
     *
     * @param[IN]   nTime       The try time
     *
     * @note
     * It's optional.
     * If the Downloader is not be told, the default will be used.
     * The default: 1
     *
     * You can try to download yourself while downloading failed
     * by invoking this method again outside
     * 
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall SetTryTime(
        IN  int             nTime
    ) = 0;

    /**
     * @brief Starts a synchronous download
     *
     * @param[IN]   pszDownloadURL      The URL of the file
     * @param[IN]   pszLocalFileName    The local path file name
     * @param[IN]   bFailIfExists       If true, return fail when local file exists. 
     *                                  If false, overwrite when local file exists 
     * @param[OUT]  ulErrorCode         The error code
     *                                  see: ENUM_DOWNLOAD_ERROR
     *
     * @note
     * The URL must be standard HTTP or FTP address
     * like: http://www.tyfo.com/sd/fing.exe
     *
     * If E_FAIL returned, ulErrorCode is filled with error code
     * You must check it
     *
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall Download(
        IN  const char*     pszDownloadURL,
        IN  const char*     pszLocalFileName,
        IN  bool            bFailIfExists,
        OUT unsigned long&  ulErrorCode
    ) = 0;

    /**
     * @brief Tell Downloader to exit
     *
     * @note
     * This call will return immediately
     *
     * @return If successfully
     * - S_OK       Successful
     *   E_FAIL     Failed
     */
    virtual HRESULT __stdcall NotifyExit(void) = 0;

};

/**
 * @enum ENUM_DOWNLOAD_ERROR
 * Define the error code for downloading file
 *
 * enumDE_SUCCEEDED         --- succeeded
 * enumDE_UNKNOWN           --- unknown error
 * enumDE_NORMAL            --- normal error
 * enumDE_FILE_EXISTS       --- file exists
 * enumDE_QUERY_INFO        --- query file information failed
 * enumDE_CREATE_CONFIG_FILE--- create config-file failed
 * enumDE_CREATE_LOCAL_FILE --- create local file failed
 * enumDE_QUERY_IE_PROXY    --- query IE's proxy failed
 */
typedef enum ENUM_DOWNLOAD_ERROR
{
    enumDE_SUCCEEDED = 0,
    enumDE_UNKNOWN,
    enumDE_NORMAL,
    enumDE_FILE_EXISTS,
    enumDE_QUERY_INFO,
    enumDE_CREATE_CONFIG_FILE,
    enumDE_CREATE_LOCAL_FILE,
    enumDE_QUERY_IE_PROXY,
    enumDE_PROTOCOL_NOT_SUPPORT
}FDECode;

/**
 * @enum  ENUM_DOWNLOADING_STATE
 * @brief Enumerate the state of downloading
 *
 * enumDS_READY             --- ready, not started
 * enumDS_QUERY_INFO        --- query information.
 *                              For example, the size of file
 * enumDS_CREATE_LOCAL_FILE --- create local file
 * enumDS_DOWNLOADING       --- be downloading
 * enumDS_SUCCEEDED         --- complete downloading successfully
 * enumDS_FAILED            --- download failed
 * enumDS_USER_STOP         --- user stop
 */
typedef enum ENUM_DOWNLOADING_STATE
{
    enumDS_READY = 0,
    enumDS_QUERYING_INFO,
    enumDS_CREATING_LOCAL_FILE,
    enumDS_DOWNLOADING,
    enumDS_USER_STOP,
    enumDS_SUCCESS,
    enumDS_FAILED
}FDSCode;

/***************************************************************
 * Interface "IDownloadCallback" is defined here.
 */
class IDownloadCallback
{
public:
    //The default constructor
    IDownloadCallback(void){}

    //The default destructor
    virtual ~IDownloadCallback(void){}

public:
    /**
     * @brief Notify the client the state of downloading
     *
     * @param[IN]   nState          The state of downloading
     *                              See: ENUM_DOWNLOADING_STATE
     * @return  None
     */
    virtual void __stdcall OnNotifyDownloadState(
        IN  FDSCode             nState
    ) = 0;

    /**
     * @brief Notify the client the progress of downloading
     *
     * @param[IN]   ulFileSize      The total size of file
     * @param[IN]   ulDownloadSize  The size that have been downloaded
     * @param[IN]   nSpeed          The speed(bytes/s)
     * @param[IN]   nThreadNumber   The number of total threads
     * @param[IN]   nThreadRunning  The number of threads running
     * @param[IN]   pszDescription  the Description of download state
     *
     * @return  None
     */
    virtual void __stdcall OnNotifyDownloadProgress(
        IN  unsigned long       ulFileSize,
        IN  unsigned long       ulDownloadSize,
        IN  int                 nSpeed,
        IN  int                 nThreadNumber,
        IN  int                 nThreadRunning,
        IN  const char*         pszDescription
    ) = 0;

    /**
     * @brief Notify the client the error while downloading
     *
     * @param[IN]   nError          The error code
     *
     * @return None
     */
    virtual void __stdcall OnNotifyDownloadError(
        IN  FDECode             nError
    ) = 0;

    /**
     * @brief Ask client whether exiting is needed
     * 
     * @note
     * Not use any more
     *
     * !!!!!!!!!!!!!!!!Pay Attention!!!!!!!!!!!!!!!!!!!!
     * You have to implement this function, but do nothing
     *
     * @return If exiting needed
     * - true   yes
     *   false  no
     */
    virtual bool __stdcall OnQueryExitNeeded(void) = 0;

private:
    //Make the copy constructor and "operator=" unable
    IDownloadCallback(const IDownloadCallback&);
    IDownloadCallback& operator=(const IDownloadCallback&);
};

/**
 * @enum ENUM_PROTOCOL_TYPE
 * Define the type of protocol
 *
 * enumPT_TYPE_UNKNOWN      ------  Unknown
 * enumPT_TYPE_HTTP         ------  HTTP
 * enumPT_TYPE_FTP          ------  FTP(Not support now)
 */
enum ENUM_PROTOCOL_TYPE
{
    enumPT_TYPE_UNKNOWN = 0,
    enumPT_TYPE_HTTP,
    enumPT_TYPE_FTP
};

/**
 * @enum ENUM_PROXY_TYPE
 * Define the type of proxy
 *
 * enumPROXY_TYPE_UNKNOWN   ------  unknown
 * enumPROXY_TYPE_SOCK4     ------  sock4
 * enumPROXY_TYPE_SOCK5     ------  sock5
 * enumPROXY_TYPE_HTTP      ------  HTTP
 */
enum ENUM_PROXY_TYPE
{
    enumPROXY_TYPE_UNKNOWN = 0,
    enumPROXY_TYPE_SOCK4,
    enumPROXY_TYPE_SOCK5,
    enumPROXY_TYPE_HTTP
};

/**
 * @enum ENUM_PROXY_METHOD
 * Define How to use proxy, we call it "method"
 *
 * enumPROXY_METHOD_UNKNOWN ------  unknown
 * enumPROXY_METHOD_DIRECT  ------  direct, no proxy
 * enumPROXY_METHOD_USE_IE  ------  same to IE
 * enumPROXY_METHOD_CUSTOM  ------  custom proxy
 */
enum ENUM_PROXY_METHOD
{
    enumPROXY_METHOD_UNKNOWN = 0,
    enumPROXY_METHOD_DIRECT,
    enumPROXY_METHOD_USE_IE,
    enumPROXY_METHOD_CUSTOM
};

//Define the GUID of interface "IFileDownloader"
static const GUID GUID_IFILEDOWNLOADER = 
    {0xbbf42c3, 0x873, 0x48ea, {0x8e, 0x89, 0x39, 0x29, 0xa9, 0xe5, 0xb6, 0xfd}};


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a FileDownloader instance.
 *
 * @param[OUT]  ppFileDownloader    The pointer to a "CFileDownloader" Object 
 *
 * @note
 * Two attentions here.
 *
 * @par
 * You will only use this when you want to use FileDownloader as static library
 * 
 * @par
 * You will lose the reference when use this function 
 * If the pointer has point to an instance
 *
 * @return If successfully
 * - true       yes
 *   false      no
 */
bool CreateFileDownloaderFromStaticLib(OUT IFileDownloader** ppFileDownloader);

#ifdef __cplusplus
}
#endif

#endif//I_FILE_Downloader_H_


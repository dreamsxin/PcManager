/**
* @file    bkquerier.h
* @brief   ...
* @author  罗鹏
* @date    2010-7-12 14:42
*/

#ifndef BKFILEQUERIER_H
#define BKFILEQUERIER_H

#include <unknwn.h>
#include "skylarkbase.h"
#include "bkengdef.h"
#include "bklevel.h"

#define defBK_QUERY_SEC_STATUS_WHITE    (0x01)
#define defBK_QUERY_SEC_STATUS_BLACK    (0x02)
#define defBK_QUERY_SEC_STATUS_GRAY     (0x03)

/// 查询结果
struct BKQUERIER_RESULT
{
	UINT32      uSize;              ///< 结构体大小
	// ------------------------------------------------------------
	// Version[1, ?]

    UINT32      uSecStatus;         ///< 文件属性：黑、白、灰
	WCHAR       szVirusName[64];    ///< 病毒名【黑的情况下使用】
	BOOLEAN     bNeedUpload;        ///< 文件需要上传
	HRESULT     hrLastError;        ///< 最后一次操作的错误码
};

/**
* @brief 云端的查询接口
*/
class __declspec(uuid("B1B4171F-3E5C-4a68-98FA-56FDCC4B4028"))
IBKFileQuerier : public IUnknown
{
public:
	/**
	* @brief        初始化云端鉴定器
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Initialize() = 0;


	/**
	* @brief        反初始化云端鉴定器
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Uninitialize() = 0;

	/**
	* @brief        中止所有的调用
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Cancel() = 0;

	/**
	* @brief        查询云端得到结果
	* @param[in]    lpszFilePath    原始文件路径
	* @param[out]    result          当前文件的查询结果
	* @param[in]    plTimeMax       超时值
	* @return       非0为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Query(
		LPCWSTR             lpszFilePath,
		BKQUERIER_RESULT&   result,
		LONG* plTimeMax
		) = 0;
};




/**
* @brief 云端的查询接口回调
*/
class IBKMultiFileQuerierCallback
{
public:
    virtual HRESULT STDMETHODCALLTYPE OnBKFileQuery(
        LPCWSTR             lpszFilePath,
        BKQUERIER_RESULT&   result,
        void*               pvFlag) = 0;
};

/**
* @brief 云端的查询接口
*/
class __declspec(uuid("3E8AC0CA-DC1D-4e8c-B2C3-CF8092B3598D"))
IBKMultiFileQuerier : public IUnknown
{
public:
	/**
	* @brief        初始化云端鉴定器
	* @return       FAILED()为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Initialize() = 0;


	/**
	* @brief        反初始化云端鉴定器
	* @return       FAILED()为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Uninitialize() = 0;

	/**
	* @brief        中止所有的调用
	* @return       FAILED()为失败
	*/
	virtual HRESULT STDMETHODCALLTYPE Cancel() = 0;

    /**
	* @brief        发起扫描
	*/
	virtual HRESULT STDMETHODCALLTYPE StartScan(IBKMultiFileQuerierCallback* piCallback) = 0;

    /**
	* @brief        等待扫描结束
	*/
    virtual HRESULT STDMETHODCALLTYPE AppendFile(
        LPCWSTR lpszFilePath,
        void*   pvFlag = NULL) = 0;

    /**
	* @brief        提交并等待引擎返回所有结果
	*/
	virtual DWORD   STDMETHODCALLTYPE WaitForFinish(DWORD dwWaitTime = INFINITE) = 0;

};

#endif // BKFILEQUERIER_H
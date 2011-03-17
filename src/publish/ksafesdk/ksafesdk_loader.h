/**
* @file    ksafesdk_loader.h
* @brief   ...
* @author  bbcallen
* @date    2010-08-06 11:34
*/

#ifndef KSAFESDK_LOADER_H
#define KSAFESDK_LOADER_H

#include <windows.h>
#include <objidl.h>
#include "ksafesdk_def.h"

//////////////////////////////////////////////////////////////////////////////

#ifndef	KSAFESDK_API
#define	KSAFESDK_API __declspec(dllimport)
#endif

typedef KSAFESDK_API HRESULT (__stdcall* PFN_ksafeInitialize)(
    /*[in]*/  PFN_ksafeCallbackW    pCallback);         ///< 回调接口
typedef KSAFESDK_API HRESULT (__stdcall* PFN_ksafeUninitialize)();
typedef KSAFESDK_API HRESULT (__stdcall* PFN_ksafeScanFileW)(
    /*[in]*/  const wchar_t*        pScanFileName,      ///< 要扫描的文件路径
    /*[in]*/  unsigned long         dwScanningMode,     ///< 扫描模式,取值为 KSAFE_S_M_... 的组合
    /*[in]*/  unsigned long         dwCleaningMode,     ///< 保留,未使用
    /*[in]*/  void*                 pUserContext,       ///< 回调参数
    /*[out]*/ KSAFE_RESULT*         pResult);           ///< 扫描结果
typedef KSAFESDK_API HRESULT (__stdcall* PFN_ksafeFlushAsyncScan)();

//////////////////////////////////////////////////////////////////////////

extern PFN_ksafeInitialize      ksafeInitialize;
extern PFN_ksafeUninitialize    ksafeUninitialize;
extern PFN_ksafeScanFileW       ksafeScanFileW;
extern PFN_ksafeFlushAsyncScan  ksafeFlushAsyncScan;

//////////////////////////////////////////////////////////////////////////////

HRESULT ksafeLoadW(const wchar_t* pLoadFolder);
HRESULT ksafeUnload();

//////////////////////////////////////////////////////////////////////////////

#endif//KSAFESDK_LOADER_H
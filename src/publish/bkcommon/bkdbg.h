/**
* @file    bkdbg.h
* @brief   ...
* @author  bbcallen
* @date    2009-12-13 10:30
*/

#ifndef BKDBG_H
#define BKDBG_H

#include <atlpath.h>

#define BEIKE_DBG_INI_FILE          L"beike.local.ini"  ///< beike 的应用程序配置文件名
#define BEIKE_DBG_INI_APP_DEBUG     L"debug"            ///< beike ini section


// 高于 BKInitDebugOutput 设置 lvl 的日志都被屏蔽
// 低于或等于 lvl 的日志会被输出
#define BKDBG_LVL_DEFAULT       0xFFFFFFFF      ///< 仅用来初始化设置
#define BKDBG_LVL_ALWAYS        0               ///< ALWAYS
#define BKDBG_LVL_CRITICAL      10              ///< CRITICAL
#define BKDBG_LVL_ERROR         20              ///< ERROR
#define BKDBG_LVL_WARNING       30              ///< WARNING
#define BKDBG_LVL_INFO          40              ///< INFO
#define BKDBG_LVL_DEBUG         50              ///< DEBUG
#define BKDBG_LVL_TRACE         60              ///< Trace






EXTERN_C BOOL WINAPI BKInitDebugOutput(DWORD dwDbgMaxLevel = BKDBG_LVL_DEFAULT);
EXTERN_C BOOL WINAPI BKIsDebugOutputEnabled(DWORD dwDbgLevel);
EXTERN_C void WINAPI BKDebugEcho(DWORD dwDbgLevel, LPCWSTR lpszFormat);
EXTERN_C void WINAPI BKDebugOutput(DWORD dwDbgLevel, LPCWSTR lpszFormat, ...);
EXTERN_C void WINAPI BKDebugOutputV(DWORD dwDbgLevel, LPCWSTR lpszFormat, va_list args);


#endif//BKDBG_H
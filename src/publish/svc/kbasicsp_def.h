///////////////////////////////////////////////////////////////////////////////
///		@file		KBasicSP_def.h
///		@author		yxdh
///		@date		2009-7-10 10:27:58
///		@brief		
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "include/kxe_serialize.h"


/**
* @brief 定义服务提供者ID
*/
typedef enum _EM_SP_ID
{
	enum_QuickScan_SP = 0,				///< 快速扫描服务提供者
	enum_DiskScan_SP	,				///< 全盘扫描服务提供者
    enum_ARP_SP
} EM_SP_ID;

KXE_JSON_DDX_ENUM(EM_SP_ID)


/**
* @brief SP的状态
*/
typedef enum _EM_SP_STATUS
{
    em_SP_Unknown       = -2,
    em_SP_Not_Ready     = -1,
	em_SP_Ready         = 0,			///< SP就绪
	em_SP_Downloading   = 1				///< 正在下载...
} EM_SP_STATUS;

KXE_JSON_DDX_ENUM(EM_SP_STATUS);


/**
 * 创建进程的参数
 */
typedef struct _KSAFE_CREATE_PROCESS_PARAMETER
{
	DWORD        dwSourcePID;      /// 调用者的进程ID，用于复制
	std::wstring strCommandLine;   /// 调用的命令行
	DWORD        dwCreateFlags;    /// 创建进程的参数，始终会带有CREATE_UNICODE_ENVIRONMENT
	BOOL         bDisalbeX64Redirection;  // 是否禁用X64的文件重定向
} KSAFE_CREATE_PROCESS_PARAMETER;

KXE_JSON_DDX_BEGIN(KSAFE_CREATE_PROCESS_PARAMETER)
    KXE_JSON_DDX_MEMBER("SourcePID", dwSourcePID)
	KXE_JSON_DDX_MEMBER("CommandLine", strCommandLine)
	KXE_JSON_DDX_MEMBER("CreateFlags", dwCreateFlags)
	KXE_JSON_DDX_MEMBER("DisalbeX64Redirection", bDisalbeX64Redirection)
KXE_JSON_DDX_END()
//=================================================================
/**
* @file ksautoinstdef.h
* @brief
* @author zhengyousheng <zhengyousheng@kingsoft.com>
* @data 2010-11-04 12:52
*/
//=================================================================

#pragma once

#include <windows.h>

///< 安装状态
typedef enum _EM_KSAI_INSTALL_STATUS
{
	em_Status_Unknown = 0,           ///< 未知状态
	em_Status_CheckXmlBufSizeFail,   ///< 检查xmlbuffersize失败
	em_Status_ParseXmlBufferFail,    ///< 解析xmlbuffer失败
	em_Status_CreateDesktopFail,     ///< 创建虚拟桌面失败
	em_Status_CreateSvrProcFail,     ///< 创建服务端进程失败
	em_Status_SetupTypError,         ///< 安装类型错误, xml中1表示静默安装,2表示GUI安装,其它值的话在After回调中会返回此错误
	em_Status_StartInstProcFail,     ///< 启动安装进程失败
	em_Status_Installing,            ///< 正在安装
	em_Status_UserStop,              ///< 安装过程中用户停止
	em_Status_EnumWndFail,           ///< 安装过程中枚举不到指定的窗口
	em_Status_Timeout,               ///< 安装超时
	em_Status_SvrProcExit,           ///< 安装过程中服务端进程退出
	em_Status_Complete               ///< 安装完成
}EM_KSAI_INSTALL_STATUS;

///< 软件基本信息
typedef struct _S_KSAI_SOFT_BASE 
{
	DWORD dwSoftID;                ///< 软件ID
	_S_KSAI_SOFT_BASE()
	{
		dwSoftID = 0;
	};
}S_KSAI_SOFT_BASE;

///< 安装过程回调出来的进度信息
typedef struct _S_KSAI_STATUS 
{
	S_KSAI_SOFT_BASE struSoftBase; ///< 软件基本信息
	DWORD dwCurProgressCount;      ///< 当前进度数据
	DWORD dwTotalProgressCount;    ///< 总的进度数据
	DWORD dwInstallStatus;         ///< 安装状态,见EM_KSAI_INSTALL_STATUS

	_S_KSAI_STATUS()
	{
		dwTotalProgressCount = 0;
		dwCurProgressCount   = 0;
		dwInstallStatus      = 0;
	}
}S_KSAI_STATUS;

///< 安装前的回调数据
typedef struct _S_KSAI_SOFT_INFO
{
	S_KSAI_SOFT_BASE struSoftBase;  ///< 软件基本信息
}S_KSAI_SOFT_INFO;

///< 安装后的回调数据
typedef struct _S_KSAI_INST_RESULT
{
	S_KSAI_STATUS struStatus;       ///< 软件最后的进度信息
}S_KSAI_INST_RESULT;
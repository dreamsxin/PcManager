#pragma once
#include "kbasicsp_def.h"

class KBasicSPWrapperClient
{
public:
	KBasicSPWrapperClient(void);
	~KBasicSPWrapperClient(void);

	/**
	* @brief        启动SP
	* @return		0 成功, 其他为失败错误码，已经加载的返回E_KXEBASIC_SERVICE_PROVIDER_STARTED
	*/
	int __stdcall StartSP(
		/* [in] */ const EM_SP_ID&		SPID,
		/* [out]*/ KXE_NULL_PARAEMETER&
		);

	/**
	* @brief        关闭SP
	* @return		0 成功, 其他为失败错误码
	*/
	int __stdcall StopSP(
		/* [in] */ const EM_SP_ID&		SPID,
		/* [out]*/ KXE_NULL_PARAEMETER&
		);

	/**
	* @brief        查询SP状态
	* @return		0 成功, 其他为失败错误码
	*/
	int __stdcall QuerySPStatus(
		/* [in] */ const EM_SP_ID&		SPID,
		/* [out]*/ EM_SP_STATUS&		Status
		);

	/**
	* @brief 使用普通用户权限启动进程
	* @param[in] parameter 创建进程的参数
	* @param[out] dwProcessHandle 返回的进程句柄，将复制到指定PID内，由于不能序列化HANDLE，所以这里写成DWORD
	* @return 0 成功，其他为失败错误码
	*/
	int __stdcall CreateProcessAsNormalUser(
		const KSAFE_CREATE_PROCESS_PARAMETER& parameter, 
		DWORD& dwProcessHandle
		);

};

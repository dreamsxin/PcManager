
#pragma once
//=============================================================================
/**
* @file kexectrlloader.h
* @brief 
* @author qiuruifeng <qiuruifeng@kingsoft.com>
* @date 2010-8-18   1:32
*/
//=============================================================================
#include "kexectrl_def.h"

#define		KSM_SERVICE_MULTI_EXE_LOADER			_T("kexectrl.dll")


class KExeCtrlLoader
{
public:
	KExeCtrlLoader()
	{
		m_hModule					= NULL;
		m_pfnAddExecItemOnServer	= NULL;
		m_pfnRemoveExecItemOnServer	= NULL;
		m_pfnStartOnServer			= NULL;
		m_pfnStopOnServer			= NULL;
		m_pfnNotifyExecItemToServer = NULL;
	}
	~KExeCtrlLoader()
	{
		UnInit();
	}

	int Init(const wchar_t* pszModulePath)
	{
		if (NULL == pszModulePath)
			return -1;

		if( !m_hModule )
		{
			m_hModule = LoadLibraryW(pszModulePath);
			if (NULL == m_hModule)
				return -1;

			m_pfnAddExecItemOnServer	= (pfnAddExecItemOnServerType)::GetProcAddress(m_hModule, "AddExecItemOnServer");
			m_pfnRemoveExecItemOnServer	= (pfnRemoveExecItemOnServerType)::GetProcAddress(m_hModule, "RemoveExecItemOnServer");
			m_pfnStartOnServer			= (pfnStartOnServerType)::GetProcAddress(m_hModule, "StartOnServer");
			m_pfnStopOnServer			= (pfnStopOnServerType)::GetProcAddress(m_hModule, "StopOnServer");
			m_pfnNotifyExecItemToServer	= (pfnNotifyExecItemToServerType)::GetProcAddress(m_hModule, "NotifyExecItemToServer");
		}

		return 0;
	}

	int UnInit(void)
	{
		if (m_hModule)
		{
			FreeLibrary(m_hModule);
			m_hModule					= NULL;
			m_pfnAddExecItemOnServer	= NULL;
			m_pfnRemoveExecItemOnServer	= NULL;
			m_pfnStartOnServer			= NULL;
			m_pfnStopOnServer			= NULL;
			m_pfnNotifyExecItemToServer = NULL;
		}
		return 0;
	}

	/**
	* @brief		添加执行对象,一旦收到消息就会启动设置的进程(服务端调用)  
	* @param[in]	ExecItem 执行exe的设置,当pszCmdLine为空或uItemID和已经设置的重复时会失败
	* @return		0 成功,	其他为失败错误码
	*/
	int AddExecItemOnServer(const EXEC_ITEM_INFO* pExecItem)
	{
		return m_pfnAddExecItemOnServer ? m_pfnAddExecItemOnServer(pExecItem) : -1;
	}

	/**
	* @brief		删除执行对象
	* @param[in]	uCtrlID  执行对象的唯一标识
	* @return		0 成功,	其他为失败错误码
	*/
	int RemoveExecItemOnServer(unsigned int uItemID)
	{
		return m_pfnRemoveExecItemOnServer ? m_pfnRemoveExecItemOnServer(uItemID) : -1;
	}

	/**
	* @brief		开始监听(服务端调用) 
	* @return		0 成功,	其他为失败错误码
	*/
	int StartOnServer(void)
	{
		return m_pfnStartOnServer ? m_pfnStartOnServer() : -1;
	}

	/**
	* @brief		停止监听(服务端调用) 
	* @return		0 成功,	其他为失败错误码
	*/
	int StopOnServer(void)
	{
		return m_pfnStopOnServer ? m_pfnStopOnServer() : -1;
	}

	/**
	* @brief		通知服务端动扫描后台进程(客户端调用) 
	* @return		0 成功,	其他为失败错误码
	*/
	int NotifyExecItemToServer(unsigned int uItemID)
	{
		return m_pfnNotifyExecItemToServer ? m_pfnNotifyExecItemToServer(uItemID) : -1;
	}

private:
	HMODULE							m_hModule;
	pfnAddExecItemOnServerType		m_pfnAddExecItemOnServer;
	pfnRemoveExecItemOnServerType	m_pfnRemoveExecItemOnServer;
	pfnStartOnServerType			m_pfnStartOnServer;
	pfnStopOnServerType				m_pfnStopOnServer;
	pfnNotifyExecItemToServerType	m_pfnNotifyExecItemToServer;


};
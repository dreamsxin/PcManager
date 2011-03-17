//=============================================================================
/**
* @file kmultiqs_api.h
* @brief 
* @author qiuruifeng <qiuruifeng@kingsoft.com>
* @date 2010-8-14   16:30
*/
//=============================================================================
#pragma once 

extern "C"
{
	typedef struct _EXEC_ITEM_INFO
	{
		unsigned int	uDataSize;	///< sizeof(_EXEC_ITEM_INFO)
		const wchar_t*	pszCmdLine; ///< 执行的命令行参数
		unsigned int	uItemID;	///< 执行对象的唯一标识

	}EXEC_ITEM_INFO;

	/**
	* @brief		添加执行对象,一旦收到消息就会启动设置的进程(服务端调用)  
	* @param[in]	ExecItem 执行exe的设置,当pszCmdLine为空或uItemID和已经设置的重复时会失败
	* @return		0 成功,	其他为失败错误码
	*/
	typedef int (__stdcall *pfnAddExecItemOnServerType)(const EXEC_ITEM_INFO* pExecItem);

	/**
	* @brief		删除执行对象
	* @param[in]	uCtrlID  执行对象的唯一标识
	* @return		0 成功,	其他为失败错误码
	*/
	typedef int (__stdcall *pfnRemoveExecItemOnServerType)(unsigned int uItemID);

	/**
	* @brief		开始监听(服务端调用) 
	* @return		0 成功,	其他为失败错误码
	*/
	typedef int (__stdcall *pfnStartOnServerType)(void);

	/**
	* @brief		停止监听(服务端调用) 
	* @return		0 成功,	其他为失败错误码
	*/
	typedef int (__stdcall *pfnStopOnServerType)(void);
	
	/**
	* @brief		通知服务端动扫描后台进程(客户端调用) 
	* @return		0 成功,	其他为失败错误码
	*/
	typedef int (__stdcall *pfnNotifyExecItemToServerType)(unsigned int uItemID);

};
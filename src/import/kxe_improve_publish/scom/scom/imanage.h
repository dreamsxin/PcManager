#pragma once

#include "wtypes.h"
#include <Unknwn.h>

struct IEventSender;
class CKisRpcManager;
struct IUpdMgr;

MIDL_INTERFACE("A2351875-2EA5-4F43-1236-01BEC5D2DA23")
IManage:public IUnknown
{
	virtual HRESULT __stdcall Init(IEventSender *piEventSender) = 0;
	virtual HRESULT __stdcall Uninit() = 0;

	virtual HRESULT __stdcall BindRpc(CKisRpcManager *pKisRpcManager) = 0;
	virtual HRESULT __stdcall UnbindRpc() = 0;

	//Return:
	//	S_OK	可以处理，已退出指定文件
	//	S_FALSE	可以处理，但需要退出本组件
	//	E_ABORT	不能处理，禁止退出该组件
	//Param:
	//	dwDataType		更新的方式，在index文件中定义
	//	pwsUpdFileList	多个文件名以"|"分开
	virtual HRESULT __stdcall NotifyUpdate(IN DWORD dwDataType, IN const wchar_t* pwsUpdFileList) = 0;

	//Return:
	//	S_OK	可以处理，已退出指定组件
	//	S_FALSE	可以处理，但需要退出本组件
	//	E_ABORT	不能处理，禁止退出该组件
	//Param:
	//	clsUpd		待更新组件的CLSID
	virtual HRESULT __stdcall NotifyUpdate(IN GUID clsUpd) = 0;


	//Return:
	// S_OK 成功重新加载卸载掉的数据或模块
	// E_FAIL 失败
	virtual HRESULT __stdcall CompleteUpdate() = 0;
};

MIDL_INTERFACE("6C1027F9-FB80-41AC-9054-567574B7CFE3")
IManageUpdEx:public IUnknown
{
	virtual HRESULT __stdcall SetUpdManagr(IUpdMgr *piUpdManager) = 0;
};
#pragma once

#include "IManage.h"
#include "Event/IEventSender.h"
#include "KisRpc/kisrpcmanager.h"

template<
	class T0,
	class T1=LokiRpc::NullType,
	class T2=LokiRpc::NullType,  
	class T3=LokiRpc::NullType,
	class T4=LokiRpc::NullType,
	class T5=LokiRpc::NullType,
	class T6=LokiRpc::NullType,
	class T7=LokiRpc::NullType,
	class T8=LokiRpc::NullType,
	class T9=LokiRpc::NullType,
	class T10=LokiRpc::NullType,
	class T11=LokiRpc::NullType,
	class T12=LokiRpc::NullType,
	class T13=LokiRpc::NullType,
	class T14=LokiRpc::NullType,
	class T15=LokiRpc::NullType,
	class T16=LokiRpc::NullType,
	class T17=LokiRpc::NullType,
	class T18=LokiRpc::NullType,
	class T19=LokiRpc::NullType,
	class T20=LokiRpc::NullType
>
class KSManage: public IManage
{
public:
	KSManage():m_piEventSender(NULL), m_pRpcMgr(NULL){};

	virtual ~KSManage(){};

	HRESULT __stdcall Init(IEventSender *piEventSender)
	{
		/* IManage接口的默认初始化顺序是：
		*	1.Init()
		*	2.BindRpc()
		*/
		m_piEventSender = piEventSender;

		return S_OK;
	}

	HRESULT __stdcall Uninit()
	{
		/* IManage接口的默认反初始化顺序是：
		*	1.UnbindRpc()
		*	2.Uninit()
		*/

		return S_OK;
	}

	/* BindRpc()
	* Return:
	*	S_OK	成功绑定
	*	S_FALSE 未传入pKisRpcManager，不需要绑定
	*/
	HRESULT __stdcall BindRpc(CKisRpcManager *pKisRpcManager)
	{
		if (pKisRpcManager)
		{
			if (m_pRpcMgr == NULL)
			{

				pKisRpcManager->BindClass<T1>((T1*)(T0*)this);
				pKisRpcManager->BindClass<T2>((T2*)(T0*)this);
				pKisRpcManager->BindClass<T3>((T3*)(T0*)this);
				pKisRpcManager->BindClass<T4>((T4*)(T0*)this);
				pKisRpcManager->BindClass<T5>((T5*)(T0*)this);
				pKisRpcManager->BindClass<T6>((T6*)(T0*)this);
				pKisRpcManager->BindClass<T7>((T7*)(T0*)this);
				pKisRpcManager->BindClass<T8>((T8*)(T0*)this);
				pKisRpcManager->BindClass<T9>((T9*)(T0*)this);
				pKisRpcManager->BindClass<T10>((T10*)(T0*)this);
				pKisRpcManager->BindClass<T11>((T11*)(T0*)this);
				pKisRpcManager->BindClass<T12>((T12*)(T0*)this);
				pKisRpcManager->BindClass<T13>((T13*)(T0*)this);
				pKisRpcManager->BindClass<T14>((T14*)(T0*)this);
				pKisRpcManager->BindClass<T15>((T15*)(T0*)this);
				pKisRpcManager->BindClass<T16>((T16*)(T0*)this);
				pKisRpcManager->BindClass<T17>((T17*)(T0*)this);
				pKisRpcManager->BindClass<T18>((T18*)(T0*)this);
				pKisRpcManager->BindClass<T19>((T19*)(T0*)this);
				pKisRpcManager->BindClass<T20>((T20*)(T0*)this);


				m_pRpcMgr = pKisRpcManager;
			}

			return S_OK;
		}
		else
		{
			return S_FALSE;
		}

	}
	
	/* UnbindRpc()
	* Return:
	*	S_OK	成功解除绑定
	*	S_FALSE 之前未传入pKisRpcManager进行绑定，所以也不需要解绑定
	*/
	HRESULT __stdcall UnbindRpc()
	{
		if (m_pRpcMgr)
		{
			m_pRpcMgr->UnBindClass<T1>();
			m_pRpcMgr->UnBindClass<T2>();
			m_pRpcMgr->UnBindClass<T3>();
			m_pRpcMgr->UnBindClass<T4>();
			m_pRpcMgr->UnBindClass<T5>();
			m_pRpcMgr->UnBindClass<T6>();
			m_pRpcMgr->UnBindClass<T7>();
			m_pRpcMgr->UnBindClass<T8>();
			m_pRpcMgr->UnBindClass<T9>();
			m_pRpcMgr->UnBindClass<T10>();
			m_pRpcMgr->UnBindClass<T11>();
			m_pRpcMgr->UnBindClass<T12>();
			m_pRpcMgr->UnBindClass<T13>();
			m_pRpcMgr->UnBindClass<T14>();
			m_pRpcMgr->UnBindClass<T15>();
			m_pRpcMgr->UnBindClass<T16>();
			m_pRpcMgr->UnBindClass<T17>();
			m_pRpcMgr->UnBindClass<T18>();
			m_pRpcMgr->UnBindClass<T19>();
			m_pRpcMgr->UnBindClass<T20>();


			m_pRpcMgr = NULL;
			return S_OK;
		}
		else
		{
			return S_FALSE;
		}

	}

	HRESULT __stdcall SetEventNotify(IEventSender *piEventSender)
	{	
		//拿到IEventSender指针后可以用来发起事件
		m_piEventSender = piEventSender;
		return S_OK;
	};


	HRESULT __stdcall NotifyUpdate(IN DWORD dwDataType, IN const wchar_t* pwsUpdFileList)
	{
		// 默认返回S_FALSE，表示允许通过退出自身来释放被占用的指定文件
		return S_FALSE;
	}

	HRESULT __stdcall NotifyUpdate(IN KSCLSID clsUpd)
	{
		// 默认返回S_FALSE，表示允许通过退出自身来释放被占用的指定组件
		return S_FALSE;
	}

	HRESULT __stdcall CompleteUpdate()
	{
		return S_OK;
	}

public:
	IEventSender *m_piEventSender;
	CKisRpcManager *m_pRpcMgr;
};

class KSManageUpdEx: public IManageUpdEx
{
public:
	KSManageUpdEx():m_piUpdManager(NULL){};
	//////////////////////////////////////////////////////////////////////////
	//用于升级模块管理组件的重启
	HRESULT __stdcall SetUpdManagr(IUpdMgr *piUpdManager)
	{
		if (piUpdManager)
		{
			if (m_piUpdManager == NULL)
			{
				m_piUpdManager = piUpdManager;
			}
			return S_OK;
		}

		return E_FAIL;
	}

public:
	IUpdMgr *m_piUpdManager;
};
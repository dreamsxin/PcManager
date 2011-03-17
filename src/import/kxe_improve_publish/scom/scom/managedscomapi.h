/********************************************************************
* CreatedOn: 2005-10-24   11:58
* FileName: SCOMAPI.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef SCOMEXTAPI_H
#define SCOMEXTAPI_H

#include "SCOMDef.h"
#include "SCOMBase.h"
#include "SCOM.h"
#include "ISCOMMalloc.h"
#include "../../kisrpc/rfcdef.h"
#include "kis/kpfw/kpfwsvr/iremotescommgr.h"


/**
	RPC调用和SCOM调用的区别在于：
		RPC调用返回的是RPC代理的指针，这个指针的实体是在调用者内部创建的，所以除了跟COM调用方式相同的AddRef,Release过程之外，在使用完后还应该delete该指针，或者使用智能指针如：auto_ptr<IRpcHello> piHello;
*/
enum SCOM_EX_CREATE_TYPE
{
	SCOM_EX_CREATE_TYPE_RPC = 0,			//通过RPC的方式进行调用
	SCOM_EX_CREATE_TYPE_SCOM				//通过进程内COM的方式进行创建对象用指针调用，不向服务注册本依赖关系(跟直接调用KSCoCreateInstance相比，只是增加了自动完成KSCoInitialize的动作)
};

class SCOMAutoInit
{
public:
	SCOMAutoInit():m_bInited(FALSE){};

	~SCOMAutoInit()
	{
		KSCoUninitialize();
	};

	HRESULT init()
	{
		if (!m_bInited)
		{
			HRESULT hRes = E_FAIL;

			hRes = KSCoInitialize(NULL);

			if (SUCCEEDED(hRes))
				m_bInited = TRUE;

			return hRes;
		}
		
		return S_OK;
	}


	BOOL m_bInited;	
};

//远程创建组件，远程组件需要实现IManage接口，用于普通服务提供者的RPC初始化
template<class I>
HRESULT __stdcall KSCoManagedCreateInstance(
	const KSCLSID& clsid,	//需要创建的组件的ID
	I **ppv,
	const KSCLSID& callerClsid, //调用者本身的组件ID，如果是在服务提供者内部用GetCLSID()获得当前组件，如果是在界面进程内调用可直接提从一个空值KSCLSID()
	SCOM_EX_CREATE_TYPE createtype = SCOM_EX_CREATE_TYPE_RPC
	)
{
	
	HRESULT hRes = E_FAIL;

	static PipeEndpoint _endpoint((PTCHAR)KRpcName());
	static RcfClient<IRemoteSCOMMgr> rcSCOMMgr(_endpoint);
	static IRemoteSCOMMgr *piRemoteSCOMMgr = &rcSCOMMgr;
	static BOOL s_bLocalSCOMInited = FALSE;
	static DWORD s_dwCurPID = GetCurrentProcessId();

	if (createtype == SCOM_EX_CREATE_TYPE_RPC)
	{
		hRes = piRemoteSCOMMgr->CreateRemoteInstance(clsid, callerClsid, s_dwCurPID);
		if (SUCCEEDED(hRes))
		{
			RcfClient<I> *pClient = new RcfClient<I>(_endpoint);	

			*ppv = (I*)pClient;

			hRes = S_OK;

			goto Exit0;
		}
	}
	else if (createtype == SCOM_EX_CREATE_TYPE_SCOM)
	{//如果需要在进程内创建,先确保SCOM已初始化

		if (!s_bLocalSCOMInited)
		{
			static SCOMAutoInit scomAI;
			hRes = scomAI.init();

			if (SUCCEEDED(hRes))
				s_bLocalSCOMInited = TRUE;
			else
				goto Exit0;
		}

		//进程内创建SCOM组件
		hRes = KSCoCreateInstance(clsid, __uuidof(I), (void**)ppv);
	}

Exit0:
	return hRes;
}


//远程创建组件，远程组件需要实现IManage接口，用于普通服务提供者的RPC初始化
template<class I>
HRESULT __stdcall KSCoManagedCreateInstance(
	const KSCLSID& clsid,	//需要创建的组件的ID
	auto_ptr<I>& rsp,
	const KSCLSID& callerClsid, //调用者本身的组件ID，如果是在服务提供者内部用GetCLSID()获得当前组件，如果是在界面进程内调用可直接提从一个空值KSCLSID()
	SCOM_EX_CREATE_TYPE createtype = SCOM_EX_CREATE_TYPE_RPC
	)
{
	HRESULT hResult = E_FAIL;
	I* pi = NULL;

	//暂时只支持RPC的调用方式
	if (createtype != SCOM_EX_CREATE_TYPE_RPC)
		return E_NOTIMPL;

	hResult = KSCoManagedCreateInstance(clsid, &pi, callerClsid, createtype);
	rsp.reset(pi);

	return hResult;
}


#endif //SCOMEXTAPI_H
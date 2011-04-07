
#pragma once
#include <atlstr.h>

#ifndef VIRTUAL_ENG
#define VIRTUAL_ENG virtual
#endif

#define VERSION_PLUG_INTERFACE_LOW				3
#define VERSION_PLUG_INTERFACE_HIGH				7

//////////////////////////////////////////////////////////////////////////
/// 返回结果定义
#define S_FAILED_CANCEL						0x80000001
#define S_FAILED_LIB_NO_EXIST				0x80000002
#define S_FAILED_LIB_ERROR					0x80000003
#define S_FALIED_LIB_LOCK					0x80000004
#define S_FAILED_ALREADY_RUN				0x80000005
#define S_FAILED_LIB_NO_ITEM				0x80000006
#define S_FAILED_RES_NO_ITEM				0x80000007
#define S_FAILED_RES_MORE_DATA				0x80000008
#define S_FAILED_RES_INVALID_PARAM			0x80000009
#define S_FAILED_REMOVE_RES_RESTART			0x8000000a
#define S_FAILED_INVALID_PARAM				0x8000000b
#define S_FAILED_NO_FIND					0x8000000c
#define S_FAILED_REMOVE_RES_KILLER			0x8000000d
#define S_FAILED_SET_PLUG_TRUST				0x8000000e
//////////////////////////////////////////////////////////////////////////

class IEngineCallback
{
public:

	// 扫描开始
	VIRTUAL_ENG HRESULT		ScanStart() = 0;

	// 扫描完成，只有收到这个回调的时候才表示扫描完成
	VIRTUAL_ENG HRESULT		ScanComplete(BOOL bCancel) = 0;

	// 开始扫描某个插件
	VIRTUAL_ENG HRESULT		ScanItemStart(DWORD nIndex, DWORD nTotal, LPCTSTR lpName) = 0;

	// 扫描到了某个插件，外围需要把ID存起来去query具体信息
	VIRTUAL_ENG HRESULT		FindItemExist(DWORD nItemId) = 0;

	// 扫描某个插件已经完成
	VIRTUAL_ENG HRESULT		ScanItemComplete(DWORD nItem) = 0;

	// 快扫开始
	VIRTUAL_ENG HRESULT		QuickScanStart() = 0;

	// 快扫完成
	VIRTUAL_ENG HRESULT		QuickScanComplete() = 0;

	// 开始清理某个插件
	VIRTUAL_ENG HRESULT		RemoveItemStart(DWORD nItemId, DWORD nRate) = 0;

	// 清理某个插件结束
	VIRTUAL_ENG HRESULT		RemoveItemEnd(DWORD nItemId, HRESULT hRes ) = 0;

	// 清理过程结束
	VIRTUAL_ENG	HRESULT		RemoveComplete(BOOL bNeedRestart) = 0;
	
	// 以下暂时不需要处理
	VIRTUAL_ENG HRESULT		UnkItemChange(DWORD nItemId) = 0;
	VIRTUAL_ENG HRESULT		LibLoadFailed() = 0;
};

struct PLUG_DAT_INFO
{
	DWORD		nItemId;
	DWORD		nLevel;
	LPCTSTR		lpName;
	LPCTSTR		lpCompany;
	LPCTSTR		lpstrBehavior;
	LPCTSTR		lpstrDesc;
};


struct PLUG_SCAN_RESULT 
{
	DWORD		nItemId;		// 插件的唯一标示ID
	DWORD		nLevel;			// 插件的级别  大于3表示危险
	BOOL		bActive;		// 是否活跃，现在不使用
	BOOL		bUnknownID;		// 是否是未知插件
	BOOL		bDelRestart;	// 是否之前已经清除过，需要重启后删除
	LPCTSTR		lpName;			// 插件名字
	LPCTSTR		lpCompany;		// 插件出品公司
	LPCTSTR		lpstrType;		// 类型描述
	LPCTSTR		lpFirstFile;	// 扫到的第一个文件
	LPCTSTR		lpBehavior;		// 插件行为
	LPCTSTR		lpKiller;		// 是否需要专杀，专杀的下载地址
	DWORD		nFileSize;		// 扫到的文件个数
	LPCTSTR*	pFileBuffer;	// 扫到的文件buffer指针
	DWORD		nRegSize;		// 扫到的注册表个数
	LPCTSTR*	pRegBuffer;		// 扫到的注册表buffer指针

	//接口为4的时候添加的
	BOOL		bTrust;			// 是否是信任插件
	LPCTSTR		lpCLSID;		// 未知插件的CLSID
};

typedef struct _LogRemovePlug
{
	DWORD		nItemId;
	LPCTSTR		lpstrName;
	LPCTSTR		lpstrTime;
	PVOID		pBuffer;		// 这个东西保留的是 CString 的指针
}LOG_PLUG_REMOVE, *PLOG_PLUG_REMOVE;

class IDelOpCallback
{
public:
	VIRTUAL_ENG BOOL DeleteFile( LPCTSTR lpFileName ) = 0;
	VIRTUAL_ENG BOOL DeleteFolder( LPCTSTR lpDirName ) = 0;
	VIRTUAL_ENG BOOL DeleteKey(HKEY hRoot, LPCTSTR lpKey ) = 0;
	VIRTUAL_ENG BOOL DeleteKeyValue(HKEY hRoot, LPCTSTR lpKey, LPCTSTR lpValue ) = 0;
};

class IPlugEng
{
public:
	//设置回调，在扫描的过程中进行控制和状态设置
	VIRTUAL_ENG HRESULT	SetCallBack(IEngineCallback* callback, LPVOID Param) = 0;

	//如果不使用默认的库，则需要设置路径
	VIRTUAL_ENG HRESULT	SetLibPath(LPCTSTR lpLibPath=NULL) = 0;	

	//快速扫描获取插件个数，恶意插件个数，所有的Scan，Remove动作不允许同时存在，只能一个一个来
	VIRTUAL_ENG HRESULT	QuickScan(BOOL bOnlyDanger=FALSE, BOOL bSyn = FALSE) = 0;

	//完全扫描，先用库规则扫描，然后扩展扫描系统插件
	VIRTUAL_ENG HRESULT	ScanAll(BOOL bSyn=FALSE) = 0;

	//部分扫描，提供一个ID数组
	VIRTUAL_ENG HRESULT	ScanArray(DWORD nNumber, DWORD* pIDBuffer,BOOL bSyn=FALSE) = 0;

	//停止扫描，在回调中会收到停止扫描的信息
	VIRTUAL_ENG HRESULT	StopScan() = 0;

	//根据插件ID来获取插件的详细信息
	VIRTUAL_ENG HRESULT	GetItemInfo(DWORD nItemId, PVOID itemInfo) = 0;

	//获取已安装插件的个数, 第一个参数为总插件个数，第二个为恶意插件的个数
	VIRTUAL_ENG HRESULT	GetPluginCount(DWORD & dwPlugNumber, DWORD & dwAdwareNumber) = 0;

	//获取扫描结果中插件信息，从0开始一直向后枚举
	VIRTUAL_ENG HRESULT	GetNextPluginInfo(DWORD & dwPos, PLUG_SCAN_RESULT** info ) = 0;

	//如果扫描到插件的话，根据ID来获取其文件和注册表等详细信息
	VIRTUAL_ENG HRESULT	GetItemScanResultInfo(DWORD nItemid, PLUG_SCAN_RESULT** pItemInfo) = 0;	// 库里的信息加扫描结果的详细信息

	// 由获取函数得到的指针的释放
	VIRTUAL_ENG HRESULT FreeItemScanResInfo(PLUG_SCAN_RESULT* itemInfo) = 0;

	//删除插件，如果同步则阻塞，否则在回调里通知删除结果
	VIRTUAL_ENG HRESULT	Remove(DWORD nItemId, BOOL bSyn=FALSE) = 0;

	//删除一组插件，nNumber代表个数，pBuffer包含删除的ItemId数组
	VIRTUAL_ENG	HRESULT Remove(DWORD nNumber, DWORD* pBuffer, BOOL bSyn=FALSE) = 0;

	//清除扫描结果
	VIRTUAL_ENG HRESULT	ClearResult() = 0;

	//设置插件是否安全/恶意/一般	0为取消信任，1为设置信任
	VIRTUAL_ENG HRESULT	SetPluginSafe(DWORD nItemId, BOOL bTrust) = 0;

	//检查要清除的插件是否有插入explorer.exe的
	VIRTUAL_ENG HRESULT GetInjectExplorer(DWORD nSize, DWORD* pBuffer, BOOL & bInject) = 0;

	// 生成一个LOG_PLUG_REMOVE的指针数组，用完需要释放
	VIRTUAL_ENG HRESULT GetPlugRemoveLog( DWORD & nOutSize, PLOG_PLUG_REMOVE** ppLogArray) = 0;

	// 释放Log数组
	VIRTUAL_ENG HRESULT	FreePlugRemoveLog( DWORD nSize, PLOG_PLUG_REMOVE* pLogArray) = 0;

	// 清除所有日志
	VIRTUAL_ENG HRESULT ClearAllLog() = 0;

	// 设置重启清除标志
	VIRTUAL_ENG HRESULT SetRebootClean(DWORD nNumber, DWORD* pIDBuffer) = 0;

	// 获取库的版本号
	VIRTUAL_ENG HRESULT	GetDataBaseVersion(DWORD & nVersion) = 0;

	// 接口为 4 的时候添加的 *************************************
	//完全扫描，使用库规则扫描恶意插件
	VIRTUAL_ENG HRESULT	ScanAllDanger(BOOL bSyn=FALSE) = 0;

	// 接口为 5 的时候添加的 *************************************
	//设置是否使用扩展扫描
	VIRTUAL_ENG HRESULT SetUseExtend(BOOL bUse=FALSE) = 0;

	// 接口为 6 的时候添加的 *************************************
	// 设置使用外部的删除接口
	VIRTUAL_ENG HRESULT SetDelOpCallBack(IDelOpCallback* pOpCallback) = 0;

	// 接口为7 的时候添加的 *************************************
	// 获取已经信任的列表
	VIRTUAL_ENG HRESULT GetTrustPlugDetailInfo(PLUG_SCAN_RESULT** pInfo, DWORD* pdwSize) = 0;
	VIRTUAL_ENG HRESULT	SetCLSIDNoTrust(LPCTSTR lpCLSID) = 0;
	VIRTUAL_ENG HRESULT	SetReturnTrustScanRes(BOOL bRet) = 0;
	VIRTUAL_ENG HRESULT	LoadLibrary() = 0;//hub
};
struct PLUG_EXTEND_INFO
{
	CSimpleArray<CStringA>	strCLSIDArray;
	CSimpleArray<CStringA>	strRegArray;
	CSimpleArray<CStringA>	strFileArray;
	CSimpleArray<CStringA>	strDirArray;
};

class ICheckInterface
{
public:
	VIRTUAL_ENG HRESULT	Init() = 0;
	VIRTUAL_ENG HRESULT	GetInfoByCLSID( LPCTSTR lpCLSID, PLUG_DAT_INFO* pPlugInfo ) = 0;
	VIRTUAL_ENG	HRESULT	GetInfoByFilePath( LPCTSTR lpFilePath, PLUG_DAT_INFO* pPlugInfo ) = 0;
	VIRTUAL_ENG HRESULT	GetInfoByRegKey( HKEY hRoot, LPCTSTR lpKey, PLUG_DAT_INFO* pPlugInfo  ) = 0;
	VIRTUAL_ENG HRESULT	GetInfoByRegValue( HKEY hRoot, LPCTSTR lpKey, LPCTSTR lpValue, PLUG_DAT_INFO* pPlugInfo ) = 0;
	VIRTUAL_ENG HRESULT	ForceUnLoadRule() = 0;
	VIRTUAL_ENG HRESULT InitExt() = 0;
	VIRTUAL_ENG HRESULT GetExtendInfo(PLUG_EXTEND_INFO** ppInfo) = 0;
	VIRTUAL_ENG HRESULT FreeExtendInfo(PLUG_EXTEND_INFO* pInfo) = 0;
};


typedef IPlugEng* (WINAPI *PFNGetEngine)(DWORD );
typedef VOID (WINAPI *PFNFreeEngine)(IPlugEng* );

typedef ICheckInterface* (WINAPI *PFNGetCheckEngine)(DWORD);
typedef VOID (WINAPI *PFNFreeCheckEngine)(ICheckInterface* );


#define XX_PLUG_ENGINE_DLL_NAME _T("kplugeng.dll")
#define XX_PLUG_ENG_GET_STR			"f"
#define XX_PLUG_ENG_FREE_STR		"b"
#define XX_PLUG_ENG_REBOOTCLEAN		"c"
#define XX_PLUG_ENG_GET_CHECK_STR	"g"
#define XX_PLUG_ENG_FREE_CHECK_STR	"e"


class CPluginLoader
{
public:
// 	CPluginLoader(void)
// 	{
// 		m_hModDLL	= NULL;
// 		pfnGet		= NULL;
// 		pfnFree		= NULL;
//         pfnGetCheck = NULL;
//         pfnFreeCheck = NULL;
// 
// 		m_hModDLL	= ::LoadLibrary(XX_PLUG_ENGINE_DLL_NAME);
// 		if ( m_hModDLL != NULL )
// 		{
// 			pfnGet  = (PFNGetEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_GET_STR);
// 			pfnFree = (PFNFreeEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_FREE_STR);
// 
// 			pfnGetCheck  = (PFNGetCheckEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_GET_CHECK_STR);
// 			pfnFreeCheck = (PFNFreeCheckEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_FREE_CHECK_STR);
// 		}
// 		LoadEng(XX_PLUG_ENGINE_DLL_NAME);
// 	}

	CPluginLoader(CString strAppPath = L"")
	{
		m_hModDLL	= NULL;
		pfnGet		= NULL;
		pfnFree		= NULL;
		pfnGetCheck = NULL;
		pfnFreeCheck = NULL;
		m_pPlugEng   = NULL;
		CString strDllPath = strAppPath;

		if (TRUE == strAppPath.IsEmpty())
		{
			strDllPath = XX_PLUG_ENGINE_DLL_NAME;
		}
		else
		{
			strDllPath.Append(L"\\");
			strDllPath.Append(XX_PLUG_ENGINE_DLL_NAME);
		}

		LoadEng(strDllPath);
// 		m_hModDLL	= ::LoadLibrary(strAppPath);
// 		if ( m_hModDLL != NULL )
// 		{
// 			pfnGet  = (PFNGetEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_GET_STR);
// 			pfnFree = (PFNFreeEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_FREE_STR);
// 
// 			pfnGetCheck  = (PFNGetCheckEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_GET_CHECK_STR);
// 			pfnFreeCheck = (PFNFreeCheckEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_FREE_CHECK_STR);
// 		}
	}

	~CPluginLoader(void)
	{
		if ( m_hModDLL != NULL )
		{
			::FreeLibrary(m_hModDLL);
			m_hModDLL = NULL;
		}
	}

	void LoadEng(CString strDll)
	{
		m_hModDLL	= ::LoadLibrary(strDll);
		if ( m_hModDLL != NULL )
		{
			pfnGet  = (PFNGetEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_GET_STR);
			pfnFree = (PFNFreeEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_FREE_STR);

			pfnGetCheck  = (PFNGetCheckEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_GET_CHECK_STR);
			pfnFreeCheck = (PFNFreeCheckEngine)::GetProcAddress( m_hModDLL, XX_PLUG_ENG_FREE_CHECK_STR);
		}

	}

	IPlugEng* GetPlugEngine()
	{
// 		if ( pfnGet != NULL )
// 			return pfnGet(VERSION_PLUG_INTERFACE_HIGH);
// 
// 		return NULL;
		if (m_pPlugEng == NULL)
		{
			if ( pfnGet != NULL )
				m_pPlugEng = pfnGet(VERSION_PLUG_INTERFACE_HIGH);
		}		
		return m_pPlugEng;
	}
	
	VOID FreePlugEngine(IPlugEng* pEng)
	{
		if ( pfnFree != NULL )
		{
			pfnFree(pEng);
			m_pPlugEng = NULL;
		}
	}

	ICheckInterface* GetCheckInterface()
	{
		if ( pfnGetCheck != NULL )
			return pfnGetCheck(VERSION_PLUG_INTERFACE_HIGH);

		return NULL;
	}
	VOID FreeCheckInterface(ICheckInterface* p)
	{
		if ( pfnFreeCheck )
			pfnFreeCheck(p);
	}

protected:
	HMODULE				m_hModDLL;
	PFNGetEngine		pfnGet;
	PFNFreeEngine		pfnFree;
	PFNFreeCheckEngine	pfnFreeCheck;
	PFNGetCheckEngine	pfnGetCheck;
	IPlugEng*			m_pPlugEng;//hub
};

class CPlugNullCallback : public IEngineCallback
{
public:
	CPlugNullCallback(){}
	~CPlugNullCallback(){}

public:
	virtual HRESULT		ScanStart(){return S_OK;}
	virtual HRESULT		ScanComplete(BOOL bCancel){return S_OK;}
	virtual HRESULT		ScanItemStart(DWORD nIndex, DWORD nTotal, LPCTSTR lpName){return S_OK;}
	virtual HRESULT		FindItemExist(DWORD nItemId){return S_OK;}
	virtual HRESULT		ScanItemComplete(DWORD nItem){return S_OK;}
	virtual HRESULT		UnkItemChange(DWORD nItemId){return S_OK;}

	virtual HRESULT		QuickScanStart(){return S_OK;}
	virtual HRESULT		QuickScanComplete(){return S_OK;}

	virtual HRESULT		RemoveItemStart(DWORD nItemId, DWORD nRate){return S_OK;}
	virtual HRESULT		RemoveItemEnd(DWORD nItemId, HRESULT hRes ){return S_OK;}
	virtual HRESULT		RemoveComplete(BOOL bNeedRestart){return S_OK;}
	virtual HRESULT		LibLoadFailed(){return S_OK;}
};

// inline CPluginLoader* GlobalGetPluginLoader()
// {	
// 	static CPluginLoader loader;
// 	return &loader;
// }

inline CPluginLoader* GlobalGetPluginLoader(CString strAppPath = L"")
{	
	static CPluginLoader loader(strAppPath);
	return &loader;
}

#ifdef __ATLSIMPCOLL_H__
class CDWORDBuffer
{
public:
	CDWORDBuffer(CSimpleArray<DWORD>& arr)
	{
		nSize	= 0;
		pBuffer	= NULL;

		if (arr.GetSize() > 0 )
		{
			pBuffer = new DWORD[arr.GetSize()];
			nSize	= arr.GetSize();
			for ( int i = 0; i < arr.GetSize(); i++)
			{
				pBuffer[i] = arr[i];
			}
		}
	}

	~CDWORDBuffer()
	{
		if ( pBuffer )
		{
			delete pBuffer;
			pBuffer = NULL;
		}
	}

	DWORD*	GetBuffer()
	{
		return pBuffer;
	}
	DWORD GetSize()
	{
		return nSize;
	}

protected:
	DWORD	nSize;
	DWORD*	pBuffer;
};
#endif
#pragma once
#include "vulfixspecifics.h"
typedef CSimpleArray<int> IntArray;
struct TUpdateItem;
typedef TUpdateItem *LPTUpdateItem;

struct TUpdateItemDetail
{
	TUpdateItemDetail() : nPatchSize(0)
	{
        bCheckCert = TRUE;
	}
	
	CString strWebPage;	// 
	CString strDownurl;	// 下载链接 : 中文&英文 
	CString strInstallParam;// 安装参数: 
	CString strMD5;		//补丁包在MD5
	int	nPatchSize;			//中文版补丁包的大小
	CString strMirrorurl;	// 镜像下载链接 : 中文&英文 
    BOOL    bCheckCert;
};

// 严重等级 
// 严重等级, 1-5 : rating_map = {'过期的':'-1', 可选':'0', '低':'1', '中等':'2', '重要':'3', '严重':'4'} 
enum E_WarnLevel	
{
	EWarn_Superseded = -1,
	EWarn_Option = 0,
	EWarn_Low,
	EWarn_Midium,
	EWarn_Important,
	EWarn_Serious,
	EWarn_Fatal,
};

enum E_InstalledResult
{
	EINST_SUCCESS = 0,				// 返回值很对 
	EINST_SUCCESS_CHECKED,			// 在注册表里面找到了相应项
	EINST_SUCCESS_CHECK_FAILED,		// 可能安装成功了, 复查的时候没查到	
	EINST_FAIL,						// 安装失败 : 根据返回值, 完全失败 
	EINST_FAIL_EXECUTE,				// 运行失败, 可能文件下载有问题 
	EINST_FAIL_REQUIRE_REBOOT,		// 需要重启 
	EINST_FAIL_NOT_APPLICABLE,		// 完全失败 : 根据WIN7 的exit code 
	EINST_FAIL_HIJACKED,			// 被360 拦截了
	EINST_FAIL_FILEBUSY,			// User Patcher 的时候, 文件忙 
};

struct T_UserPatchInfo
{
	struct PatchFileInfo
	{
		CString strFilenameFrom, strFilenameTo;
	};

	INT nKBID;
	CString strName;
	CString strProductKey, strPatchKey, strPatchValue;
	CSimpleArray<PatchFileInfo> files;

	void AddPatch(LPCTSTR sfrom, LPCTSTR sto)
	{
		PatchFileInfo pi;
		pi.strFilenameFrom = sfrom;
		pi.strFilenameTo = sto;
		files.Add( pi );
	}
};

enum E_VFlags
{
	VFLAG_NULL				= 0,
	VFLAG_SERVICE_PATCH		= 1, 
	VFLAG_IGNORE_INSTALLED	= 2,
};

inline BOOL IsFlagOn( E_VFlags flag, DWORD dwFlags )
{
	return ( ( flag & dwFlags ) == flag );
}

inline void SetFlagOn( E_VFlags flag, DWORD &dwFlags )
{
	dwFlags |= flag;
}

struct TUpdateItem
{
	TUpdateItem() : nID(-1), nWarnLevel(0), isExclusive(0), m_nType(0), dwVFlags(0)
		, bUserPatcher(FALSE), isIgnored(FALSE), isInstalled(FALSE), isExpired(FALSE)
	{
	}
	
	int		nID;			// 
	CString strMsid;			// 公告号
	CString strName;			// 漏洞名称
	int		nWarnLevel;			// E_WarnLevel, -1 表示过期
	CString strPubdate;			// 发布时间
	CString strDescription;		// 摘要

	CString strWebpage;			// 公告网址

	CString strInstallparam;	// 安装参数
	int	isExclusive;			// 是否独立安装
	DWORD dwVFlags;				// 包参数  
	
	TUpdateItemDetail detail;	// 下载的详细信息

	CString strAffects;			// 影响
	CString strSuggestion;		// 建议
	BOOL bUserPatcher;			// 是否使用自定义Patcher 	
	CString strCondition;		// 检测条件, 用以后续Office 检测 

	// RUNTIME 
	int		m_nType;			// 补丁的类型, sys, office 
	CString m_strFilePath;		// 补丁文件, 如果存在的话 
	
	BOOL isIgnored;			// 是否忽略
	BOOL isInstalled;		// check result 
	BOOL isExpired;			// 
};

struct TItemFixed
{
	TItemFixed() : nID(0), fromDB(FALSE)
	{}
	TItemFixed(const TItemFixed& v)
		: nID(v.nID),  fromDB(FALSE)
	{
		strName = v.strName;
		strWebPage = v.strWebPage;
		strInstallDate = v.strInstallDate;
	}
	
	INT nID;
	CString strName;
	CString strWebPage;
	CString strInstallDate;
	BOOL fromDB;
};

struct TReplacedUpdate{
	int nKBID, nKBID2;
	CString strPubdate;
};

struct TOptionCondition 
{
	int nWinVer, nWinSP;	
	int nIEVer, nIESP;	

	int nJS;
	int nMediaplayer;	
	int nDX;			
	int nOE;
	int nDataAccess;
	int nFramework;
	int nXML;
	int nMSN;
	
	CString strOffice;			//office 的版本
	CString m_strCondition;		// 条件 
	CString m_strExpInstalled;	// 
};

enum T_SoftVulActionType
{
	SOFTACTION_NONE,		// 无操作
	SOFTACTION_UPDATE,		// 升级主程序
	SOFTACTION_PATCH,		// 下载patch
	SOFTACTION_DISABLECOM,	// 禁用COM 
};

enum T_SoftComState
{
	COM_NOT_DISABLED,
	COM_PART_DISABLED,
	COM_ALL_DISABLED,
};

union T_VulState
{
	int nstate;
	struct 
	{
		char action;	// T_SoftVulActionType
		bool bMatched;	// clsid 是否满足条件
		bool bFounded;	// clsid 是否在注册表中找到
		char comState;	// T_SoftComState com 状态
	};
};

struct TVulSoftSubitem
{
	CString  strName;			//名称
	CString  strWebpage;		//软件官网
	CString  strDownpage;		//下载页面
	CString  strDownUrl;		//下载URL	
	int nAction;				//动作类型表示是，升级最新版本，或下载官方补丁,或者是COM 禁用
	int nSize;					//软件大小
	
	TOptionCondition conditon;			//
};

struct TVulSoft
{
	int	nID;
	CString strName;			//漏洞名称
	CString strPubdate;			//发布日期
	CString strOrigUrl;			// 漏洞来源URL
	CString strAffects;		//存在漏洞版本
	CString	strDescription;			// 简短描述
	CString strDetailDescription;	// 详细描述
	CString	strSideaffect;
	int nLevel;				//等级
	int nDisableCom;		//是否禁用COM
	
	CSimpleArray<CString> arrCLSID;			//CLSID列表  CLSID|VFrom|VTo|
	TVulSoftSubitem matchedItem;				// Detail subitem 
	
	// 
	T_VulState state;
	
	int isIgnored:1;			// 是否忽略
	int isToFix:1;				// 是否安装
	int isInstalled:8;			// 安装是否成功: 0 未安装, 1 安装成功, -1 安装失败 
};

typedef TVulSoftSubitem* LPTVulSoftSubitem;
typedef TVulSoft* LPTVulSoft;

enum VulType
{
	VTYPE_IGNORED = -1,		//忽略的 
	VTYPE_UNKOWN = 0,		
	VTYPE_OFFICE,		
	VTYPE_WINDOWS,		
	VTYPE_SOFTLEAK,		
};

enum TVulFixEventType {
	
	EVulfix_Error,				// 错误				
	EVulfix_ScanBegin,			// Scan 开始		0, Num 
	EVulfix_ScanProgress,		// Scan 进度		0, nCurrentItem 

	EVulfix_DownloadBegin,		// 下载开始			id  currentnum,	totalnum  
	EVulfix_DownloadProcess,	// 下载进度			id, downloadedsize, fullsize
	EVulfix_DownloadEnd,		// 下载结束			id, currentnum, down.result
	EVulfix_DownloadError,		// 下载失败			id, currentnum, 
	
	EVulfix_InstallBegin,		// 安装开始			id, currentnum,	totalnum 
	EVulfix_InstallBeginUser,	// 自定义安装开始	
	EVulfix_InstallEnd,			// 安装结束			id, currentnum, install.result 
	EVulfix_InstallError,		// 安装失败			id, currentnum 

	EVulfix_Task_Complete,		// 任务完全完成 
	EVulfix_Task_Error,			// 任务有错, 可能部分完成 

    EVulfix_Download_Check_Error,// 下载下来的补丁数字签名校验失败
};

class IVulfixObserver
{
public:
	virtual void OnVulfixEvent(TVulFixEventType evt, int nKbId, DWORD dwParam1, DWORD dwParam2) = 0;
};


// Scan flags 
#define VULSCAN_USE_IUPDATE			1	// Use iupdate for vista, win7
#define VULSCAN_EXPRESS_SCAN		2	// Express Scan, 扫描到一个高危漏洞就退出!! 

class IVulIgnore
{
public:
	virtual ~IVulIgnore() {} 
	virtual HRESULT LoadIgnoreDB() = 0;
	virtual HRESULT SaveIgnoreDB() = 0;					// 保存忽略漏洞到文件 
	virtual HRESULT Ignore(int nKBID, BOOL bIgnore) = 0;// 忽略漏洞
	virtual HRESULT AutoIgnore(int nKBID, BOOL bIgnored) = 0; // 提供给智能修复 
	virtual int IsIgnored(int nKBID) = 0;		// 是否被忽略了 1:用户忽略 2:系统忽略 
};

// 系统 & office 漏洞扫描 
class IVulfix
{
public:
	virtual ~IVulfix() {} 
	
	virtual void SetObserver(IVulfixObserver *pObserver) = 0;

	virtual HRESULT Scan(DWORD dwFlags=0)=0;	// 扫描
	virtual HRESULT Cancel() = 0;
	
	virtual const CSimpleArray<LPTUpdateItem>& GetResults() = 0;
	virtual const CSimpleArray<LPTVulSoft>& GetSoftVuls() = 0;

	virtual const CSimpleArray<TItemFixed*>& GetFixedVuls() = 0;
	virtual const CSimpleArray<LPTUpdateItem>& GetIgnoredVuls() = 0;
	virtual const CSimpleArray<LPTUpdateItem>& GetInvalidVuls() = 0;
	virtual const CSimpleArray<TReplacedUpdate*>& GetReplacedVuls() = 0;
	
	
	virtual BOOL Ignore(int nKBID, BOOL bIgnore)=0;		// 忽略漏洞
	virtual BOOL PersistIgnored()=0;					// 保存忽略漏洞到文件 
	
	virtual HRESULT EnableVulCOM(int nId, BOOL bEnable=FALSE) = 0; // 禁用COM 

	virtual BOOL GetLatestPackgeDate(INT &nYear, INT &nMonth, INT &nDay) = 0;
	virtual BOOL GetLatestInstalledKBDate(FILETIME &latestInstallTime) = 0;
	// Interface for Vista quick scan by date 
	// -1 for db<os.fix	 0 for db==os.fix  1 for db>os.fix, need to repaire
	virtual INT ExpressScanSystem() = 0; 
};

class IVulfixed
{
public:
	virtual ~IVulfixed() {} 
	virtual void SetObserver(IVulfixObserver *pObserver) = 0;
	virtual HRESULT Scan(DWORD dwFlags=0)=0;			// 扫描
	virtual HRESULT Cancel() = 0;						// 取消扫描 
	virtual int Count() = 0;
	virtual const CSimpleArray<TItemFixed*>& GetResults() = 0;
	virtual const CSimpleArray<TReplacedUpdate*>& GetReplacedUpdates() = 0;
};

// 第三方软件漏洞扫描 
class ISoftVulfix
{
public:
	virtual ~ISoftVulfix() {} 
	
	virtual void SetObserver(IVulfixObserver *pObserver) = 0;
	virtual HRESULT Scan(DWORD dwFlags=0)=0;
	virtual HRESULT Cancel() = 0;
	virtual int Count() = 0;
	virtual const CSimpleArray<LPTVulSoft>& GetResults() = 0;

	virtual HRESULT EnableVulCOM(int nId, BOOL bEnable=FALSE) = 0;
};

#define VULFLAG_REPAIR_IMPORT_LOCAL				1
#define VULFLAG_REPAIR_DOWNLOAD_IF_FAIL_IMPORT	2
class IRepairVul 
{
public:
	virtual ~IRepairVul() {} 

	virtual HRESULT Reset() = 0;
	virtual HRESULT SetObserver(IVulfixObserver *pObserver) = 0;

	virtual HRESULT AddVul(IVulfix *pVulfix, const IntArray&arrayId) = 0;
	virtual HRESULT AddSoftVul(IVulfix *pSoftVulfix, const IntArray&arrayId) = 0;

	virtual HRESULT Repair(LPCTSTR sDownloadPath, LPCTSTR szImportPath, DWORD dwFlags) = 0;
	virtual HRESULT StopRepair() = 0;
	virtual HRESULT WaitForRepairDone() = 0;
	virtual HRESULT GetItemFilePath(INT nID, BSTR *bstr) = 0;
};

BOOL RequireUsingInterface();
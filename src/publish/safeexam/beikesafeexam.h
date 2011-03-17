#pragma once

#include <Unknwn.h>
#include <atlcoll.h>

#define BK_SAFEEXAM_DLL_VERSION 4

#define EXAM_GUEST_STR L"GUEST"
#define EXAM_REMOTEDESK_STR L"REMOTEDESK"
#define EXAM_DUBA	L"Duba"
#define EVENT_EXAM_SHARE_DATA	L"Global\\EB225E90-D3CF-43b6-B618-9333BCF198C0"
#define NAME_EXAM_SHARE_MEM		L"Global\\196B4819-9676-4ec9-8FC8-7DE9CFB5E047"
#define EVENT_EXAM_RECV			L"Global\\D7A5FD89-4CFF-4b43-9D65-2722661A4301"
#define SIZE_EXAM_SHARE_MEM		0x400*0x400

//体检中会触发的事件
#define MSG_EXAM_SCAN_ITEM           (WM_APP + 1)
#define MSG_EXAM_SCAN_PROGRESS		 (WM_APP + 11)



//体检项目对应的ID，里面可能有一部分ID已经不在使用了，但是为了保持原来的顺序就没有清理掉
namespace BkSafeExamItem
{
    enum
    {
        Invalid = 0,						//体检结束
        CriticalPathVirusScan   = 1,        // 关键目录病毒扫描->1
        SystemLeakScan,                     // 系统漏洞扫描->2
        MalwareScan,                        // 恶意插件扫描->3
        HandShake,                          // 握手->4
        ConfigAutoReport,                   // 自动上传文件设置->5
        ConfigAutoUpdate,                   // 自动升级设置->6
        ConfigSystemMonitor,                // 监控状态->7
        DenyChangeIEMainPage,               // 阻止用户更改IE首页->8
        LongTimeNoScan,                     // 太久没全盘扫描了->9
        LocalInfo,                          // 本地信息->10
		//////////////////////////////////////////////////////////////////////////
		//以下为体检新增加项目
		EXAM_HOMEPAGE=11,				    //检测IE首页->11
		EXAM_IE_LNKFILE_PARAM,				//IE 快捷方式指向正常->12
		EXAM_IE_RIGHT_MENU,					//IE右键菜单->13
		EXAM_IE_TOOLBAR_ICON,				//工具栏图标->14
		EXAM_IE_TOOLBAR,					//危险工具条->15
		EXAM_IE_DEFAULT_SEARCH_ENG,			//IE默认搜索引擎->16
		EXAM_IE_DIY_SEARCH_ENG,				//IE自定义搜索引擎->17
		EXAM_IE_SEARCH_PAGE,				//IE默认搜索页面->18
		EXAM_HOSTS_FILE,					//检测HOSTS文件->19
		EXAM_IE_SAFE_WEB,					//IE受信任站点->20
		EXAM_IE_BROWSE,						//INTERNET选项->21
		EXAM_HTML_FILE,						//HTML文件关联->22
		EXAM_PROTOCOL_DEFAULT,				//检测默认协议关联->23
		EXAM_PROXY_SERVER,					//代理服务器->24
		EXAM_HOMEPAGE_CAN_SET,				//检测IE首页是否可以自定义设置->25
		EXAM_REGEDIT_RUN,					//检测注册表编辑器是否可用->26
		EXAM_GPEDIT_RUN,					//检测组策略是否可用->27
		EXAM_GUEST_RUN,						//检测GUEST账户是否可用->28
		EXAM_HIDE_FOLDER_RUN,				//检测隐藏文件夹是否可用->29
		EXAM_SHARE_RESOUCE,					//检测共享资源->30
		EXAM_REMOTE_DESKTOP,				//检测远程桌面->31
		EXAM_KWS_MON,						//检测网盾保护->32			
        EXAM_CONNECT_SRV,					//握手->33
		EXAM_TRASH,							//检测系统中是否存在垃圾->34 新增加垃圾清理 [7/19/2010 zhangbaoliang]
		EXAM_DRIVERORDIRECTORYOPEN,			//检测磁盘，文件夹打开是否正常->35
		//////////////////////////////////////////////////////////////////////////
		//以下是体检中要使用网盾进行体检的项目
		EXAM_FAVORITES_URL,					//检测收藏夹中是否存在恶意推广的网址->36
		EXAM_SYSTEM_EXCEPTION,				//是否存在系统异常->37
		EXAM_NOTIE_LNK,						//非IE浏览器快捷方式是否被篡改->38
		EXAM_DESKTOP_SYSTEM_EXCEPTION_ICON,	//检测桌面图标->39
		EXAM_DESKTOP_EXCEPTION_ICON,		//异常的桌面图标->40
		EXAM_RUNOPT_ONKEY,					//一键优化->41
		EXAM_CHECK_UPDATE,					//检测软件升级->42
		EXAM_CHECK_AV,						//检测杀毒软件->43

        Count                               // 终止标记
    };

	//
	struct STRUCT_BKSAFE_EXAM_ITEM
	{
		DWORD dwID;
		int nGroupID;
		int nSubItemID;
		LPCTSTR pszCaption;

	};
}

//体检扫描结果导出数据结构
namespace BKSafeExamItemEx
{
	struct SHARE_RESOURCES 
	{
		CString pszName;
		CString pszPath;
		SHARE_RESOURCES()
		{
			pszPath.Empty();
			pszName.Empty();
		}
	};

	//漏洞扩展结果
// 	struct BKSafeExamLeakEx 
// 	{
// 		int nHightLeakCount;//高危
// 		int nOptionLeakCount;//可选
// 		int nSpLeakCount;//SP级别
// 		BKSafeExamLeakEx()
// 		{
// 			nHightLeakCount = 0;
// 			nOptionLeakCount = 0;
// 			nSpLeakCount = 0;
// 		}
// 	};

	struct BKSafeExamEx
	{
		union
		{
			//漏洞扩展结果
			struct BKSafeExamLeakEx 
			{
				int nHightLeakCount;//高危
				int nOptionLeakCount;//可选
				int nSpLeakCount;//SP级别
				void Reset()
				{
					nHightLeakCount = 0;
					nOptionLeakCount = 0;
					nSpLeakCount = 0;
				}
			}ExamLeakEx;
			//网盾扫描扩展信息
			struct BKSafeExamKWSResultEx
			{
				int nItemID;
				int nDangerCount;
				void Reset()
				{
					nItemID = -1;
					nDangerCount = 0;
				}
			}ExamKwsResultEx;
			//垃圾
			struct BKSafeExamTrash
			{
				int nSize;
				void Reset()
				{
					nSize = -1;
				}
			}Trash;
			//软件升级,需要升级的软件个数
			struct BKSafeCheckUpdate
			{
				int nNeedUpdateCount;
				void Reset()
				{
					nNeedUpdateCount = 0;
				}
			}BKSafeUpdate;
            //实时保护返回结果
            struct BKSafeMonitorInfo
            {
                int nDisableCount;
                int arrDisableID[10];

                void Reset()
                {
                    nDisableCount = 0;
                    memset(arrDisableID, -1, sizeof(arrDisableID));
                }
            }BKSafeMonitor;
		}u;
		BOOL bInvalid;

		void Reset()
		{
			bInvalid = FALSE;
		}

	};
}

//体检扫描时需要的信息
class BKSAFE_EXAM_RESULT_ITEM
{
public:

	int     nLevel;
	CString strItemNoticeText;
	CString strOperationText;
	CString strItemNavigateString;
	CString strItemDetailUrl;
	CString strDetail;
	BKSafeExamItemEx::BKSafeExamEx SafeExamItemEx;
};

//体检修复时需要的信息
struct REMOVE_EXAM_RESULT_ITEM
{
	union
	{
		struct _RemoveItemInfo 
		{
			DWORD dwItemID;					//清除ID
			DWORD dwTotalDangerCount;		//当前ID总共有多少个危险项目
			DWORD dwRemoveDangerCount;		//这次清除了几个危险项目
			int Reset()
			{//重置
				dwItemID = -1;
				dwTotalDangerCount = 0;
				dwRemoveDangerCount = 0;
				return 0;
			}
			_RemoveItemInfo& operator =(int nItem)
			{
				dwItemID = nItem;
				dwTotalDangerCount = 0;
				dwRemoveDangerCount = 0;

				return *this;
			}
		}RemoveItemInfo;
	}uRemoveItem;
};
//体检共享数据结构

struct BKSafe_Exam_ShareData
{
	DWORD	cbSize;
	UINT	uMsg;
	int		nItem;
	union
	{
		struct BKSAFE_EXAM_SCAN_ITEM
		{
			int     nLevel;
// 			WCHAR   pszItemNoticeText[MAX_PATH];
// 			WCHAR	pszOperationText[MAX_PATH];
// 			WCHAR	pszItemNavigateString[MAX_PATH];
// 			WCHAR	pszItemDetailUrl[MAX_PATH*2];
// 			WCHAR	pszDetail[MAX_PATH];
			BKSafeExamItemEx::BKSafeExamEx SafeExamItemEx;
			void Reset()
			{
				nLevel = -1;
// 				ZeroMemory(pszDetail, sizeof(pszDetail));
// 				ZeroMemory(pszItemDetailUrl, sizeof(pszItemDetailUrl));
// 				ZeroMemory(pszItemNavigateString, sizeof(pszItemNavigateString));
// 				ZeroMemory(pszItemNoticeText, sizeof(pszItemNoticeText));
// 				ZeroMemory(pszOperationText, sizeof(pszOperationText));
			}

			BKSAFE_EXAM_SCAN_ITEM& operator =(BKSAFE_EXAM_RESULT_ITEM& scanitem)
			{
				this->nLevel = scanitem.nLevel;
// 				_sntprintf_s(pszDetail, sizeof(pszDetail), L"%s", scanitem.strDetail);
// 				_sntprintf_s(pszItemDetailUrl, sizeof(pszItemDetailUrl), L"%s", scanitem.strItemDetailUrl);
// 				_sntprintf_s(pszItemNavigateString, sizeof(pszItemNavigateString), L"%s", scanitem.strItemNavigateString);
// 				_sntprintf_s(pszItemNoticeText, sizeof(pszItemNoticeText), L"%s", scanitem.strItemNoticeText);
// 				_sntprintf_s(pszOperationText, sizeof(pszOperationText), L"%s", scanitem.strOperationText);
				SafeExamItemEx = scanitem.SafeExamItemEx;

				return *this;
			}
		}BKSafe_Exam_Scan_Item;

		struct BKSAFE_EXAM_REMOVE_ITEM 
		{
			REMOVE_EXAM_RESULT_ITEM RemoveItemData;
		}BKSafe_Exam_Remove_Item;

		struct BKSAFE_EXAM_PROGRESS
		{
			int nProgress;
		}BKSafe_Exam_Progress;

		struct BKSAFE_EXAM_STATE
		{//0，开始体检，1，取消体检，2，完成体检
			int nState;
		}ExamState;

	}u;
};
//本地信任列表数据结构
namespace BkSafeExamLib
{
	class LocalLibExport
	{
	public:
		LocalLibExport(){ZeroMemory(pszValue, 1024);nType = -1;};
		int nType;
		WCHAR pszValue[1024];
	};
	class LocalLib
	{
	public:
		LocalLib(){nType = -1;strValue = L"";};
		~LocalLib(){nType = -1;strValue = L"";};
		int nType;
		CString strValue;
	};

	class LocalLibInfo
	{
	public:
		CString strCaption;
		CAtlArray<LocalLib> arrLib;	
	};

	struct HostFileItem
	{
		int nGroupID;
		int nItemID;
		int nLevel;
		CString strHostItem;
		HostFileItem()
		{
			nLevel = -1;
			nGroupID = -1;
			nItemID = -1;
			strHostItem.Empty();
		}
	};
}

//体检中可能返回的危险等级
namespace BkSafeExamItemLevel
{
    enum
    {
        Unknown   			= -1,		//未知
        Safe      			= 0,		//安全
        Info      			= 1,		//未使用
        RiskInfo  			= 2,		//未使用
        Critical  			= 3,		//危险
        Risk_0    			= 4,		//风险
		Scan_Error			= 5,		//扫描失败
		Optimization		= 6			//是优化项目
    };
}

namespace BkSafeExamItemExOperate
{
	enum
	{
		UnKnown = -1,
		RemoveItem = 1000,	//进行修复
	};

	enum
	{
		Unknown = -1,
		ErrorMsg = 2000,
	};
}

interface IBkSafeExamineCallback
{
    virtual HRESULT WINAPI OnItemExamFinish(
        int     nItem, 
        int     nLevel, 
        LPCWSTR lpszItemNoticeText, 
        LPCWSTR lpszOperationText, 
        LPCWSTR lpszItemNavigateString, 
        LPCWSTR lpszItemDetailUrl, 
        LPCWSTR lpszDetail) = 0;
    virtual HRESULT WINAPI OnFinish() = 0;
    virtual HRESULT WINAPI OnProgress(DWORD dwProgress, int nExamItem = -1) = 0;
};

interface  __declspec(uuid("EA024B3E-9484-4536-8841-FDFA3FC0EA0E"))
IBkSafeExaminer
    : public IUnknown
{

	//@
	//@:Initialize
	//@:体检模块的初始化接口，加载模块之后先调用这个初始化功能
	//@
    virtual HRESULT WINAPI Initialize(LPVOID pvParam) = 0;

	//@
	//@:Uninitialize
	//@;反初始化
	//@
    virtual HRESULT WINAPI Uninitialize() = 0;

	//@
	//@:Examine体检接口
	//@:piCallback体检结果回调接口指针，hStopEvent停止事件句柄
	//@
    virtual HRESULT WINAPI Examine(IBkSafeExamineCallback *piCallback, HANDLE hStopEvent, BOOL bSlowMode = FALSE) = 0;

	//@
	//@:ExpressExamine
	//@:老接口暂时已经不使用
	//@
    virtual HRESULT WINAPI ExpressExamine(IBkSafeExamineCallback *piCallback, HANDLE hStopEvent) = 0;

	//@
	//@:Echo
	//@老接口已经不使用
	//@
    virtual HRESULT WINAPI Echo(IBkSafeExamineCallback *piCallback, HANDLE hStopEvent) = 0;

	//@
	//@:FixItem
	//@:体检模块的修复接口(因为卫士中的体检修复基本都是通过切换主程序TAB到相应的模块来完后成修复，
	//  所以目前这个接口，只是被用来修复：共享资源，远程桌面，GUEST账户几个有限的问题)
	//@:nItemID需要修复项目的ID，pExData修复某项要使用的数据
	//@
	virtual HRESULT WINAPI FixItem(int nItemID, void* pExData = NULL) = 0;

	//@
	//@:ResetIgnoreArray
	//@:老接口，已经不使用
	//@
    virtual HRESULT WINAPI ResetIgnoreArray() = 0;

	//@
	//@:PushAnIgnoreItem
	//@:老接口，已经不使用
	//@
	virtual HRESULT WINAPI PushAnIgnoreItem(int nItemID) = 0;

	//@
	//@:GetItemCount
	//@:用户来获取体检到的危险项目下的子项个数,比如共享资源，这个返回存在共享资源的个数
	//@:nItemID体检项目ID，nCount 子项数目
	//@
    virtual HRESULT WINAPI GetItemCount(int nItemID, int& nCount) = 0;

	//@
	//@:EnumItemData
	//@:枚举体检项目中的所有子项
	//@：nItemID 体检项目ID，nIndex：体检项目子项索引，pData：子项数据
    virtual HRESULT WINAPI EnumItemData(int nItemID, int nIndex, void** pData) = 0;

	//@
	//@:SetItemSafe
	//@:本地信任功能.体检允许对个别项目设置信任功能。(远程桌面，guest账户)
	//@:nItemID：体检项目ID， pData：设置信任项目的信息(远程桌面传递：REMOTEDESK, guest传递：GUEST，这两个东西，文件顶部有宏定义)
	//@
    virtual HRESULT WINAPI SetItemSafe(int nItemID, void** pData) = 0;

	//@
	//@:SetHostItemFix
	//@:老接口暂不使用
	//@
	virtual HRESULT WINAPI SetHostItemFix(int nIndex) = 0;

	//@
	//@:RemoveLocalLibItem
	//@:取消信任某项，从信任列表中移除(本地信任文件：cfg/LocalLib.dat)
	//@:nItemID：体检项目ID，pValue：取消信任信息(和设置信任信息一样)
	//@
	virtual HRESULT WINAPI RemoveLocalLibItem(int nItemID, void* pValue) = 0;

	//@
	//@:GetSafeLibItemCount
	//@:获取本地信任列表中信任数据的条数
	//@:nCount：返回值，信任数据条数
	//@
	virtual HRESULT WINAPI GetSafeLibItemCount(int& nCount) = 0;

	//@
	//@:EnumSafeLibItem
	//@:枚举信任数据(主要是本地共享资源使用)
	//@
	virtual HRESULT WINAPI EnumSafeLibItem(int nIndex, BkSafeExamLib::LocalLibExport& libItem) = 0;

	//@
	//@:LocalLibIsHaveItem
	//@:主要用户GUEST,和远程桌面，检测是否被忽略过
	//@
	virtual HRESULT WINAPI LocalLibIsHaveItem(int nItemID, BOOL& bHave) = 0;

	//@
	//@:StartupAnUse
	//@:开启某项功能。(现在主用使用在GUEST,和远程桌面，体检的时候如果发现GUEST,
	//  远程桌面已经被禁用，提供开启功能，方便匿名用户访问共享目录)
	//@
	virtual HRESULT WINAPI StartupAnUse(int nItemID) = 0;

	//@
	//@:ExamItem
	//@:单独体检某项，并直接返回接口,这个接口现在只是被垃圾清理调用，因为垃圾清理的时候选择的项目有时候和
	//  体检默认调用的选项是不一样的，所以增加了二次回扫
	//@:ExamItemData体检结果返回数据
	virtual HRESULT WINAPI ExamItem(int nItemID, BKSafeExamItemEx::BKSafeExamEx& ExamItemData) = 0;
};


#define FN_BkSafeCreateObject               "BkSafeCreateObject"
typedef HRESULT (WINAPI *PFN_BkSafeCreateObject)(DWORD dwNeedDllVersion, REFCLSID rclsid, LPVOID *ppiOut);

#define BKSF_EXAM_LOCAL_INFO_FILE_NAME      "msg.ini"

namespace ExamPubFunc
{
	const static DWORD g_KwsItem[] = {11, 40, 12, 39, 21, 16, 38, 36, 22,37};
	class CExamPubFunc
	{
	public:
		~CExamPubFunc(){};
		static CExamPubFunc* GetPubFuncPtr()
		{
			static CExamPubFunc func;
			return &func;
		}
		
		int GetKwsExamItem(CSimpleArray<int>& arrExamItem)
		{
			arrExamItem.RemoveAll();
			int nCount = sizeof(g_KwsItem)/sizeof(DWORD);
			for (int i = 0; i < nCount; i++)
			{
				arrExamItem.Add(g_KwsItem[i]);
			}
			return arrExamItem.GetSize();
		}

		int ConvertKWSIDToExamID(int nKwsID)
		{
			int nExamID = -1;
			switch(nKwsID)
			{
			case 0:
				{//检测浏览器主页是否被恶意篡改指向异常网址
					nExamID = BkSafeExamItem::EXAM_HOMEPAGE;
				}
				break;
			case 1:
				{//检测是否存在流氓软件推广的异常桌面图标
					nExamID = BkSafeExamItem::EXAM_DESKTOP_EXCEPTION_ICON;
				}
				break;
			case 2:
				{//检测IE浏览器快捷方式是否被恶意篡改
					nExamID = BkSafeExamItem::EXAM_IE_LNKFILE_PARAM;
				}
				break;
			case 3:
				{//检测常用桌面图标是否被恶意篡改
					nExamID = BkSafeExamItem::EXAM_DESKTOP_SYSTEM_EXCEPTION_ICON;
				}
				break;
			case 4:
				{//检测Internet选项和IE浏览器功能是否存在异常
					nExamID = BkSafeExamItem::EXAM_IE_BROWSE;
				}
				break;
			case 5:
				{//检测浏览器默认搜索引擎是否存在异常
					nExamID = BkSafeExamItem::EXAM_IE_DEFAULT_SEARCH_ENG;
				}
				break;
			case 6:
				{//检测非IE的其他浏览器的快捷方式是否被恶意篡改
					nExamID = BkSafeExamItem::EXAM_NOTIE_LNK;
				}
				break;
			case 7:
				{//检测收藏夹是否存在恶意推广的网址
					nExamID = BkSafeExamItem::EXAM_FAVORITES_URL;
				}
				break;
			case 8:
				{//检测http协议及网页文件关联是否正常
					nExamID = BkSafeExamItem::EXAM_HTML_FILE;
				}
				break;
			case 9:
				{//检测是否存在其他的系统异常问题
					nExamID = BkSafeExamItem::EXAM_SYSTEM_EXCEPTION;
				}
				break;
			case 10:
				{
				}
				break;
			case 11:
				{
				}
				break;
			case 12:
				{
				}
				break;
			}
			return nExamID;
		}

	};
}

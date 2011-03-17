//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KLogic.h
//  Creator     :   YangXD(yxdh)
//  Date        :   2008-8-22 10:14:50
//  Comment     :   Interface for the KLogic class.
//
//////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "exectrl/kexectrlloader.h"
#include "kbootcare_def.h"
#include "kxescanservice_def.h"

#ifndef IESCANENG_EXPORTS
#include "../common/kserviceutil.h"
#include "../common/ksmsvcfix.h"
#include "../kws/commonfun.h"
#endif

class IKBootCare;
class IKxEScanService;
class IKxEScanServiceControl;

enum{
	enum_BC_trojan	= 0,
	enum_BC_ie		= 1,
	enum_BC_exam	= 2,
	enum_BC_plug	= 3
};

class KBootCarePtr
{
public:
	KBootCarePtr(IKBootCare* p, int nBcType);

	~KBootCarePtr();

	operator IKBootCare*() const throw();

	IKBootCare* operator->() const throw();

private:
	IKBootCare* m_p;
	int m_nBcType;
};


class KLogic
{
public:
	KLogic( int nBcType = enum_BC_trojan );
	~KLogic();

	//这些函数是经常会在非主线程调用。不可以用m_pBookCare
	HRESULT			StartScan(EM_SCAN_TYPE Type);
	HRESULT			CloseSession();
	HRESULT			StartFix(std::vector<S_FIX_ITEM>& FixList);
	HRESULT			RemoveHistoryRecord(S_REMOVE_HISTORY_RECORD_OPTION&	RemoveOption);
	HRESULT			QueryHistoryList( int nItemCnt,std::vector<S_HISTORY_ITEM>& HisList);
	HRESULT			QueryHistoryListEx( int nItemCnt, std::vector<S_HISTORY_ITEM_EX>& HisListEx);
	HRESULT			QueryUserTrustList(const EM_TRUST_ITEM_TYPE& itemType,std::vector<std::wstring>& vecItemList);
	BOOL			StartScanSingle(EM_SCAN_TYPE Type);	//尝试开始扫描
	

	HRESULT			QueryRecordCount( DWORD& dwCount );
	HRESULT			StopScan();
	HRESULT			QuerySessionStatus(S_SCAN_STATUS& Status, std::vector<S_FIX_ITEM_EX>& FixList);
	HRESULT			QuerySessionCategoryDetail(EM_SCAN_TYPE Type, std::vector<S_SCAN_CATEGORY_DETAIL>& CateList);//hub  大文件列表
	HRESULT			QuerySubItemScanDetail(std::vector<S_SCAN_ITEM_DETAIL>& SubItemList);//hub 动态查询的值
	HRESULT			QuerySessionFixListEx(S_QUERY_SETTING& Setting, std::vector<S_FIX_ITEM_EX>& FixList);
	HRESULT			SetCloudProxyInfo(KXE_CLOUD_PROXY_INFO& proxyInfo);//hub  代理服务器
	HRESULT			QueryCloudProxyInfo(KXE_CLOUD_PROXY_INFO& proxyInfo);
	HRESULT			StartRecover( std::vector<__time64_t>& HistoryList);
	HRESULT			StartRecoverEx( std::vector<S_RECOVER_ITEM>&	RecoverList );
	HRESULT			GetUUID(std::wstring& strUUID);
	HRESULT			QueryNeedReboot(BOOL& bReboot);
	HRESULT			Reboot();
	HRESULT			PauseScan();
	HRESULT			ResumeScan();
	HRESULT			FSReportFile();
	//信任列表查询与设置
    HRESULT         GetUnknownFileReport(std::vector<std::wstring>& vecUnknownFileReport);
	HRESULT         IsReportFileFinish(BOOL& bIsFinish);

	void			GetTypeString(EM_SCAN_TYPE Type, CString& strType);
	void			GetEnginString( EM_SCAN_TYPE nType, EM_SCAN_ENGINE_TYPE nEngin, CString& strEngin );
	COLORREF		GetLevelString(EM_POINT_LEVEL Level, CString& strLevel);
	void			GetActionString(S_POINT_PROCESS_TYPE Action, CString& strAction);
	void			GetTypeStringEx(EM_SCAN_TYPE type,EM_POINT_LEVEL level,CString& strItemName ,CString& strType);
	void			GetItemNameString(EM_SCAN_TYPE Type,CString& strSrcItemName,CString& strFileName,CString& strSubKey,CString& strItemName);
	void			GetDescStringEx(EM_SCAN_TYPE emType,EM_POINT_LEVEL emLevel, S_POINT_PROCESS_TYPE ActionType, 
									CString& strItemName,CString& strSrcDesc,CString& strDesc);
	void			GetDescString(EM_SCAN_TYPE type,CString& strSrcDesc,CString& strDesc);
	void			AliPayVirus( LPCTSTR pszVirusName, CString& strOutput );

	void			AddItem(EM_POINT_LEVEL Level);
	DWORD			GetItemIndex(EM_POINT_LEVEL Level);
	BOOL			IsRealVirus( EM_SCAN_TYPE nType, EM_POINT_LEVEL nLevel );

	//////////////////////////////////////////////////////////////////////////
	bool			IsEmergency();

	bool			SpTestAndStart();

	//////////////////////////////////////////////////////////////////////////
	BOOL			IsScanSPStarted();
	BOOL			IsSPMgrSPStarted();
	//////////////////////////////////////////////////////////////////////////

	// 定制版
	//////////////////////////////////////////////////////////////////////////
	HRESULT			GetCustomTitle(CString& strTitle);
	HRESULT			GetCustomMainPage(CString& strTitle, CString& strUrl);
	//////////////////////////////////////////////////////////////////////////

	// 检测云安全中心连接
	bool			IsSecurityCloudValid();

#ifndef IESCANENG_EXPORTS
	//修复网盾服务
	BOOL			CheckAndFixService();
	HRESULT			SetUserTrustList(const S_TRUST_LIST& trustList);
#endif

	IKBootCare*		getBootCare();
	BOOL	WaitQuickScanEngReadySingle();		//等待一次快扫引擎准备
	BOOL	InitStartScanState();
	DWORD	BcTypeToStartIndex( int nBcType );
	BOOL	WaitSVCStartSingle();				//等待一次服务启动

//关闭session
	//毒霸调用引擎
	HRESULT CloseSCanSession();
	HRESULT QueryScanStatus(S_KXE_SCAN_STATUS& ScanStatus );
	HRESULT KXEStartFullScan( BOOL bAutoClear );
	HRESULT KXEStartScan();
	HRESULT	KXEReadyCustomScan( BOOL bAutoClear );
	HRESULT KXEStartCustomScan( );
	HRESULT KXEAppendScanTargetPath(const std::wstring& strPath, BOOL bScanSubDir = TRUE);
	HRESULT KXEStopScan();
	HRESULT KXEPauseScan();
	HRESULT KXEResumeScanSession();
	HRESULT KXEQueryNeedReboot(BOOL& bReBoot);
	HRESULT KXEReportUnknown();
	HRESULT KXEGetUnknownFileList( std::vector<std::wstring>& vecUnknownFileReport );
	HRESULT KXEQueryProgressInfo(/*[out]*/S_KXE_PROGRESS_INFO& ScanProgress);

	HRESULT GetFileEngineOpt( BOOL& bAutoFix, BOOL& bScanArchieve );
	HRESULT SetFileEngineOpt( BOOL bAutoFix, BOOL bScanArchieve );
	HRESULT KXEStartFix( std::vector<DWORD>* pVecFixList, 
							std::vector<S_KXE_THREAT_PROCESS_RESULT>& ResultList );
	HRESULT QueryKXEFixList( const S_KXE_QUERY_THREAT& QueryFileThreat, 
							std::vector<S_KXE_FILEVIRUS_THREAT>& FileThreatList);
	int		KXEGetCleanFailCnt();

protected:

	//创建会话
	HRESULT _DoCreateScanSession(DWORD dwScanType = em_Target_Computer);
	//关闭会话
	HRESULT _DoCloseScanSession();
	HRESULT SetScanAutoClear( BOOL bAutoClear );
	HRESULT ProcessScanResult( const S_KXE_PROCESS_SCAN_TARGET& ProcessScanTarget);
	bool	KXESpTestAndStart();
private:
	DWORD							m_dwBlackIndex;
	DWORD							m_dwDoubtIndex;
	DWORD							m_dwDangerIndex;
	DWORD							m_dwAbnormalIndex;
	DWORD							m_dwRootkitIndex;
	DWORD							m_dwRiskIndex;
	DWORD							m_dwUnknownIndex;

	int								m_nSkip;
	int								m_nCleanFailed;
	bool							m_bEmergencyFlag;
	bool							m_bStopScan;

	int								m_nBcType;
	KExeCtrlLoader		m_loaderMulti;
	KBootCarePtr			m_pBookCare;

	//毒霸全盘和自定义
	IKxEScanService*    m_pScanService;
	IKxEScanServiceControl * m_pScanServiceControl;

	S_KXE_GENERICSCAN_HANDLE								m_ScanHandle;
	BOOL																		m_bInitKXESPFlag;
	static  S_KXE_GENERICSCAN_HANDLE				m_ExtScanHandle;
};


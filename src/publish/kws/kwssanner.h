#pragma once
#include "scan/KLogic.h"
#include <comproxy/bkscanfile.h>
//#include "bksafevirusscanner.h"


#define TIMER_ID_EXAMIN_PROCESS		20
#define TIMER_ID_CLEAN_PROCESS		21
#define TIMER_ID_KXE_SCAN_PROCESS	22
#define TIMER_ID_KXE_CLEAN_PROCESS	23

enum{
	KWS_TYPE_FASTSCAN = 0,
	KWS_TYPE_FULLSCAN,
	KWS_TYPE_CUSTOMSCAN,
};

class KwsScanner
{
public:
	KwsScanner( int nBcType = enum_BC_trojan )
		:m_hFastScan(NULL),
		m_hFullScan(NULL),
		m_hFixThread(NULL),
		m_nClean(0),
		m_nMayCleanCnt(0),
		m_dwScanTime(0),
		m_dwScanFile(0),
		m_dwUnknownCnt(0),
		m_dwCleanedCnt(0),
		m_nCurScanType(KWS_TYPE_FASTSCAN),
		m_dwFindVirus(0),
		m_dwBkVirusIndex(0),
		m_bExtend(FALSE),
		m_bMsgExt(FALSE),
		m_bCleanDoubt(FALSE),
		m_bHasMsgReport(FALSE),
		m_bKxeClean(FALSE),
		m_bFsClean(FALSE),
		m_nPluginFix(0),
		m_nPluginFind(0),
		m_nUnPeFix(0),
		m_nUnPeFind(0),
		m_nPeFix(0),
		m_nPeFind(0),
		m_hScanScript(NULL),
		m_hLoadScanScript(NULL),
		m_uCleanFinish(0),
		m_Logic(nBcType)
	{
		
	}

	~KwsScanner()
	{

	}

	static DWORD WINAPI PreLoadScriptThread(LPVOID pvParam)
	{
		KwsScanner* pScanner = (KwsScanner*)pvParam;
		if( pScanner )
		{
			pScanner->DoLoadScriptEng();
		}
		return 0;
	}

	static DWORD WINAPI ScanScriptThread(LPVOID pvParam)
	{
		KwsScanner* pScanner = (KwsScanner*)pvParam;
		if( pScanner )
		{
			pScanner->DoScanScript();
		}
		return 0;
	}

	static DWORD WINAPI FastScanThread(LPVOID pvParam)
	{
		KwsScanner* pScanner = (KwsScanner*)pvParam;
		if( pScanner )
		{
			pScanner->DoFastScan();
		}
		return 0;
	}

	static DWORD WINAPI FullScanThread(LPVOID pvParam)
	{
		KwsScanner* pScanner = (KwsScanner*)pvParam;
		if( pScanner )
		{
			pScanner->DoFullScan();
		}
		return 0;
	}

	static DWORD WINAPI FixThread(LPVOID pvParam)
	{
		KwsScanner* pScanner = (KwsScanner*)pvParam;
		if( pScanner )
		{
			pScanner->DoFixThread();
		}
		return 0;
	}

	void ScanPlugScript( HWND hWndNotify )
	{
		m_hWndNotify = hWndNotify;
		m_nCurScanType = KWS_TYPE_FASTSCAN;
		ResetInfo();

		if( m_hScanScript )
			CloseHandle( m_hScanScript );
		m_hScanScript = ::CreateThread(NULL, 0, ScanScriptThread, this, 0, NULL);
	}

	void LoadPlugScript( HWND hWndNotify )
	{
		if( m_hLoadScanScript )
			CloseHandle( m_hLoadScanScript );

		m_hLoadScanScript = ::CreateThread(NULL, 0, PreLoadScriptThread, this, 0, NULL);
	}

	void FastScan( HWND hWndNotify, BOOL bExtend )
	{
		m_hWndNotify = hWndNotify;

		m_nCurScanType = KWS_TYPE_FASTSCAN;
		m_bExtend = bExtend;

		ResetInfo();

		if( m_hFastScan )
			CloseHandle( m_hFastScan );

		m_hFastScan = ::CreateThread(NULL, 0, FastScanThread, this, 0, NULL);
	}

	void FullScan( HWND hWndNotify, UINT uNotifyMsg )
	{
		m_hWndNotify = hWndNotify;
		m_nCurScanType = KWS_TYPE_FULLSCAN;

		ResetInfo();

		if( m_hFullScan )
			CloseHandle( m_hFullScan );

		m_hFullScan = ::CreateThread(NULL, 0, FullScanThread, this, 0, NULL);
	}

	void FixStart( std::vector<S_FIX_ITEM>& FixList, HWND hWndNotify, UINT uCleanMsg )
	{
		m_FixList = FixList;
		m_hWndNotify = hWndNotify;
		m_uCleanFinish = uCleanMsg;
		m_hFixThread = ::CreateThread(NULL, 0, FixThread, this, 0, NULL);
	}

	HRESULT QuerySessionStatus(S_SCAN_STATUS& Status, std::vector<S_FIX_ITEM_EX>& FixList)
	{
		return m_Logic.QuerySessionStatus( Status, FixList );
	}
	//hub  大文件列表
	HRESULT QuerySessionCategoryDetail( std::vector<S_SCAN_CATEGORY_DETAIL>& CateList )
	{
		EM_SCAN_TYPE Type;//扫描类型
		if ( m_bExtend )//需要扩展扫描
		{
			Type = enum_All_With_Process_Expand_Scan;
		}
		else
		{
			Type =  enum_All;
		}
		return m_Logic.QuerySessionCategoryDetail( Type, CateList );
	}
	HRESULT QuerySubItemScanDetail( std::vector<S_SCAN_ITEM_DETAIL>& SubItemList )
	{
		return m_Logic.QuerySubItemScanDetail( SubItemList );
	}
	//hub 向后台进行服务器代理设置
	HRESULT SetCloudProxyInfo(KXE_CLOUD_PROXY_INFO& proxyInfo)
	{
		return  m_Logic.SetCloudProxyInfo(proxyInfo);
	}
	HRESULT QueryCloudProxyInfo(KXE_CLOUD_PROXY_INFO& proxyInfo)
	{
		return  m_Logic.SetCloudProxyInfo(proxyInfo);
	}
    HRESULT GetUnknownFileReport(std::vector<std::wstring>& vecUnknownFileReport)
    {
        return m_Logic.GetUnknownFileReport(vecUnknownFileReport);
    }

	HRESULT Stop()
	{
		HRESULT hr = E_FAIL;
		return hr;
	}

	HRESULT UnInit()
	{
		HRESULT hr = E_FAIL;

		if( m_hFastScan )
		{
			if( ::WaitForSingleObject( m_hFastScan, 100 ) != WAIT_OBJECT_0 )
				::TerminateThread( m_hFastScan, 0 );

			CloseHandle( m_hFastScan );
			m_hFastScan = NULL;
		}

		if( m_hFixThread )
		{
			if( ::WaitForSingleObject( m_hFixThread, 100 ) != WAIT_OBJECT_0 )
				::TerminateThread( m_hFixThread, 0 );

			CloseHandle( m_hFixThread );
			m_hFixThread = NULL;
		}

		if( m_hFullScan )
		{
			if( ::WaitForSingleObject( m_hFullScan, 100 ) != WAIT_OBJECT_0 )
				::TerminateThread( m_hFullScan, 0 );

			CloseHandle( m_hFullScan );
			m_hFullScan = NULL;
		}

		if( m_hScanScript )
		{
			if( ::WaitForSingleObject( m_hScanScript, 100 ) != WAIT_OBJECT_0 )
				::TerminateThread( m_hScanScript, 0 );

			CloseHandle( m_hScanScript );
			m_hScanScript = NULL;
		}

		if( m_hLoadScanScript )
		{
			if( ::WaitForSingleObject( m_hLoadScanScript, 100 ) != WAIT_OBJECT_0 )
				::TerminateThread( m_hLoadScanScript, 0 );

			CloseHandle( m_hLoadScanScript );
			m_hLoadScanScript = NULL;
		}

		hr = S_OK;
		return hr;
	}

	COLORREF GetLevelString(EM_POINT_LEVEL Level, CString& strLevel)
	{
		return m_Logic.GetLevelString( Level, strLevel );
	}
	
	void GetActionString(S_POINT_PROCESS_TYPE Action, CString& strAction)
	{
		m_Logic.GetActionString( Action, strAction );
	}

	DWORD GetItemIndex(EM_POINT_LEVEL Level)
	{
		return m_Logic.GetItemIndex( Level );
	}

	int GetCleanProgress()
	{
		if( m_nClean >= 0 && m_nClean <= 75 )
		{
			m_nClean += 2.5;
		}
		else if ( m_nClean > 75 && m_nClean < 99 )
		{
			m_nClean += 0.35;
		}
		else
			m_nClean = 99;

		return m_nClean;
	}

	BOOL NeedReBoot()
	{
		BOOL bReBoot = FALSE;
		HRESULT	hr = m_Logic.QueryNeedReboot(bReBoot);
		return bReBoot;
	}

	void SetHasMsgExt( BOOL bMsgExt )
	{
		m_bMsgExt = bMsgExt;
	}

	BOOL HasCleanedDoubt()
	{
		return m_bCleanDoubt;
	}

	void SetCleanedDoubt( BOOL bDoubt )
	{
		m_bCleanDoubt = bDoubt;
	}

	BOOL GetHasMsgExt( )
	{
		return m_bMsgExt;
	}

	void SetMayClean( int nCnt )
	{
		m_nMayCleanCnt = nCnt;
	}

	int GetMayClean()
	{
		return m_nMayCleanCnt;
	}

	void SetScanFile( DWORD dwFile )
	{
		m_dwScanFile = dwFile;
	}

	DWORD GetScanFile()
	{
		return m_dwScanFile;
	}

	void SetScanTime( DWORD dwTime )
	{
		m_dwScanTime = dwTime;
	}

	DWORD GetScanTime()
	{
		return m_dwScanTime;
	}

	void SetUnknownCnt( DWORD dwCnt )
	{
		m_dwUnknownCnt = dwCnt;
	}

	DWORD GetUnknownCnt()
	{
		return m_dwUnknownCnt;	
	}

	void SetCleanedCnt( DWORD dwCnt )
	{
		m_dwCleanedCnt = dwCnt;
	}

	DWORD GetCleanedCnt()
	{
		return m_dwCleanedCnt;
	}

	int GetScanType()
	{
		return m_nCurScanType;
	}

	void SetScanType( int nType )
	{
		m_nCurScanType = nType;
	}

	int GetFastScanProgress()
	{
		if( m_nCurScanType == KWS_TYPE_FASTSCAN )
			return 100;

		if( m_nCurScanType == KWS_TYPE_FULLSCAN )
			return 20;

		return 0;
	}

	int GetFullScanProgress()
	{
		if( m_nCurScanType == KWS_TYPE_FULLSCAN )
			return 80;

		if( m_nCurScanType == KWS_TYPE_CUSTOMSCAN )
			return 100;

		return 100;
	}

	void ResetInfo()
	{
		m_nClean		= 0;
		m_nMayCleanCnt	= 0;
		m_dwScanTime	= 0;
		m_dwScanFile	= 0;
		m_dwUnknownCnt	= 0;
		m_dwCleanedCnt	= 0;
		m_dwFindVirus	= 0;
		m_dwBkVirusIndex = 0;
		m_bKxeClean		= FALSE;
		m_bFsClean		= FALSE;
		m_bCleanDoubt	 = FALSE;
		m_bHasMsgReport	= FALSE;
		m_nPluginFix	= 0;
		m_nPluginFind	= 0;
		m_nUnPeFix		= 0;
		m_nUnPeFind		= 0;
		m_nPeFix		= 0;
		m_nPeFind		= 0;
	}

	void SetHasMsgReport( BOOL bMsg )
	{	
		m_bHasMsgReport = bMsg;
	}

	BOOL GetHasMsgReport( )
	{
		return m_bHasMsgReport;
	}

	DWORD GetFindVirusCnt()
	{
		return m_dwFindVirus;
	}

	void SetFindVirusCnt( DWORD dwCnt )
	{
		m_dwFindVirus = dwCnt;
	}

	void SetBkVirusIndex( int nIndex )
	{
		m_dwBkVirusIndex = nIndex;
	}

	int GetBkVirusIndex()
	{
		return m_dwBkVirusIndex;
	}

	BOOL BkToKwsItem( BK_FILE_INFO& info, S_FIX_ITEM_EX& Item )
	{
		BOOL bRet = TRUE;

		Item.strItemName = info.FileResult.VirusName;
		Item.emType = enum_BK_TROJAN_Point;
		Item.dwID = (DWORD)-1;							//表示为贝壳数据转换来的
		Item.Advice =  enum_PROCESS_TYPE_CLEAN;
		Item.emLevel = enum_POINT_LEVEL_TROJAN;
		Item.strFile = info.FileResult.FileName;
		Item.uIndex = info.Index;
		if( (SLPTL_LEVEL_MARK___LOCAL_AVENGINE == info.FileResult.SecLevel) )
			Item.scanEngineType = enum_SCAN_ENGINE_FILEENGINE_TYPE;
		else
			Item.scanEngineType = enum_SCAN_ENGINE_BEIKE_TYPE;

		return bRet;
	}

	void PauseScan()
	{
		if( m_nCurScanType != KWS_TYPE_CUSTOMSCAN )
			m_Logic.PauseScan();
	}

	void StopScan()
	{
		if( m_nCurScanType != KWS_TYPE_CUSTOMSCAN )
			m_Logic.StopScan();
	}

	void ResumeScan()
	{
		if( m_nCurScanType != KWS_TYPE_CUSTOMSCAN )
			m_Logic.ResumeScan();
	}

	void AddWhiteUrlList( std::vector<std::wstring>& vecUrl )
	{
		S_TRUST_LIST TrustList;
		TrustList.operation = enum_TRUST_LIST_ADD;
		TrustList.itemType  = enum_TRUST_ITEM_TYPE_URL;
		TrustList.vecItemList = vecUrl;

		HRESULT lRetCode = m_Logic.SetUserTrustList(TrustList);
	}

	void AddWhiteFileList( std::vector<std::wstring>& vecFile )
	{
		S_TRUST_LIST TrustList;
		TrustList.operation = enum_TRUST_LIST_ADD;
		TrustList.itemType  = enum_TRUST_ITEM_TYPE_FILE;
		TrustList.vecItemList = vecFile;

		HRESULT lRetCode = m_Logic.SetUserTrustList(TrustList);
	}

	void AddWhiteFileList( LPCTSTR pszFile )
	{
		S_TRUST_LIST TrustList;
		TrustList.operation = enum_TRUST_LIST_ADD;
		TrustList.itemType  = enum_TRUST_ITEM_TYPE_FILE;
		TrustList.vecItemList.push_back( std::wstring(pszFile) );

		HRESULT lRetCode = m_Logic.SetUserTrustList(TrustList);
	}

	BOOL IsRealVirus( EM_SCAN_TYPE nType, EM_POINT_LEVEL nLevel )
	{
		return m_Logic.IsRealVirus( nType, nLevel );
	}

	BOOL IsExtScan()
	{
		return m_bExtend;
	}

	void SetCleanType( BOOL bFastScanClean, BOOL bKxeClean )
	{
		m_bFsClean = bFastScanClean;
		m_bKxeClean = bKxeClean;
	}

	BOOL GetKxeClean( )
	{
		return m_bKxeClean;
	}
	void AddOnePluginFix()
	{
		m_nPluginFix++;
	}

	void AddOnePeFix()
	{
		m_nPeFix++;
	}
	void AddOneUnPeFix()
	{
		m_nUnPeFix++;
	}

	void AddOnePlugFind()
	{
		m_nPluginFind++;
	}

	void AddOnePeFind()
	{
		m_nPeFind++;
	}

	void AddOneUnPeFind()
	{
		m_nUnPeFind++;
	}

	int GetPluginFix()
	{
		return m_nPluginFix;
	}

	int GetPluginFind()
	{
		return m_nPluginFind;
	}

	int GetPeFix()
	{
		return m_nPeFix;
	}

	int GetPeFind()
	{
		return m_nPeFind;
	}

	int GetUnPeFix()
	{
		return m_nUnPeFix;
	}

	int GetUnPeFind()
	{
		return m_nUnPeFind;
	}

	void GetTypeString(EM_SCAN_TYPE Type, CString& strType)
	{
		m_Logic.GetTypeString( Type, strType );
	}

	void CloseSession()
	{
		m_Logic.CloseSession();
	}

protected:

	void DoFastScan()
	{
		
		HRESULT lRetCode = E_FAIL;

		if( m_bExtend )
			lRetCode = m_Logic.StartScan( enum_All_With_Process_Expand_Scan );
		else
			lRetCode = m_Logic.StartScan( enum_All );
		
		if (SUCCEEDED(lRetCode) && m_hWndNotify )
		{
			::SetTimer(m_hWndNotify, TIMER_ID_EXAMIN_PROCESS, 100, NULL);
		}
		else
			ATLASSERT( FALSE );
	}

	void DoLoadScriptEng()
	{
		for ( int iIndex= 0; iIndex < 200; iIndex++)
		{
			if ( m_Logic.WaitSVCStartSingle() )
				break;

			Sleep(100);
		}
	}

	void DoScanScript()
	{
		if( !m_hLoadScanScript )
			return;

		if( WaitForSingleObject( m_hLoadScanScript, 30000 ) == WAIT_OBJECT_0 )
		{
			LRESULT lRetCode = m_Logic.StartScan( enum_System_Repair_Diagnosis );
			if ( FAILED(lRetCode) )
			{
				ATLASSERT( FALSE );
			}

			if( SUCCEEDED(lRetCode) && m_hWndNotify )
				::SetTimer( m_hWndNotify, XX_PLUG_PROGRESS_SCAN_TIMER_ID, 100, NULL );
		}
	}

	void DoFullScan()
	{

	}

	void DoFixThread()
	{
		//网盾修复借口
		m_nClean = 0;
		HRESULT lRetCode = m_Logic.StartFix( m_FixList );
		if (SUCCEEDED(lRetCode) && m_hWndNotify && m_uCleanFinish )
		{
			::PostMessage( m_hWndNotify, m_uCleanFinish, m_FixList.size(), NULL );
		}
	}


protected:

	HWND	m_hWndNotify;
	HANDLE	m_hFastScan;
	HANDLE	m_hFixThread;
	HANDLE	m_hScanScript;
	HANDLE	m_hLoadScanScript;
	HANDLE	m_hFullScan;
	KLogic	m_Logic;
	double	m_nClean;
	int		m_nMayCleanCnt;
	DWORD	m_dwScanTime;
	DWORD	m_dwScanFile;
	DWORD	m_dwUnknownCnt;
	DWORD	m_dwCleanedCnt;
	int		m_nCurScanType;
	DWORD	m_dwFindVirus;
	int		m_dwBkVirusIndex;
	BOOL	m_bExtend;
	BOOL	m_bMsgExt;					//避免一次扫描多次弹出是否询问扩展扫描
	BOOL	m_bCleanDoubt;
	BOOL	m_bHasMsgReport;
	int		m_nPluginFix;
	int		m_nPluginFind;
	int		m_nUnPeFix;
	int		m_nUnPeFind;
	int		m_nPeFix;
	int		m_nPeFind;
	UINT	m_uCleanFinish;

	BOOL	m_bFsClean;
	BOOL	m_bKxeClean;

	std::vector< S_FIX_ITEM > m_FixList;
	
};
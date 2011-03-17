
// dummyz@126.com

#pragma once

#include <atlcoll.h>
#include <atlstr.h>
#include "safemon/safemonitor.h"
class IPEFile;
class CScanServer;

class CHeuristic
{
public:
#ifdef _KSFMON_DLL_
	// 不提供白名单过滤，这个调用可能发生在其他进程空间。
	// 只使用监控的扩展模块
	static BOOL Scan(IN LPCTSTR lpFilePath,
		IN LPCTSTR lpParam, // 可以为空
		IN LPCTSTR lpHostPath, // 可以为空
		OUT LPTSTR lpVirusName, // 可以为空
		OUT LPTSTR lpVirusPath, // 可以为空
		OUT SM_FILE_ENGINE& dwFileEngine,
		IN OUT DWORD& dwFileType
		);

	static DWORD	Clean(LPCTSTR lpFilePath, LPCTSTR lpVirusName, DWORD dwFileType, DWORD dwCtrl, CScanServer& scanServer);
#endif
	static BOOL		GetVirusDummyName(LPCTSTR lpVirusName, LPTSTR lpDummyName);
};

//
//  监控使用的接口
//
BOOL	WScript_CheckRisk(CAtlArray<CString>& fileArray, LPCTSTR lpHostPath);
BOOL	BlankName_CheckRisk(LPCTSTR lpFilePath);
BOOL	DoubleExt_CheckRisk(LPCTSTR lpFilePath);
BOOL	SexNameTrick_CheckRisk(LPCTSTR lpFilePath);

enum enumHeuRiskLevel
{
	enumHeuRiskLevelNone = 0,
	enumHeuRiskLevelLow = 1,
	enumHeuRiskLevelHig = 2
};
DWORD	XRar_CheckRisk(LPCTSTR lpFilePath);
DWORD	LnkFile_CheckRisk(LPCTSTR lpFilePath);
DWORD	TaobaoNameTrick_CheckRisk(LPCTSTR lpFilePath, LPCTSTR lpHostPath);

//
// 下载保护使用中的接口
//
enum enumBindSetupType
{
	enumBST_IEPlugin	= 1,
	enumBST_IEHomePage	= 2,
	enumBST_ThirdSetup	= 4,

	enumBST_ALL			= enumBST_IEPlugin | enumBST_IEHomePage | enumBST_ThirdSetup
};
DWORD	BindSetup_Guesser(IPEFile* lpFile);
BOOL	BindSetup_CheckRisk(LPCTSTR lpFilePath, LPTSTR lpVirusName);
BOOL	TaobaoNameTrick_CheckRisk(LPCTSTR lpFilePath, LPCTSTR lpArchiveFile, BOOL bArchiveFile); // 下载保护使用

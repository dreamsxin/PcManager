// dummyz@126.com

#pragma once

#include "PEParser.h"
#include "PEiDentifier.h"

class CPEJudger
{
public:
	enum RiskLevel
	{
		RiskLevelUnknown = 0,
		RiskLevelLow = 1,
		RiskLevelMid = 2,
		RiskLevelHig = 3
	};

	enum RiskType
	{
		RiskTypeUnknown = 0,
		RiskTypeTrojan = 1,
		RiskTypeVirus = 2
	};

public:
	CPEJudger();
	virtual ~CPEJudger();

	// MAKELONG(RiskType, RiskLevel)
	DWORD			GuessRiskLevel(CPEParser* lpParser, LPTSTR lpName);

private:
	static DWORD	GuessPackerOrVirus(CPEParser* lpParser); // MAKELONG(nScorePacker, nScoreVirus)
#ifdef _PEDISAM_
	static DWORD	GuessVirusByOep(CPEParser* lpParser); // 入口特征 猜测是否是木马、病毒
#endif

public:
	static BOOL		IsRiskIcon(CPEParser* lpParser); // 图标是文件夹，图片
	static BOOL		IsSingleIcon(CPEParser* lpParser); // 只有一个 Icon 资源

	static BOOL		IsOverlayBindExe(CPEParser* lpParser); // 附加数据是否捆绑 exe
	static BOOL		IsResourceBindExe(CPEParser* lpParser); // 资源数据是否捆绑 exe

	static BOOL		ScanPEStruct(CPEParser* lpParser, ULONG lFilePointer);

public:
	BOOL	__IsSkipFile(CPEParser* lpParser);
};
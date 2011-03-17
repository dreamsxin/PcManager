// dummyz@126.com

#pragma once

enum {
	VC_Linker		= 1,
	VB_Linker		= 2,
	Delphi_Linker	= 4,
	Eyuyan_Linker	= 8
};

#include "PEParser.h"
#include <vector>
#include <string>

class CPEiDentifier
{
public:
	CPEiDentifier();
	virtual ~CPEiDentifier();

	BOOL	Load(LPCTSTR lpFilePath);
	void	Unload();

	LPCTSTR Scan(LPCTSTR lpFilePath);
	LPCTSTR	Scan(CPEParser* lpParser);

	// 通过外貌特征判断
	static BOOL		IsVCLink(CPEParser* lpParser);
	static BOOL		IsVBLink(CPEParser* lpParser); // 连接器是否是 vb
	static BOOL		IsDelphiLink(CPEParser* lpParser); // 连接器是否是 delphi
	static BOOL		IsEyuyanLink(CPEParser* lpParser); // 连接起是否是 易语言
	static BOOL		IsDotNetLink(CPEParser* lpParser);

	static BOOL		IsRiskPacker(CPEParser* lpParser);
	static BOOL		IsNormalPacker(CPEParser* lpParser);

	static BOOL		IsResourcePacked(CPEParser* lpParser);
	static BOOL		IsImportPacked(CPEParser* lpParser);
	static BOOL		IsSectionPacked(CPEParser* lpParser);

	static BOOL		IsResourceBin(CPEParser* lpParser); 
	

	// 获取文件生成相关信息
	static BOOL		GetLinkerInfo(CPEParser* lpParser, BOOL& bNotPacker, DWORD& dwLinker);
	static BOOL		IsAntiVirusCleaned(CPEParser* lpParser); // 是否是杀毒软件清除过的文件

protected:
	LPCTSTR	ScanOep(CPEParser* lpParser);
	LPCTSTR ScanNoOep(CPEParser* lpParser);
	
protected:
	struct SIGNATURE
	{
		std::wstring		strName;
		std::vector<WORD>	data;
		PWORD				lpwPointer;
	};

	DWORD					m_dwOepMaxLen;
	DWORD					m_dwNoOepMaxLen;

	std::list<SIGNATURE>	m_OepSignaList;
	std::list<SIGNATURE>	m_NoOepSignaList;
};
#include "stdafx.h"
#include "KSoftMgrUpdateInfo.h"
#include <miniutil/bkini.h>
using namespace BKIni;
#include <fstream>
#include <algorithm>
using namespace std;

namespace ksm
{

// 配置信息
static const wchar_t sIniFile[]		= L"Cfg\\bksoftmgr.ini";
static const wchar_t sTipSec[]		= L"update_tip";
static const wchar_t sTipType[]		= L"tip_type";
static const wchar_t sTipTime[]		= L"tip_time";
// 缺省提示类型
static const int	 sDefTipType	= 1;

// 路径结尾加反斜线
static inline CString _PathAddBackslash(const CString &path);
//
static bool operator<(const UpdateInfo &left, const UpdateInfo &right) 
{ return right._rank < left._rank; }
//////////////////////////////////////////////////////////////////////////

BOOL CUpdateInfoMgr::LoadIgnore(const CString &kSafePath)
{
	if(kSafePath.IsEmpty()) return FALSE;

	Document doc;
	CString strPath;
	strPath = GetDataFilePath(kSafePath);
	strPath.Append(L"_ignore");
	HRESULT hr = doc.LoadFile(strPath);
	if(!SUCCEEDED(hr)) return FALSE;

	m_updateInfoArrayIgnore.RemoveAll();

	for(Section sec = doc.FindFirstSection(); !sec.IsNull(); sec = sec.FindNextSection())
	{
		UpdateInfo ui;

		if(!sec.HasKey(L"id")) continue;
		ui._id = static_cast<int>(sec[L"id"].GetValueDWORD());

		if(!sec.HasKey(L"rank")) continue;
		ui._rank = static_cast<int>(sec[L"rank"].GetValueDWORD());

		if(!sec.HasKey(L"name")) continue;
		ui._name = sec[L"name"].GetValueString();

		if(!sec.HasKey(L"date")) continue;
		ui._date = sec[L"date"].GetValueString();

		if(!sec.HasKey(L"cur_ver")) continue;
		ui._curVer = sec[L"cur_ver"].GetValueString();

		if(!sec.HasKey(L"up_ver")) continue;
		ui._upVer = sec[L"up_ver"].GetValueString();

		if(!sec.HasKey(L"size")) continue;
		ui._dwSize = static_cast<int>(sec[L"size"].GetValueDWORD());

		if(!sec.HasKey(L"attri")) continue;
		ui._attri = static_cast<int>(sec[L"attri"].GetValueDWORD());

		if( sec.HasKey(L"mainexe") )
			ui._mainexe = sec[L"mainexe"].GetValueString();

		m_updateInfoArrayIgnore.Add(ui);
	}

	return TRUE;
}

BOOL CUpdateInfoMgr::SaveIgnore(const CString &kSafePath)
{
	if(kSafePath.IsEmpty()) return FALSE;

	CString strFilePath;
	strFilePath = GetDataFilePath(kSafePath);
	strFilePath.Append(L"_ignore");

	{
		ofstream os(strFilePath);
		if(!os) return FALSE;

		for(int i = 0; i < m_updateInfoArrayIgnore.GetSize(); ++i)
		{
			const UpdateInfo &ui = m_updateInfoArrayIgnore[i];

			os<<"[soft"		<<i							<<"]\n";
			os<<"id="		<<ui._id					<<'\n';
			os<<"name="		<<(LPCSTR)CW2A(ui._name)	<<'\n';
			os<<"rank="		<<ui._rank					<<'\n';
			os<<"date="		<<(LPCSTR)CW2A(ui._date)	<<'\n';
			os<<"cur_ver="	<<(LPCSTR)CW2A(ui._curVer)	<<'\n';
			os<<"up_ver="	<<(LPCSTR)CW2A(ui._upVer)	<<'\n';
			os<<"size="		<<ui._dwSize				<<'\n';
			os<<"attri="	<<ui._attri					<<'\n';
			os<<"mainexe="	<<(LPCSTR)CW2A(ui._mainexe)	<<'\n';
		}
	}

	if (m_updateInfoArrayIgnore.GetSize() == 0)
		::DeleteFile(strFilePath);

	return TRUE;
}

BOOL CUpdateInfoMgr::IsIgnoreItem(UpdateInfo& ui)
{
	for(int i = 0; i < m_updateInfoArrayIgnore.GetSize(); ++i)
	{
		const UpdateInfo &uiIgnore = m_updateInfoArrayIgnore[i];
		if (uiIgnore._upVer == ui._upVer)
			return TRUE;
	}

	return FALSE;
}

BOOL CUpdateInfoMgr::IsMajorItem(UpdateInfo& ui)
{
	if ((ui._attri&SA_Major) == SA_Major)
		return TRUE;

	return FALSE;
}

BOOL CUpdateInfoMgr::AddIgnorItem(const UpdateInfo &ui)
{
	BOOL bNeedAdd = TRUE;
	for(int i = 0; i < m_updateInfoArrayIgnore.GetSize(); ++i)
	{
		UpdateInfo &uiIgnore = m_updateInfoArrayIgnore[i];
		if (uiIgnore._id == ui._id && uiIgnore._upVer != ui._upVer)
		{
			uiIgnore._curVer = ui._curVer;
			uiIgnore._date = ui._date;
			uiIgnore._id = ui._id;
			uiIgnore._name = ui._name;
			uiIgnore._rank = ui._rank;
			uiIgnore._upVer = ui._upVer;
			uiIgnore._dwSize = ui._dwSize;
			uiIgnore._attri = ui._attri;
			uiIgnore._mainexe = ui._mainexe;
			bNeedAdd = FALSE;
		}
	}

	if (bNeedAdd)
		m_updateInfoArrayIgnore.Add(ui);

	return TRUE;
}

BOOL CUpdateInfoMgr::Load(const CString &kSafePath, BOOL bOnlyMajor = FALSE, LPCWSTR pszMainExe = NULL)
{
	LoadIgnore(kSafePath);

	if(kSafePath.IsEmpty()) return FALSE;

	Document doc;
	HRESULT hr = doc.LoadFile(GetDataFilePath(kSafePath));
	if(!SUCCEEDED(hr)) return FALSE;

	m_updateInfoArray.RemoveAll();

	// 加载软件更新列表
	for(Section sec = doc.FindFirstSection(); !sec.IsNull(); sec = sec.FindNextSection())
	{
		UpdateInfo ui;

		if(!sec.HasKey(L"id")) continue;
		ui._id = static_cast<int>(sec[L"id"].GetValueDWORD());

		if(!sec.HasKey(L"rank")) continue;
		ui._rank = static_cast<int>(sec[L"rank"].GetValueDWORD());

		if(!sec.HasKey(L"name")) continue;
		ui._name = sec[L"name"].GetValueString();

		if(!sec.HasKey(L"date")) continue;
		ui._date = sec[L"date"].GetValueString();

		if(!sec.HasKey(L"cur_ver")) continue;
		ui._curVer = sec[L"cur_ver"].GetValueString();

		if(!sec.HasKey(L"up_ver")) continue;
		ui._upVer = sec[L"up_ver"].GetValueString();

		if(!sec.HasKey(L"size")) continue;
		ui._dwSize = static_cast<int>(sec[L"size"].GetValueDWORD());

		if(!sec.HasKey(L"attri")) continue;
		ui._attri = static_cast<int>(sec[L"attri"].GetValueDWORD());

		if(!sec.HasKey(L"mainexe")) continue;
			ui._mainexe = sec[L"mainexe"].GetValueString();

		if (bOnlyMajor && !IsMajorItem(ui))
			continue;

		if (!bOnlyMajor && (pszMainExe && wcslen(pszMainExe)))
		{
			if (ui._mainexe.CompareNoCase(pszMainExe) != 0)
				continue;
		}

		if (!IsIgnoreItem(ui))
			m_updateInfoArray.Add(ui);
	}

	// 按Rank进行排序
	if(m_updateInfoArray.GetSize() != 0)
	{
		std::sort(m_updateInfoArray.GetData(), m_updateInfoArray.GetData() + m_updateInfoArray.GetSize());
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CUpdateInfoMgr::Save(const CString &kSafePath)
{
	if(kSafePath.IsEmpty()) return FALSE;

	CString strFilePath;
	strFilePath = GetDataFilePath(kSafePath);

	{
		ofstream os(strFilePath);
		if(!os) return FALSE;

		// 保存软件更新列表
		for(int i = 0; i < m_updateInfoArray.GetSize(); ++i)
		{
			const UpdateInfo &ui = m_updateInfoArray[i];

			os<<"[soft"		<<i							<<"]\n";
			os<<"id="		<<ui._id					<<'\n';
			os<<"name="		<<(LPCSTR)CW2A(ui._name)	<<'\n';
			os<<"rank="		<<ui._rank					<<'\n';
			os<<"date="		<<(LPCSTR)CW2A(ui._date)	<<'\n';
			os<<"cur_ver="	<<(LPCSTR)CW2A(ui._curVer)	<<'\n';
			os<<"up_ver="	<<(LPCSTR)CW2A(ui._upVer)	<<'\n';
			os<<"size="		<<ui._dwSize				<<'\n';
			os<<"attri="	<<ui._attri					<<'\n';
			os<<"mainexe="	<<(LPCSTR)CW2A(ui._mainexe) <<'\n';
		}
	}

	if (m_updateInfoArray.GetSize() == 0)
		::DeleteFile(strFilePath);
	
	return TRUE;
}

CString CUpdateInfoMgr::GetDataFilePath(const CString &kSafePath)
{
	return _PathAddBackslash(kSafePath) + L"AppData\\ksoft_update_tip";
}

CString CUpdateInfoMgr::GetSoftIdByMainExe(const CString &szMainExe)
{
	CString strResult;

	for(int i = 0; i < m_updateInfoArray.GetSize(); ++i)
	{
		if (m_updateInfoArray[i]._mainexe.CompareNoCase(szMainExe) == 0)
		{
			strResult.Format(L"%d", m_updateInfoArray[i]._id);
			break;
		}
	}

	return strResult;
}

BOOL CUpdateInfoMgr::Need2TipOneUpdate(const CString &kSafePath, CUpdateInfoMgr &updateInfoMgr, BOOL bOnlyMajor)
{
	// 加载提示选项，默认需要提醒，修改请同步到bksafeconfig.h
	CString iniPath = _PathAddBackslash(kSafePath) + sIniFile;
	int tipChoice = ::GetPrivateProfileInt(L"major_update_tip", L"tip_type", 1, iniPath);
	if(tipChoice == 0) 
	{
		// 不提示
		return FALSE;
	}

	WCHAR szMainExe[MAX_PATH] = { 0 };
	if (!bOnlyMajor)
	{
		::GetPrivateProfileString(L"major_update_tip", _T("mainexe"), _T(""), szMainExe, MAX_PATH, iniPath);
		if (wcslen(szMainExe) == 0)
			return FALSE;
	}

	__time32_t curTime = _time32(NULL);
	if (bOnlyMajor)
	{
		// 加载最后提示时间
		__time32_t updateTime = static_cast<__time32_t>(
			::GetPrivateProfileInt(L"major_update_tip", L"tip_time", 0, iniPath)
			);

		// 小于一天不提示
		if(curTime - updateTime < 24*60*60) 
			return FALSE;
	}

	// 加载更新列表
	if(!updateInfoMgr.Load(kSafePath, bOnlyMajor, szMainExe)) return FALSE;

	if (bOnlyMajor)
	{
		// 更新升级提示时间
		CString strCurTime;	strCurTime.Format(L"%d", curTime);
		::WritePrivateProfileString(L"major_update_tip", L"tip_time", strCurTime, iniPath);
	}
	else
	{
		CString strSoftId;
		strSoftId = updateInfoMgr.GetSoftIdByMainExe(szMainExe);

		// 加载最后提示时间
		__time32_t updateTime = static_cast<__time32_t>(
			::GetPrivateProfileInt(L"major_update_tip", strSoftId, 0, iniPath)
			);

		// 小于一天不提示
		if(curTime - updateTime < 24*60*60) 
			return FALSE;

		// 更新升级提示时间
		CString strCurTime;	
		strCurTime.Format(L"%d", curTime);
		::WritePrivateProfileString(L"major_update_tip", strSoftId, strCurTime, iniPath);
	}

	return TRUE;
}

BOOL CUpdateInfoMgr::Need2TipUpdate(const CString &kSafePath, CUpdateInfoMgr &updateInfoMgr)
{
	// 加载提示选项
	CString iniPath = _PathAddBackslash(kSafePath) + sIniFile;
	int tipChoice = ::GetPrivateProfileInt(sTipSec, sTipType, sDefTipType, iniPath);

	if(tipChoice == 2) 
	{
		// 不提示
		return FALSE;
	}

	// 加载最后提示时间
	__time32_t curTime = _time32(NULL);
	__time32_t updateTime = static_cast<__time32_t>(
		::GetPrivateProfileInt(sTipSec, sTipTime, 0, iniPath)
		);

	if(tipChoice == 0)
	{
		// 每天提示
		if(curTime - updateTime < 24*60*60) return FALSE;
	}
	else
	{
		// 每周提示
		if(curTime - updateTime < 7*24*60*60) return FALSE;
	}

	// 加载更新列表
	if(!updateInfoMgr.Load(kSafePath)) return FALSE;

	// 加载成功后，删除记录文件，防止重复提示
	//::DeleteFileW(CUpdateInfoMgr::GetDataFilePath(kSafePath));

	// 更新升级提示时间
	CString strCurTime;	strCurTime.Format(L"%d", curTime);
	::WritePrivateProfileString(sTipSec, sTipTime, strCurTime, iniPath);

	return TRUE;
}

int CUpdateInfoMgr::GetTipType(const CString &kSafePath)
{
	CString iniPath = _PathAddBackslash(kSafePath) + sIniFile;
	return ::GetPrivateProfileInt(sTipSec, sTipType, sDefTipType, iniPath);
}

void CUpdateInfoMgr::SetTipType(const CString &kSafePath, int type)
{
	CString strTipType;	strTipType.Format(L"%d", type);
	CString iniPath = _PathAddBackslash(kSafePath) + sIniFile;
	::WritePrivateProfileString(sTipSec, sTipType, strTipType, iniPath);
}

inline CString _PathAddBackslash(const CString &path)
{
	if(
		path.IsEmpty() ||
		(path[path.GetLength() - 1] != '\\' && path[path.GetLength() - 1] != '/')
		)
	{
		return path + L'\\';
	}

	return path;
}

}
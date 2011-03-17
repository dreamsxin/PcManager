#pragma once

#include <atlsimpcoll.h>
#include <time.h>

namespace ksm
{

//
// 升级信息
//
typedef struct UpdateInfo
{
	int		_id;
	int		_rank;
	CString _name;
	CString _date;
	CString	_curVer;
	CString _upVer;
	CString _mainexe;
	DWORD	_dwSize;
	ULONG	_attri;	
} *PUpdateInfo;
typedef const UpdateInfo* PCUpdateInfo;
typedef CSimpleArray<UpdateInfo> UpdateInfoArray;

// 软件属性
enum SoftAttri : ULONG
{
	SA_None				= (0<<0),
	SA_Beta				= (1<<0),
	SA_Green			= (1<<1),
	SA_Major			= (1<<2),
};

//
// 软件升级信息
//
class CUpdateInfoMgr
{
public:
	// 得到数据文件路径
	static CString GetDataFilePath(const CString &kSafePath);
	// 是否需要提示升级单个软件
	static BOOL Need2TipOneUpdate(const CString &kSafePath, CUpdateInfoMgr &updateInfoMgr, BOOL bOnlyMajor);
	// 是否需要提示升级多个软件
	static BOOL Need2TipUpdate(const CString &kSafePath, CUpdateInfoMgr &updateInfoMgr);
	// 获取/设置提示类型信息
	static int GetTipType(const CString &kSafePath);
	static void SetTipType(const CString &kSafePath, int type);

public:
	BOOL Load(const CString &kSafePath, BOOL bOnlyMajor /*= FALSE*/, LPCWSTR pszMainExe /*= NULL*/);
	BOOL Save(const CString &kSafePath);

	BOOL LoadIgnore(const CString &kSafePath);
	BOOL SaveIgnore(const CString &kSafePath);
	BOOL AddIgnorItem(const UpdateInfo &ui);

	const UpdateInfoArray& GetUpdateInfoArray() const;

	void ClsUpdateInfo();

	void AddUpdateInfo(const UpdateInfo &ui);
	void AddUpdateInfo(int id, int rank, const CString &name, const CString &date, 
		const CString &curVer, const CString &upVer, const DWORD &dwSize, const ULONG attri, const CString& strMainExe);

protected:
	BOOL IsIgnoreItem(UpdateInfo& ui);
	BOOL IsMajorItem(UpdateInfo& ui);

	CString GetSoftIdByMainExe(const CString &szMainExe);
private:
	UpdateInfoArray m_updateInfoArray;
	UpdateInfoArray m_updateInfoArrayIgnore;
};
//////////////////////////////////////////////////////////////////////////
inline void CUpdateInfoMgr::ClsUpdateInfo()
{
	m_updateInfoArray.RemoveAll();
}

inline const UpdateInfoArray& CUpdateInfoMgr::GetUpdateInfoArray() const
{
	return m_updateInfoArray;
}

inline void CUpdateInfoMgr::AddUpdateInfo(const UpdateInfo &ui)
{
	m_updateInfoArray.Add(ui);
}

inline void CUpdateInfoMgr::AddUpdateInfo(int id, int rank, const CString &name, const CString &date, 
									   const CString &curVer, const CString &upVer, const DWORD &dwSize,
									   const ULONG attri, const CString& strMainExe)
{
	UpdateInfo ui;
	{
		ui._id = id;
		ui._name = name;
		ui._rank = rank;
		ui._date = date;
		ui._curVer = curVer;
		ui._upVer = upVer;
		ui._dwSize = dwSize;
		ui._attri = attri;
		ui._mainexe = strMainExe;
	}
	AddUpdateInfo(ui);
}

}

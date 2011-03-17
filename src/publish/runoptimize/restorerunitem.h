
#pragma once

#include <runoptimize/interface.h>
#include <common/utility.h>
#include <common/markup.h>
#include <safemon/safetrayshell.h>

#define IS_SAME_BOOL(x,y)	((x&&y)||(!x&&!y))
#define RESTORE_INI_PATH	_T("ksrunlog2.log")
#define DELETE_INI_PATH	_T("ksrundel.log")
#define DISABLE_INI_PATH	_T("ksrundis.log")

class CDelHistory
{
public:
	struct DelLogItem
	{
		DWORD		nType;
		DWORD		nType2;
		CString		strName;
	};

	CDelHistory()
	{
		m_bDirty = FALSE;


		CAppPath::Instance().GetLeidianCfgPath(m_strFileName,TRUE);
		m_strFileName.Append(_T("\\"));
		m_strFileName.Append(DELETE_INI_PATH);
		Load();
	}
	virtual ~CDelHistory()
	{
		Save();
	}

protected:
	CString	m_strFileName;
	BOOL	m_bDirty;
#define STR_XML_ROOT		_T("xml")
#define STR_XML_ITEM		_T("item")
#define STR_ATTRIB_TYPE		_T("type")
#define STR_ATTRIB_TYPE2	_T("type2")
#define STR_ATTRIB_TYPE3	_T("type3")
#define STR_ATTRIB_NAME		_T("namex")
#define STR_ATTRIB_NAME2	_T("name2")

	BOOL Load()
	{
		ATLASSERT(m_logList.GetSize()==0);
		m_bDirty = FALSE;

		CMarkup	markup;
		if (markup.Load(m_strFileName) && markup.FindElem(STR_XML_ROOT))
		{
			markup.IntoElem();
			while (markup.FindElem(STR_XML_ITEM))
			{
				DelLogItem	pItem;
				pItem.nType		= _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE));
				pItem.nType2	= _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE2));
				pItem.strName	= markup.GetAttrib(STR_ATTRIB_NAME);

				if (!pItem.strName.IsEmpty())
					m_logList.Add(pItem);
			}
			markup.OutOfElem();
		}
		return TRUE;
	}

	BOOL Save()
	{
		if (m_bDirty)
		{
			::DeleteFile(m_strFileName);

			SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_UNICODE)); 
			if ( m_logList.GetSize()>0 )
			{
				CMarkup	markup;

				markup.InsertElem(STR_XML_ROOT);
				markup.IntoElem();

				for (int i=0; i<m_logList.GetSize(); i++)
				{
					DelLogItem*	pItem = &m_logList[i];
					markup.InsertElem(STR_XML_ITEM);
					markup.SetAttrib(STR_ATTRIB_TYPE,pItem->nType);
					markup.SetAttrib(STR_ATTRIB_TYPE2,pItem->nType2);
					markup.SetAttrib(STR_ATTRIB_NAME,pItem->strName);
				}
				markup.OutOfElem();
				markup.Save(m_strFileName);
			}
		}

		return TRUE;
	}
public:

	static CDelHistory* GetPtr()
	{	
		static CDelHistory	run;
		return &run;
	}

	BOOL FindDelRunItem(DWORD nType, LPCTSTR lpstrName)
	{
		for (int i=0; i<m_logList.GetSize(); i++)
		{
			DelLogItem&	log = m_logList[i];

			if (log.nType == KSRUN_TYPE_STARTUP)
			{
				if ( IsSameRunType(nType, log.nType2) )
				{
					if ( IsTypeRunReg(nType) )
					{
						if (log.strName.CompareNoCase(lpstrName) == 0)
							return TRUE;
					}
					else
					{
						CString strName1 = GetFileName(lpstrName);
						CString strName2 = GetFileName(log.strName);

						if (strName1.CompareNoCase(strName2)==0)
							return TRUE;
					}
				}

			}
		}

		return FALSE;
	}

	void LogDelRun(DWORD nType, LPCTSTR lpstrName)
	{
		m_bDirty = TRUE;

		DelLogItem	item;
		item.nType	= KSRUN_TYPE_STARTUP;
		item.nType2	= nType;
		item.strName= lpstrName;

		m_logList.Add(item);
	}

	void LogDelService(LPCTSTR lpservName)
	{
		return;

		DelLogItem	item;
		item.nType	= KSRUN_TYPE_SERVICE;
		item.strName= lpservName;
		m_logList.Add(item);
	}

	BOOL FindDelTaskItem(DWORD nType, LPCTSTR lpstrName)
	{
		for (int i=0; i<m_logList.GetSize(); i++)
		{
			DelLogItem&	log = m_logList[i];

			if (log.nType == KSRUN_TYPE_TASK)
			{
				if (log.nType2 == nType )
				{
					if ( log.strName.CompareNoCase(lpstrName) == 0)
						return TRUE;
				}
			}
		}
		return FALSE;
	}

	BOOL FindDelSrvItem(DWORD nType, LPCTSTR lpstrName)
	{
		for (int i=0; i<m_logList.GetSize(); i++)
		{
			DelLogItem&	log = m_logList[i];

			if (log.nType == KSRUN_TYPE_SERVICE)
			{
				if (log.nType2 == nType )
				{
					if ( log.strName.CompareNoCase(lpstrName) == 0)
						return TRUE;
				}
			}
		}
		return FALSE;
	}

	void LogDelTask(DWORD nType, LPCTSTR lpstrName)
	{
		m_bDirty = TRUE;

		DelLogItem	item;
		item.nType	= KSRUN_TYPE_TASK;
		item.nType2	= nType;
		item.strName= lpstrName;
		m_logList.Add(item);
	}

protected:
	CSimpleArray<DelLogItem>	m_logList;
};

class RunLogItem
{
public:
	RunLogItem()
	{
		nExtType= 0;
		bSystem = FALSE;
		nID		= 0;
		strDate = "-";
		nType	= 0;
		nInitState = 0;
		dwNewState = 0;
	}
	virtual ~RunLogItem()
	{}

	DWORD	nType;			// 类型
	DWORD	nInitState;		// 初始化状态
	CString	strName;		// 标记名字
	CString	strName2;		// 辅助标记名字
	DWORD	nExtType;
	BOOL	bSystem;		// 是否是系统分类
	DWORD	nID;			// ID
	CString	strDate;		// 优化日期
	DWORD   dwNewState;     // 设置标记，用于支持区分延迟启动还是禁止启动
	CString strSubKey;		// 子键
};


class IEnumLogCallBack
{
public:
	virtual BOOL EnumLog(RunLogItem* pItem, PVOID lParam) = 0;
};

class CRestoreRunner
{
public:
	CRestoreRunner()
	{
		m_bDirty = FALSE;

		CAppPath::Instance().GetLeidianCfgPath(m_strFileName,TRUE);

		m_strFileName.Append(_T("\\"));
		m_strFileName.Append(RESTORE_INI_PATH);
		Load();
	}
	virtual ~CRestoreRunner()
	{
		Save();
		_free();
	}

	static CRestoreRunner* GetPtr()
	{	
		static CRestoreRunner	run;
		return &run;
	}	

protected:

#define STR_XML_ROOT		_T("xml")
#define STR_XML_ITEM		_T("item")
#define STR_ATTRIB_TYPE		_T("type")
#define STR_ATTRIB_TYPE2	_T("type2")
#define STR_ATTRIB_TYPE3	_T("type3")
#define STR_ATTRIB_TYPE4	_T("type4")
#define STR_ATTRIB_TYPE5	_T("type5")
#define STR_ATTRIB_NAME		_T("namex")
#define STR_ATTRIB_NAME2	_T("name2")
#define STR_ATTRIB_NAME3	_T("name3")
#define STR_ATTRIB_NAME4	_T("name4")
#define STR_ATTRIB_NAME5	_T("name5")

	BOOL Load()
	{

		CAtlMap<CString, RunLogItem*	>::CPair *pName = NULL;

		ATLASSERT(m_logList.GetSize()==0);
		m_bDirty = FALSE;
		m_nDelayCount = 0;

		CMarkup	markup;
		if (markup.Load(m_strFileName) && markup.FindElem(STR_XML_ROOT))
		{
			markup.IntoElem();
			while (markup.FindElem(STR_XML_ITEM))
			{
				RunLogItem*	pItem = new RunLogItem;
				pItem->nType	  = _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE));
				pItem->nInitState = _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE2));
				pItem->nExtType	  = _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE3));
				pItem->bSystem	  = _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE4));
				pItem->strName	  = markup.GetAttrib(STR_ATTRIB_NAME);
				pItem->strName2	  = markup.GetAttrib(STR_ATTRIB_NAME2);
				pItem->nID		  = _ttoi(markup.GetAttrib(STR_ATTRIB_NAME3));
				pItem->strDate	  = markup.GetAttrib(STR_ATTRIB_NAME4);
				pItem->strSubKey  = markup.GetAttrib(STR_ATTRIB_NAME5);
				CString cNewFlag = markup.GetAttrib(STR_ATTRIB_TYPE5);
				if (cNewFlag == _T(""))
				{
					pItem->dwNewState = KSRUN_START_DISABELE;
				}
				else 
				{
					pItem->dwNewState = _ttoi(cNewFlag.GetString());
				}
				
				if (pItem->dwNewState == KSRUN_START_DELAY)
				{
					m_nDelayCount++;
				}

				if (pItem->nType != KSRUN_TYPE_SYSCFG)
				{
					CString cTmp;
					cTmp = pItem->strName;
					cTmp.Append(markup.GetAttrib(STR_ATTRIB_TYPE));

					pName = m_mapLogItem.Lookup(cTmp.GetString());
					if (!pName)
					{
						m_mapLogItem.SetAt(cTmp.GetString(), pItem);
						if (!pItem->strName.IsEmpty())
							m_logList.Add(pItem);
						else
							delete pItem;
					}
				}
				else
					m_logList.Add(pItem);
			}
			markup.OutOfElem();
		}
		return TRUE;
	}

	BOOL Save()
	{
		SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_UNICODE)); 
		if (m_bDirty)
		{
			//::DeleteFile(m_strFileName);
			
			if ( m_logList.GetSize()>0 )
			{
				CMarkup	markup;

				markup.InsertElem(STR_XML_ROOT);
				markup.IntoElem();

				for (int i=0; i<m_logList.GetSize(); i++)
				{
					RunLogItem*	pItem = m_logList[i];
					markup.InsertElem(STR_XML_ITEM);
					markup.SetAttrib(STR_ATTRIB_TYPE,pItem->nType);
					markup.SetAttrib(STR_ATTRIB_TYPE2,pItem->nInitState);
					markup.SetAttrib(STR_ATTRIB_TYPE3,pItem->nExtType);
					markup.SetAttrib(STR_ATTRIB_TYPE4,pItem->bSystem);
					markup.SetAttrib(STR_ATTRIB_TYPE5,pItem->dwNewState);
					markup.SetAttrib(STR_ATTRIB_NAME,pItem->strName);
					markup.SetAttrib(STR_ATTRIB_NAME2,pItem->strName2);
					markup.SetAttrib(STR_ATTRIB_NAME3,pItem->nID);
					markup.SetAttrib(STR_ATTRIB_NAME4,pItem->strDate);
					markup.SetAttrib(STR_ATTRIB_NAME5,pItem->strSubKey);
				}
				markup.OutOfElem();
				markup.Save(m_strFileName);
				CSafeMonitorTrayShell::RunOptDelHisUpdated();
			}
			else
			{
				CMarkup	markup;
				markup.InsertElem(STR_XML_ROOT);
				markup.Save(m_strFileName);
				CSafeMonitorTrayShell::RunOptDelHisUpdated();
			}
		}

		return TRUE;
	}

public:
	void OperateService(LPCTSTR lpServName, DWORD nID,DWORD nInitState, DWORD bNewState, BOOL bSystem)
	{
		m_bDirty = TRUE;

		BOOL	bFindLog = FALSE;
		for (int i = m_logList.GetSize() - 1; i >= 0; i--)
		{
			RunLogItem*	plogItem = m_logList[i];

			if (nID != 0 && plogItem->nID != 0)
			{
				if (plogItem->nID == nID)
				{
					bFindLog = TRUE;
					if (bNewState)
					{
						delete plogItem;
						m_logList.RemoveAt(i);
					}
				}
			}
			else
			{
				if (plogItem->nType==KSRUN_TYPE_SERVICE && plogItem->strName.CompareNoCase(lpServName)==0 )
				{
					bFindLog = TRUE;
					if (IS_SAME_BOOL(plogItem->nInitState,bNewState) && bNewState)
					{
						delete plogItem;
						m_logList.RemoveAt(i);
					}
				}
			}
		}
		if (!bFindLog && bNewState != TRUE)
		{
			RunLogItem*	plogItem = new RunLogItem;
			plogItem->strName	 = lpServName;
			plogItem->nType		 = KSRUN_TYPE_SERVICE;
			plogItem->nInitState = nInitState;
			plogItem->dwNewState = bNewState;
			plogItem->bSystem	 = bSystem;
			plogItem->nID		 = nID;
			plogItem->strDate	 = GetCurrrentDate();
			m_logList.Add(plogItem);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//参数说明：
	//		strName：		启动项名称
	//		nID：			启动项ID
	//		nInitRegType：	操作之前的注册表类型
	//		nNewRegType：	操作之后的注册表类型
	//		bNewState：		操作之后的状态
	//		bSystem：		是否系统类型
	//////////////////////////////////////////////////////////////////////////
	void OperateRun(CString strName, DWORD nID,DWORD nInitRegType, DWORD nNewRegType, DWORD bNewState,BOOL bSystem)
	{
		m_bDirty = TRUE;

		BOOL	bFindLog = FALSE;
		for (int i = m_logList.GetSize() - 1; i >= 0; i--)
		{
			RunLogItem*	plogItem = m_logList[i];
			
			if (nID != 0 && plogItem->nID != 0)	//如果不是未知项，就用ID匹配
			{
				if (plogItem->nID == nID)
				{
					bFindLog = (bNewState == KSRUN_START_ENABLE || bNewState == KSRUN_START_DELAY_FAIL);
					delete plogItem;
					m_logList.RemoveAt(i);
				}
			}
			else	//如果是未知项，没有id，就用strName和nType匹配
			{
				if ( plogItem->nType == KSRUN_TYPE_STARTUP 
					 && plogItem->strName.CompareNoCase(strName) == 0 )
				{
					bFindLog = (bNewState == KSRUN_START_ENABLE || bNewState == KSRUN_START_DELAY_FAIL);
					delete plogItem;
					m_logList.RemoveAt(i);
				}
			}
		}
		if (!bFindLog && (bNewState == KSRUN_START_DISABELE || bNewState == KSRUN_START_DELAY))
		{
			RunLogItem*	plogItem = new RunLogItem;
			plogItem->strName	 = strName;
			plogItem->nType		 = KSRUN_TYPE_STARTUP;
			plogItem->nInitState = nInitRegType;
			plogItem->dwNewState = bNewState;
			plogItem->nExtType	 = nNewRegType;
			plogItem->bSystem	 = bSystem;
			plogItem->nID		 = nID;
			plogItem->strDate	 = GetCurrrentDate();
			plogItem->strName2	 = GetCurrentUserForSID();

			m_logList.Add(plogItem);
		}
	}

	void OperateTaskJob(LPCTSTR lpJobPath,DWORD nID,DWORD nJobType, BOOL bInitEnable, DWORD bNewEnable, BOOL bSystem)
	{
		m_bDirty = TRUE;

		BOOL	bFindLog = FALSE;
		for (int i = m_logList.GetSize() - 1; i >= 0; i--)
		{
			RunLogItem*	plogItem = m_logList[i];
			
			if (nID != 0 && plogItem->nID != 0)
			{
				if (plogItem->nID == nID)
				{
					bFindLog = TRUE;
					if (bNewEnable)
					{
						delete plogItem;
						m_logList.RemoveAt(i);
					}
				}
			}
			else
			{
				if (plogItem->nType==KSRUN_TYPE_TASK && plogItem->strName.CompareNoCase(lpJobPath)==0 && plogItem->nExtType==nJobType)
				{
					bFindLog = TRUE;
					if ( IS_SAME_BOOL(plogItem->nInitState,bNewEnable) && bNewEnable)
					{
						delete plogItem;
						m_logList.RemoveAt(i);
					}
				}
			}
		}
		if (!bFindLog && bNewEnable != TRUE)
		{
			RunLogItem*	plogItem = new RunLogItem;
			plogItem->strName	 = lpJobPath;
			plogItem->nType		 = KSRUN_TYPE_TASK;
			plogItem->nInitState = bInitEnable;
			plogItem->dwNewState = bNewEnable;
			plogItem->nExtType	 = nJobType;
			plogItem->bSystem	 = bSystem;
			plogItem->nID		 = nID;
			plogItem->strDate	 = GetCurrrentDate();
			m_logList.Add(plogItem);
		}
	}

	void OperateSysCfg(DWORD dwID, DWORD dwType, DWORD dwFlags, CString strName, CString strSubKey, CString strInitValue)
	{
		m_bDirty = TRUE;

		BOOL	bFindLog = FALSE;
		for (int i = m_logList.GetSize() - 1; i >= 0; i--)
		{
			RunLogItem*	plogItem = m_logList[i];
			if (plogItem->nType == dwType 
				&& plogItem->nID == dwID
				&& plogItem->strName.CompareNoCase(strName) == 0
				&& plogItem->strSubKey.CompareNoCase(strSubKey) == 0)
			{
				bFindLog = TRUE;
				if (dwFlags != KSAFE_SYSCONFIG_OPTIMIZE)
				{
					delete plogItem;
					m_logList.RemoveAt(i);
				}
			}
		}
		if (!bFindLog && dwFlags == KSAFE_SYSCONFIG_OPTIMIZE)
		{
			RunLogItem*	plogItem = new RunLogItem;
			plogItem->strName	 = strName;
			plogItem->nType		 = KSRUN_TYPE_SYSCFG;
			plogItem->strName2	 = strInitValue;
			plogItem->nID		 = dwID;
			plogItem->strDate	 = GetCurrrentDate();
			plogItem->strSubKey  = strSubKey;
			m_logList.Add(plogItem);	
		}
	}

	LPTSTR GetCurrentUserForSID () 
	{
		DWORD dwSize = 0, dwResult = 0;
		HANDLE hToken;
		PTOKEN_USER pUserInfo;
		LPTSTR lpszSid = NULL;

		if (!OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken )) 
			goto Exit0;

		if(!GetTokenInformation(hToken, TokenUser, NULL, dwSize, &dwSize)) 
		{
			dwResult = GetLastError();
			if( dwResult != ERROR_INSUFFICIENT_BUFFER )
				goto Exit0;
		}

		pUserInfo = (PTOKEN_USER) GlobalAlloc( GPTR, dwSize );

		if(! GetTokenInformation(hToken, TokenUser, pUserInfo, 
			dwSize, &dwSize ) ) 
		{
			goto Exit0;
		}

		ConvertSidToStringSid(pUserInfo->User.Sid, &lpszSid);

		if ( pUserInfo )
			GlobalFree( pUserInfo );
Exit0:
		return lpszSid;
	}

	void EnumItem(IEnumLogCallBack* pCallback, PVOID lParam)
	{
		for (int i=0; i<m_logList.GetSize(); i++)
		{
			RunLogItem*	plogItem = m_logList[i];
			if ( !pCallback->EnumLog(plogItem,lParam) )
			{
				m_bDirty = TRUE;
				m_logList.RemoveAt(i);
				delete plogItem;
				i--;
			}
		}
	}

	BOOL FindItem(LPCTSTR lpName, DWORD dwType, DWORD dwExtType)
	{
		BOOL bResult = FALSE;

		const DWORD dwCount = m_logList.GetSize();
		for ( DWORD i = 0; i < dwCount; i++ )
		{
			const RunLogItem* p = m_logList[i];
			if ( p->nType == dwType && IsSameRunType(p->nInitState, dwExtType))
			{
				if ( p->strName.CompareNoCase(lpName) == 0 )
				{
					bResult = TRUE;
					break;
				}
			}
		}

		return bResult;
	}

	int GetCount()
	{
		return m_logList.GetSize();
	}

	RunLogItem* GetStartUpItem(LPCTSTR lpName)
	{
		CAtlMap<CString, RunLogItem*>::CPair *pPair = NULL;
		CString cKey;
		CString cTmp;

		cTmp.Format(_T("%i"), KSRUN_TYPE_STARTUP);

		cKey = lpName;
		cKey.Append(cTmp.GetString());

		pPair = m_mapLogItem.Lookup(cKey);

		if (pPair)
		{
			return pPair->m_value;
		}
		else
			return NULL;
	}

	RunLogItem* GetItem(int i)
	{
		return m_logList[i];
	}

	CString GetCurrrentDate()
	{
		SYSTEMTIME timeCur;
		GetLocalTime(&timeCur);
		CString strDate;
		strDate.Format(TEXT("%04d-%02d-%02d"), timeCur.wYear, timeCur.wMonth, timeCur.wDay);
		return strDate;
	}

	BOOL DeleteItemFromLog(LPCTSTR lpName, DWORD dwType,DWORD dwID)
	{
		BOOL bResult = FALSE;
		int nCount = m_logList.GetSize();
		for ( int i = nCount - 1; i >= 0 ; i-- )
		{
			RunLogItem* p = m_logList[i];
			if (dwID == p->nID && dwID != 0)
			{
				bResult = TRUE;
				m_logList.RemoveAt(i);
				m_bDirty = TRUE;
				delete p;
			}
			else
			{
				if(( p->nType == dwType )&&( p->strName.CompareNoCase(lpName) == 0 ))
				{
					bResult = TRUE;
					m_logList.RemoveAt(i);
					m_bDirty = TRUE;
					delete p;
				}
			}
		}
		return bResult;
	}

	BOOL HasRunDelay()
	{
		return m_nDelayCount > 0;
	}

protected:
	void _free()
	{
		for (int i=0; i<m_logList.GetSize(); i++)
		{
			delete m_logList[i];
		}
		m_logList.RemoveAll();
	}

protected:
	CSimpleArray<RunLogItem*>	m_logList;
	BOOL						m_bDirty;
	CString						m_strFileName;
	CAtlMap<CString, RunLogItem*> m_mapLogItem;
	DWORD						m_nDelayCount;
};

class CDisableList
{
public:
	CDisableList()
	{
		m_bDirty = FALSE;

		CAppPath::Instance().GetLeidianCfgPath(m_strFileName,TRUE);

		m_strFileName.Append(_T("\\"));
		m_strFileName.Append(DISABLE_INI_PATH);
		Load();
	}
	virtual ~CDisableList()
	{
		Save();
		_free();
	}

	static CDisableList* GetPtr()
	{	
		static CDisableList	run;
		return &run;
	}	

protected:

#define STR_XML_ROOT		_T("xml")
#define STR_XML_ITEM		_T("item")
#define STR_ATTRIB_TYPE		_T("type")
#define STR_ATTRIB_TYPE2	_T("type2")
#define STR_ATTRIB_TYPE3	_T("type3")
#define STR_ATTRIB_TYPE4	_T("type4")
#define STR_ATTRIB_TYPE5	_T("type5")
#define STR_ATTRIB_NAME		_T("namex")
#define STR_ATTRIB_NAME2	_T("name2")
#define STR_ATTRIB_NAME3	_T("name3")
#define STR_ATTRIB_NAME4	_T("name4")

	BOOL Load()
	{

		CAtlMap<CString, RunLogItem*>::CPair *pName = NULL;
		m_bDirty = FALSE;

		CMarkup	markup;
		if (markup.Load(m_strFileName) && markup.FindElem(STR_XML_ROOT))
		{
			markup.IntoElem();
			while (markup.FindElem(STR_XML_ITEM))
			{
				RunLogItem*	pItem = new RunLogItem;
				pItem->nType	  = _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE));
				pItem->nInitState = _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE2));
				pItem->nExtType	  = _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE3));
				pItem->bSystem	  = _ttoi(markup.GetAttrib(STR_ATTRIB_TYPE4));
				pItem->strName	  = markup.GetAttrib(STR_ATTRIB_NAME);
				pItem->strName2	  = markup.GetAttrib(STR_ATTRIB_NAME2);
				pItem->nID		  = _ttoi(markup.GetAttrib(STR_ATTRIB_NAME3));
				pItem->strDate	  = markup.GetAttrib(STR_ATTRIB_NAME4);

				CString cNewFlag = markup.GetAttrib(STR_ATTRIB_TYPE5);
				if (cNewFlag == _T(""))
				{
					pItem->dwNewState = KSRUN_START_DISABELE;
				}
				else 
				{
					pItem->dwNewState = _ttoi(cNewFlag.GetString());
				}

				CString cTmp;
				CString cNum;

				cNum.Format(_T("%i"), pItem->nType);

				cTmp = pItem->strName;
				cTmp.Append(cNum.GetString());

				if (pItem->nType == KSRUN_TYPE_TASK)
				{
					pName = mName.Lookup(cTmp.GetString());
					if (!pName)
					{
						mName.SetAt(cTmp.GetString(), pItem);
					}
					else
						delete pItem;
				}
				else if(pItem->nType == KSRUN_TYPE_SERVICE)
				{
					pName = msName.Lookup(cTmp.GetString());
					if (!pName)
					{
						msName.SetAt(cTmp.GetString(), pItem);
					}
					else
						delete pItem;
				}

			}
			markup.OutOfElem();
		}
		return TRUE;
	}

	BOOL Save()
	{
		SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_UNICODE)); 
		if (m_bDirty)
		{
			::DeleteFile(m_strFileName);

			CAtlMap<CString, RunLogItem*>::CPair *pName = NULL;
			POSITION Pos = mName.GetStartPosition();
			CMarkup	markup;

			markup.InsertElem(STR_XML_ROOT);
			markup.IntoElem();

			while (Pos)
			{
				pName = mName.GetNext(Pos);

				RunLogItem*	pItem = pName->m_value;
				markup.InsertElem(STR_XML_ITEM);
				markup.SetAttrib(STR_ATTRIB_TYPE,pItem->nType);
				markup.SetAttrib(STR_ATTRIB_TYPE2,pItem->nInitState);
				markup.SetAttrib(STR_ATTRIB_TYPE3,pItem->nExtType);
				markup.SetAttrib(STR_ATTRIB_TYPE4,pItem->bSystem);
				markup.SetAttrib(STR_ATTRIB_TYPE5,pItem->dwNewState);
				markup.SetAttrib(STR_ATTRIB_NAME,pItem->strName);
				markup.SetAttrib(STR_ATTRIB_NAME2,pItem->strName2);
				markup.SetAttrib(STR_ATTRIB_NAME3,pItem->nID);
				markup.SetAttrib(STR_ATTRIB_NAME4,pItem->strDate);
			}

			Pos = msName.GetStartPosition();
			while (Pos)
			{
				pName = msName.GetNext(Pos);

				RunLogItem*	pItem = pName->m_value;
				markup.InsertElem(STR_XML_ITEM);
				markup.SetAttrib(STR_ATTRIB_TYPE,pItem->nType);
				markup.SetAttrib(STR_ATTRIB_TYPE2,pItem->nInitState);
				markup.SetAttrib(STR_ATTRIB_TYPE3,pItem->nExtType);
				markup.SetAttrib(STR_ATTRIB_TYPE4,pItem->bSystem);
				markup.SetAttrib(STR_ATTRIB_TYPE5,pItem->dwNewState);
				markup.SetAttrib(STR_ATTRIB_NAME,pItem->strName);
				markup.SetAttrib(STR_ATTRIB_NAME2,pItem->strName2);
				markup.SetAttrib(STR_ATTRIB_NAME3,pItem->nID);
				markup.SetAttrib(STR_ATTRIB_NAME4,pItem->strDate);
			}

			markup.OutOfElem();
			markup.Save(m_strFileName);
		}

		return TRUE;
	}

public:
	void OperateService(LPCTSTR lpServName, DWORD nID,DWORD nInitState, DWORD bNewState, BOOL bSystem)
	{
		m_bDirty = TRUE;
		RunLogItem*	plogItem = new RunLogItem;
		plogItem->strName	 = lpServName;
		plogItem->nType		 = KSRUN_TYPE_SERVICE;
		plogItem->nInitState = nInitState;
		plogItem->dwNewState = bNewState;
		plogItem->bSystem	 = bSystem;
		plogItem->nID		 = nID;
		msName.SetAt(lpServName, plogItem);
		
	}

	void OperateRun(LPCTSTR lpFileName, DWORD nID,DWORD nInitType, DWORD nNewType, DWORD bNewState,BOOL bSystem)
	{
		m_bDirty = TRUE;

		RunLogItem*	plogItem = new RunLogItem;
		plogItem->strName	 = lpFileName;
		plogItem->nType		 = KSRUN_TYPE_STARTUP;
		plogItem->nInitState = nInitType;
		plogItem->dwNewState = bNewState;
		plogItem->nExtType	 = nNewType;
		plogItem->bSystem	 = bSystem;
		plogItem->nID		 = nID;
		mName.SetAt(lpFileName, plogItem);
	}

	void OperateTaskJob(LPCTSTR lpJobPath,DWORD nID,DWORD nJobType, BOOL bInitEnable, DWORD bNewEnable, BOOL bSystem)
	{
		m_bDirty = TRUE;
		RunLogItem*	plogItem = new RunLogItem;
		plogItem->strName	 = lpJobPath;
		plogItem->nType		 = KSRUN_TYPE_TASK;
		plogItem->nInitState = bInitEnable;
		plogItem->dwNewState = bNewEnable;
		plogItem->nExtType	 = nJobType;
		plogItem->bSystem	 = bSystem;
		plogItem->nID		 = nID;
		mName.SetAt(lpJobPath, plogItem);
	}

	//是否需要回避的计划任务
	int IsIgnore(LPCTSTR lpcName)
	{
		CAtlMap<CString, RunLogItem*>::CPair *pName = NULL;	

		CString cTest;
		CString cNum;

		cNum.Format(_T("%i"), KSRUN_TYPE_TASK);
		cTest = lpcName;
		cTest.Append(cNum);

		pName = mName.Lookup(cTest);

		if (pName)
			return TRUE;
		else 
			return FALSE;
	}

	//是否需要延迟启动的服务
	int IsDelayRunService(LPCTSTR lpcName)
	{
		CAtlMap<CString, RunLogItem*>::CPair *pName = NULL;	
		CString cTest;
		CString cNum;

		cNum.Format(_T("%i"), KSRUN_TYPE_SERVICE);
		cTest = lpcName;
		cTest.Append(cNum);

		pName = msName.Lookup(cTest);

		if (pName)
			return TRUE;
		else 
			return FALSE;
	}

protected:
	void _free()
	{

		CAtlMap<CString, RunLogItem*>::CPair *pName = NULL;
		POSITION Pos = mName.GetStartPosition();

		while (Pos)
		{
			pName = mName.GetNext(Pos);

			RunLogItem*	pItem = pName->m_value;
			delete pItem;
		}
	}

protected:
	CAtlMap<CString, RunLogItem*> mName;
	CAtlMap<CString, RunLogItem*> msName;
	BOOL						m_bDirty;
	CString						m_strFileName;
};
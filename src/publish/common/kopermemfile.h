////////////////////////////////////////////////////////////////////////////////
//            
//      File      : kopermemfile.h
//      Version   : 1.0
//      
//      Create at : 2011-1-27
//		Motify by : 
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "sharememlock.h"
#define DEFSHAREMEMLOCKEVENTNAME		_T("{D5551A46-BFB0-4fd6-A280-EBD7AAD4397E}_flow")


typedef struct stElemShare 
{
	int m_nDayLimited;				//日达到该数据流量时，提醒
	int m_nMonthLimited;			//月达到该数据流量时，提醒
	int m_nDayLast;					//日对话框最后一次显示的时间。
	int m_nMonthLast;				//月对话框最后一次显示的时间
	int m_nDayMark;					//用户选择本日不提醒时记录的时间
	int m_nMonthMark;				//用户选择本月不提醒时记录的时间
	int	m_nMonthWait;				//当日和当月同时达到弹框条件时。某框先弹时的时间。
	int m_nHasOpenMoniter;			//是否打开监控，如果打开监控了。则一秒钟写一次数据。
	int m_nIsWriteDB;				//是否写了数据库

	LONGLONG m_nAllMoniterTime;		//记录的今天的上传的监控时间
	LONGLONG m_lUpData;				//记录的今天的上传的数据的开始值
	LONGLONG m_lDownData;			//记录的今天的下载的数据的开始值
	LONGLONG m_lAllData;			//记录的所有的上传+下载数据的开始值
	

	//ShareMemLock	m_shareLock;	//锁

}MemShareFlowElem;


class KOperMemFile
{
private:
	HANDLE				m_hFileMap;
	LPVOID				m_pBufView;
	BOOL				m_bIsInitSuccess;


private:
	KOperMemFile():m_hFileMap(NULL),
		m_pBufView(NULL),
		m_bIsInitSuccess(FALSE)
	{
	}

	~KOperMemFile()
	{
		Uninit();
	}

public:

	BOOL IsInitHad(void)
	{
		return m_bIsInitSuccess;
	}

	HRESULT	Init(BOOL bOnlyRead = FALSE)
	{
		HANDLE hFileMap = NULL;
		BYTE* pMapBuf = NULL;
		SIZE_T sizeToMap = {0};

		
		if (m_hFileMap != NULL && m_pBufView != NULL)
			return S_OK;
	/*	else
		{
			//如果文件已经打开，则关闭
			if (m_pBufView)
			{
				::UnmapViewOfFile(m_pBufView);
				m_pBufView = NULL;
			}

			if (m_hFileMap)
			{
				::CloseHandle(m_hFileMap);
				m_hFileMap = NULL;
			}
		}
*/
		UINT uMark = PAGE_READWRITE;
		if (bOnlyRead)
			uMark = PAGE_READONLY;
		
		BOOL bIsNeedInitLocker = FALSE;
		hFileMap = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, uMark, 0, sizeof(MemShareFlowElem), DEFSHAREMEMLOCKEVENTNAME);
		if (NULL == hFileMap)
		{
			printf("OpenFileMapping failed: %d\n", ::GetLastError());
			goto exit0;
		}

		if (GetLastError() == ERROR_ALREADY_EXISTS)
			bIsNeedInitLocker = TRUE;

		pMapBuf = (BYTE*)::MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if (!pMapBuf)
		{
			printf("MapViewOfFile failed: %d\n", ::GetLastError());
			goto exit0;
		}

		m_hFileMap = hFileMap;
		m_pBufView = pMapBuf;

/*		if (bIsNeedInitLocker)
		{
			MemShareFlowElem* pElem = (MemShareFlowElem *)m_pBufView;
			pElem->m_shareLock.InitLock();
		}
*/
		m_bIsInitSuccess = TRUE;
		return S_OK;

exit0:
		if (pMapBuf)
		{
			::UnmapViewOfFile(pMapBuf);
			pMapBuf = NULL;
		}

		if (hFileMap)
		{
			::CloseHandle(hFileMap);
			hFileMap = NULL;
		}

		return E_FAIL;
	}

	HRESULT	Uninit()
	{
		if (m_pBufView && m_hFileMap)
		{
			::UnmapViewOfFile(m_pBufView);
			m_pBufView = NULL;
		}
		if (m_hFileMap)
		{
			::CloseHandle(m_hFileMap);
			m_hFileMap = NULL;
		}

		return S_OK;
	}

	static KOperMemFile& Instance()
	{
		static KOperMemFile _ins;
		return	_ins;
	}

public:


	void SetAllData(int nDayData, int nMonthData, int nDayLastData, int nMonthLastData, int nDayMark, int nMonthMark, int nWaitTime)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem *)m_pBufView;
	//	pElem->m_shareLock.LockWrite();
		pElem->m_nDayLimited = nDayData;
		pElem->m_nMonthLimited = nMonthData;
		pElem->m_nDayLast = nDayLastData;
		pElem->m_nMonthLast = nMonthLastData;
		pElem->m_nDayMark = nDayMark;
		pElem->m_nMonthMark = nMonthMark;
		pElem->m_nMonthWait = nWaitTime;
	//	pElem->m_shareLock.UnLockWrite();
	}

	int GetDayLimited(void)
	{
		return _GetData(0);
	}

	void SetDayLimited(int nDayLimited)
	{
		_SetData(0, nDayLimited);
	}

	int GetMonthLimited(void)
	{
		return _GetData(1);
	}

	void SetMonthLimited(int nMonthLimited)
	{
		_SetData(1, nMonthLimited);
	}

	int GetDayLastData(void)
	{
		return _GetData(2);
	}

	void SetDayLastData(int nLastData)
	{
		_SetData(2, nLastData);
	}

	int GetMonthLastData(void)
	{
		return _GetData(3);
	}

	void SetMonthLastData(int nLastData)
	{
		_SetData(3, nLastData);
	}

	int GetDayMark(void)
	{
		return _GetData(4);
	}

	void SetDayMark(int nMark)
	{
		_SetData(4, nMark);
	}

	int GetMonthMark(void)
	{
		return _GetData(5);
	}

	void SetMonthMark(int nMark)
	{
		_SetData(5, nMark);
	}

	int GetMonthWait(void)
	{
		return _GetData(6);
	}

	void SetMonthWait(int nWait)
	{
		_SetData(6, nWait);
	}

	int GetWaitMoniterOpen(void)
	{
		return _GetData(7);
	}

	void SetWaitMoniterOpen(int nWait)
	{
		_SetData(7, nWait);
	}

	int GetIsWriteDB(void)
	{
		return _GetData(8);
	}

	void SetIsWriteDB(int nWait)
	{
		_SetData(8, nWait);
	}


	//获取当前的数据
	int GetAllMoniterTime(void)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem != NULL)
			return pElem->m_nAllMoniterTime;

		return 0;
	}

	void SetAllMoniterTime(LONGLONG nWait)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem != NULL)
			pElem->m_nAllMoniterTime = nWait;
	}

	LONGLONG GetAllUpData(void)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem != NULL)
			return pElem->m_lUpData;

		return 0;
	}

	void SetAllUpData(LONGLONG lUpData)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem != NULL)
			pElem->m_lUpData = lUpData;
	}

	int GetAllDownData(void)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem != NULL)
			return pElem->m_lDownData;

		return 0;
	}

	void SetAllDownData(LONGLONG lAllDownData)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem != NULL)
			pElem->m_lDownData = lAllDownData;
	}

	int GetAllData(void)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem != NULL)
			return pElem->m_lAllData;

		return 0;
	}

	void SetAllData(LONGLONG lAllData)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem != NULL)
			pElem->m_lAllData = lAllData;
	}

private:
	void _SetData(int nIndex, int nData)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem == NULL)
			return;
//		pElem->m_shareLock.LockWrite();
		*((int*)((char*)pElem + sizeof(int) * nIndex)) = nData;
//		pElem->m_shareLock.UnLockWrite();
	}

	int  _GetData(int nIndex)
	{
		MemShareFlowElem* pElem = (MemShareFlowElem*)m_pBufView;
		if (pElem == NULL)
			return 0;
//		pElem->m_shareLock.LockRead();
		int nData = *((int*)((char*)(pElem) + sizeof(int) * nIndex));
//		pElem->m_shareLock.UnLockRead();

		return nData;
	}

};

//__declspec(selectany) LPVOID KNetFluxCacheReader::m_pBufViewInfo = NULL;
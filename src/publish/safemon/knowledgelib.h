
#pragma once
#include <assert.h>
#include <tchar.h>

struct KNOWLEDGELIB_SOFT
{
	DWORD	dwID;
	DWORD	dwLevel;
	LPCTSTR lpName;
};

class IKnowledgeLib
{
public:
	virtual BOOL Load(LPCTSTR lpFilePath) = 0;
	virtual void Unload() = 0;

	virtual BOOL GetSoftInfoByFilePath(LPCTSTR lpPath, KNOWLEDGELIB_SOFT* lpSoftInfo) = 0;
	virtual BOOL GetSoftInfoByRegPath(LPCTSTR lpRegPath, LPCTSTR lpValueName, KNOWLEDGELIB_SOFT* lpSoftInfo) = 0;
	virtual BOOL GetSoftInfoByClsid(LPCTSTR lpClsid, KNOWLEDGELIB_SOFT* lpSoftInfo) = 0;
	virtual BOOL GetSoftInfoByURL(LPCTSTR lpUrl, KNOWLEDGELIB_SOFT* lpSoftInfo) = 0;
	virtual BOOL GetSoftInfoByMD5(BYTE* lpMD5, KNOWLEDGELIB_SOFT* lpSoftInfo) = 0;

	virtual BOOL GetSoftInfoByRun(LPCTSTR lpRun, KNOWLEDGELIB_SOFT* lpSoftInfo) = 0;
	virtual BOOL GetSoftInfoByServices(LPCTSTR lpServices, KNOWLEDGELIB_SOFT* lpSoftInfo) = 0;
};

#define KNWLIB_DLL_NAME			_T("ksfmon.dll")
#define KNWLIB_NEW_FUN_NAME		"B1"
#define KNWLIB_FREE_FUN_NAME	"B2"

typedef IKnowledgeLib* (WINAPI* PFN_NewKnowledgeLib)();
typedef void (WINAPI* PFN_FreeKnowledgeLib)(IKnowledgeLib** lppObj);

IKnowledgeLib* WINAPI NewKnowledgeLib();
void WINAPI FreeKnowledgeLib(IKnowledgeLib** lppObj);

//////////////////////////////////////////////////////////////////////////

class CKnowledgeLibLoader
{
public:
	CKnowledgeLibLoader()
	{
		m_pfnNew = NULL;
		m_pfnFree = NULL;

		m_hModule = LoadLibrary(KNWLIB_DLL_NAME);
		if ( m_hModule != NULL )
		{
			m_pfnNew = (PFN_NewKnowledgeLib)GetProcAddress(m_hModule, KNWLIB_NEW_FUN_NAME);
			m_pfnFree = (PFN_FreeKnowledgeLib)GetProcAddress(m_hModule, KNWLIB_FREE_FUN_NAME);
		}
	}

	~CKnowledgeLibLoader()
	{
		FreeLibrary(m_hModule);
	}

	operator bool ()
	{
		return (m_hModule != NULL) && (m_pfnFree != NULL) && (m_pfnNew != NULL);
	}

	IKnowledgeLib* NewKnowledge()
	{
		assert(m_pfnNew != NULL);

		return (m_pfnNew != NULL) ? m_pfnNew() : NULL;
	}

	void FreeKnowledge(IKnowledgeLib** lppObj)
	{
		assert(m_pfnFree != NULL);

		if ( m_pfnFree != NULL )
		{
			m_pfnFree(lppObj);
		}
	}

protected:
	PFN_NewKnowledgeLib m_pfnNew;
	PFN_FreeKnowledgeLib m_pfnFree;
	HMODULE		m_hModule;
};
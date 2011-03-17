#pragma once

#define	  MODULE_AVMGR_NAME	L"kavmgr.dll"


namespace AVProductMgr
{

	class __declspec(uuid("8811F106-867B-48d2-99E0-4DC86D68B1C8"))
IAVProductMgr : public IUnknown
	{
	public:
		virtual UINT32 STDMETHODCALLTYPE LoadAVLib(LPCWSTR lpszPath) = 0;
		virtual LPCWSTR STDMETHODCALLTYPE Check() = 0;
		virtual UINT32 STDMETHODCALLTYPE IsCanUninstall() = 0;
		virtual UINT32 STDMETHODCALLTYPE UnInstall() = 0;
		virtual LPCWSTR STDMETHODCALLTYPE GetInstallAVs() = 0;
	};

#define FN_CreateAVProductMgr			"CreateAVProductMgr"
	typedef HRESULT (WINAPI *PFN_CreateAVProductMgr)( REFIID riid, void** ppvObj );
	extern "C" HRESULT WINAPI CreateAVProductMgr( REFIID riid, void** ppvObj );

#define FN_FreeAVProductMgr				"FreeAVProductMgr"
	typedef HRESULT (WINAPI *PFN_FreeAVProductMgr)(void* ppvObj );
	extern "C" VOID WINAPI FreeAVProductMgr(IAVProductMgr* pIAVProductMgr);


	class CAVMgrInterface
	{
	public:
		static CAVMgrInterface* GetPtr(CString strAppPath = L"")
		{
			static CAVMgrInterface av(strAppPath);
			return &av;
		}
	public:
		CAVMgrInterface(CString strAppPath = L""):
		  m_hModule(NULL),
			  m_ptrInterface(NULL),
			  m_pfnCrateInterface(NULL),
			  m_pfnFreeInterface(NULL)
		  {
			  CString strPath = strAppPath;
			  if (TRUE == strAppPath.IsEmpty())
			  {
				  strPath = MODULE_AVMGR_NAME;
			  }
			  else
			  {
				  strPath.Append(L"\\");
				  strPath.Append(MODULE_AVMGR_NAME);
			  }

			  LoadEng(strPath);
		  }
		  IAVProductMgr*	CrateInterface()
		  {
			  if (NULL == m_pfnCrateInterface)
				  return NULL;
			  if (NULL != m_ptrInterface)
				  return m_ptrInterface;
			  if (FAILED(m_pfnCrateInterface(__uuidof(IAVProductMgr), (void**)&m_ptrInterface)))
				  return NULL;

			  return m_ptrInterface;
		  }

		  void FreeInterface(IAVProductMgr* ptr)
		  {
			  if (NULL != m_ptrInterface &&
				  NULL != m_pfnFreeInterface)
				  m_pfnFreeInterface(m_ptrInterface);

			  if (NULL != m_hModule)
				  ::FreeLibrary(m_hModule);
		  }
		  ~CAVMgrInterface()
		  {
			  if (NULL != m_ptrInterface &&
				  NULL != m_pfnFreeInterface)
				  m_pfnFreeInterface(m_ptrInterface);
			  if (NULL != m_hModule)
				  ::FreeLibrary(m_hModule);
		  }
	private:
		IAVProductMgr*	m_ptrInterface;
		PFN_CreateAVProductMgr	m_pfnCrateInterface;
		PFN_FreeAVProductMgr	m_pfnFreeInterface;

		HMODULE			m_hModule;
		void	LoadEng(LPCTSTR pszModuleFullPath)
		{
			if (NULL == m_ptrInterface)
			{
				if (NULL == m_hModule)
					m_hModule = ::LoadLibrary(pszModuleFullPath);

				if (NULL != m_hModule)
				{
					m_pfnCrateInterface = (PFN_CreateAVProductMgr)::GetProcAddress(m_hModule, FN_CreateAVProductMgr);
					m_pfnFreeInterface = (PFN_FreeAVProductMgr)::GetProcAddress(m_hModule, FN_FreeAVProductMgr);
				}
			}
		}
	};
}


#pragma once
#include "IVulFix.h"

#define BEIKESAFE_VULSCAN_DLL_VERSION 0

class __declspec(uuid("BDC90F00-4F58-41e4-857F-4BFFC194E1D7")) 
IVulCollection : public IUnknown
{
public:
	virtual void Destroy() = 0;
	// 如果S_OK , 根据STATUS , 否则根据 get_Item 来做 
	// status code :-1, 0 表示可能无风险, 1 表示可能有风险 
	// -1 for db<os.fix	 0 for db==os.fix  1 for db>os.fix, need to repaire
	virtual HRESULT get_Status(LONG *ptrStatus) = 0;
	virtual HRESULT get_Count(ULONG *ptrCount) = 0;
	virtual HRESULT get_Item(ULONG n, int *pKBID, int *pLevel, BSTR* pTitle, BSTR* pDescription) = 0;
	virtual HRESULT get_Item2(ULONG n, int *pKBID, int *pLevel, int *pNType, DWORD *pVFlags, BSTR* pTitle, BSTR* pDescription) = 0;
	virtual HRESULT get_Item3(ULONG n, int *pKBID, int *pLevel, int *pNType, DWORD *pVFlags, BSTR* pTitle, BSTR* pDescription, BSTR *pPubDate) = 0;
	virtual HRESULT get_VulCount(INT *pCntHigh, INT *pCntOption, INT *pCntSP) = 0;
};

class __declspec(uuid("F62B416A-14DB-48d5-B0B4-E77F19B57400")) IVulEnvironment : public IUnknown {
public:
	virtual HRESULT IsUsingInterface(BOOL *pRet) = 0;
	virtual HRESULT IsSystemSupport(BOOL *pRet) = 0;
};

class __declspec(uuid("C17348CA-944C-4729-A3BC-5A2956C016B5")) IVulScan: public IUnknown {
public:
	virtual HRESULT ScanLeakEx(IVulCollection **pVulCollection, DWORD dwFlags) = 0;
};


extern "C" {

#ifdef LEAKSCAN_EXPORTS
    HRESULT			WINAPI ExpressScanLeak(IVulCollection **pVulCollection, DWORD dwFlags, BOOL bQuickScan);
	IVulfix*		WINAPI CreateVulFix(int version = BEIKESAFE_VULSCAN_DLL_VERSION);
	IVulfixed*		WINAPI CreateVulFixed(int version = BEIKESAFE_VULSCAN_DLL_VERSION);
	ISoftVulfix*	WINAPI CreateSoftVulFix(int version = BEIKESAFE_VULSCAN_DLL_VERSION);
	IRepairVul*		WINAPI CreateRepairVul(int version = BEIKESAFE_VULSCAN_DLL_VERSION);
	HRESULT			WINAPI CreateUpdateHelper(REFIID riid, void** ppvObj);
	HRESULT			WINAPI CreateObject(REFIID riid, void** ppvObj);
	HRESULT			WINAPI CreateObjectEx( LPCTSTR lpszVersion, REFIID riid, void** ppvObj );
#else
#   define FUNCNAME_EXPRESSSCANLEAK    "ExpressScanLeak"
    typedef HRESULT (WINAPI *FnExpressScanLeak)( IVulCollection **pVulCollection, DWORD dwFlags, BOOL bQuickScan );

#   define FUNCNAME_CREATEVULFIX       "CreateVulFix"
	typedef IVulfix* (WINAPI *FnCreateVulFix)(int version/* = BEIKESAFE_VULSCAN_DLL_VERSION*/);

#   define FUNCNAME_CREATEVULFIXED     "CreateVulFixed"
	typedef IVulfixed* (WINAPI *FnCreateVulFixed)(int version/* = BEIKESAFE_VULSCAN_DLL_VERSION*/);

#   define FUNCNAME_CREATESOFTVULFIX   "CreateSoftVulFix"
	typedef ISoftVulfix* (WINAPI *FnCreateSoftVulFix)(int version/* = BEIKESAFE_VULSCAN_DLL_VERSION*/);

#   define FUNCNAME_CREATEREPAIRVUL    "CreateRepairVul"
	typedef IRepairVul* (WINAPI *FnCreateRepairVul)(int version/* = BEIKESAFE_VULSCAN_DLL_VERSION*/);

#	define FUNCNAME_CreateUpdateHelper	"CreateUpdateHelper"
	typedef HRESULT (WINAPI *FnCreateUpdateHelper)( REFIID riid, void** ppvObj );

#	define FUNCNAME_CreateObject "CreateObject"
	typedef HRESULT (WINAPI *FnCreateObject)( REFIID riid, void** ppvObj );

#	define FUNCNAME_CreateObjectEx "CreateObjectEx"
	typedef HRESULT (WINAPI *FnCreateObjectEx)( LPCTSTR lpszVersion, REFIID riid, void** ppvObj );

#endif
};





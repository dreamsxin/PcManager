#ifndef _UTIL_H
#define _UTIL_H

#ifndef STDCALL
#define STDCALL __stdcall
#endif

#include <vector>
#include <string>
#include <windows.h>
#include <wininet.h>

#ifndef array_size
#define array_size(x) ( sizeof(x) / sizeof((x)[0]) )
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) private: \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)
#endif

enum WinVer 
{
  WINVER_UNKNOWN = 0,
  WINVER_2000,
  WINVER_XP,
  WINVER_XP_X64,
  WINVER_SERVER_2003,
  WINVER_HOME_SERVER,
  WINVER_SERVER_2003_R2,
  WINVER_VISTA,
  WINVER_SERVER_2008,
  WINVER_SERVER_2008_R2,
  WINVER_WIN7
};

// 获取WINDOWS版本, 该函数只能获取WIN 2000(包括WIN 2000)以上的WINDOWS版本.
WinVer STDCALL GetWindowVersion();

// -- string utilities -------------------------------------------------------

namespace string_util
{

  bool tokenizeW(std::vector<std::wstring> &results, const std::wstring &src, 
    const std::wstring &tok, bool trim = false, std::wstring null_subst = L"");
}

// -- URLCracker -------------------------------------------------------------

class CURLCracker : public URL_COMPONENTSW
{
public: 
    CURLCracker(LPCWSTR szUrl, DWORD dwComponents, DWORD dwFlags = ICU_DECODE);
    ~CURLCracker();

    enum
    {
        UCF_SHEME       = 0x00000001,
        UCF_HOSTNAME    = 0x00000002,
        UCF_USERNAME    = 0x00000008,
        UCF_PASSWORD    = 0x00000010,
        UCF_PATH        = 0x00000020,
        UCF_EXTRAINFO   = 0x00000040,
        UCF_MAINDOMAIN  = (0x00000080 | UCF_HOSTNAME)
    };

    LPCWSTR  Sheme(PDWORD pdwSchemeLength);
    LPCWSTR  HostName(PDWORD pdwHostNameLength);
    LPCWSTR  UserName(PDWORD pdwUserNameLength);
    LPCWSTR  Password(PDWORD pdwPasswordLength);
    LPCWSTR  UrlPath(PDWORD pdwUrlPathLength);
    LPCWSTR  ExtraInfo(PDWORD pdwExtraInfoLength);
    LPCWSTR  MainDomainName(PDWORD pdwMainDomainNameLength);
    DWORD    InternetScheme();
    INTERNET_PORT Port();

    // Call this method after constructor to check if URL has been successfully
    // cracked. If URL has been successfully cracked, the function returns
    // ERROR_SUCCESS, otherwise, an error code is returned.
    DWORD    ErrorCode() { return m_dwErrorCode; }

private:
    HRESULT GetMainDomainName();
    static BOOL IsKnownDomain(LPCWSTR lpwszDomain);

    static LPCWSTR ms_lpwszKnownDomains[];
    std::wstring m_wstrMainDomainName;
    DWORD m_dwErrorCode;
};

// -- Import function wrapper --------------------------------------------

template <typename Fn> class CImportFunction
{
public:
    CImportFunction(LPCSTR lpszModule, LPCSTR lpszFunction, BOOL bLoad = FALSE, 
        BOOL bFree = FALSE) 
        : m_pfn(NULL), m_bNeedFree(FALSE), m_hModule(NULL)
        { Initialize(lpszModule, lpszFunction, bLoad, bFree); }

    ~CImportFunction() 
        { if (m_bNeedFree && m_hModule) FreeLibrary(m_hModule); }

    operator Fn() { return m_pfn; }

private:
    void Initialize(LPCSTR lpszModule, LPCSTR lpszFunction, BOOL bLoad, 
        BOOL bFree)
    {
        if (!lpszModule || !lpszFunction)
            return;

		m_hModule = GetModuleHandleA(lpszModule);
        if (!m_hModule)
        {
            if (!bLoad)
                return;
            m_hModule = LoadLibraryA(lpszModule);
            if (!m_hModule)
                return;
            if (bFree)
                m_bNeedFree = TRUE;
        }

        m_pfn = (Fn)GetProcAddress(m_hModule, lpszFunction);
    }

    Fn      m_pfn;          // Address of import function
    BOOL    m_bNeedFree;    // Whether we need call FreeLibrary in destructor
    HMODULE m_hModule;      // 
};

// -- kws utilities -------------------------------------------------------

namespace kws_util
{
	enum{IE_UNKNOWN = 0,IE_6, IE_7, IE_8};
	int GetCurrentSystemIEVersion();
}

#endif
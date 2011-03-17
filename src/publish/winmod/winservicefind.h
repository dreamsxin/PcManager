/**
* @file    winservicefind.h
* @brief   ...
* @author  bbcallen
* @date    2009-04-25  14:49
*/

#ifndef WINSERVICEFIND_H
#define WINSERVICEFIND_H

#include <atlstr.h>
#include <atlbase.h>
#include "winmodbase.h"
#include "winpath.h"
#include "winregkeyfind.h"

NS_WINMOD_BEGIN

#define WINMOD_REGKEY_CURRENT_CONTRON_SET       L"SYSTEM\\CurrentControlSet\\Services"
#define WINMOD_REGKEY_CURRENT_SET_001           L"SYSTEM\\CurrentSet001\\Services"
#define WINMOD_REGKEY_CURRENT_SET_002           L"SYSTEM\\CurrentSet002\\Services"
#define WINMOD_REGKEY_CURRENT_SET_003           L"SYSTEM\\CurrentSet003\\Services"
#define WINMOD_REGKEY_CURRENT_SET_004           L"SYSTEM\\CurrentSet004\\Services"




class CWinServiceFind
{
public:

    CWinServiceFind();
    virtual ~CWinServiceFind();

    // Operations
public:

    void    Reset();

    BOOL    FindFirstService(REGSAM samDesired = KEY_ENUMERATE_SUB_KEYS);

    BOOL    FindNextService();
    BOOL    IsEndOfFind() const;

    /**
    * @brief    Obtain service type
    * @retval   ULONG_MAX                   0xFFFFFFFF  invalid service type
    * @retval   SERVICE_ADAPTER             0x00000004  Reserved.
    * @retval   SERVICE_FILE_SYSTEM_DRIVER  0x00000002  File system driver service.
    * @retval   SERVICE_KERNEL_DRIVER       0x00000001  Driver service.
    * @retval   SERVICE_RECOGNIZER_DRIVER   0x00000008  Reserved.    
    * @retval   SERVICE_WIN32_OWN_PROCESS   0x00000010  Service that runs in its own process.
    * @retval   SERVICE_WIN32_SHARE_PROCESS 0x00000020  Service that shares a process with one or more other services. For more information, see Service Programs.
    */
    DWORD   GetServiceType() const;
        

    /**
    * @brief    Obtain start type
    * @retval   ULONG_MAX                   0xFFFFFFFF  invalid start type
    * @retval   SERVICE_AUTO_START          0x00000002  A service started automatically by the service control manager during system startup. For more information, see Automatically Starting Services.
    * @retval   SERVICE_BOOT_START          0x00000000  A device driver started by the system loader. This value is valid only for driver services.
    * @retval   SERVICE_DEMAND_START        0x00000003  A service started by the service control manager when a process calls the StartService function. For more information, see Starting Services on Demand.
    * @retval   SERVICE_DISABLED            0x00000004  A service that cannot be started. Attempts to start the service result in the error code ERROR_SERVICE_DISABLED.
    * @retval   SERVICE_SYSTEM_START        0x00000001  A device driver started by the IoInitSystem function. This value is valid only for driver services.
    */
    DWORD   GetServiceStart() const;

    LPCWSTR GetDisplayName() const;

    LPCWSTR GetDescription() const;

    const CWinPath& GetImagePath() const;

    const CWinPath& GetServiceDll() const;



    LPCWSTR GetServiceTypeAsString() const;

    LPCWSTR GetServiceStartAsString() const;

    CString GetConvertedDisplayName() const;

    CString GetConvertedDescription() const;



    HRESULT Disable();

    HRESULT Remove();



private:
    // Disabled
    CWinServiceFind(const CWinServiceFind&);
    CWinServiceFind& operator=(const CWinServiceFind&);

public:
    CWinRegKeyFind  m_hRegKeyFind;

protected:

    CString     DoConvertString(const CString& strMessage) const;

    void        DoReadStandardValues();

    CWinPath    m_imagePath;
    CWinPath    m_serviceDll;

    CString     m_strDisplayName;
    CString     m_strDescription;
    DWORD       m_dwServiceType;
    DWORD       m_dwServiceStart;
};

inline CWinServiceFind::CWinServiceFind()
{
    Reset();
}

inline CWinServiceFind::~CWinServiceFind()
{
}

inline void CWinServiceFind::Reset()
{
    m_hRegKeyFind.Reset();

    m_imagePath.m_strPath   = L"";
    m_strDescription        = L"";
    m_dwServiceType         = ULONG_MAX;
    m_dwServiceStart        = ULONG_MAX;
}

inline BOOL CWinServiceFind::IsEndOfFind() const
{
    return m_hRegKeyFind.IsEndOfFind();
}

inline DWORD CWinServiceFind::GetServiceType() const
{
    return m_dwServiceType;
}

inline DWORD CWinServiceFind::GetServiceStart() const
{
    return m_dwServiceStart;
}

inline LPCWSTR CWinServiceFind::GetDisplayName() const
{
    return m_strDisplayName;
}

inline LPCWSTR CWinServiceFind::GetDescription() const
{
    return m_strDescription;
}


inline const CWinPath& CWinServiceFind::GetImagePath() const
{
    return m_imagePath;
}

inline const CWinPath& CWinServiceFind::GetServiceDll() const
{
    return m_serviceDll;
}


NS_WINMOD_END

#endif//WINSERVICEFIND_H
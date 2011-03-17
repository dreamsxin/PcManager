/**
* @file    wintrustmod.h
* @brief   ...
* @author  bbcallen
* @date    2009-02-11  11:19
*/

#ifndef WINTRUSTMOD_H
#define WINTRUSTMOD_H

#include <Wintrust.h>
#include <softpub.h>
#include <Mscat.h>
#include "winmodule.h"

NS_WINMOD_BEGIN

typedef BOOL (WINAPI *PFN_CryptCATAdminAcquireContext)(
    HCATADMIN *phCatAdmin,
    const GUID *pgSubsystem,
    DWORD dwFlags);

typedef BOOL (WINAPI *PFN_CryptCATAdminReleaseContext)(
    HCATADMIN hCatAdmin,
    DWORD dwFlags);

typedef BOOL (WINAPI *PFN_CryptCATAdminReleaseCatalogContext)(
    HCATADMIN hCatAdmin,
    HCATINFO hCatInfo,
    DWORD dwFlags);

typedef BOOL (WINAPI *PFN_CryptCATCatalogInfoFromContext)(
    HCATINFO hCatInfo,
    CATALOG_INFO *psCatInfo,
    DWORD dwFlags);

typedef BOOL (WINAPI *PFN_CryptCATAdminCalcHashFromFileHandle)(
    HANDLE hFile,
    DWORD *pcbHash,
    BYTE *pbHash,
    DWORD dwFlags);

typedef HCATINFO (WINAPI *PFN_CryptCATAdminEnumCatalogFromHash)(
    HCATADMIN hCatAdmin,
    BYTE *pbHash,
    DWORD cbHash,
    DWORD dwFlags,
    HCATINFO *phPrevCatInfo);

typedef LONG (WINAPI *PFN_WinVerifyTrust)(
    HWND hwnd,
    GUID *pgActionID,
    LPVOID pWVTData);

typedef CRYPT_PROVIDER_DATA* (WINAPI *PFN_WTHelperProvDataFromStateData)(
    HANDLE hStateData);

typedef CRYPT_PROVIDER_SGNR* (WINAPI *PFN_WTHelperGetProvSignerFromChain)(
    CRYPT_PROVIDER_DATA* pProvData,
    DWORD idxSigner,
    BOOL fCounterSigner,
    DWORD idxCounterSigner);

typedef CRYPT_PROVIDER_CERT* (WINAPI *PFN_WTHelperGetProvCertFromChain)(
    CRYPT_PROVIDER_SGNR* pSgnr,
    DWORD idxCert);


class CWinModule_wintrust: public CWinModule
{
public:
    CWinModule_wintrust()
        : m_pfnCryptCATAdminAcquireContext(NULL)
        , m_pfnCryptCATAdminReleaseContext(NULL)
        , m_pfnCryptCATAdminReleaseCatalogContext(NULL)
        , m_pfnCryptCATCatalogInfoFromContext(NULL)
        , m_pfnCryptCATAdminCalcHashFromFileHandle(NULL)
        , m_pfnCryptCATAdminEnumCatalogFromHash(NULL)
        , m_pfnWinVerifyTrust(NULL)
        , m_pfnWTHelperProvDataFromStateData(NULL)
        , m_pfnWTHelperGetProvSignerFromChain(NULL)
        , m_pfnWTHelperGetProvCertFromChain(NULL)

    {

    }

    virtual void FreeLib()
    {
        m_pfnCryptCATAdminAcquireContext            = NULL;
        m_pfnCryptCATAdminReleaseContext            = NULL;
        m_pfnCryptCATAdminReleaseCatalogContext     = NULL;
        m_pfnCryptCATCatalogInfoFromContext         = NULL;
        m_pfnCryptCATAdminCalcHashFromFileHandle    = NULL;
        m_pfnCryptCATAdminEnumCatalogFromHash       = NULL;
        m_pfnWinVerifyTrust                         = NULL;
        m_pfnWTHelperProvDataFromStateData          = NULL;
        m_pfnWTHelperGetProvSignerFromChain         = NULL;
        m_pfnWTHelperGetProvCertFromChain           = NULL;

        return CWinModule::FreeLib();
    }

    BOOL WINAPI CryptCATAdminAcquireContext(
        HCATADMIN *phCatAdmin,
        const GUID *pgSubsystem,
        DWORD dwFlags)
    {
        if (NULL == m_pfnCryptCATAdminAcquireContext)
        {
            m_pfnCryptCATAdminAcquireContext =
                (PFN_CryptCATAdminAcquireContext) GetProcAddr("CryptCATAdminAcquireContext");

            if (!m_pfnCryptCATAdminAcquireContext)
                return FALSE;
        }

        return m_pfnCryptCATAdminAcquireContext(
            phCatAdmin,
            pgSubsystem,
            dwFlags);
    }

    BOOL WINAPI CryptCATAdminReleaseContext(
        HCATADMIN hCatAdmin,
        DWORD dwFlags)
    {
        if (NULL == m_pfnCryptCATAdminReleaseContext)
        {
            m_pfnCryptCATAdminReleaseContext =
                (PFN_CryptCATAdminReleaseContext) GetProcAddr("CryptCATAdminReleaseContext");

            if (!m_pfnCryptCATAdminReleaseContext)
                return FALSE;
        }

        return m_pfnCryptCATAdminReleaseContext(
            hCatAdmin,
            dwFlags);
    }

    BOOL WINAPI CryptCATAdminReleaseCatalogContext(
        HCATADMIN hCatAdmin,
        HCATINFO hCatInfo,
        DWORD dwFlags)
    {
        if (NULL == m_pfnCryptCATAdminReleaseCatalogContext)
        {
            m_pfnCryptCATAdminReleaseCatalogContext =
                (PFN_CryptCATAdminReleaseCatalogContext) GetProcAddr("CryptCATAdminReleaseCatalogContext");

            if (!m_pfnCryptCATAdminReleaseCatalogContext)
                return FALSE;
        }

        return m_pfnCryptCATAdminReleaseCatalogContext(
            hCatAdmin,
            hCatInfo,
            dwFlags);
    }

    BOOL WINAPI CryptCATCatalogInfoFromContext(
        HCATINFO hCatInfo,
        CATALOG_INFO *psCatInfo,
        DWORD dwFlags)
    {
        if (NULL == m_pfnCryptCATCatalogInfoFromContext)
        {
            m_pfnCryptCATCatalogInfoFromContext =
                (PFN_CryptCATCatalogInfoFromContext) GetProcAddr("CryptCATCatalogInfoFromContext");

            if (!m_pfnCryptCATCatalogInfoFromContext)
                return FALSE;
        }

        return m_pfnCryptCATCatalogInfoFromContext(
            hCatInfo,
            psCatInfo,
            dwFlags);
    }

    BOOL WINAPI CryptCATAdminCalcHashFromFileHandle(
        HANDLE hFile,
        DWORD *pcbHash,
        BYTE *pbHash,
        DWORD dwFlags)
    {
        if (NULL == m_pfnCryptCATAdminCalcHashFromFileHandle)
        {
            m_pfnCryptCATAdminCalcHashFromFileHandle =
                (PFN_CryptCATAdminCalcHashFromFileHandle) GetProcAddr("CryptCATAdminCalcHashFromFileHandle");

            if (!m_pfnCryptCATAdminCalcHashFromFileHandle)
                return FALSE;
        }

        return m_pfnCryptCATAdminCalcHashFromFileHandle(
            hFile,
            pcbHash,
            pbHash,
            dwFlags);
    }

    HCATINFO WINAPI CryptCATAdminEnumCatalogFromHash(
        HCATADMIN hCatAdmin,
        BYTE *pbHash,
        DWORD cbHash,
        DWORD dwFlags,
        HCATINFO *phPrevCatInfo)
    {
        if (NULL == m_pfnCryptCATAdminEnumCatalogFromHash)
        {
            m_pfnCryptCATAdminEnumCatalogFromHash =
                (PFN_CryptCATAdminEnumCatalogFromHash) GetProcAddr("CryptCATAdminEnumCatalogFromHash");

            if (!m_pfnCryptCATAdminEnumCatalogFromHash)
                return NULL;
        }

        return m_pfnCryptCATAdminEnumCatalogFromHash(
            hCatAdmin,
            pbHash,
            cbHash,
            dwFlags,
            phPrevCatInfo);
    }

    LONG WINAPI WinVerifyTrust(
        HWND hwnd,
        GUID *pgActionID,
        LPVOID pWVTData)
    {
        if (NULL == m_pfnWinVerifyTrust)
        {
            m_pfnWinVerifyTrust =
                (PFN_WinVerifyTrust) GetProcAddr("WinVerifyTrust");

            if (!m_pfnWinVerifyTrust)
                return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
        }

        return m_pfnWinVerifyTrust(
            hwnd,
            pgActionID,
            pWVTData);
    }

    CRYPT_PROVIDER_DATA* WINAPI WTHelperProvDataFromStateData(HANDLE hStateData)
    {
        if (NULL == m_pfnWTHelperProvDataFromStateData)
        {
            m_pfnWTHelperProvDataFromStateData =
                (PFN_WTHelperProvDataFromStateData) GetProcAddr("WTHelperProvDataFromStateData");

            if (!m_pfnWTHelperProvDataFromStateData)
                return NULL;
        }

        return m_pfnWTHelperProvDataFromStateData(hStateData);
    }

    CRYPT_PROVIDER_SGNR* WINAPI WTHelperGetProvSignerFromChain(
        CRYPT_PROVIDER_DATA* pProvData,
        DWORD idxSigner,
        BOOL fCounterSigner,
        DWORD idxCounterSigner)
    {
        if (NULL == m_pfnWTHelperGetProvSignerFromChain)
        {
            m_pfnWTHelperGetProvSignerFromChain =
                (PFN_WTHelperGetProvSignerFromChain) GetProcAddr("WTHelperGetProvSignerFromChain");

            if (!m_pfnWTHelperGetProvSignerFromChain)
                return NULL;
        }

        return m_pfnWTHelperGetProvSignerFromChain(
            pProvData,
            idxSigner,
            fCounterSigner,
            idxCounterSigner);
    }

    CRYPT_PROVIDER_CERT* WINAPI WTHelperGetProvCertFromChain(
        CRYPT_PROVIDER_SGNR* pSgnr,
        DWORD idxCert)
    {
        if (NULL == m_pfnWTHelperGetProvCertFromChain)
        {
            m_pfnWTHelperGetProvCertFromChain =
                (PFN_WTHelperGetProvCertFromChain) GetProcAddr("WTHelperGetProvCertFromChain");

            if (!m_pfnWTHelperGetProvCertFromChain)
                return NULL;
        }

        return m_pfnWTHelperGetProvCertFromChain(pSgnr, idxCert);
    }


private:

    PFN_CryptCATAdminAcquireContext         m_pfnCryptCATAdminAcquireContext;
    PFN_CryptCATAdminReleaseContext         m_pfnCryptCATAdminReleaseContext;
    PFN_CryptCATAdminReleaseCatalogContext  m_pfnCryptCATAdminReleaseCatalogContext;
    PFN_CryptCATCatalogInfoFromContext      m_pfnCryptCATCatalogInfoFromContext;
    PFN_CryptCATAdminCalcHashFromFileHandle m_pfnCryptCATAdminCalcHashFromFileHandle;
    PFN_CryptCATAdminEnumCatalogFromHash    m_pfnCryptCATAdminEnumCatalogFromHash;
    PFN_WinVerifyTrust                      m_pfnWinVerifyTrust;
    
    PFN_WTHelperProvDataFromStateData       m_pfnWTHelperProvDataFromStateData;
    PFN_WTHelperGetProvSignerFromChain      m_pfnWTHelperGetProvSignerFromChain;
    PFN_WTHelperGetProvCertFromChain        m_pfnWTHelperGetProvCertFromChain;
};

NS_WINMOD_END

#endif//WINTRUSTMOD_H
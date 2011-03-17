/**
* @file    wintrustverifier.cpp
* @brief   ...
* @author  bbcallen
* @date    2009-02-11  17:22
*/

#include "stdafx.h"
#include "wintrustverifier.h"

#include <assert.h>
#include <imagehlp.h>
#include <atlfile.h>
#include <atlstr.h>

#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "crypt32.lib")

using namespace WinMod;

#define MAX_LOOP_OF_ENUM 5

#define WINMOD_EICAR_TEXT "X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*"

DWORD CWinTrustVerifier::VerifyFile(LPCWSTR pwszFileFullPath, CWinTrustSignerInfoChain* pSignInfoChain)
{
    assert(pwszFileFullPath);
    CAtlFile hFile;
    HRESULT  hr = hFile.Create(
        pwszFileFullPath,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        OPEN_EXISTING);
    if (FAILED(hr))
        return hr;

    return VerifyFile(pwszFileFullPath, hFile, pSignInfoChain);
}

DWORD CWinTrustVerifier::VerifyFile(LPCWSTR pwszFileFullPath, HANDLE hFile, CWinTrustSignerInfoChain* pSignInfoChain)
{
    assert(hFile && hFile != INVALID_HANDLE_VALUE);
    DWORD dwRet = 0;

    if (HasEmbeddedSignature(hFile))
    {
        dwRet = VerifyEmbeddedWinTrustFile(pwszFileFullPath, hFile, pSignInfoChain);
        if (ERROR_SUCCESS == dwRet)
            return ERROR_SUCCESS;

        dwRet = VerifyCatalogSignature(pwszFileFullPath, hFile, pSignInfoChain);
        if (ERROR_SUCCESS == dwRet)
            return ERROR_SUCCESS;
    }
    else
    {
        dwRet = VerifyCatalogSignature(pwszFileFullPath, hFile, pSignInfoChain);
        if (ERROR_SUCCESS == dwRet)
            return ERROR_SUCCESS;
    }

    return dwRet;
}

DWORD CWinTrustVerifier::VerifyEmbeddedWinTrustFile(LPCWSTR pwszFileFullPath, HANDLE hFile, CWinTrustSignerInfoChain* pSignInfoChain)
{
    GUID WinTrustVerifyGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    return VerifyEmbeddedSignature(pwszFileFullPath, hFile, WinTrustVerifyGuid, pSignInfoChain);
}


// modified from MSDN
DWORD CWinTrustVerifier::VerifyEmbeddedSignature(LPCWSTR pwszFileFullPath, HANDLE hFile, GUID& policyGUID, CWinTrustSignerInfoChain* pSignInfoChain)
{
    assert(pwszFileFullPath);

    // Load wintrust.dll
    HRESULT hRet = TryLoadDll();
    if (FAILED(hRet))
        return (DWORD)hRet;



    // Initialize the WINTRUST_FILE_INFO structure.

    WINTRUST_FILE_INFO FileData;
    memset(&FileData, 0, sizeof(FileData));
    FileData.cbStruct       = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath  = pwszFileFullPath;
    FileData.hFile          = hFile;
    FileData.pgKnownSubject = NULL;

    /*
    WVTPolicyGUID specifies the policy to apply on the file
    WINTRUST_ACTION_GENERIC_VERIFY_V2 policy checks:
    
    1) The certificate used to sign the file chains up to a root 
    certificate located in the trusted root certificate store. This 
    implies that the identity of the publisher has been verified by 
    a certification authority.
    
    2) In cases where user interface is displayed (which this example
    does not do), WinVerifyTrust will check for whether the  
    end entity certificate is stored in the trusted publisher store,  
    implying that the user trusts content from this publisher.
    
    3) The end entity certificate has sufficient permission to sign 
    code, as indicated by the presence of a code signing EKU or no 
    EKU.
    */


    // Initialize the WinVerifyTrust input data structure.
    GUID          WVTPolicyGUID = policyGUID;
    WINTRUST_DATA WinTrustData;
    memset(&WinTrustData, 0, sizeof(WinTrustData));
    WinTrustData.cbStruct = sizeof(WinTrustData);
    WinTrustData.pPolicyCallbackData    = NULL;                     // Use default code signing EKU.
    WinTrustData.pSIPClientData         = NULL;                     // No data to pass to SIP.
    WinTrustData.dwUIChoice             = WTD_UI_NONE;              // Disable WVT UI.
    WinTrustData.fdwRevocationChecks    = WTD_REVOKE_NONE;          // No revocation checking.
    WinTrustData.dwUnionChoice          = WTD_CHOICE_FILE;          // Verify an embedded signature on a file.
    WinTrustData.dwStateAction          = pSignInfoChain ? WTD_STATEACTION_VERIFY : 0;
    WinTrustData.hWVTStateData          = NULL;                     // Not applicable for default verification of embedded signature.
    WinTrustData.pwszURLReference       = NULL;                     // Not used.
    //WinTrustData.dwProvFlags            = WTD_SAFER_FLAG;           // !!! NOTE !!!  this flag may cause WinVerityTrust hang in XP/2k
    WinTrustData.dwProvFlags            = WTD_REVOCATION_CHECK_NONE;// Revocation checking is not performed.
    WinTrustData.dwUIContext            = 0;                        // Not used.
    WinTrustData.pFile                  = &FileData;                // Set pFile.

    // WinVerifyTrust verifies signatures as specified by the GUID 
    // and Wintrust_Data.
    LONG lStatus = m_modWinTrust.WinVerifyTrust(
        (HWND)INVALID_HANDLE_VALUE,
        &WVTPolicyGUID,
        &WinTrustData);
    if (ERROR_SUCCESS == lStatus)
    {
        if (pSignInfoChain)
        {
            GetSignerInfo(&WinTrustData, pSignInfoChain);

            // close verify
            WinTrustData.dwStateAction = WTD_STATEACTION_CLOSE;
            m_modWinTrust.WinVerifyTrust(
                (HWND)INVALID_HANDLE_VALUE,
                &WVTPolicyGUID,
                &WinTrustData);
        }
    }

    return lStatus;
}


DWORD CWinTrustVerifier::VerifyCatalogSignature(LPCWSTR pwszFileFullPath, HANDLE hFile, CWinTrustSignerInfoChain* pSignInfoChain)
{
    assert(pwszFileFullPath);
    assert(hFile && hFile != INVALID_HANDLE_VALUE);

    // Load wintrust.dll
    HRESULT hRet = TryLoadDll();
    if (FAILED(hRet))
        return (DWORD)hRet;


    // Create verify context
    GUID      WVTPolicyGUID = DRIVER_ACTION_VERIFY;
    HCATADMIN hCatAdmin = NULL;
    BOOL bRet = m_modWinTrust.CryptCATAdminAcquireContext(
        &hCatAdmin,
        &WVTPolicyGUID,
        0);
    if (!bRet)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    // Calculate hash of file
    BYTE  pbyHashBuf[BUFSIZ];
    DWORD dwHashSize = BUFSIZ;
    bRet = m_modWinTrust.CryptCATAdminCalcHashFromFileHandle(
        hFile,
        &dwHashSize,
        pbyHashBuf,
        0);
    if (!bRet)
    {
        m_modWinTrust.CryptCATAdminReleaseContext(hCatAdmin, 0);
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
    }



    // Guess member tag
    CString strMemberFilePath = pwszFileFullPath;
    strMemberFilePath.MakeLower();
    LPCWSTR lpFileName = ::PathFindFileName(strMemberFilePath);


    // Initialize the WINTRUST_CATALOG_INFO structure.
    WINTRUST_CATALOG_INFO CatalogData;
    memset(&CatalogData, 0, sizeof(CatalogData));
    CatalogData.cbStruct                = sizeof(WINTRUST_CATALOG_INFO);
    CatalogData.dwCatalogVersion        = 0;                    // optional: Catalog version number
    CatalogData.pcwszCatalogFilePath    = NULL;                 // required: path/name to Catalog file
    CatalogData.pcwszMemberTag          = lpFileName;           // optional: tag to member in Catalog
    CatalogData.pcwszMemberFilePath     = strMemberFilePath;    // required: path/name to member file
    CatalogData.hMemberFile             = hFile;                // optional: open handle to pcwszMemberFilePath
    CatalogData.pbCalculatedFileHash    = pbyHashBuf;           // optional: pass in the calculated hash
    CatalogData.cbCalculatedFileHash    = dwHashSize;           // optional: pass in the count bytes of the calc hash
    CatalogData.pcCatalogContext        = NULL;                 // optional: pass in to use instead of CatalogFilePath.



    DWORD dwEnumCounter = 0;
    // Look up catalog
    HCATINFO hPrevCat = NULL;
    HCATINFO hCatInfo = m_modWinTrust.CryptCATAdminEnumCatalogFromHash(
        hCatAdmin,
        pbyHashBuf,
        dwHashSize,
        0,
        &hPrevCat);
    while (hCatInfo)
    {


        // limit loop of enumeration
        if (dwEnumCounter++ > MAX_LOOP_OF_ENUM)
            break;

        CATALOG_INFO CatInfo;
        memset(&CatInfo, 0, sizeof(CatInfo));
        CatInfo.cbStruct = sizeof(CATALOG_INFO);


        // retrieve catalog info
        bRet = m_modWinTrust.CryptCATCatalogInfoFromContext(hCatInfo, &CatInfo, 0);
        if (bRet)
        {
            CatalogData.pcwszCatalogFilePath = CatInfo.wszCatalogFile;


            // Initialize the WinVerifyTrust input data structure.
            WINTRUST_DATA WinTrustData;
            memset(&WinTrustData, 0, sizeof(WinTrustData));
            WinTrustData.cbStruct = sizeof(WinTrustData);
            WinTrustData.pPolicyCallbackData    = NULL;                         // Use default code signing EKU.
            WinTrustData.pSIPClientData         = NULL;                         // No data to pass to SIP.
            WinTrustData.dwUIChoice             = WTD_UI_NONE;                  // Disable WVT UI.
            WinTrustData.fdwRevocationChecks    = WTD_REVOKE_NONE;              // No revocation checking.
            WinTrustData.dwUnionChoice          = WTD_CHOICE_CATALOG;           // Verify signature embedded in a file.
            WinTrustData.dwStateAction          = pSignInfoChain ? WTD_STATEACTION_VERIFY : 0;
            WinTrustData.hWVTStateData          = NULL;                         //
            WinTrustData.pwszURLReference       = NULL;                         // Not used.
            WinTrustData.dwProvFlags            = WTD_REVOCATION_CHECK_NONE;    // Default.
            WinTrustData.dwUIContext            = 0;                            // Not used.
            WinTrustData.pCatalog               = &CatalogData;                 // Set pCatalog.


            LONG lStatus = m_modWinTrust.WinVerifyTrust(
                (HWND)INVALID_HANDLE_VALUE,
                &WVTPolicyGUID,
                &WinTrustData);


            if (ERROR_SUCCESS == lStatus)
            {   // we're done here
                if (pSignInfoChain)
                {
                    GetSignerInfo(&WinTrustData, pSignInfoChain);


                    // close verify
                    WinTrustData.dwStateAction = WTD_STATEACTION_CLOSE;
                    m_modWinTrust.WinVerifyTrust(
                        (HWND)INVALID_HANDLE_VALUE,
                        &WVTPolicyGUID,
                        &WinTrustData);
                }


                m_modWinTrust.CryptCATAdminReleaseCatalogContext(hCatAdmin, hCatInfo, 0);
                m_modWinTrust.CryptCATAdminReleaseContext(hCatAdmin, 0);
                return ERROR_SUCCESS;
            }
        }


        // look up next catalog
        hPrevCat = hCatInfo;
        hCatInfo = m_modWinTrust.CryptCATAdminEnumCatalogFromHash(
            hCatAdmin,
            pbyHashBuf,
            dwHashSize,
            0,
            &hPrevCat);
    }

    if (hCatInfo)
        m_modWinTrust.CryptCATAdminReleaseCatalogContext(hCatAdmin, hCatInfo, 0);

    m_modWinTrust.CryptCATAdminReleaseContext(hCatAdmin, 0);
    return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
}

HRESULT CWinTrustVerifier::GetSignerInfo(const WINTRUST_DATA* pWintrustData, CWinTrustSignerInfoChain* pSignerInfoChain)
{
    assert(pWintrustData);


    if (!pWintrustData || !pSignerInfoChain)
        return E_POINTER;


    pSignerInfoChain->m_chain.RemoveAll();


    // Load wintrust.dll
    HRESULT hRet = TryLoadDll();
    if (FAILED(hRet))
        return (DWORD)hRet;


    //ISSUER_INFO_LIST_ITEM item;

    CRYPT_PROVIDER_DATA* pCryptProviderData = m_modWinTrust.WTHelperProvDataFromStateData(pWintrustData->hWVTStateData);
    if (!pCryptProviderData)
        return GetLastError() ? AtlHresultFromLastError() : E_FAIL;


    for (int i = 0; i < (int)pCryptProviderData->csSigners; ++i)
    {
        CRYPT_PROVIDER_SGNR* pCryptProviderSgnr = m_modWinTrust.WTHelperGetProvSignerFromChain(
            pCryptProviderData, i, FALSE, 0);
        if (NULL == pCryptProviderSgnr)
            break;



        for (int j = 0; j < (int)pCryptProviderSgnr->csCertChain; ++j)
        {
            CRYPT_PROVIDER_CERT* pCert = m_modWinTrust.WTHelperGetProvCertFromChain(pCryptProviderSgnr, j);
            if (NULL == pCert)
                break;

            CWinTrustSignerInfo signerInfo;

            GetCertNameCString(pCert->pCert, signerInfo.m_strIsserName);
            CertNameToCString(&pCert->pCert->pCertInfo->Subject, signerInfo.m_strIsserNameRaw);

            pSignerInfoChain->m_chain.AddTail(signerInfo);
        }
    }


    return GetLastError() ? AtlHresultFromLastError() : E_FAIL;
}

HRESULT CWinTrustVerifier::GetCertNameCString(PCCERT_CONTEXT pCertContext, CString& strIssuerName)
{
    if (!pCertContext)
        return E_POINTER;


    strIssuerName.Empty();


    DWORD dwLen = ::CertGetNameStringW(
        pCertContext,
        CERT_NAME_SIMPLE_DISPLAY_TYPE,
        0,
        NULL,
        NULL,
        NULL);
    if (!dwLen)
        return S_FALSE;

    if (dwLen > 65535)
        return S_FALSE;



    LPWSTR  lpszBuffer = strIssuerName.GetBuffer(dwLen);
    if (!lpszBuffer)
        return E_OUTOFMEMORY;


    DWORD dwRet = ::CertGetNameStringW(
        pCertContext,
        CERT_NAME_SIMPLE_DISPLAY_TYPE, 
        0,
        NULL,
        lpszBuffer,
        dwLen);
    if (!dwRet)
    {
        strIssuerName.ReleaseBuffer(0);
        return S_FALSE;
    }
    else
    {
        lpszBuffer[dwLen - 1] = L'\0';
        strIssuerName.ReleaseBuffer();
    }


    return S_OK;
}


HRESULT CWinTrustVerifier::CertNameToCString(PCERT_NAME_BLOB pName, CString& strIssuerNameRaw)
{
    if (!pName)
        return E_POINTER;


    strIssuerNameRaw.Empty();


    DWORD dwLen = ::CertNameToStrW(
        1,
        pName,
        CERT_X500_NAME_STR | CERT_NAME_STR_REVERSE_FLAG,
        NULL,
        0);
    if (!dwLen)
        return S_FALSE;

    if (dwLen > 65535)
        return S_FALSE;




    LPWSTR lpszBuffer = strIssuerNameRaw.GetBuffer(dwLen);
    if (!lpszBuffer)
        return E_OUTOFMEMORY;



    DWORD dwRet = ::CertNameToStrW(
        1, 
        pName,
        CERT_X500_NAME_STR | CERT_NAME_STR_REVERSE_FLAG,
        lpszBuffer,
        dwLen);
    if (!dwRet)
    {
        strIssuerNameRaw.ReleaseBuffer(0);
        return S_FALSE;
    }
    else
    {
        lpszBuffer[dwLen - 1] = L'\0';
        strIssuerNameRaw.ReleaseBuffer();
    }


    return S_OK;
}



BOOL CWinTrustVerifier::HasEmbeddedSignature(HANDLE hFile)
{
    assert(hFile && hFile != INVALID_HANDLE_VALUE);

    // Get Certificate Header
    WIN_CERTIFICATE CertHead;   
    CertHead.dwLength  = 0;   
    CertHead.wRevision = WIN_CERT_REVISION_1_0;   
    if (!::ImageGetCertificateHeader(hFile, 0, &CertHead))   
    {   
        return FALSE;
    }   

    return TRUE;
}

BOOL CWinTrustVerifier::IsPEFile(LPCWSTR pwszFileFullPath)
{
    assert(pwszFileFullPath);

    CAtlFile hFile;
    HRESULT  hr = hFile.Create(
        pwszFileFullPath,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        OPEN_EXISTING);
    if (FAILED(hr))
        return hr;

    return IsPEFile(hFile);
}

BOOL CWinTrustVerifier::IsPEFile(HANDLE hFile)
{
    assert(hFile && hFile != INVALID_HANDLE_VALUE);

    // locate MZ header
    DWORD nNewPos = ::SetFilePointer(hFile, 0, 0, FILE_BEGIN);
    if (nNewPos == INVALID_SET_FILE_POINTER)
        return FALSE;

    // read MZ header
    IMAGE_DOS_HEADER DosHeader;
    DWORD dwBytesRead;
    BOOL br = ::ReadFile(hFile, &DosHeader, sizeof(DosHeader), &dwBytesRead, NULL);
    if (!br || dwBytesRead != sizeof(DosHeader))
        return FALSE;

    // check MZ magic number
    if (IMAGE_DOS_SIGNATURE != DosHeader.e_magic)
        return FALSE;

    // local PE header
    nNewPos = ::SetFilePointer(hFile, DosHeader.e_lfanew, 0, FILE_BEGIN);
    if (nNewPos == INVALID_SET_FILE_POINTER)
        return FALSE;

    // read PE header
    DWORD dwPeSign = 0;
    br = ::ReadFile(hFile, &dwPeSign, sizeof(DWORD), &dwBytesRead, NULL);
    if (!br || dwBytesRead != sizeof(DWORD))
        return FALSE;
    
    // check PE magic number
    if (IMAGE_NT_SIGNATURE != dwPeSign)
        return FALSE;

    return TRUE;
}

BOOL CWinTrustVerifier::IsWinTrustRetCode(DWORD dwRetCode)
{
    if (ERROR_SUCCESS == dwRetCode)
        return TRUE;

    if (dwRetCode & 0x30000000) // Customer code flag
        return FALSE;

    if (HRESULT_FACILITY(dwRetCode) == FACILITY_WIN32)
    {   // win32 error
        switch (HRESULT_CODE(dwRetCode))
        {
        case ERROR_NOT_FOUND:
            return TRUE;
        }
    }
    else if (HRESULT_FACILITY(dwRetCode) == FACILITY_CERT)
    {   // Cert error
        return TRUE;
    }

    return FALSE;
}

HRESULT CWinTrustVerifier::TryLoadDll()
{
    if (m_modWinTrust)
        return S_OK;

    return m_modWinTrust.LoadLib(L"wintrust.dll");
}
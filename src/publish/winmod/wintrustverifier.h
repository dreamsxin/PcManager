/**
* @file    wintrustverifier.h
* @brief   ...
* @author  bbcallen
* @date    2009-02-11  17:22
*/

#ifndef WINTRUSTVERIFIER_H
#define WINTRUSTVERIFIER_H

#include <wincrypt.h>
#include <atlcoll.h>
#include "winmodbase.h"
#include "wintrustmod.h"

NS_WINMOD_BEGIN


class CWinTrustSignerInfo
{
public:
    CString m_strIsserName;
    CString m_strIsserNameRaw;
};

class CWinTrustSignerInfoChain
{
public:
    typedef CAtlList<CWinTrustSignerInfo> CSignInfoChain;

    CSignInfoChain m_chain;
};


/// MSDN says WinVerifyTrust may be altered or unavailable 
/// in subsequent versions, so we call LoadLibaray to obtain WinVerifyTrust
class CWinTrustVerifier
{
public:

    DWORD   VerifyFile(LPCWSTR pwszFileFullPath, CWinTrustSignerInfoChain* pSignInfoChain = NULL);

    DWORD   VerifyFile(LPCWSTR pwszFileFullPath, HANDLE hFile, CWinTrustSignerInfoChain* pSignInfoChain);

    DWORD   VerifyEmbeddedWinTrustFile(LPCWSTR pwszFileFullPath, HANDLE hFile, CWinTrustSignerInfoChain* pSignInfoChain);

    DWORD   VerifyEmbeddedSignature(LPCWSTR pwszFileFullPath, HANDLE hFile, GUID& policyGUID, CWinTrustSignerInfoChain* pSignInfoChain);

    DWORD   VerifyCatalogSignature(LPCWSTR pwszFileFullPath, HANDLE hFile, CWinTrustSignerInfoChain* pSignInfoChain);

    HRESULT GetSignerInfo(const WINTRUST_DATA* pWintrustData, CWinTrustSignerInfoChain* pSignInfoChain);

    HRESULT GetCertNameCString(PCCERT_CONTEXT pCertContext, CString& strIssuerName);
    
    HRESULT CertNameToCString(PCERT_NAME_BLOB pName, CString& strIssuerNameRaw);

    BOOL    HasEmbeddedSignature(HANDLE hFile);


    static BOOL IsPEFile(LPCWSTR pwszFileFullPath);

    static BOOL IsPEFile(HANDLE hFile);

    static BOOL IsWinTrustRetCode(DWORD dwRetCode);



    HRESULT TryLoadDll();



    

    CWinModule_wintrust m_modWinTrust;
};

NS_WINMOD_END

#endif//WINTRUSTVERIFIER_H
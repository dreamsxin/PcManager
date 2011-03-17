#pragma once

#include <skylark2/bkbak.h>
#include <bkcommon/bkerror.h>


struct BkSafeQuarantineInfo
{
    Skylark::BKBAK_BACKUP_ID            id;
    Skylark::BKBAK_BACKUP_SHORT_INFO    info;
    CString                             strPath;
    HRESULT                             hRet;
};

struct BkSafeQuarantineOperateItemInfo 
{
    int nIndex;
    Skylark::BKBAK_BACKUP_ID id;
	BOOL bHashed;
	BYTE szMd5[SKYLARK_MD5_BYTES];
};

#define QUARANTINE_DLL_FILE_NAME L"ksafebak.dll"

class CBkSafeQuarantine
{
public:
    CBkSafeQuarantine()
    {
    }

    ~CBkSafeQuarantine()
    {
    }

    int GetCount()
    {
        HMODULE hMod = NULL;
        PFN_BKBakCreateObject BKBakCreateObject = NULL;
        Skylark::IBKFileBackupFinder *piBackupFinder = NULL;
        BkSafeQuarantineInfo infoFind;
        int nCount = 0;

        CString strDllName = _Module.GetAppDirPath();
        strDllName += QUARANTINE_DLL_FILE_NAME;

        hMod = ::LoadLibrary(strDllName);
        if (NULL == hMod)
            goto Exit0;

        BKBakCreateObject = (PFN_BKBakCreateObject)::GetProcAddress(hMod, FN_BKBakCreateObject);
        if (NULL == BKBakCreateObject)
            goto Exit0;

        BKBakCreateObject(__uuidof(Skylark::IBKFileBackupFinder), (void**)&piBackupFinder, BKENG_CURRENT_VERSION);
        if (NULL == piBackupFinder)
            goto Exit0;

        BOOL bFindRet = piBackupFinder->FindFirstBackupFile(&infoFind.id, NULL);
        if (!bFindRet)
            goto Exit0;

        while (bFindRet) 
        {
            nCount ++;
            bFindRet = piBackupFinder->FindNextBackupFile(&infoFind.id, NULL);
        }

    Exit0:

        if (piBackupFinder)
        {
            piBackupFinder->Release();
            piBackupFinder = NULL;
        }

        if (NULL != hMod)
        {
            ::FreeLibrary(hMod);
            hMod = NULL;
        }

        return nCount;
    }

    BOOL Load(CAtlArray<BkSafeQuarantineInfo>& arrQuarantineInfo)
    {
        BOOL bRet = FALSE;
        HMODULE hMod = NULL;
        PFN_BKBakCreateObject BKBakCreateObject = NULL;
        Skylark::IBKFileBackupFinder *piBackupFinder = NULL;
        BkSafeQuarantineInfo infoFind;

        arrQuarantineInfo.RemoveAll();

        CString strDllName = _Module.GetAppDirPath();
        strDllName += QUARANTINE_DLL_FILE_NAME;

        hMod = ::LoadLibrary(strDllName);
        if (NULL == hMod)
            goto Exit0;

        BKBakCreateObject = (PFN_BKBakCreateObject)::GetProcAddress(hMod, FN_BKBakCreateObject);
        if (NULL == BKBakCreateObject)
            goto Exit0;

        BKBakCreateObject(__uuidof(Skylark::IBKFileBackupFinder), (void**)&piBackupFinder, BKENG_CURRENT_VERSION);
        if (NULL == piBackupFinder)
            goto Exit0;

        int nCount = 0, nID = 0;
        BOOL bFindRet = piBackupFinder->FindFirstBackupFile(&infoFind.id, NULL);
        if (!bFindRet)
            goto Exit0;

        while (bFindRet) 
        {
            nCount ++;
            bFindRet = piBackupFinder->FindNextBackupFile(&infoFind.id, NULL);
        }

        arrQuarantineInfo.SetCount(nCount);

        BSTR bstrPath;

        bFindRet = piBackupFinder->FindFirstBackupFile(&infoFind.id, &infoFind.info, &bstrPath);
        if (!bFindRet)
            goto Exit0;

        while (bFindRet) 
        {
            infoFind.strPath = bstrPath;

            ::SysFreeString(bstrPath);

            bstrPath = NULL;
            infoFind.hRet = S_FALSE;

            arrQuarantineInfo[nID ++] = infoFind;

            bFindRet = piBackupFinder->FindNextBackupFile(&infoFind.id, &infoFind.info, &bstrPath);
        }

        bRet = TRUE;

Exit0:

        if (piBackupFinder)
        {
            piBackupFinder->Release();
            piBackupFinder = NULL;
        }

        if (NULL != hMod)
        {
            ::FreeLibrary(hMod);
            hMod = NULL;
        }

        return bRet;
    }

    BOOL Remove(CAtlList<BkSafeQuarantineOperateItemInfo>& lstQuarantineIDs, HWND hWndNotify, UINT uMsgNotify)
    {
        BOOL bRet = FALSE;
        HMODULE hMod = NULL;
        PFN_BKBakCreateObject BKBakCreateObject = NULL;
        Skylark::IBKFileBackup *piBackup = NULL;

        CString strDllName = _Module.GetAppDirPath();
        strDllName += QUARANTINE_DLL_FILE_NAME;

        hMod = ::LoadLibrary(strDllName);
        if (NULL == hMod)
            goto Exit0;

        BKBakCreateObject = (PFN_BKBakCreateObject)::GetProcAddress(hMod, FN_BKBakCreateObject);
        if (NULL == BKBakCreateObject)
            goto Exit0;

        BKBakCreateObject(__uuidof(Skylark::IBKFileBackup), (void**)&piBackup, BKENG_CURRENT_VERSION);
        if (NULL == piBackup)
            goto Exit0;

        POSITION pos = lstQuarantineIDs.GetHeadPosition();

        while (pos)
        {
            const BkSafeQuarantineOperateItemInfo info = lstQuarantineIDs.GetNext(pos);
            piBackup->RemoveBackupFile(&info.id);
            if (hWndNotify && ::IsWindow(hWndNotify))
                ::PostMessage(hWndNotify, uMsgNotify, info.nIndex, S_OK);
        }

        bRet = TRUE;

    Exit0:

        if (piBackup)
        {
            piBackup->Release();
            piBackup = NULL;
        }

        if (NULL != hMod)
        {
            ::FreeLibrary(hMod);
            hMod = NULL;
        }

        return bRet;
    }

	static void GetStrMd5( const BYTE md5[], CString& StrMd5 )
	{
		StrMd5.Format(
			L"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
			md5[0],  md5[1],  md5[2],  md5[3], 
			md5[4],  md5[5],  md5[6],  md5[7], 
			md5[8],  md5[9],  md5[10], md5[11], 
			md5[12], md5[13], md5[14], md5[15] );
	}

    BOOL Retrieve(CAtlList<BkSafeQuarantineOperateItemInfo>& lstQuarantineIDs, HWND hWndNotify, UINT uMsgNotify)
    {
        BOOL bRet = FALSE;
        HMODULE hMod = NULL;
        PFN_BKBakCreateObject BKBakCreateObject = NULL;
        Skylark::IBKFileBackup *piBackup = NULL;
        HRESULT hRet = E_FAIL;
		int			i = 0;
		CString		strMD5S;

        CString strDllName = _Module.GetAppDirPath();
        strDllName += QUARANTINE_DLL_FILE_NAME;

        hMod = ::LoadLibrary(strDllName);
        if (NULL == hMod)
            goto Exit0;

        BKBakCreateObject = (PFN_BKBakCreateObject)::GetProcAddress(hMod, FN_BKBakCreateObject);
        if (NULL == BKBakCreateObject)
            goto Exit0;

        BKBakCreateObject(__uuidof(Skylark::IBKFileBackup), (void**)&piBackup, BKENG_CURRENT_VERSION);
        if (NULL == piBackup)
            goto Exit0;

        POSITION pos = lstQuarantineIDs.GetHeadPosition();

        while (pos)
        {
            const BkSafeQuarantineOperateItemInfo info = lstQuarantineIDs.GetNext(pos);
            hRet = piBackup->RetrieveFile(&info.id, TRUE);
            if (SUCCEEDED(hRet))
                piBackup->RemoveBackupFile(&info.id);

			CString		strMD5;
			GetStrMd5(info.szMd5, strMD5);
			strMD5S.Append(strMD5);
			strMD5S.Append(_T(","));
			if ( i++ == 20 )
			{
				i = 0;
				strMD5S.Empty();
			}

            if (hWndNotify && ::IsWindow(hWndNotify))
                ::PostMessage(hWndNotify, uMsgNotify, info.nIndex, hRet);
        }

		if (!strMD5S.IsEmpty())
		{
			strMD5S.Empty();
		}

        bRet = TRUE;

Exit0:

        if (piBackup)
        {
            piBackup->Release();
            piBackup = NULL;
        }

        if (NULL != hMod)
        {
            ::FreeLibrary(hMod);
            hMod = NULL;
        }

        return bRet;
    }

    BOOL GetBackupInfo( UINT64 backupId, BkSafeQuarantineInfo &backupInfo )
    {
        BOOL bRet = FALSE;
        HMODULE hMod = NULL;
        Skylark::BKBAK_BACKUP_ID    Id;
        PFN_BKBakCreateObject BKBakCreateObject = NULL;
        Skylark::IBKFileBackupFinder *piBackupFinder = NULL;
        BkSafeQuarantineInfo infoFind;

        Skylark::BKENG_INIT( &Id );
        Skylark::BKENG_INIT( &backupInfo.id );
        Skylark::BKENG_INIT( &backupInfo.info );

        Id.uBackupID = backupId;
        backupInfo.id.uBackupID = backupId;
        backupInfo.hRet = S_FALSE;

        CString strDllName = _Module.GetAppDirPath();
        strDllName += QUARANTINE_DLL_FILE_NAME;

        hMod = ::LoadLibrary(strDllName);
        if (NULL == hMod)
            goto Exit0;

        BKBakCreateObject = (PFN_BKBakCreateObject)::GetProcAddress(hMod, FN_BKBakCreateObject);
        if (NULL == BKBakCreateObject)
            goto Exit0;

        BKBakCreateObject(__uuidof(Skylark::IBKFileBackupFinder), (void**)&piBackupFinder, BKENG_CURRENT_VERSION);
        if (NULL == piBackupFinder)
            goto Exit0;

        BSTR bstrPath;
        HRESULT hFindRet = piBackupFinder->GetBackupShortInfo( &Id, &backupInfo.info, &bstrPath );

        if ( FAILED( hFindRet ) )
            goto Exit0;

        backupInfo.strPath = bstrPath;
        backupInfo.hRet = S_OK;

        ::SysFreeString( bstrPath );
        bstrPath = NULL;

        bRet = TRUE;

Exit0:

        if (piBackupFinder)
        {
            piBackupFinder->Release();
            piBackupFinder = NULL;
        }

        if (NULL != hMod)
        {
            ::FreeLibrary(hMod);
            hMod = NULL;
        }

        return bRet;
    }
protected:


};

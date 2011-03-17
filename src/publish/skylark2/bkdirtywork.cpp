/**
* @file    bkdirtywork.cpp
* @brief   ...
* @author  bbcallen
* @date    2010-01-26 14:49
*/

#include "stdafx.h"
#include "bkdirtywork.h"

#include "bklog.h"

NS_SKYLARK_USING

HRESULT CBKDirtyWork::Execute()
{
    RemoveExpiredBackupFile();
    RemoveExpiredScanLog();
    RemoveExpiredCache();

    return S_OK;
}


HRESULT CBKDirtyWork::RemoveExpiredBackupFile()
{
    CWinPath pathModule;
    pathModule.GetModuleFileName((HMODULE)&__ImageBase);
    pathModule.RemoveFileSpec();
    pathModule.Append(BKMOD_NAME_BKBAK);


    CWinModule_bkbak hMod_bkbak;
    HRESULT hr = hMod_bkbak.LoadLib(pathModule);
    if (FAILED(hr))
        return hr;


    CComPtr<IBKFileBackup> spiBackup;
    hr = hMod_bkbak.BKBakCreateObject(__uuidof(IBKFileBackup), (void**)&spiBackup);
    if (FAILED(hr))
        return hr;


    hr = spiBackup->RemoveExpiredBackupFiles();
    if (FAILED(hr))
        return hr;


    return S_OK;
}

HRESULT CBKDirtyWork::RemoveExpiredScanLog()
{
    CWinPath pathModule;
    pathModule.GetModuleFileName((HMODULE)&__ImageBase);
    pathModule.RemoveFileSpec();
    pathModule.Append(BKMOD_NAME_BKCACHE);


    CWinModule_bkcache hMod_bkcache;
    HRESULT hr = hMod_bkcache.LoadLib(pathModule);
    if (FAILED(hr))
        return hr;


    CComPtr<IBKEngScanLogWriter> spiLogWriter;
    hr = hMod_bkcache.BKCacheCreateObject(__uuidof(IBKEngScanLogWriter), (void**)&spiLogWriter);
    if (FAILED(hr))
        return hr;


    hr = spiLogWriter->OpenStandard(BKCACHE_STD_SCAN_LOG);
    if (FAILED(hr))
        return hr;


    hr = spiLogWriter->DeleteExpiredLogs();
    if (FAILED(hr))
        return hr;


    return S_OK;
}

HRESULT CBKDirtyWork::RemoveExpiredCache()
{
    CWinPath pathModule;
    pathModule.GetModuleFileName((HMODULE)&__ImageBase);
    pathModule.RemoveFileSpec();
    pathModule.Append(BKMOD_NAME_BKCACHE);


    CWinModule_bkcache hMod_bkcache;
    HRESULT hr = hMod_bkcache.LoadLib(pathModule);
    if (FAILED(hr))
        return hr;





    CComPtr<IBKCacheHashToLevel> spiCacheHashToLevel;
    hr = hMod_bkcache.BKCacheCreateObject(__uuidof(IBKCacheHashToLevel), (void**)&spiCacheHashToLevel);
    if (SUCCEEDED(hr))
    {
        hr = spiCacheHashToLevel->OpenStandard(BKCACHE_STD_HASH_TO_LEVEL);
        if (SUCCEEDED(hr))
        {
            hr = spiCacheHashToLevel->DeleteExpired();
        }
    }
    spiCacheHashToLevel.Release();


    CComPtr<IBKCachePathToHash> spiCachePathToHash;
    hr = hMod_bkcache.BKCacheCreateObject(__uuidof(IBKCachePathToHash), (void**)&spiCachePathToHash);
    if (SUCCEEDED(hr))
    {
        hr = spiCachePathToHash->OpenStandard(BKCACHE_STD_PATH_TO_HASH);
        if (SUCCEEDED(hr))
        {
            hr = spiCachePathToHash->DeleteExpired();
        }
    }
    spiCachePathToHash.Release();


    CComPtr<IBKCachePathToHRESULT> spiCachePathToNonPE;
    hr = hMod_bkcache.BKCacheCreateObject(__uuidof(IBKCachePathToHRESULT), (void**)&spiCachePathToNonPE);
    if (SUCCEEDED(hr))
    {
        hr = spiCachePathToNonPE->OpenStandard(BKCACHE_STD_PATH_TO_NONPE);
        if (SUCCEEDED(hr))
        {
            hr = spiCachePathToNonPE->DeleteExpired();
        }
    }
    spiCachePathToNonPE.Release();


    CComPtr<IBKCachePathToHRESULT> spiCachePathToWinTrust;
    hr = hMod_bkcache.BKCacheCreateObject(__uuidof(IBKCachePathToHRESULT), (void**)&spiCachePathToWinTrust);
    if (SUCCEEDED(hr))
    {
        hr = spiCachePathToWinTrust->OpenStandard(BKCACHE_STD_PATH_TO_WINTRUST);
        if (SUCCEEDED(hr))
        {
            hr = spiCachePathToWinTrust->DeleteExpired();
        }
    }
    spiCachePathToWinTrust.Release();


    CComPtr<IBKCachePathToHRESULT> spiCachePathToAveFT;
    hr = hMod_bkcache.BKCacheCreateObject(__uuidof(IBKCachePathToHRESULT), (void**)&spiCachePathToAveFT);
    if (SUCCEEDED(hr))
    {
        hr = spiCachePathToAveFT->OpenStandard(BKCACHE_STD_PATH_TO_AVEFT);
        if (SUCCEEDED(hr))
        {
            hr = spiCachePathToAveFT->DeleteExpired();
        }
    }
    spiCachePathToAveFT.Release();


    CComPtr<IBKCachePathToHRESULT> spiCachePathToAveBWM;
    hr = hMod_bkcache.BKCacheCreateObject(__uuidof(IBKCachePathToHRESULT), (void**)&spiCachePathToAveBWM);
    if (SUCCEEDED(hr))
    {
        hr = spiCachePathToAveBWM->OpenStandard(BKCACHE_STD_PATH_TO_AVEBWM);
        if (SUCCEEDED(hr))
        {
            hr = spiCachePathToAveBWM->DeleteExpired();
        }
    }
    spiCachePathToAveBWM.Release();




    return S_OK;
}
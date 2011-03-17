#ifndef K_FILE_DOWNLOADER_CREATE_H_
#define K_FILE_DOWNLOADER_CREATE_H_

#include "IFileDownloader.h"


typedef HRESULT __stdcall FN_DLL_GET_CLASS_OBJECT(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

#define FN_DLL_GET_CLASS_OBJECT_NAME "DllGetClassObject"

#ifdef __cplusplus
extern "C"{
#endif

inline bool CreateFileDownloader(HMODULE hModule, IFileDownloader** ppFd)
{
    CLSID   clsid     = { 0 };
    void*   pv        = NULL;

    FN_DLL_GET_CLASS_OBJECT* pfnDllGetClassObject = NULL; 
    pfnDllGetClassObject = (FN_DLL_GET_CLASS_OBJECT *)GetProcAddress(hModule, FN_DLL_GET_CLASS_OBJECT_NAME);
    if(NULL == pfnDllGetClassObject)
        return false;

    HRESULT krRetCode = pfnDllGetClassObject(clsid, __uuidof(IFileDownloader), (void** )ppFd);
    if(S_OK != krRetCode)
        return false;

    return TRUE;
}

#ifdef __cplusplus
}
#endif

#endif//K_FILE_DOWNLOADER_CREATE_H_


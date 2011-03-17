// KnInstAD.cpp : main source file for KnInstAD.exe
//

#include "stdafx.h"
#include "KnInstAD.h"
#include "MainDlg.h"
#include "bkcmdline.h"
#include "beikesafeminidownloader.h"

CKnInstADModule _Module;

HRESULT CKnInstADModule::Init(ATL::_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID)
{
    return __super::Init(pObjMap, hInstance, pLibID);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

    CBkCmdLine cmdLine;

    cmdLine.Analyze(lpstrCmdLine);

    if (cmdLine.HasParam(L"u"))
    {
//         CString strMsg;
// 
//         strMsg.Format(L"u=%s\np=%s", (CString)cmdLine[L"u"], (CString)cmdLine[L"p"]);
// 
//         ::MessageBox(NULL, strMsg, NULL, 0);

        CBkMiniDownloader downloader;
        CAtlTemporaryFile tmpFile;
        CString strTempExe, strParam = cmdLine[L"p"];

        HRESULT hRet = tmpFile.Create(NULL, GENERIC_WRITE | GENERIC_READ);
        if (FAILED(hRet))
            return -1;

        for (int i = 0; i < 3; i ++)
        {
            hRet = downloader.Download((CString)cmdLine[L"u"], tmpFile, NULL);
            if (S_OK == hRet || E_ABORT == hRet)
                break;
        }

        if (FAILED(hRet))
            return -2;

        strTempExe = tmpFile.TempFileName();
        strTempExe += L".EXE";

        tmpFile.Close(strTempExe);

        SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };

        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.hwnd = NULL;
        sei.lpVerb = _T("open");
        sei.lpFile = strTempExe;
        sei.lpParameters = (LPWSTR)(LPCWSTR)strParam;
        sei.nShow = SW_SHOWNORMAL;

        BOOL bRet = ::ShellExecuteEx(&sei);
        if (bRet)
        {
            ::WaitForSingleObject(sei.hProcess, INFINITE);
            ::CloseHandle(sei.hProcess);
        }

        ::DeleteFile(strTempExe);

        return 0;
    }

	int nRet = 0;
	// BLOCK: Run application
	{
        CWindow wndParent;
        CMainDlg dlgMain;

        if (!cmdLine.HasParam(L"test"))
        {
            wndParent = (HWND)::StrToInt((LPCTSTR)(CString)cmdLine[L"h"]);

            if (!wndParent.IsWindow())
                return -1;
        }

        dlgMain.SetAppInfo(cmdLine[L"t"], cmdLine[L"i"]);

        CRect rcWndParent(0, 0, 0, 0);
        if (wndParent.IsWindow())
        {
            if (0 == (wndParent.GetStyle() & WS_MINIMIZE))
                wndParent.GetWindowRect(rcWndParent);
        }

        dlgMain.SetPosition(rcWndParent);

		nRet = dlgMain.DoModal();
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}

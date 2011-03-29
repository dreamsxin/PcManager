// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KnInstAD.h"
#include "MainDlg.h"
#include "beikesafeminidownloader.h"
#include "bkjson.h"
#include "bkcmdline.h"

#define LIST_URL_FORMAT L"http://rm.yx.keniu.com/partner_request.php?appid=%d"
#define POST_URL_FORMAT L"http://stat.ijinshan.com/partner_checked.htm?appid=%d&id=%d&check=%d"

BOOL CMainDlg::_DownloadInfo()
{
    BOOL bRetCode = FALSE;
    CBkMiniDownloader downloader;
    CAtlTemporaryFile tmpFile;
    CString strPushAddr;
    CStringA strPushContent;
    ULONGLONG ullFileSize = 0;
    DWORD dwRead = 0;
    BkJson::Document doc;
    BOOL bFirst = m_bKSafeInstalled;

    HRESULT hRet = tmpFile.Create(NULL, GENERIC_WRITE | GENERIC_READ);
    if (FAILED(hRet))
        goto Exit0;

    strPushAddr.Format(LIST_URL_FORMAT, m_nAppID);

    hRet = downloader.Download(strPushAddr, tmpFile, NULL);
    if (FAILED(hRet))
        goto Exit0;

    hRet = tmpFile.GetSize(ullFileSize);
    if (FAILED(hRet))
        goto Exit0;

    hRet = tmpFile.Seek(0, FILE_BEGIN);
    if (FAILED(hRet))
        goto Exit0;

    hRet = tmpFile.Read(strPushContent.GetBuffer((int)(ullFileSize + 2)), (int)(ullFileSize), dwRead);
    if (FAILED(hRet))
    {
        strPushContent.ReleaseBuffer(0);
        goto Exit0;
    }

    strPushContent.ReleaseBuffer((int)ullFileSize);

    {
        CStringA strPushAll;

        strPushAll = "{\"content\":";
        strPushAll += strPushContent;
        strPushAll += "}";

        if (!doc.LoadString(CA2T(strPushAll, CP_UTF8)))
            goto Exit0;
    }

    {
        BkJson::Handle jList = doc[L"content"];

        int nListSize = jList.Count();

        for (int i = 0; i < nListSize; i ++)
        {
            BkJson::Handle jItem = jList[i];
            _SoftInfo info;

            info.lID            = ::StrToLong(jItem[L"id"]);
            info.lType          = ::StrToLong(jItem[L"type"]);
            info.strName        = jItem[L"name"];
            info.strUrl         = jItem[L"url"];
            info.strParam       = jItem[L"params"];
            info.strCondition   = jItem[L"condition"];

            CString strDesc = jItem[L"describe"];
            int nSplit = strDesc.Find(L'|');
            if (-1 == nSplit)
            {
                info.strDesc1   = strDesc;
                info.strDesc2   = L"";
            }
            else
            {
                info.strDesc1   = strDesc.Left(nSplit);
                info.strDesc2   = strDesc.Mid(nSplit + 1);
            }

            if (!_IsInstalled(info))
            {
                bRetCode = TRUE;

                if (bFirst)
                {
                    m_SoftInfo1 = info;
                    bFirst = FALSE;
                }
                else
                {
                    m_SoftInfo2 = info;
                    break;
                }
            }
        }
    }

Exit0:

    return bRetCode;
}

INT_PTR CMainDlg::DoModal(HWND hWndParent, LPARAM dwInitParam)
{
    _SoftInfo SoftInfoKSafe;
    SoftInfoKSafe.lID           = -1;
    SoftInfoKSafe.lType         = _SoftInfo::TypeSoftware;
    SoftInfoKSafe.strCondition  = L"exist_regv(HKLM,SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Maxthon3,DisplayName)";
    SoftInfoKSafe.strName       = L"推荐安装傲游3双核浏览器,并设为默认浏览器";
    SoftInfoKSafe.strDesc1      = L"搭载独立浏览核心，上网速度全面提升。\r\n专为国人习惯优化，更高效，更安全，更私密。";
    SoftInfoKSafe.strDesc2      = L"上网从傲游开始！";
    SoftInfoKSafe.strParam      = L"";
// #ifdef _SHANDIAN_VERSION
//     SoftInfoKSafe.strUrl        = L"http://www.keniu.com/dl.php?p=ksafe";
// #else
    SoftInfoKSafe.strUrl        = L"http://mk.maxthon.cn/jinshan4/maxthon3_jinshan.exe";
//#endif

    m_bKSafeInstalled = _IsInstalled(SoftInfoKSafe);

    if (!m_bKSafeInstalled)
        m_SoftInfo1 = SoftInfoKSafe;
    else
        return IDCANCEL;
// #ifdef _SHANDIAN_VERSION
//     if (m_bKSafeInstalled)
//         return IDCANCEL;
// #else
//     if (!_DownloadInfo() && m_bKSafeInstalled)
//         return IDCANCEL;
// #endif

    return __super::DoModal(hWndParent, dwInitParam);
}

BOOL CMainDlg::OnInitDialog(HWND hWndFocus, LPARAM lInitParam)
{
    if (0 != m_rcPosition.Width())
        MoveWindow(m_rcPosition);
    else
        CenterWindow();

    // set icons
    HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
        IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
        IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    SetIcon(hIconSmall, FALSE);

    CString strTitle;

    strTitle.Format(L"%s 安装", m_strAppTitle);

    SetWindowText(strTitle);

    if (m_SoftInfo1.strName.IsEmpty())
    {
        GetDlgItem(IDC_CHK_SOFT_1).ShowWindow(SW_HIDE);
        GetDlgItem(IDC_LBL_SOFT_1_DESCRIPTION_1).ShowWindow(SW_HIDE);
        GetDlgItem(IDC_LBL_SOFT_1_DESCRIPTION_2).ShowWindow(SW_HIDE);
    }
    else
    {
        SetDlgItemText(IDC_CHK_SOFT_1, m_SoftInfo1.strName);
        SetDlgItemText(IDC_LBL_SOFT_1_DESCRIPTION_1, m_SoftInfo1.strDesc1);
        SetDlgItemText(IDC_LBL_SOFT_1_DESCRIPTION_2, m_SoftInfo1.strDesc2);
    }

    if (m_SoftInfo2.strName.IsEmpty())
    {
        GetDlgItem(IDC_CHK_SOFT_2).ShowWindow(SW_HIDE);
        GetDlgItem(IDC_LBL_SOFT_2_DESCRIPTION_1).ShowWindow(SW_HIDE);
        GetDlgItem(IDC_LBL_SOFT_2_DESCRIPTION_2).ShowWindow(SW_HIDE);
    }
    else
    {
        SetDlgItemText(IDC_CHK_SOFT_2, m_SoftInfo2.strName);
        SetDlgItemText(IDC_LBL_SOFT_2_DESCRIPTION_1, m_SoftInfo2.strDesc1);
        SetDlgItemText(IDC_LBL_SOFT_2_DESCRIPTION_2, m_SoftInfo2.strDesc2);
    }

    CheckDlgButton(IDC_CHK_SOFT_1, BST_CHECKED);
    CheckDlgButton(IDC_CHK_SOFT_2, BST_CHECKED);

    LOGFONT logFont;
    ::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &logFont);

    //logFont.lfHeight = -14;

    CFontHandle font;
    font.CreateFontIndirect(&logFont);
    GetDlgItem(IDC_CHK_SOFT_1).SetFont(font.Detach());
    font.CreateFontIndirect(&logFont);
    GetDlgItem(IDC_CHK_SOFT_2).SetFont(font.Detach());

    HMENU hMenu = ::GetSystemMenu(m_hWnd, FALSE);

    ::EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
    ::DestroyMenu(hMenu);

    return TRUE;
}

void CMainDlg::OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
    ShowWindow(SW_HIDE);
    EndDialog(IDOK);

    if (IsDlgButtonChecked(IDC_CHK_SOFT_1))
    {
        _Install(m_SoftInfo1);
        if (m_bKSafeInstalled)
            _PostInstall(m_SoftInfo1, TRUE);
    }
    else
    {
        if (m_bKSafeInstalled)
            _PostInstall(m_SoftInfo1, FALSE);
    }

    if (!m_SoftInfo2.strName.IsEmpty() && IsDlgButtonChecked(IDC_CHK_SOFT_2))
    {
        _Install(m_SoftInfo2);
        _PostInstall(m_SoftInfo2, TRUE);
    }
    else
    {
        _PostInstall(m_SoftInfo2, FALSE);
    }
}

void CMainDlg::OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
    ShowWindow(SW_HIDE);
    EndDialog(IDCANCEL);

    if (m_bKSafeInstalled)
        _PostInstall(m_SoftInfo1, FALSE);

    if (!m_SoftInfo2.strName.IsEmpty())
        _PostInstall(m_SoftInfo2, FALSE);
}

void CMainDlg::SetAppInfo(const CString &strAppTitle, int nAppID)
{
    m_strAppTitle = strAppTitle;
    m_nAppID = nAppID;
}

void CMainDlg::SetPosition(LPRECT lprect)
{
    m_rcPosition = lprect;
}

HBRUSH CMainDlg::OnCtlColor(CDCHandle dc, CStatic /*wndStatic*/)
{
    dc.SetBkMode(TRANSPARENT);
    dc.SetBkColor(RGB(0xFF, 0xFF, 0xFF));

    return (HBRUSH)::GetStockObject(WHITE_BRUSH);
}

HRESULT CheckCondition(LPCTSTR lpszCondition, int nSize);
HRESULT CheckHomePage(LPCTSTR lpszCondition, int nSize);
void SetHomePage(LPCTSTR lpszUrl);

BOOL CMainDlg::_IsInstalled(_SoftInfo &Info)
{
    HRESULT hr = E_FAIL;
    switch (Info.lType)
    {
    case _SoftInfo::TypeSoftware:
        hr = CheckCondition(Info.strCondition, Info.strCondition.GetLength());
        return (S_FALSE != hr);

    case _SoftInfo::TypeHomepage:
        hr = CheckHomePage(Info.strCondition, Info.strCondition.GetLength());
        return (S_FALSE != hr);
    }

    return TRUE;
}

BOOL CMainDlg::_Install(_SoftInfo &Info)
{
    switch (Info.lType)
    {
    case _SoftInfo::TypeSoftware:
        {
            CBkCmdLine cmdLine;
            CString strAppPath;

            ::GetModuleFileName(NULL, strAppPath.GetBuffer(MAX_PATH + 10), MAX_PATH);
            strAppPath.ReleaseBuffer();

            cmdLine.SetParam(L"u", Info.strUrl);
            cmdLine.SetParam(L"p", Info.strParam);

            cmdLine.Execute(strAppPath, FALSE, FALSE);
        }
        break;

    case _SoftInfo::TypeHomepage:
        SetHomePage(Info.strUrl);
        break;
    }

    return TRUE;
}

void CMainDlg::_PostInstall(_SoftInfo &SoftInfo, BOOL bChecked)
{
    if (!SoftInfo.strName.IsEmpty())
    {
        CBkMiniDownloader downloader;
        CAtlTemporaryFile tmpFile;
        CString strPostAddr;

        HRESULT hRet = tmpFile.Create(NULL, GENERIC_WRITE | GENERIC_READ);
        if (SUCCEEDED(hRet))
        {
            class _SimpleCallback : public IBkMiniDownloaderCallback
            {
            public:
                BOOL OnProgress(DWORD /*dwTotalSize*/, DWORD /*dwDownloadedSize*/)
                {
                    return FALSE;
                }
            };
            strPostAddr.Format(POST_URL_FORMAT, m_nAppID, SoftInfo.lID, bChecked);

            downloader.Download(strPostAddr, tmpFile, &_SimpleCallback());
        }
    }
}

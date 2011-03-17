// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:

    void SetAppInfo(const CString &strAppTitle, int nAppID);
    void SetPosition(LPRECT lprect);

    INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL);

protected:

    class _SoftInfo
    {
    public:
        enum {
            TypeSoftware = 1,
            TypeHomepage = 2,
        };
        LONG    lID;
        LONG    lType;
        CString strName;
        CString strDesc1;
        CString strDesc2;
        CString strUrl;
        CString strParam;
        CString strCondition;
    };

    _SoftInfo m_SoftInfo1;
    _SoftInfo m_SoftInfo2;

    CString m_strAppTitle;
    int     m_nAppID;
    CRect   m_rcPosition;
    BOOL    m_bKSafeInstalled;

    BOOL _DownloadInfo();
    BOOL _IsInstalled(_SoftInfo &Info);
    BOOL _Install(_SoftInfo &Info);
    void _PostInstall(_SoftInfo &SoftInfo, BOOL bChecked);

    BOOL OnInitDialog(HWND hWndFocus, LPARAM lInitParam);
    void OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/);
    void OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/);
    HBRUSH OnCtlColor(CDCHandle dc, CStatic wndStatic);

public:
    enum { IDD = IDD_MAINDLG };

    BEGIN_MSG_MAP_EX(CMainDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        MSG_WM_CTLCOLORSTATIC(OnCtlColor)
        MSG_WM_CTLCOLORBTN(OnCtlColor)
    END_MSG_MAP()

};

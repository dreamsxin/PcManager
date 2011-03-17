//////////////////////////////////////////////////////////////////////////
//
//  Create Author: ZhangZexin
//  Create Date  : 2011-03-04
//  Description  : 垃圾清理界面，清理完成时，提示跳过那些软件的控件
//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////

typedef struct  _SKIP_ITEM  
{
    UINT nItemID;
    CString strName;
    CString strIconPath;
    ULONGLONG uSize;
} TRASH_SKIP_ITEM, *LPTRASH_SKIP_ITEM;

//////////////////////////////////////////////////////////////////////////
class CTrashSkipCtrl : public CWindowImpl<CTrashSkipCtrl>
{
public:
    DECLARE_WND_CLASS(L"TrashSkipCtrl");

    CTrashSkipCtrl(void);
    virtual ~CTrashSkipCtrl(void);

public:

    void Initialize(HWND hWnd);

    BOOL SubclassWindow(HWND hWnd);

public:
    void Clear();

    BOOL AddItem(TRASH_SKIP_ITEM item);

    BOOL AddItem(UINT id, CString& strName, ULONGLONG uSize);

    size_t GetSize();
protected:
    CRect m_rcClient;
    std::vector<TRASH_SKIP_ITEM> m_vecItems;
    HWND m_hNotifyHwnd;

    HICON m_iconIE;
    HICON m_iconUnknow;

public:

    BEGIN_MSG_MAP_EX(CTrashSkipCtrl)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_ERASEBKGND(OnEarseBKGND)
        MSG_WM_SIZE(OnSize)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    void OnPaint(CDCHandle dc);
    BOOL OnEarseBKGND(CDCHandle dc);
    void OnSize(UINT nType, CSize size);

protected:
    void _OnRealPaint(CDC& memDC);

    BOOL _GetItemIcon(LPCTSTR lpFileName, HICON& icon);

    void _DrawText(CDC& memDC, CRect& rcItem, LPCTSTR pszText , COLORREF colorText , HFONT hFont);

    void _DrawLine(CDC& memDC, CPoint ptStart, CPoint ptEnd, COLORREF color, DWORD dwPenTyple = PS_SOLID);
};

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "trashpublicfunc.h"
#include "ktrashskipctrl.h"

//////////////////////////////////////////////////////////////////////////

CTrashSkipCtrl::CTrashSkipCtrl(void)
{
}

CTrashSkipCtrl::~CTrashSkipCtrl(void)
{
    DestroyIcon(m_iconIE);
    DestroyIcon(m_iconUnknow);
}

void CTrashSkipCtrl::Initialize(HWND hWnd)
{
    KAppRes& res = KAppRes::Instance();

    m_hNotifyHwnd = hWnd;
    
    m_iconIE = res.GetIcon("IDI_BROWER_IE");
    m_iconUnknow = res.GetIcon("IDI_UNKNOW_BROWER");
}

BOOL CTrashSkipCtrl::SubclassWindow(HWND hWnd)
{
    BOOL bRet = FALSE;

    bRet = CWindowImpl<CTrashSkipCtrl>::SubclassWindow(hWnd);
    GetClientRect(&m_rcClient);
    return bRet;
}
//////////////////////////////////////////////////////////////////////////
BOOL CTrashSkipCtrl::AddItem(UINT id, CString& strName, ULONGLONG uSize)
{
    TRASH_SKIP_ITEM item;
    std::vector<CString> vecProName;
    item.nItemID = id;
    item.strName = strName;
    item.uSize = uSize;

    GetPorcessNameByID(id, vecProName);

    if (vecProName.size() > 0)
    {
        GetProcessPathFromName(vecProName[0], item.strIconPath);
    }

    m_vecItems.push_back(item);

    return TRUE;
}

void CTrashSkipCtrl::Clear()
{
    m_vecItems.clear();   
}
size_t CTrashSkipCtrl::GetSize()
{
    return m_vecItems.size();
}
//////////////////////////////////////////////////////////////////////////
void CTrashSkipCtrl::OnPaint(CDCHandle dcHandle)
{
    CPaintDC dc(m_hWnd);
    CBitmap memBmp;
    CDC memDC;
    CRect rcDraw(m_rcClient);
    
    memDC.CreateCompatibleDC(dc.m_hDC);
    memBmp.CreateCompatibleBitmap(dc.m_hDC, rcDraw.Width(), rcDraw.Height());
    memDC.SetBkMode(TRANSPARENT);
    SelectObject(memDC, memBmp);
    
    memDC.FillSolidRect(rcDraw, RGB(0xff, 0xff, 0xff));

    _OnRealPaint(memDC);

    dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), memDC.m_hDC, 0, 0, SRCCOPY);

    ReleaseDC(memDC);
}
BOOL CTrashSkipCtrl::OnEarseBKGND(CDCHandle dc)
{
    return TRUE;
}
void CTrashSkipCtrl::OnSize(UINT nType, CSize size)
{
    GetClientRect(&m_rcClient);
}
//////////////////////////////////////////////////////////////////////////
BOOL CTrashSkipCtrl::_GetItemIcon(LPCTSTR lpFileName, HICON& icon)
{
    BOOL bRet = FALSE;
    KAppRes& res = KAppRes::Instance();

    if (lpFileName == NULL)
        goto Clear0;

    icon = ExtractIcon(NULL, lpFileName, 0);

Clear0:
    return bRet;
}
//////////////////////////////////////////////////////////////////////////
void CTrashSkipCtrl::_OnRealPaint(CDC& memDC)
{
    CRect rcDraw(m_rcClient);
    HFONT hFont;
    CString strNum;
    
    rcDraw.top += 10;
    rcDraw.left += 15;
    rcDraw.bottom = rcDraw.top + 20;
    
    hFont = KuiFontPool::GetFont(FALSE, FALSE, FALSE, 0);
    _DrawText(memDC, rcDraw, L"清理时已跳过", RGB(0,0,0), hFont);

    strNum.Format(L"%d", GetSize());

    rcDraw.left = rcDraw.right + 5;
    rcDraw.right = m_rcClient.right;

    _DrawText(memDC, rcDraw, strNum, RGB(255,0,0), hFont);

    rcDraw.left = rcDraw.right + 5;
    rcDraw.right = m_rcClient.right;

    _DrawText(memDC, rcDraw, L"个正在运行的程序：", RGB(0,0,0), hFont);

    _DrawLine(memDC, CPoint(15, rcDraw.bottom + 5), CPoint(m_rcClient.right - 20, rcDraw.bottom + 5), RGB(224, 232, 246));

    for (size_t iCount = 0; iCount < m_vecItems.size(); ++iCount)
    {
        TRASH_SKIP_ITEM item = m_vecItems[iCount];
        HICON hIcon = NULL;
        HFONT nameFont = KuiFontPool::GetFont(TRUE, FALSE, FALSE);
        
        if (iCount >= 11)
        {
            _DrawText(memDC, rcDraw, L"-----", RGB(0,0,0), nameFont);
            break;
        }

        _GetItemIcon(item.strIconPath, hIcon);



        if (iCount % 3 == 0)
        {
            rcDraw.top = rcDraw.bottom + 20;
            rcDraw.bottom = rcDraw.top + 32;
            rcDraw.left = 15;
            rcDraw.right = rcDraw.left + 32;
        }
        else if (iCount % 3 == 1)
        {
            rcDraw.left = 210;
            rcDraw.right = rcDraw.left + 32;
        }
        else if (iCount % 3 == 2)
        {
            rcDraw.left = 400;
            rcDraw.right = rcDraw.left + 32;
        }
        if (!hIcon)
        {            
            KAppRes& res = KAppRes::Instance();

            if (item.nItemID == BROWER_IE)
            {
                hIcon = m_iconIE;
            }
            else
            {
                hIcon = m_iconUnknow;
            }
            memDC.DrawIcon(rcDraw.left, rcDraw.top, hIcon);
        }
        else
        {
            memDC.DrawIconEx(rcDraw.left, rcDraw.top, hIcon, 32, 32);
        }
        rcDraw.left = rcDraw.right + 5;
        rcDraw.right = m_rcClient.right;

        _DrawText(memDC, rcDraw, item.strName, RGB(0,0,0), nameFont);


        if (hIcon && (hIcon != m_iconIE && hIcon != m_iconUnknow))
        {
            DestroyIcon(hIcon);
        }
     }

    rcDraw.left = 15;
    rcDraw.right = m_rcClient.right;
    rcDraw.top = rcDraw.bottom + 15;
    rcDraw.bottom = rcDraw.top + 20;

    _DrawText(memDC, rcDraw, L"关闭上述软件后重新扫描，还可以释放空间约", RGB(0,0,0), hFont);
    
    ULONGLONG totalSize = 0;
    CString strSize;

    for (size_t iCount = 0; iCount < m_vecItems.size(); ++iCount)
    {
        totalSize += m_vecItems[iCount].uSize;
    }

    GetFileSizeTextString(totalSize, strSize);
    rcDraw.left = rcDraw.right + 5;
    rcDraw.right = m_rcClient.right;

    _DrawText(memDC, rcDraw, strSize, RGB(255,0,0), hFont);

    rcDraw.left = rcDraw.right + 5;
    rcDraw.right = m_rcClient.right;

    _DrawText(memDC, rcDraw, L"磁盘空间。", RGB(0,0,0), hFont);

    
}
void CTrashSkipCtrl::_DrawText(CDC& memDC, CRect& rcItem, 
                                LPCTSTR pszText , COLORREF colorText , HFONT hFont)
{
    if (NULL == pszText)
        return;

    CRect rcText = rcItem;

    memDC.SetBkMode(TRANSPARENT);
    HFONT hftOld = memDC.SelectFont(hFont);
    COLORREF crOld = memDC.SetTextColor(colorText);
    DWORD dwFormat = DT_SINGLELINE|DT_LEFT|DT_NOPREFIX|DT_END_ELLIPSIS|DT_VCENTER;

    CSize sizetemp;
    memDC.GetTextExtent(pszText, (int)_tcslen(pszText), &sizetemp);
    if(rcText.right - 20 >= rcText.left + sizetemp.cx)
        rcText.right = rcText.left + sizetemp.cx;
    else 
        rcText.right = rcText.right - 20;


    //	rcText.OffsetRect(0,6);
    memDC.DrawText(pszText, (int)_tcslen(pszText), rcText, dwFormat);
    memDC.SelectFont(hftOld);
    memDC.SetTextColor(crOld);

    rcItem = rcText;
}
void CTrashSkipCtrl::_DrawLine(CDC& memDC, CPoint ptStart, CPoint ptEnd, COLORREF color, DWORD dwPenTyple /*=PS_SOLID*/)
{
    HDC hDC = memDC.m_hDC;
    if (NULL == hDC)
        return ;

    HPEN hPen = ::CreatePen(dwPenTyple , 1, color), hOldPen = NULL;

    hOldPen = (HPEN)::SelectObject(hDC, hPen);

    ::MoveToEx(hDC,ptStart.x, ptStart.y, NULL);
    ::LineTo(hDC, ptEnd.x, ptEnd.y);

    ::SelectObject(hDC, hOldPen);

    ::DeleteObject(hPen);
}
//////////////////////////////////////////////////////////////////////////

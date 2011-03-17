//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkDlgView
// Description: Dialog View, Real Container of BkWindow
//     Creator: ZhangXiaoxuan
//     Version: 2010.5.11 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkdlgview.h"

//////////////////////////////////////////////////////////////////////////
// Beike List View Notify

// Get Display Info Notify
#define BKLVM_GET_DISPINFO  1

typedef struct _BKLVMGETDISPINFO
{
    NMHDR       hdr;
    int         nListItemID;
    BOOL        bHover;
    BOOL        bSelect;
} BKLVMGETDISPINFO, *LPBKLVMGETDISPINFO;

// Item Click Notify
#define BKLVM_ITEMCLICK     2

typedef struct _BKLVMITEMCLICK
{
    NMHDR       hdr;
    int         nListItemID;
    UINT        uCmdID;
} BKLVMITEMCLICK, *LPBKLVMITEMCLICK;

//////////////////////////////////////////////////////////////////////////

typedef CWinTraits<WS_CHILD | WS_TABSTOP | WS_VISIBLE | LVS_REPORT | LVS_OWNERDATA | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER, 0> CBkListViewTraits;

template <class T, class TBase = CListViewCtrl, class TWinTraits = CBkListViewTraits>
class CBkListViewImpl
    : public ATL::CWindowImpl<T, TBase, TWinTraits>
    , public CBkViewImpl<T>
    , public CCustomDraw<T>
{
    friend CBkViewImpl;

protected:

    typedef ATL::CWindowImpl<T, TBase, TWinTraits>  __baseClass;
    typedef CBkListViewImpl<T, TBase, TWinTraits>   __thisClass;

    CBkDialog       m_bkListTemplate;
    CToolTipCtrl    m_wndToolTip;
    BOOL            m_bXmlLoaded;
    CBkImage        m_imgMem;
    int             m_nItemHeight;

    // Only one hover control
    HBKWND          m_hBkWndHover;
    // Only one pushdown control
    HBKWND          m_hBkWndPushDown;

    int             m_nHoverItem;
    int             m_nPushItem;
    int             m_nLastDrawItem;
	BOOL			m_bWhenResizeScrollTop;

    // Tracking flag
    BOOL            m_bTrackFlag;

    BOOL            m_bCaptureSetted;
public:

    CBkListViewImpl()
        : m_bXmlLoaded(FALSE)
        , m_nItemHeight(0)
        , m_hBkWndHover(NULL)
        , m_hBkWndPushDown(NULL)
        , m_bTrackFlag(FALSE)
        , m_bCaptureSetted(FALSE)
        , m_nHoverItem(-1)
        , m_nPushItem(-1)
        , m_nLastDrawItem(-1)
		, m_bWhenResizeScrollTop(FALSE)
    {
    }

    ~CBkListViewImpl()
    {
		if(NULL != m_imgMem.m_hBitmap)
			m_imgMem.DeleteObject();
    }

    HWND Create(
        HWND hWndParent, _U_RECT rect = NULL, DWORD dwStyle = 0, 
        DWORD dwExStyle = 0, _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
    {
        if (NULL != m_hWnd)
            return m_hWnd;

        HWND hWnd = __baseClass::Create(hWndParent, rect, NULL, dwStyle, dwExStyle, MenuOrID, lpCreateParam);

        if (hWnd)
        {
            SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

            InsertColumn(0, L"", LVCFMT_LEFT, 1);

            m_bkListTemplate.SetContainer(m_hWnd);

            m_wndToolTip.Create(hWnd);
            m_wndToolTip.SetMaxTipWidth(400);

            CToolInfo ti(0, hWnd);
            m_wndToolTip.AddTool(ti);
            m_wndToolTip.Activate(TRUE);
        }

        return hWnd;
    };

    HWND Create(HWND hWndParent, _U_MENUorID MenuOrID)
    {
        return Create(hWndParent, NULL, 0, 0, MenuOrID, NULL);
    };

    BOOL Load(UINT uResID)
    {
        CStringA strXml;

        BOOL bRet = BkResManager::LoadResource(uResID, strXml);

        if (!bRet)
            return FALSE;

        return SetXml(strXml);
    }

    BOOL XmlLoaded()
    {
        return m_bXmlLoaded;
    }

    BOOL SetXml(LPCSTR lpszXml)
    {
        CStringA strValue;

        TiXmlDocument xmlDoc;

        m_bXmlLoaded    = FALSE;

        m_bkListTemplate.BkSendMessage(WM_DESTROY);

        { // Free stack
            xmlDoc.Parse(lpszXml, NULL, TIXML_ENCODING_UTF8);
        }

        if (xmlDoc.Error())
        {
//             _Redraw();
            return FALSE;
        }

        TiXmlElement *pXmlRootElem = xmlDoc.RootElement();

        strValue = pXmlRootElem->Value();
        if (strValue != "listitem")
        {
//             _Redraw();
            return FALSE;
        }

        int nItemHeight = 0;
        pXmlRootElem->Attribute("height", &nItemHeight);

        if (0 != nItemHeight)
        {
            _SetItemHeight(nItemHeight);

            m_bkListTemplate.Load(pXmlRootElem);
            CStringA strPos;
            strPos.Format("0,0,-0,%d", nItemHeight);
            m_bkListTemplate.SetAttribute("pos", strPos, TRUE);
        }

        m_bXmlLoaded = TRUE;

        if (!m_hWnd)
            return TRUE;

//         _Redraw();

//         m_hBkWndHover = NULL;
//         m_hBkWndPushDown = NULL;

        return TRUE;
    }

    void RedrawItem(int nItem)
    {
        _RedrawItem(nItem, TRUE);
    }

    CBkWindow* FindChildByCmdID(UINT uCmdID)
    {
        return m_bkListTemplate.FindChildByCmdID(uCmdID);
    }

    BOOL SetItemText(UINT uItemID, LPCTSTR lpszText)
    {
        return CBkViewImpl<T>::SetItemText(uItemID, lpszText);
    }

	VOID SetWhenResizeScrollToTop( BOOL bAutoTop )
	{
		m_bWhenResizeScrollTop = bAutoTop;
	}

//     BOOL SetItemCount(int nItems)
//     {
//         SetColumnWidth(0, 1);
// 
//         BOOL bRet = __super::SetItemCount(nItems);
// 
//         _ResizeColumn();
// 
//         return bRet;
//     }

public:

    DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
    {
        return CDRF_NOTIFYITEMDRAW;
    }

    DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
    {
        LPNMLVCUSTOMDRAW lpcd = (LPNMLVCUSTOMDRAW)lpNMCustomDraw;

        BkWinManager::EnterPaintLock();

        int nItem = (int)lpcd->nmcd.dwItemSpec;

        if (m_nLastDrawItem != nItem)
        {
            _GetItemDisplayInfo(nItem);
        }

        CDC dcMem;
        CDCHandle dcMemHandle;
        HDC hDCDesktop = ::GetDC(NULL);
        dcMem.CreateCompatibleDC(hDCDesktop);
        ::ReleaseDC(NULL, hDCDesktop);
        HBITMAP hbmpOld = dcMem.SelectBitmap(m_imgMem);

        HFONT hftOld = dcMem.SelectFont(BkFontPool::GetFont(BKF_DEFAULTFONT));

        dcMem.SetBkMode(TRANSPARENT);

        CRgn rgnNull;

        dcMemHandle.Attach(dcMem);

        CBkWindow *pWndHover = NULL, *pWndPush = NULL;

        if (nItem != m_nHoverItem && NULL != m_hBkWndHover)
        {
            pWndHover = BkWnds::GetWindow(m_hBkWndHover);
            pWndHover->ModifyState(0, BkWndState_Hover);
        }

        if (nItem != m_nPushItem && NULL != m_hBkWndPushDown)
        {
            pWndPush = BkWnds::GetWindow(m_hBkWndPushDown);
            pWndPush->ModifyState(0, BkWndState_PushDown);
        }

        m_bkListTemplate.RedrawRegion(dcMemHandle, rgnNull);

        if (pWndHover)
            pWndHover->ModifyState(BkWndState_Hover, 0);

        if (pWndPush)
            pWndPush->ModifyState(BkWndState_PushDown, 0);

        dcMemHandle.Detach();

        dcMem.SelectFont(hftOld);
        dcMem.SelectBitmap(hbmpOld);

        BkWinManager::LeavePaintLock();

        CRect rcItem;
        TBase::GetItemRect(nItem, rcItem, LVIR_BOUNDS);

        //CDCHandle dc = lpcd->nmcd.hdc;

        //dc.FillSolidRect(rcItem, RGB(0xCC, 0xCC, 0xCC));

        m_imgMem.Draw(lpcd->nmcd.hdc, rcItem.left, rcItem.top);

        return CDRF_SKIPDEFAULT;
    }

	void SetItemHeight(int nHeight)
	{	
		_SetItemHeight( nHeight );
	}

protected:

    BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
    {
        return FALSE;
    }

    void _RedrawItem(int nItem, BOOL bReload = FALSE)
    {
        if (-1 == nItem)
            return;

        if (bReload)
            m_nLastDrawItem = -1;

        ATLTRACE(L"RedrawItem(%d)\r\n", nItem);

        RedrawItems(nItem, nItem);
    }

    void _SetItemHeight(int nHeight)
    {
        CImageList imglst;

        imglst.Create(1, nHeight - 1, ILC_COLOR24, 0, 1);

        __super::SetImageList(imglst, LVSIL_SMALL);
        __super::SetImageList(NULL, LVSIL_SMALL);

        imglst.Destroy();

        m_nItemHeight = nHeight;

        CRect rcClient;

        GetClientRect(rcClient);

        if (rcClient.Width() == 0)
            return;

        if (m_imgMem.M_HOBJECT)
            m_imgMem.DeleteObject();

        m_imgMem.CreateBitmap(rcClient.Width(), m_nItemHeight, RGB(0, 0, 0));
    }

    void _ModifyWndState(CBkWindow *pWnd, DWORD dwStateAdd, DWORD dwStateRemove)
    {
        pWnd->ModifyState(dwStateAdd, dwStateRemove);
    }

    void _GetItemDisplayInfo(int nItem)
    {
        BKLVMGETDISPINFO nms;
        nms.hdr.code    = BKLVM_GET_DISPINFO;
        nms.hdr.hwndFrom = m_hWnd;
        nms.hdr.idFrom  = GetDlgCtrlID();
        nms.bHover      = nItem == m_nHoverItem;
        nms.bSelect     = nItem == CListViewCtrl::GetSelectedIndex();
        nms.nListItemID = nItem;
        ::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

        m_nLastDrawItem = nItem;
    }

    void _InvalidateControl(CBkWindow *pWnd, BOOL bCheckVisible = TRUE)
    {
    }

    int _HitTest(CPoint& point)
    {
        UINT uFlags = 0;
        int nItem = CListViewCtrl::HitTest(point, &uFlags);

        HBKWND hBkWndHitTest = NULL;

        if (-1 != nItem)
        {
            CRect rcItem;
            TBase::GetItemRect(nItem, rcItem, LVIR_BOUNDS);
            point -= rcItem.TopLeft();
        }

        return nItem;
    }

    void _ResizeColumn()
    {
        CRect rcClient;

        GetWindowRect(rcClient);

        if (rcClient.Width() == 0)
            return;

        if (m_imgMem.M_HOBJECT)
            m_imgMem.DeleteObject();

        rcClient.MoveToXY(0, 0);
        rcClient.right -= ::GetSystemMetrics(SM_CXVSCROLL);

		ModifyStyle(WS_HSCROLL,0);

		SetColumnWidth(0, rcClient.Width());

        m_imgMem.CreateBitmap(rcClient.Width(), m_nItemHeight, RGB(0, 0, 0));

        WINDOWPOS WndPos = { 0, 0, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_SHOWWINDOW };

        m_bkListTemplate.BkSendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&WndPos);

    }

    void OnSize(UINT nType, CSize /*size*/)
    {
        _ResizeColumn();
   }

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		SetMsgHandled(FALSE);

		if ( m_bWhenResizeScrollTop && GetItemCount() > 0 )
		{
			Scroll( CSize(0,-65536) );
		}

		return 0;
	}

    void OnMouseMove(UINT nFlags, CPoint point)
    {
        if (!m_bTrackFlag)
        {
            m_nLastDrawItem = -1;

            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.hwndTrack = m_hWnd;
            tme.dwFlags = TME_LEAVE;
            tme.dwHoverTime = 0;
            m_bTrackFlag = _TrackMouseEvent(&tme);
        }

        HBKWND hBkWndHitTest = NULL;
        int nItem = _HitTest(point);

        if (-1 != nItem)
            hBkWndHitTest = m_bkListTemplate.BkGetHWNDFromPoint(point, TRUE);

        if (m_nHoverItem != nItem)
        {
            CBkWindow* pWndHoverOld = BkWnds::GetWindow(m_hBkWndHover);

            if (pWndHoverOld && !pWndHoverOld->IsDisabled(TRUE))
            {
                _ModifyWndState(pWndHoverOld, 0, BkWndState_Hover);
                pWndHoverOld->BkSendMessage(WM_MOUSELEAVE);
            }

            m_hBkWndHover = NULL;

            int nOldHoverItem = m_nHoverItem;

            m_nHoverItem = nItem;

            _RedrawItem(nOldHoverItem, TRUE);
            _RedrawItem(m_nHoverItem, TRUE);
            m_wndToolTip.UpdateTipText(L"", m_hWnd);
        }

        CBkWindow* pWndHover = BkWnds::GetWindow(hBkWndHitTest);

        if (pWndHover)
        {
            pWndHover->BkSendMessage(WM_MOUSEMOVE, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

            if (hBkWndHitTest != m_hBkWndHover)
            {
                CRect rcInvalidate;

                if (NULL != m_hBkWndHover)
                {
                    CBkWindow* pWndHoverOld = BkWnds::GetWindow(m_hBkWndHover);

                    if (pWndHoverOld && !pWndHoverOld->IsDisabled(TRUE))
                    {
                        _ModifyWndState(pWndHoverOld, 0, BkWndState_Hover);
                        if (pWndHoverOld->NeedRedrawWhenStateChange())
                            _RedrawItem(m_nHoverItem);
                        pWndHoverOld->BkSendMessage(WM_MOUSELEAVE);
                    }
                }

                if (!pWndHover->IsDisabled(TRUE))
                {
                    _ModifyWndState(pWndHover, BkWndState_Hover, 0);
                    if (pWndHover->NeedRedrawWhenStateChange())
                        _RedrawItem(m_nHoverItem);
                }

                m_hBkWndHover = hBkWndHitTest;

                if (pWndHover)
                {
                    _GetItemDisplayInfo(m_nHoverItem);
                    m_wndToolTip.UpdateTipText(pWndHover->GetToolTipText(), m_hWnd);
                }
            }
        }
    }

    void OnMouseLeave()
    {
        m_wndToolTip.UpdateTipText(L"", m_hWnd);

        m_bTrackFlag = FALSE;

        if (m_hBkWndHover)
        {
            CBkWindow* pWndHover = BkWnds::GetWindow(m_hBkWndHover);
            if (pWndHover)
            {
                _ModifyWndState(pWndHover, 0, BkWndState_Hover);
                pWndHover->BkSendMessage(WM_MOUSELEAVE);
            }

            m_hBkWndHover = NULL;
        }

//         if (m_hBkWndPushDown)
//         {
//             CBkWindow* pWndPushdown = BkWnds::GetWindow(m_hBkWndPushDown);
//             if (pWndPushdown)
//                 _ModifyWndState(pWndPushdown, 0, BkWndState_PushDown);
// 
//             m_hBkWndPushDown = NULL;
//         }

        _RedrawItem(m_nHoverItem, TRUE);

        m_nHoverItem = -1;
    }

    void OnLButtonDown(UINT nFlags, CPoint point)
    {
        SetFocus();

        HBKWND hBkWndHitTest = NULL;
        int nItem = _HitTest(point), nSelectItem = CListViewCtrl::GetSelectedIndex();

        if (-1 != nItem)
            hBkWndHitTest = m_bkListTemplate.BkGetHWNDFromPoint(point, TRUE);

        CListViewCtrl::SelectItem(nItem);

        if (nItem != nSelectItem)
        {
            int nOldSelect = nSelectItem;
            nSelectItem = nItem;
            _RedrawItem(nOldSelect, TRUE);
        }

        CBkWindow* pWndPushDown = BkWnds::GetWindow(hBkWndHitTest);

        if (pWndPushDown)
        {
            if (pWndPushDown->IsDisabled(TRUE))
                return;

            if (!m_bCaptureSetted)
            {
                SetCapture();
                m_bCaptureSetted = TRUE;

                if (hBkWndHitTest != m_hBkWndHover)
                {
                    // Hover和Pushdown不同的原因是：鼠标一直没动，界面刷新，切换了鼠标所在位置的控件
                    //ATLASSERT(FALSE);
                    m_hBkWndHover = hBkWndHitTest;
                }

                m_hBkWndPushDown = hBkWndHitTest;

                _ModifyWndState(pWndPushDown, BkWndState_PushDown, 0);

                m_nPushItem = nItem;

                pWndPushDown->BkSendMessage(WM_LBUTTONDOWN, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

                if (pWndPushDown->NeedRedrawWhenStateChange())
                    _RedrawItem(nItem);
            }
        }
    }

    void OnLButtonUp(UINT nFlags, CPoint point)
    {
        BOOL bNotifyClick = FALSE;
        UINT uCmdID = 0;
        CBkWindow* pWndClick = NULL;

        if (m_bCaptureSetted)
        {
            ReleaseCapture();
            m_bCaptureSetted = FALSE;
        }

        if (m_hBkWndPushDown)
        {
            pWndClick = BkWnds::GetWindow(m_hBkWndPushDown);

            if (pWndClick)
            {
                _ModifyWndState(pWndClick, 0, BkWndState_PushDown);

                if (m_hBkWndPushDown == m_hBkWndHover && m_nHoverItem == m_nPushItem)
                {
                    if (m_nLastDrawItem != m_nPushItem)
                        _GetItemDisplayInfo(m_nPushItem);

                    pWndClick->BkSendMessage(WM_LBUTTONUP, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

                    LPCTSTR lpszUrl = pWndClick->GetLinkUrl();
                    if (lpszUrl && lpszUrl[0])
                    {
                        HINSTANCE hRet = ::ShellExecute(NULL, L"open", lpszUrl, NULL, NULL, SW_SHOWNORMAL);
                    }
                    else if (pWndClick->GetCmdID())
                    {
                        bNotifyClick = TRUE;
                        uCmdID = pWndClick->GetCmdID();
                    }
                }

                if (bNotifyClick)
                {
                    BKLVMITEMCLICK nms;
                    nms.hdr.code    = BKLVM_ITEMCLICK;
                    nms.hdr.hwndFrom = m_hWnd;
                    nms.hdr.idFrom  = GetDlgCtrlID();
                    nms.nListItemID = m_nPushItem;
                    nms.uCmdID      = uCmdID;

                    LRESULT lRet = ::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
                }

                _RedrawItem(m_nPushItem, pWndClick->IsClass("check") || pWndClick->IsClass("radio"));
                m_nPushItem = -1;
            }

            m_hBkWndPushDown = NULL;
        }

    }

    BOOL OnSetCursor(CWindow /*wnd*/, UINT nHitTest, UINT message)
    {
        if (m_hBkWndHover)
        {
            CBkWindow *pBkWndHover = BkWnds::GetWindow(m_hBkWndHover);

            if (pBkWndHover && !pBkWndHover->IsDisabled(TRUE))
            {
                pBkWndHover->SetCursor();
                return TRUE;
            }
        }

        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));

        return TRUE;
    }

    BOOL OnEraseBkgnd(CDCHandle dc)
    {
        int nTop = GetTopIndex();
        int nBottom = nTop + GetCountPerPage();

        nBottom = min(nBottom, GetItemCount() - 1);

        CRect rcClient, rcItemTop, rcItemBottom;
        COLORREF crBg = RGB(0xFF, 0xFF, 0xFF);

        GetClientRect(rcClient);
        TBase::GetItemRect(nTop, rcItemTop, LVIR_BOUNDS);
        TBase::GetItemRect(nBottom, rcItemBottom, LVIR_BOUNDS);

        dc.FillSolidRect(rcItemBottom.right, rcItemTop.top, rcClient.right - rcItemBottom.right, rcClient.bottom - rcItemTop.top, crBg);
        dc.FillSolidRect(rcClient.left, rcItemBottom.bottom, rcItemBottom.Width(), rcClient.bottom - rcItemBottom.bottom, crBg);

        return TRUE;
    }

    BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
    {
        DefWindowProc();

        ScreenToClient(&pt);

        OnMouseMove(nFlags, pt);

        return FALSE;
    }

    LRESULT OnToolTipEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if(m_wndToolTip.IsWindow())
        {
            MSG msg = { m_hWnd, uMsg, wParam, lParam };

            m_wndToolTip.RelayEvent(&msg);
        }

        SetMsgHandled(FALSE);

        return 0;
    }

    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
    {

    }

protected:

    BEGIN_MSG_MAP_EX(CBkListViewImpl)
        MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnToolTipEvent)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_MOUSELEAVE(OnMouseLeave)
        MSG_WM_MOUSEWHEEL(OnMouseWheel)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_LBUTTONDBLCLK(OnLButtonDown)
        MSG_WM_SIZE(OnSize)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
        MSG_WM_SETCURSOR(OnSetCursor)
        MSG_WM_KEYDOWN(OnKeyDown)
        CHAIN_MSG_MAP_ALT(CCustomDraw<T>, 1)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
};

class CBkListView : public CBkListViewImpl<CBkListView>
{
};

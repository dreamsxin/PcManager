//////////////////////////////////////////////////////////////////////////
//  Class Name: CBkListBox
//     Creator: yykingking
//     Version: 2010.9.10 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <bkwin/bkdlgview.h>
#include <bkwin/listboxbase.h>

//////////////////////////////////////////////////////////////////////////
// Beike List Box Notify

// Get Display Info Notify
#define BKLBM_GET_DISPINFO  1

typedef struct _BKLBMGETDISPINFO
{
	NMHDR       hdr;
	int         nListItemID;
	BOOL        bHover;
	BOOL        bSelect;
	DWORD		nHeight;
} BKLBMGETDISPINFO, *LPBKLBMGETDISPINFO;

// Item Click Notify
#define BKLBM_ITEMCLICK     2

typedef struct _BKLBMITEMCLICK
{
	NMHDR       hdr;
	int         nListItemID;
	UINT        uCmdID;
} BKLBMITEMCLICK, *LPBKLBMITEMCLICK;

#define BKLBM_CALC_ITEM_HEIGHT   3

typedef struct _BKLBITEMMEASUREITEM
{
	NMHDR       hdr;
	int			nListItemId;
	BOOL        bHover;
	BOOL        bSelect;
	DWORD		nHeight;
} BKLBITEMCACLHEIGHT, *LPBKLBITEMMEASUREITEM;

#define BKLBM_CALC_MAX_HEIGHT   4

typedef struct _BKLBITEMCALCMAXITEM
{
	NMHDR       hdr;
	DWORD		nMaxHeight;
} BKLBITEMCALCMAXITEM, *LPBKLBITEMCALCMAXITEM;

// Item Click Notify
#define BKLBM_ITEMDBCLICK     5

typedef struct _BKLBMITEMDBCLICK
{
	NMHDR       hdr;
	int         nListItemID;
	UINT        uCmdID;
} BKLBMITEMDBCLICK, *LPBKLBMITEMDBCLICK;

#define BKRBM_ITEMCLICK			7

typedef struct _BKRBMITEMCLICK : public _BKLBMITEMCLICK
{
} BKRBMITEMCLICK, *LPBKRBMITEMCLICK;

//current mousemove msg
typedef struct _MOUSE_MOVE_MSG_INFO
{
	UINT	uFlag;
	CPoint  point;
	_MOUSE_MOVE_MSG_INFO()
	{
		uFlag = 0;
		point = CPoint(-1,-1);
	}
}MOUSEMOVEMSGINFO, *LPMOUSEMOVEMSGINFO;
//////////////////////////////////////////////////////////////////////////



template <class T>
class CBkListBoxImpl : 
	public CBkListBoxBase
	, public CBkViewImpl<T>
{
	friend CBkViewImpl;

protected:

	typedef CBkListBoxBase		__baseClass;
	typedef CBkListBoxImpl<T>   __thisClass;

	CBkDialog       m_bkListTemplate;
	CToolTipCtrl    m_wndToolTip;
	BOOL            m_bXmlLoaded;
	CBkImage        m_imgMem;
	int             m_nMaxItemHeight;

	// Only one hover control
	HBKWND          m_hBkWndHover;
	// Only one pushdown control
	HBKWND          m_hBkWndPushDown;

	int             m_nHoverItem;
	int             m_nPushItem;
	int             m_nLastDrawItem;
	int				m_nFixHeight;

	// Tracking flag
	BOOL            m_bTrackFlag;

	BOOL            m_bCaptureSetted;
	BOOL			m_bCanGetFocus;

	//current mousemove msg info
	MOUSEMOVEMSGINFO m_MouseMoveMsgInfo;
public:

	CBkListBoxImpl()
		: m_bXmlLoaded(FALSE)
		, m_nMaxItemHeight(0)
		, m_hBkWndHover(NULL)
		, m_hBkWndPushDown(NULL)
		, m_bTrackFlag(FALSE)
		, m_bCaptureSetted(FALSE)
		, m_nHoverItem(-1)
		, m_nPushItem(-1)
		, m_nLastDrawItem(-1)
		, m_bCanGetFocus(TRUE)
		, m_nFixHeight(-1)
	{
	}

	~CBkListBoxImpl()
	{
		if (m_imgMem.m_hBitmap)
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
			//    SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

			ModifyStyle(LBS_MULTIPLESEL, 0);

			//   InsertColumn(0, L"", LVCFMT_LEFT, 1);

			m_bkListTemplate.SetContainer(m_hWnd);

			m_wndToolTip.Create(hWnd);
			m_wndToolTip.SetMaxTipWidth(400);

			CToolInfo ti(0, hWnd);
			m_wndToolTip.AddTool(ti);
			m_wndToolTip.Activate(TRUE);
		}

		return hWnd;
	};
	//reset mouse_move msg
	void ResetMouseMoveMsg()
	{
		if (-1 != m_MouseMoveMsgInfo.point.x &&
			-1 != m_MouseMoveMsgInfo.point.y)
		{
			m_nHoverItem	= -1;
			m_nPushItem		= -1;
			m_nLastDrawItem	= -1;
			m_hBkWndHover	= NULL;

			OnMouseMove(m_MouseMoveMsgInfo.uFlag, m_MouseMoveMsgInfo.point);
		}
	}

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
			_SetMaxItemHeight(nItemHeight);

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

	void ForceRefresh()
	{
		m_nLastDrawItem = -1;
		__baseClass::Referesh(TRUE);
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

	BOOL SetCanGetFocus(BOOL bCan)
	{
		m_bCanGetFocus = bCan;
		return TRUE;
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

	VOID SetItemFixHeight(DWORD nHeight)
	{
		m_nFixHeight = nHeight;
	}
	// 此方法会触发一次重算所有的高度
	void SetItemCount(int nCount)
	{
		__baseClass::SetRedraw2(FALSE);

		m_nLastDrawItem = -1;
		m_nHoverItem	= -1;
		m_nPushItem		= -1;

		__baseClass::ResetContent();
		__baseClass::SetCount(nCount);

		if ( m_nFixHeight == -1 )
		{
			BKLBITEMCALCMAXITEM	calcMax;

			calcMax.hdr.code     = BKLBM_CALC_MAX_HEIGHT;
			calcMax.hdr.hwndFrom = m_hWnd;
			calcMax.hdr.idFrom   = GetDlgCtrlID();
			calcMax.nMaxHeight   = 0;

			::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)calcMax.hdr.idFrom, (WPARAM)&calcMax);
			_SetMaxItemHeight(calcMax.nMaxHeight);
		}
		else
		{
			_SetMaxItemHeight(m_nFixHeight);
		}

		__baseClass::SetRedraw2(TRUE);
		__baseClass::InvalidateRect(NULL,FALSE);
	}

	void DrawItem(LPDRAWITEMSTRUCT lpcd)
	{
		int nItem = (int)lpcd->itemID;

		if ( nItem < 0 )
			return;

		BkWinManager::EnterPaintLock();

		CRect rcItem;
		__baseClass::GetItemRect(nItem, rcItem);

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


		// 如果两次drawitem的大小不一样，那么重新布局一下
		CRect	rcLast;
		m_bkListTemplate.GetRect(&rcLast);
		if ( rcLast.Width() != rcItem.Width() || rcLast.Height() != rcItem.Height() )
		{
			WINDOWPOS WndPos = { 0, 0, 0, 0, rcItem.Width(), rcItem.Height(), SWP_SHOWWINDOW };

			m_bkListTemplate.BkSendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&WndPos);
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

		m_imgMem.Draw2( lpcd->hDC, rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height() );
	}

	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
		if ( m_nFixHeight == -1 )
		{
			BKLBITEMCACLHEIGHT		nms;
			int						nItem = lpMeasureItemStruct->itemID;

			nms.hdr.code     = BKLBM_CALC_ITEM_HEIGHT;
			nms.hdr.hwndFrom = m_hWnd;
			nms.hdr.idFrom   = GetDlgCtrlID();
			nms.nListItemId	 = nItem;
			nms.bHover       = nItem == m_nHoverItem;
			nms.bSelect      = nItem == __baseClass::GetCurSel();
			nms.nHeight		 = 0;

			::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

			lpMeasureItemStruct->itemHeight = nms.nHeight;
		}
		else
		{
			lpMeasureItemStruct->itemHeight = m_nFixHeight;
		}
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

		__baseClass::ReDrawItem(nItem);
	}

	void _SetMaxItemHeight(int nHeight)
	{
		if (nHeight <= m_nMaxItemHeight)
			return;

		m_nMaxItemHeight = nHeight;

		CRect rcClient;

		GetClientRect(rcClient);

		if (rcClient.Width() == 0)
			return;

		if (m_imgMem.M_HOBJECT)
			m_imgMem.DeleteObject();

		m_imgMem.CreateBitmap(rcClient.Width(), m_nMaxItemHeight, RGB(0, 0, 0));
	}

	void _ModifyWndState(CBkWindow *pWnd, DWORD dwStateAdd, DWORD dwStateRemove)
	{
		pWnd->ModifyState(dwStateAdd, dwStateRemove);
	}

	void _GetItemDisplayInfo(int nItem)
	{
		BKLBMGETDISPINFO nms;
		nms.hdr.code    = BKLBM_GET_DISPINFO;
		nms.hdr.hwndFrom = m_hWnd;
		nms.hdr.idFrom  = GetDlgCtrlID();
		nms.bHover      = nItem == m_nHoverItem;
		nms.bSelect     = nItem == __baseClass::GetCurSel();
		nms.nListItemID = nItem;
		nms.nHeight		= 0;
		::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

		m_nLastDrawItem = nItem;

		// 更新xml大小
		CStringA strPos;
		if (m_nFixHeight != -1 )
			strPos.Format("0,0,-0,%d", m_nFixHeight);
		else
			strPos.Format("0,0,-0,%d", nms.nHeight);
		m_bkListTemplate.SetAttribute("pos", strPos, TRUE);
	}

	void _InvalidateControl(CBkWindow *pWnd, BOOL bCheckVisible = TRUE)
	{
	}

	int _HitTest(CPoint& point)
	{
		BOOL uFlags = 0;
		int nItem = __baseClass::ItemFromPoint(point, uFlags);

		HBKWND hBkWndHitTest = NULL;

		if (-1 != nItem)
		{
			CRect rcItem;
			__baseClass::GetItemRect(nItem, rcItem);
			point -= rcItem.TopLeft();
		}

		return nItem;
	}

	void _ResizeColumn()
	{
		CRect rcClient;

		GetClientRect(rcClient);

		if (rcClient.Width() == 0)
			return;

		if (m_imgMem.M_HOBJECT)
			m_imgMem.DeleteObject();

		rcClient.MoveToXY(0, 0);

		m_imgMem.CreateBitmap(rcClient.Width(), m_nMaxItemHeight, RGB(0, 0, 0));

		//	SetColumnWidth(rcClient.Width());

		WINDOWPOS WndPos = { 0, 0, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_SHOWWINDOW };

		m_bkListTemplate.BkSendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&WndPos);
	}

	void OnSize(UINT nType, CSize /*size*/)
	{
		_ResizeColumn();

		__baseClass::Referesh();
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		m_MouseMoveMsgInfo.uFlag = nFlags;
		m_MouseMoveMsgInfo.point = point;

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

	void OnLButtonDbClick(UINT nFlags, CPoint point)
	{
		if (m_bCanGetFocus)
			SetFocus();

		HBKWND hBkWndHitTest = NULL;
		int nItem = _HitTest(point), nSelectItem = __baseClass::GetCurSel();

		if (-1 != nItem)
			hBkWndHitTest = m_bkListTemplate.BkGetHWNDFromPoint(point, TRUE);

		__baseClass::SetCurSel(nItem);

		if (nItem != nSelectItem)
		{
			int nOldSelect = nSelectItem;
			nSelectItem = nItem;
			_RedrawItem(nOldSelect, TRUE);
			_RedrawItem(nItem, TRUE);
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

				pWndPushDown->BkSendMessage(WM_LBUTTONDBLCLK, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

				if (pWndPushDown->NeedRedrawWhenStateChange())
					_RedrawItem(nItem);
			}

			BKLBMITEMDBCLICK nms;
			nms.hdr.code    = BKLBM_ITEMDBCLICK;
			nms.hdr.hwndFrom = m_hWnd;
			nms.hdr.idFrom  = GetDlgCtrlID();
			nms.nListItemID = m_nPushItem;
			nms.uCmdID = pWndPushDown->GetCmdID();

			::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
		}
	}

	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (m_bCanGetFocus)
			SetFocus();

		HBKWND hBkWndHitTest = NULL;
		int nItem = _HitTest(point), nSelectItem = __baseClass::GetCurSel();

		if (-1 != nItem)
			hBkWndHitTest = m_bkListTemplate.BkGetHWNDFromPoint(point, TRUE);

		__baseClass::SetCurSel(nItem);

		if (nItem != nSelectItem)
		{
			int nOldSelect = nSelectItem;
			nSelectItem = nItem;
			_RedrawItem(nOldSelect, TRUE);
			_RedrawItem(nItem, TRUE);
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

	virtual void OnLButtonUp(UINT nFlags, CPoint point)
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
					BKLBMITEMCLICK nms;
					nms.hdr.code    = BKLBM_ITEMCLICK;
					nms.hdr.hwndFrom = m_hWnd;
					nms.hdr.idFrom  = GetDlgCtrlID();
					nms.nListItemID = m_nPushItem;
					nms.uCmdID      = uCmdID;

					LRESULT lRet = ::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
				}

				_RedrawItem(m_nPushItem, pWndClick->IsClass("check") || pWndClick->IsClass("radio"));
				m_nPushItem = -1;
				_ModifyWndState(pWndClick, 0, BkWndState_PushDown);
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

	BEGIN_MSG_MAP_EX(CBkListBoxImpl)
		CHAIN_MSG_MAP(__baseClass)
		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnToolTipEvent)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
		MSG_WM_SIZE(OnSize)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_KEYDOWN(OnKeyDown)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
};

class CBkListBox : public CBkListBoxImpl<CBkListBox>
{
};

class CBkNetMonitorListBox : public CBkListBox
{
public:
	void OnRButtonDown(UINT nFlags, CPoint point)
	{
		if (m_bCanGetFocus)
			SetFocus();

		HBKWND hBkWndHitTest = NULL;
		int nItem = _HitTest(point), nSelectItem = __baseClass::GetCurSel();

		if (-1 != nItem)
			hBkWndHitTest = m_bkListTemplate.BkGetHWNDFromPoint(point, TRUE);

		__baseClass::SetCurSel(nItem);

		if (nItem != nSelectItem)
		{
			int nOldSelect = nSelectItem;
			nSelectItem = nItem;
			_RedrawItem(nOldSelect, TRUE);
			_RedrawItem(nItem, TRUE);
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

				//_ModifyWndState(pWndPushDown, BkWndState_PushDown, 0);

				m_nPushItem = nItem;

				pWndPushDown->BkSendMessage(WM_RBUTTONDOWN, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

				if (pWndPushDown->NeedRedrawWhenStateChange())
					_RedrawItem(nItem);
			}
		}
	}

	virtual void OnRButtonUp(UINT nFlags, CPoint point)
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

				if (m_hBkWndPushDown == m_hBkWndHover && m_nHoverItem == m_nPushItem)
				{
					if (m_nLastDrawItem != m_nPushItem)
						_GetItemDisplayInfo(m_nPushItem);

					pWndClick->BkSendMessage(WM_RBUTTONUP, (WPARAM)nFlags, MAKELPARAM(point.x, point.y));

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

					BKRBMITEMCLICK nms;
					nms.hdr.code    = BKRBM_ITEMCLICK;
					nms.hdr.hwndFrom = m_hWnd;
					nms.hdr.idFrom  = GetDlgCtrlID();
					nms.nListItemID = m_nPushItem;
					nms.uCmdID      = uCmdID;

					LRESULT lRet = ::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
				}

				_RedrawItem(m_nPushItem, pWndClick->IsClass("check") || pWndClick->IsClass("radio"));
				m_nPushItem = -1;
				//_ModifyWndState(pWndClick, 0, BkWndState_PushDown);
			}

			m_hBkWndPushDown = NULL;
		}

	}

	void RefereshVScroll()
	{
			DWORD dwValue = 0;

		//get max
		int nTotalHeight = 0, nPage = 0;

		nTotalHeight = m_nCount * 50;
		//get page
		CRect rc;
		GetClientRect(&rc);
		

		SCROLLINFO sbInfo, sbOldInfo;
		ZeroMemory(&sbInfo, sizeof(sbInfo));
		sbInfo.cbSize = sizeof(sbInfo);
		sbInfo.fMask  = SIF_ALL;
		sbInfo.nMax = nTotalHeight;
		sbInfo.nPage = rc.Height();
		sbInfo.nMin = 0;
		sbInfo.nPos = 0;

		//get old Info
		ZeroMemory(&sbOldInfo, sizeof(sbOldInfo));
		sbOldInfo.cbSize = sizeof(sbOldInfo);

		sbOldInfo.fMask  = SIF_ALL;
		GetScrollInfo(SB_VERT, &sbOldInfo);

		if (0 == sbOldInfo.nMax || 0 == sbOldInfo.nPage)
			return;

		sbInfo.nPos = sbOldInfo.nPos;
		if (0 == ((sbOldInfo.nMax - sbOldInfo.nPos - 50) / sbOldInfo.nPage))
		{
			sbInfo.nPos = sbInfo.nMax - sbInfo.nPage;
		}

		//+ offset
		sbInfo.nPos += sbInfo.nPage/4;
		sbInfo.nMax += sbInfo.nPage/4;

		__super::SetScrollInfo(SB_VERT, &sbInfo);

			
  		SendMessage(WM_VSCROLL, dwValue, NULL);
		//SendMessage(WM_SIZE, 0, 0);
	}

	void RefereshVScrollEx(DWORD dwItemHeight)
	{
		DWORD dwValue = 0;

		//get max
		int nTotalHeight = 0, nPage = 0;

		nTotalHeight = m_nCount * dwItemHeight;
		//get page
		CRect rc;
		GetClientRect(&rc);


		SCROLLINFO sbInfo, sbOldInfo;
		ZeroMemory(&sbInfo, sizeof(sbInfo));
		sbInfo.cbSize = sizeof(sbInfo);
		sbInfo.fMask  = SIF_ALL;
		sbInfo.nMax = nTotalHeight;
		sbInfo.nPage = rc.Height();
		sbInfo.nMin = 0;
		sbInfo.nPos = 0;

		//get old Info
		ZeroMemory(&sbOldInfo, sizeof(sbOldInfo));
		sbOldInfo.cbSize = sizeof(sbOldInfo);

		sbOldInfo.fMask  = SIF_ALL;
		GetScrollInfo(SB_VERT, &sbOldInfo);

		if (0 == sbOldInfo.nMax || 0 == sbOldInfo.nPage)
			return;

		sbInfo.nPos = sbOldInfo.nPos;
		if (0 == ((sbOldInfo.nMax - sbOldInfo.nPos - dwItemHeight) / sbOldInfo.nPage))
		{
			sbInfo.nPos = sbInfo.nMax - sbInfo.nPage;
		}

		//+ offset
		sbInfo.nPos += sbInfo.nPage/4;
		sbInfo.nMax += sbInfo.nPage/4;

		__super::SetScrollInfo(SB_VERT, &sbInfo);


		SendMessage(WM_VSCROLL, dwValue, NULL);
		//SendMessage(WM_SIZE, 0, 0);
	}


	void SetItemCount(int nCount)
	{
		__super::SetItemCount(nCount);

		m_nCount = nCount;
	}

	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
	{
		m_nSBCode = nSBCode;

		SetMsgHandled(FALSE);
	}

protected:

	BEGIN_MSG_MAP_EX(CBkNetMonitorListBox)
		CHAIN_MSG_MAP(CBkListBox)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_RBUTTONUP(OnRButtonUp)
		MSG_WM_VSCROLL(OnVScroll)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
private:

	int		m_nCount;
	UINT	m_nSBCode;
};
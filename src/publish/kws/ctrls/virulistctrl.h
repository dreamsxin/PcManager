/* -------------------------------------------------------------------------
//	FileName	：	CVirusListCtrl.h
//	Creator		：	zhoufeng
//	Date		：	2010/5/21 10:57:11
//	Brief		：	一个自绘的listctrl,提供给网盾日志管理器中使用。
					父窗口需要添加 REFLECT_NOTIFICATIONS();
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __CVirusListCtrl_H__
#define __CVirusListCtrl_H__

#include <vector>
#include <wtlhelper/whwindow.h>
// -------------------------------------------------------------------------
/////////////////////////////////////
#define WM_ITEM_BUTTON_CLICK		WM_USER+123		//当子项中存在按钮，被点击时会向父窗口发送此消息。WPARAM表示行，LPARAM表示列。




///////////////////////////////////////////////
class CVirusListCtrl;
typedef CSortListViewCtrlImpl<CVirusListCtrl> CVirusListCtrlBase;
//////////////////////////////////////////////

class CWHHeaderCtrlEx : public CWHHeaderCtrl{
public:
	CWHHeaderCtrlEx()
	{

	};
	~CWHHeaderCtrlEx()
	{

	}

	BEGIN_MSG_MAP(CWHHeaderCtrlEx)
		
		MESSAGE_HANDLER( WM_MOUSEMOVE, OnMouseMove )
		MESSAGE_HANDLER( WM_NCHITTEST, OnNcHitTest )
		MESSAGE_HANDLER( WM_SETCURSOR, OnSetCursor )
		CHAIN_MSG_MAP(CWHHeaderCtrl)
	END_MSG_MAP()

	LRESULT OnMouseMove(UINT , WPARAM , LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		return TRUE;
	}

	LRESULT OnNcHitTest( UINT , WPARAM , LPARAM lParam, BOOL& bHandled )
	{
		bHandled = TRUE;
		return HTCLIENT;
	}

	LRESULT OnSetCursor( UINT , WPARAM , LPARAM lParam, BOOL& bHandled )
	{
		//SetCursor( LoadCursor( NULL, IDC_HAND) );
		bHandled = TRUE;
		return TRUE;
	}

};


class CVirusListCtrl
	: public CVirusListCtrlBase
	, public COwnerDraw<CVirusListCtrl>
{

public:

	DECLARE_WND_CLASS(L"CVirusListCtrl")

	CVirusListCtrl()
		:m_nRowHeight(-1)
		,m_nDwonBtn(-1)
		,m_nBtnWidth(0)
		,m_nBtnHeight(0),
		m_pData(NULL)
	{
		Init();
	};

	~CVirusListCtrl()
	{
		Uninit();
	};

	BEGIN_MSG_MAP(CVirusListCtrl)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKA, OnLVNHeaderItemClick)
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKW, OnLVNHeaderItemClick)
		//MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		//MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CVirusListCtrl>,1)
		CHAIN_MSG_MAP(CVirusListCtrlBase)
	END_MSG_MAP()

	static DWORD GetWndStyle(DWORD dwStyle)
	{
		return dwStyle;
	}
	static DWORD GetWndExStyle(DWORD dwExStyle)
	{
		return dwExStyle;
	}

/////////////////////////////////
//list的头部控制

	//在需要控制头部外观时，先调用这个函数。
	void HeaderSubclassWindow()
	{
		if(m_wndHeader.m_hWnd != NULL)
		{
			m_wndHeader.DestroyWindow();
			m_wndHeader.m_hWnd = NULL;
		}
		CHeaderCtrl wndHeader = GetHeader();
		if(wndHeader.m_hWnd != NULL)
		{
			m_wndHeader.SubclassWindow(wndHeader.m_hWnd);
		}
	}
	
/////////////////////////////////

	LRESULT OnLVNHeaderItemClick( int nId, LPNMHDR pnmh, BOOL& bHandle)
	{
		if (LVS_OWNERDATA == (LVS_OWNERDATA & GetStyle()))
		{
			LPNMHEADER p = (LPNMHEADER)pnmh;
			if (p->iButton == 0)
			{
				WHLVNITEMSORT nms;
				nms.hdr.code = WHLVN_ITEMSORT;
				nms.hdr.hwndFrom = m_hWnd;
				nms.hdr.idFrom = GetDlgCtrlID();
				nms.bDescending = FALSE;
				nms.nCol = p->iItem;

				::SendMessage(GetParent(), WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);
			}
		}

		SetMsgHandled(FALSE);

		return 0;
	}

	LRESULT OnDrawItem(UINT , WPARAM , LPARAM lParam, BOOL& bHandled)
	{
		LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
		return 0;
	};

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItem)
	{
		CDCHandle dc(lpDrawItem->hDC);
		CRect rc(lpDrawItem->rcItem);
		int nSelected = GetSelectedIndex();
		int nColumnCount = GetColumnCount();
		int nItemID = lpDrawItem->itemID;
		COLORREF dwTextColor = 0;

		if( !m_pData )
			return;
		if( nItemID >= m_pData->size())
			return;

		// 绘制背景
		if ((UINT)nSelected == lpDrawItem->itemID)
			dc.FillSolidRect(rc, RGB(0xB9,0xDB,0xFF));
		else
			dc.FillSolidRect(rc, RGB(0xFF, 0xFF, 0xFF));
		//每行底部的分隔线
		CRect rcDivRow = rc;
		rcDivRow.top = rcDivRow.bottom - 1;
		dc.FillSolidRect(rcDivRow, RGB( 0xEA, 0xEA, 0xEA));

		CRect rect;
		int nState = 0;
		int nWidth = 0;
		int nLeft = 0;
		CString str;
		HFONT hOldFont = NULL;
		int nFmt = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
		KLogic logic;

		S_FIX_ITEM_EX& item = (*m_pData)[nItemID];
		
		for(int i = 0 ; i < nColumnCount; i++)
		{
			rect = rc;

			nWidth = GetColumnWidth( i );
			rect.left = nLeft;
			rect.right = rect.left + nWidth;
			nLeft = rect.right;
			CSize size;
			CString strAction;
			CString strType;
			CString strLevel;

			switch( i )
			{
			case 0:
				{
					nFmt |= DT_LEFT;
					CString strItemName = item.strItemName.c_str();
					dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
					hOldFont = dc.SelectFont( m_fontBold );
					logic.GetTypeStringEx(item.emType, item.emLevel, strItemName,strType);
					str.Format( TEXT(" [%s]"), strType );
					dc.DrawText( str, -1, &rect, nFmt );

					dc.GetTextExtent( str, -1, &size );
					rect.left += size.cx;
					dc.SelectFont( m_font );

					CString strSrcItemName = item.strItemName.c_str();
					CString strFile = item.strFile.c_str();
					CString strSubKey = item.strSubKey.c_str();
					logic.GetItemNameString( item.emType, strSrcItemName, strFile, strSubKey, strItemName );
					dc.DrawText( strItemName, -1, rect, nFmt );
				}
				break;

			case 1:
				hOldFont = dc.SelectFont( m_font );
				nFmt |= DT_CENTER;
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				logic.GetEnginString( item.emType, item.scanEngineType, strLevel );
				dc.DrawText( strLevel, -1, rect,nFmt );
				break;

			case 2:
				hOldFont = dc.SelectFont( m_fontBold );
				
				nFmt |= DT_CENTER;
				dwTextColor = logic.GetLevelString( item.emLevel, strLevel );
				dwTextColor = dc.SetTextColor( dwTextColor );
				dc.DrawText( strLevel, -1, rect,nFmt );
				break;

			case 3:
				hOldFont = dc.SelectFont( m_font );
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				
				logic.GetActionString( item.Advice , strAction );
				dc.DrawText( strAction, -1, rect, nFmt );
				break;
			}

			if( hOldFont )
				dc.SelectFont( hOldFont );

			dc.SetTextColor( dwTextColor );	
		}	
	}


	void SetDataPrt( std::vector<S_FIX_ITEM_EX>* pData )
	{
		m_pData = pData;
	}

	void MeasureItem(LPMEASUREITEMSTRUCT pMeasure)
	{
		if(m_nRowHeight > 0)
		{
			pMeasure->itemHeight = m_nRowHeight;
		}
		else
		{
			COwnerDraw<CVirusListCtrl>::MeasureItem(pMeasure);
		}
	}

	int SetRowHeight(int nHeight)
	{
		int nOldHeight = m_nRowHeight;
		m_nRowHeight = nHeight;
		return nOldHeight;
	}

	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, 
		int nWidth = -1, int nSubItem = -1, int iImage = -1, int iOrder = -1)
	{
		int nRetCode = CVirusListCtrlBase::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,
			nSubItem,iImage,iOrder);
		return nRetCode;
	}

	int InsertColumn(int nCol, const LVCOLUMN* pColumn)
	{
		int nRetCode = CVirusListCtrlBase::InsertColumn(nCol,pColumn);

		return nRetCode;
	}

	BOOL DeleteItem(LPDELETEITEMSTRUCT lpItem)
	{
		return TRUE;
	}

	BOOL DeleteAllItems()
	{
		return CVirusListCtrlBase::DeleteAllItems();
	}
	
	HFONT GetFont(BOOL bBold)
	{
		CFont destFont;

		CFontHandle srcFont(m_font);
		LOGFONT lf;
		srcFont.GetLogFont(&lf);
		lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

		destFont.CreateFontIndirect(&lf);
		return (HFONT)destFont.Detach();
	}

	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		int nItem,nSubItem;
		RECT rc;
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		GetWindowRect(&rc);
		if (IsMouseInButton(pt, nItem,nSubItem))
		{
			SetCursor(LoadCursor(NULL, IDC_HAND));
			//InvalidateRect(&rc);
		}
		bHandled = TRUE;
		return 0;
	}

	LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		int nItem,nSubItem;
		RECT rc;
		GetWindowRect(&rc);
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (IsMouseInButton(pt, nItem,nSubItem))
		{
			::PostMessage(GetParent(), WM_ITEM_BUTTON_CLICK, (WPARAM)nItem,(LPARAM)nSubItem);
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnLButtonUp(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		int nItem,nSubItem;
		RECT rc;
		GetWindowRect(&rc);
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (IsMouseInButton(pt, nItem,nSubItem))
		{
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
		bHandled = FALSE;
		return 0;
	}

	BOOL IsMouseInButton(CPoint pt, int& nItem ,int & nSubItem)
	{
		return FALSE;
	}

protected:
	void Init()
	{
		LOGFONT lf;
		CFontHandle font = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		font.GetLogFont(&lf);
		_tcscpy( lf.lfFaceName, TEXT("宋体") );
		lf.lfWeight = FW_NORMAL;
		lf.lfHeight = -12;

		m_font.CreateFontIndirect(&lf);
		m_nBtnWidth = 24;
		m_nBtnHeight = 12;

		lf.lfUnderline = TRUE;
		m_fontLink.CreateFontIndirect( &lf );

		lf.lfUnderline = FALSE;
		lf.lfWeight = 700;
		m_fontBold.CreateFontIndirect( &lf );
	}
	void Uninit()
	{
		
	}

protected:
	CFont			m_font;
	CFont			m_fontLink;
	CFont			m_fontBold;
	int				m_nDwonBtn;
	int				m_nBtnWidth;
	int				m_nBtnHeight;
	int				m_nRowHeight;
	CWHHeaderCtrl	m_wndHeader;
	std::vector<S_FIX_ITEM_EX>* m_pData;
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __CREPARILISTCTRL_H__ */

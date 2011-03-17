/* -------------------------------------------------------------------------
//	FileName	：	CWhiteListCtrl.h
//	Creator		：	zhoufeng
//	Date		：	2010/5/21 10:57:11
//	Brief		：	一个自绘的listctrl,提供给网盾日志管理器中使用。
					父窗口需要添加 REFLECT_NOTIFICATIONS();
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __CWhiteListCtrl_H__
#define __CWhiteListCtrl_H__

#include <vector>
#include <wtlhelper/whwindow.h>
// -------------------------------------------------------------------------
/////////////////////////////////////
#define WM_ITEM_BUTTON_CLICK		WM_USER+123		//当子项中存在按钮，被点击时会向父窗口发送此消息。WPARAM表示行，LPARAM表示列。

enum EM_TRUST_TYPE
{
	em_trust_url	= 1,
	em_trust_file
};

class CWhiteListItemEx : public CWhiteListItem
{
public:
	int m_nItemType;
};


///////////////////////////////////////////////
class CWhiteListCtrl;
typedef CSortListViewCtrlImpl<CWhiteListCtrl> CWhiteListCtrlBase;
//////////////////////////////////////////////

//每个子项的格式化串
#define FORMAT_ALIGN_LEFT		0x00		//文字左对齐
#define FORMAT_CENTER			0x01		//文字居中
#define FORMAT_BUTTON			0x02		//子项是一个按钮项
#define FORMAT_LINK				0x04		//子项是一个连接
#define FORMAT_RED				0x08		//红色字体
#define FORMAT_BOLD				0x10		//粗字体
#define FORMAT_LEFT				0x20


class CWhiteListCtrl
	: public CWhiteListCtrlBase
	, public COwnerDraw<CWhiteListCtrl>
{

public:

	DECLARE_WND_CLASS(L"CWhiteListCtrl")

	CWhiteListCtrl()
		:m_nRowHeight(-1)
		,m_nDwonBtn(-1)
		,m_nBtnWidth(0)
		,m_nBtnHeight(0)
		,m_pData(NULL)
		,m_hNotify(NULL)
	{
		Init();
	};

	~CWhiteListCtrl()
	{
		Uninit();
	};

	BEGIN_MSG_MAP(CWhiteListCtrl)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKA, OnLVNHeaderItemClick)
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKW, OnLVNHeaderItemClick)
		//MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		//MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CWhiteListCtrl>,1)
		CHAIN_MSG_MAP(CWhiteListCtrlBase)
	END_MSG_MAP()

	static DWORD GetWndStyle(DWORD dwStyle)
	{
		return dwStyle;
	}
	static DWORD GetWndExStyle(DWORD dwExStyle)
	{
		return dwExStyle;
	}

	void SetNotifyWnd( HWND hWnd )
	{
		m_hNotify = hWnd;
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
		if( nItemID >= m_pData->GetCount())
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
		HFONT hOldFont = NULL;
		int nFmt = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;

		CWhiteListItem& log = (*m_pData)[nItemID];
		
		for(int i = 0 ; i < nColumnCount; i++)
		{
			GetSubItemRect( nItemID, i, LVIR_BOUNDS, &rect );
			CString str;
			rect.right = rect.left + GetColumnWidth(i);

			switch( i )
			{
			case 0:
				hOldFont = dc.SelectFont( m_font );
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				nFmt |= DT_LEFT;
				rect.left += 5;
				break;
			case 1:
				hOldFont = dc.SelectFont( m_fontLink );
				dwTextColor = dc.SetTextColor( RGB(48, 100, 165) );
				nFmt |= DT_CENTER;
				break;
			}
			
			GetItemText( nItemID, i, str );
			dc.DrawText( str, -1, &rect, nFmt );

			if( hOldFont )
				dc.SelectFont( hOldFont );

			dc.SetTextColor( dwTextColor );	
		}	
	}

	void SetDataPrt( CAtlArray<CWhiteListItem>* pData )
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
			COwnerDraw<CWhiteListCtrl>::MeasureItem(pMeasure);
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
		int nRetCode = CWhiteListCtrlBase::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,
			nSubItem,iImage,iOrder);
		return nRetCode;
	}

	int InsertColumn(int nCol, const LVCOLUMN* pColumn)
	{
		int nRetCode = CWhiteListCtrlBase::InsertColumn(nCol,pColumn);

		return nRetCode;
	}

	BOOL DeleteItem(LPDELETEITEMSTRUCT lpItem)
	{
		return TRUE;
	}
	
	BOOL DeleteItem( int nIndex )
	{
		return CWhiteListCtrlBase::DeleteItem( nIndex );
	}

	BOOL DeleteAllItems()
	{
		return CWhiteListCtrlBase::DeleteAllItems();
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
			SetCursor(LoadCursor(NULL, IDC_HAND));

			if( m_hNotify )
				::PostMessage( m_hNotify, WM_ITEM_BUTTON_CLICK, (WPARAM)nItem,(LPARAM)m_hWnd );
		}
		bHandled = FALSE;
		return 0;
	}

	int GetHeight()
	{
		return m_nRowHeight;
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
		BOOL retval = FALSE;
		nItem = -1;
		int nColumnCount = GetColumnCount();

		int nItemT = HitTest(pt, NULL);
		int nSubItemT = 0;
		BOOL bFind = FALSE;

		if( !m_pData )
			return FALSE;

		if( nItemT >= m_pData->GetCount() )
			return FALSE;

		if (nItemT != -1)
		{
			CRect rect;
			CWhiteListItem& log = (*m_pData)[nItemT];

			for( int i = 0 ; i < nColumnCount; i++ )
			{
				GetSubItemRect( nItemT, i, LVIR_BOUNDS, &rect );
				
				if( i == 1 )
				{
					int nWidth = rect.Width();
					if( nWidth > m_nBtnWidth )
					{
						rect.left = rect.left + ( nWidth - m_nBtnWidth ) / 2;
						rect.right = rect.right - ( nWidth - m_nBtnWidth ) / 2;
					}

					int nHeight = rect.Height();
					if( nHeight > m_nBtnHeight )
					{
						rect.top = rect.top + ( nHeight - m_nBtnHeight ) / 2;
						rect.bottom = rect.bottom - ( nHeight - m_nBtnHeight ) / 2;
					}
					
					if( rect.PtInRect( pt ) )
					{
						nSubItemT = i;
						bFind = TRUE;
						break;
					}
				}
			}
		}
		if(bFind)
		{
			nItem = nItemT;
			nSubItem = nSubItemT;
		}

		return bFind;
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
	CFont							m_font;
	CFont							m_fontLink;
	CFont							m_fontBold;
	int								m_nDwonBtn;
	int								m_nBtnWidth;
	int								m_nBtnHeight;
	int								m_nRowHeight;
	CWHHeaderCtrl					m_wndHeader;
	CAtlArray<CWhiteListItem>*		m_pData;
	HWND							m_hNotify;
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __CREPARILISTCTRL_H__ */

/* -------------------------------------------------------------------------
//	FileName	：	CLogListCtrl.h
//	Creator		：	zhoufeng
//	Date		：	2010/5/21 10:57:11
//	Brief		：	一个自绘的listctrl,提供给网盾日志管理器中使用。
					父窗口需要添加 REFLECT_NOTIFICATIONS();
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __CLOGLISTCTRL_H__
#define __CLOGLISTCTRL_H__

#include <vector>
#include <wtlhelper/whwindow.h>
// -------------------------------------------------------------------------
/////////////////////////////////////
#define WM_ITEM_BUTTON_CLICK		WM_USER+123		//当子项中存在按钮，被点击时会向父窗口发送此消息。WPARAM表示行，LPARAM表示列。
#define WM_ITEM_CHECKBOX_CLICK		WM_USER+124		//当子项中存在checkbox，被点击时会向父窗口发送此消息。WPARAM表示行，LPARAM表示列。




///////////////////////////////////////////////
class CLogListCtrl;
typedef CSortListViewCtrlImpl<CLogListCtrl> CLogListCtrlBase;
//////////////////////////////////////////////

//每个子项的格式化串
#define FORMAT_ALIGN_LEFT		0x00		//文字左对齐
#define FORMAT_CENTER			0x01		//文字居中
#define FORMAT_BUTTON			0x02		//子项是一个按钮项
#define FORMAT_LINK				0x04		//子项是一个连接
#define FORMAT_RED				0x08		//红色字体
#define FORMAT_BOLD				0x10		//粗字体
#define FORMAT_LEFT				0x20

struct KWS_SCAN_LOG
{
	UINT32							uSize;					///< 结构体大小

	__time64_t						tmHistoryTime;          ///< 日志录入时间,写入日志时,该项自动生成,注意这里是UTC时间,显示的时候需要转成本地时间
	WCHAR							szItemName[MAX_PATH*2];   ///< 文件路径(超过 MAX_PATH 会被截断)
	S_POINT_PROCESS_TYPE			Advice;					///< 建议操作
	S_POINT_PROCESS_TYPE			ActionType;				///< 具体操作
	EM_SCAN_TYPE					emType;					///< 类型
	DWORD							dwID;					///< 索引
	BOOL							bKwsLog;
	EM_HISTORY_ITEM_STATE			state;
	EM_POINT_LEVEL					emLevel;				///< 危险级别
	WCHAR							szVirusName[MAX_PATH*2];
	WCHAR							szQuaraName[MAX_PATH*2];
};


class CLogListCtrl
	: public CLogListCtrlBase
	, public COwnerDraw<CLogListCtrl>
{

public:

	DECLARE_WND_CLASS(L"CLogListCtrl")

	CLogListCtrl()
		:m_nRowHeight(-1)
		,m_nDwonBtn(-1)
		,m_nBtnWidth(0)
		,m_nBtnHeight(0)
		,m_pData(NULL)
		,m_nLinkIndex(4)

	{
		Init();
	};

	~CLogListCtrl()
	{
		Uninit();
	};

	BEGIN_MSG_MAP(CLogListCtrl)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKA, OnLVNHeaderItemClick)
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKW, OnLVNHeaderItemClick)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CLogListCtrl>,1)
		CHAIN_MSG_MAP(CLogListCtrlBase)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	static DWORD GetWndStyle(DWORD dwStyle)
	{
		return dwStyle;
	}
	static DWORD GetWndExStyle(DWORD dwExStyle)
	{
		return dwExStyle;
	}

	void SetLinkIndex( int nIndex )
	{
		m_nLinkIndex = nIndex;
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

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItem)
	{
		if( lpDrawItem->CtlID == IDC_LST_VIRSCAN_LOG )
		{
			DrawAllLogItem( lpDrawItem );
		}
		else if (  lpDrawItem->CtlID == IDC_FILE_VIRUS_LOG_LIST )
		{
			DrawFileLogItem( lpDrawItem );
		}
	}

	void DrawFileLogItem(LPDRAWITEMSTRUCT lpDrawItem)
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
		KWS_SCAN_LOG& log = (*m_pData)[nItemID];

		for(int i = 0 ; i < nColumnCount; i++)
		{
			GetSubItemRect( nItemID, i, LVIR_BOUNDS, &rect );
			rect.right = rect.left + GetColumnWidth( i );
			CString str;
			nFmt = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
			switch( i )
			{
			case 0:

				if (LVS_EX_CHECKBOXES == (LVS_EX_CHECKBOXES & GetExtendedListViewStyle()))
				{
					UINT uCheck = IsItemCheckDisabled( nItemID ) ? 0 : 
						(GetCheckState( nItemID ) ? 2 : 1);
					SIZE size;
					CRect rcCheck = rect;
					m_imglstCheck.GetIconSize(size);
					rcCheck.top += ( rect.Height() - size.cy) / 2;
					rcCheck.left += 2;
					m_imglstCheck.Draw(dc, uCheck, rcCheck.TopLeft(), ILD_TRANSPARENT );
					rect.left += size.cx + 4;
				}

				hOldFont = dc.SelectFont( m_font );
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				nFmt |= DT_LEFT;
				break;
			case 1:
				hOldFont = dc.SelectFont( m_font );
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				nFmt |= DT_LEFT;
				break;
			case 2:
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				hOldFont = dc.SelectFont( m_font );
				nFmt |= DT_LEFT;
			case 3:
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				hOldFont = dc.SelectFont( m_font );
				nFmt |= DT_CENTER;
				break;
			case 4:

				if( log.state == enum_HISTORY_ITEM_STATE_FIXED )
				{
					dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0xFF) );
					hOldFont = dc.SelectFont( m_fontLink );
				}
				else
				{
					dwTextColor = dc.SetTextColor( RGB(0x99, 0x99, 0x99) );
					hOldFont = dc.SelectFont( m_font );
				}

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

	void DrawAllLogItem(LPDRAWITEMSTRUCT lpDrawItem)
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
		KWS_SCAN_LOG& log = (*m_pData)[nItemID];

		for(int i = 0 ; i < nColumnCount; i++)
		{
			GetSubItemRect( nItemID, i, LVIR_BOUNDS, &rect );
			rect.right = rect.left + GetColumnWidth( i );
			CString str;
			nFmt = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
			switch( i )
			{
			case 0:

				if (LVS_EX_CHECKBOXES == (LVS_EX_CHECKBOXES & GetExtendedListViewStyle()))
				{
					UINT uCheck = IsItemCheckDisabled( nItemID ) ? 0 : 
								(GetCheckState( nItemID ) ? 2 : 1);
					SIZE size;
					CRect rcCheck = rect;
					m_imglstCheck.GetIconSize(size);
					rcCheck.top += ( rect.Height() - size.cy) / 2;
					rcCheck.left += 2;
					m_imglstCheck.Draw(dc, uCheck, rcCheck.TopLeft(), ILD_TRANSPARENT );
					rect.left += size.cx + 4;
				}

				hOldFont = dc.SelectFont( m_font );
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				nFmt |= DT_LEFT;
				break;
			case 1:
				hOldFont = dc.SelectFont( m_font );
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				nFmt |= DT_LEFT;
				break;
			case 2:
			case 4:
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				hOldFont = dc.SelectFont( m_font );
				nFmt |= DT_CENTER;
				break;
			case 3:
				{
					KLogic logic;
					CString strItem;
					dwTextColor = dc.SetTextColor( logic.GetLevelString( log.emLevel, strItem ) );
					hOldFont = dc.SelectFont( m_fontBold );
					nFmt |= DT_CENTER;
				}
				break;
			case 5:

				if( log.state == enum_HISTORY_ITEM_STATE_FIXED )
				{
					dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0xFF) );
					hOldFont = dc.SelectFont( m_fontLink );
				}
				else
				{
					dwTextColor = dc.SetTextColor( RGB(0x99, 0x99, 0x99) );
					hOldFont = dc.SelectFont( m_font );
				}

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

	BOOL GetCheckState(int nItem)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		if (LVS_OWNERDATA == (LVS_OWNERDATA & GetStyle()))
		{
			if (nItem >= GetItemCount() || nItem < 0)
				return FALSE;

			if (nItem >= (int)m_arrItemCheck.GetCount())
				return FALSE;

			return (2 == m_arrItemCheck[nItem]);
		}
		else
			return __super::GetCheckState(nItem);
	}


	BOOL SetCheckState(int nItem, BOOL bCheck, BOOL bForce = FALSE, BOOL bRedraw = TRUE)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		if (LVS_OWNERDATA == (LVS_OWNERDATA & GetStyle()))
		{
			if (nItem >= GetItemCount() || nItem < 0)
				return FALSE;

			if (nItem >= (int)m_arrItemCheck.GetCount())
				m_arrItemCheck.SetCount(GetItemCount());

			if (bForce || 0 != m_arrItemCheck[nItem])
			{
				m_arrItemCheck[nItem] = bCheck ? 2 : 1;

				if (bRedraw)
					RedrawItems(nItem, nItem);
			}

			return TRUE;
		}
		else
			return __super::SetCheckState(nItem, bCheck);
	}

	BOOL DisableItemCheck(int nItem, BOOL bRedraw = TRUE)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		if (LVS_OWNERDATA == (LVS_OWNERDATA & GetStyle()))
		{
			if (nItem >= GetItemCount() || nItem < 0)
				return FALSE;

			if (nItem >= (int)m_arrItemCheck.GetCount())
				m_arrItemCheck.SetCount(GetItemCount());

			m_arrItemCheck[nItem] = 0;

			if (bRedraw)
				RedrawItems(nItem, nItem);

			return TRUE;
		}
		else
			return __super::SetItemState(nItem, INDEXTOSTATEIMAGEMASK(0), LVIS_STATEIMAGEMASK);
	}

	BOOL SetItemCount(int nItems)
	{
		m_arrItemCheck.SetCount( nItems );
		for ( int i = 0; i < m_arrItemCheck.GetCount();i++ )
		{
			m_arrItemCheck[i] = 1;
		}
		return __super::SetItemCount( nItems );
	}


	BOOL IsItemCheckDisabled(int nItem)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		if (LVS_OWNERDATA == (LVS_OWNERDATA & GetStyle()))
		{
			if (nItem >= GetItemCount())
				return FALSE;

			if (nItem >= (int)m_arrItemCheck.GetCount())
				m_arrItemCheck.SetCount(GetItemCount());

			return 0 == m_arrItemCheck[nItem];
		}
		else
			return 0 == (__super::GetItemState(nItem, LVIS_STATEIMAGEMASK) >> 12);
	}

	BOOL IsAllItemChecked()
	{
		ATLASSERT(::IsWindow(m_hWnd));

		int nCount = GetItemCount();
		BOOL bAllCheck = TRUE;

		for (int i = 0; i < nCount; i ++)
		{
			if (!IsItemCheckDisabled(i))
				bAllCheck &= GetCheckState(i);
		}

		return bAllCheck;
	}

	BOOL IsAnyItemChecked()
	{
		ATLASSERT(::IsWindow(m_hWnd));

		int nCount = GetItemCount();

		for (int i = 0; i < nCount; i ++)
		{
			if (!IsItemCheckDisabled(i))
			{
				if (GetCheckState(i)) 
					return TRUE;
			}
		}

		return FALSE;
	}

	BOOL CheckAllItems(BOOL bCheck, BOOL bForce = FALSE, BOOL bRedraw = TRUE)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		int nCount = GetItemCount();

		for (int i = 0; i < nCount; i ++)
			SetCheckState(i, bCheck, bForce, bRedraw);

		return TRUE;
	}

	void SetDataPrt( CAtlArray<KWS_SCAN_LOG>* pData )
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
			COwnerDraw<CLogListCtrl>::MeasureItem(pMeasure);
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
		int nRetCode = CLogListCtrlBase::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,
			nSubItem,iImage,iOrder);
		return nRetCode;
	}

	int InsertColumn(int nCol, const LVCOLUMN* pColumn)
	{
		int nRetCode = CLogListCtrlBase::InsertColumn(nCol,pColumn);

		return nRetCode;
	}

	BOOL DeleteItem(LPDELETEITEMSTRUCT lpItem)
	{
		return TRUE;
	}

	BOOL DeleteItem( int i )
	{
		if( i < GetItemCount() )
			CLogListCtrlBase::DeleteItem( i );

		if( i < m_arrItemCheck.GetCount() )
			m_arrItemCheck.RemoveAt(i);

		return TRUE;
	}

	BOOL DeleteAllItems()
	{
		return CLogListCtrlBase::DeleteAllItems();
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

	LRESULT OnSetCursor(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		UINT uCode = LOWORD(lParam);
		UINT uMouseMsg = HIWORD(lParam);
		CPoint point;
		GetCursorPos( &point );
		ScreenToClient( &point );
		int nItem,nSubItem;

		if( uMouseMsg == 0 || uCode != HTCLIENT )
		{
			bHandled = FALSE;
			return FALSE;
		}

		if( uMouseMsg >= WM_MOUSEFIRST && uMouseMsg <= WM_MOUSELAST )
		{
			if (IsMouseInButton( point, nItem,nSubItem ))
			{
				SetCursor( LoadCursor( NULL, IDC_HAND) );
			}
			else if ( IsInCheckBox( point, nItem, nSubItem ) )
			{	
				SetCursor( LoadCursor( NULL, IDC_HAND) );
			}
			else
			{
				bHandled = FALSE;
			}
		}
		else
		{
			bHandled = FALSE;
		}
		return TRUE;
	}

	LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		int nItem,nSubItem;
		RECT rc;
		GetWindowRect(&rc);
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (IsMouseInButton(pt, nItem,nSubItem))
		{
			::SendMessage( GetParent().GetParent(), WM_ITEM_BUTTON_CLICK, (WPARAM)nItem,(LPARAM)nSubItem);
			SetCursor(LoadCursor(NULL, IDC_HAND));
		}
		else if ( LVS_EX_CHECKBOXES == (LVS_EX_CHECKBOXES & GetExtendedListViewStyle()) )
		{
			if( IsInCheckBox( pt, nItem, nSubItem ) )
			{
				BOOL bCheck = !GetCheckState(nItem);
				SetCheckState(nItem, bCheck);
				::SendMessage( GetParent().GetParent(), WM_ITEM_CHECKBOX_CLICK, (WPARAM)nItem, (LPARAM)nSubItem );
			}
		}

		bHandled = FALSE;
		return 0;
	}

	BOOL IsInCheckBox( CPoint pt, int& nItem ,int & nSubItem )
	{
		BOOL bRet = FALSE;
		nItem = HitTest(pt, NULL);
		nSubItem = 0;

		CRect rect;
		GetSubItemRect( nItem, nSubItem, LVIR_BOUNDS, &rect );
		SIZE size;
		CRect rcCheck = rect;
		
		m_imglstCheck.GetIconSize(size);
		rcCheck.top += ( rect.Height() - size.cy) / 2;
		rcCheck.left += 2;
		rcCheck.bottom = rcCheck.top + size.cy;
		rcCheck.right = rcCheck.left + size.cx;

		return rcCheck.PtInRect( pt );
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
			KWS_SCAN_LOG& log = (*m_pData)[nItemT];

			for( int i = 0 ; i < nColumnCount; i++ )
			{
				GetSubItemRect( nItemT, i, LVIR_BOUNDS, &rect );
				rect.right = rect.left + GetColumnWidth( i );
				
				if( i == m_nLinkIndex && log.state == enum_HISTORY_ITEM_STATE_FIXED )
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

	void ShowCheckBox(BOOL bShow)
	{
		if (bShow)
		{
			SetExtendedListViewStyle(LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES);
			__super::SetImageList(m_imglstCheck, LVSIL_STATE);

		}
		else
		{
			__super::SetImageList(NULL, LVSIL_STATE);
			SetExtendedListViewStyle(0, LVS_EX_CHECKBOXES);
		}
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

		m_imglstCheck.Create();
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
	CAtlArray<KWS_SCAN_LOG>*	m_pData;
	CWH3StateCheckImageList		m_imglstCheck;
	CAtlArray<int>				m_arrItemCheck;
	int							m_nLinkIndex;
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __CREPARILISTCTRL_H__ */

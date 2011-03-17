/* -------------------------------------------------------------------------
//	FileName	：	CBlockLogListCtrl.h
//	Creator		：	zhoufeng
//	Date		：	2010/5/21 10:57:11
//	Brief		：	一个自绘的listctrl,提供给网盾日志管理器中使用。
					父窗口需要添加 REFLECT_NOTIFICATIONS();
//
//	$Id: $
// -----------------------------------------------------------------------*/
#ifndef __BLOCK_LOGLISTCTRL_H__
#define __BLOCK_LOGLISTCTRL_H__

#include <vector>

// -------------------------------------------------------------------------
/////////////////////////////////////
#define WM_ITEM_BUTTON_CLICK		WM_USER+123		//当子项中存在按钮，被点击时会向父窗口发送此消息。WPARAM表示行，LPARAM表示列。




///////////////////////////////////////////////
class CBlockLogListCtrl;
typedef CSortListViewCtrlImpl<CBlockLogListCtrl> CBlockLogListCtrlBase;
//////////////////////////////////////////////

//每个子项的格式化串
#define FORMAT_ALIGN_LEFT		0x00		//文字左对齐
#define FORMAT_ALIGN_CENTER		0x01		//文字居中
#define FORMAT_BUTTON			0x02		//子项是一个按钮项

//每个子项的数据
typedef struct tagSubItemData
{
	CString		strText;
	COLORREF	crText;
	BOOL		bTextBold;
	int			nFormat;
	tagSubItemData()
	{
		Reset();
	}
	void Reset()
	{
		strText = L"";
		crText = RGB(0,0,0);
		nFormat = FORMAT_ALIGN_LEFT;
		bTextBold = FALSE;
	}
}SubItemData;

//每一行的数据
typedef struct tagItemData
{
	std::vector<SubItemData> subItems;
}ItemData;

//一个list的数据
typedef std::vector<ItemData> ListItemData;

class CBlockLogListCtrl : public CBlockLogListCtrlBase, 
	                      public COwnerDraw<CBlockLogListCtrl>
{
public:

	DECLARE_WND_CLASS(L"CBlockLogListCtrl")

	CBlockLogListCtrl()
		:m_nRowHeight(-1)
		,m_nDwonBtn(-1)
		,m_nBtnWidth(0)
		,m_nBtnHeight(0)
	{
		Init();
	};

	~CBlockLogListCtrl()
	{
		Uninit();
	};

	BEGIN_MSG_MAP(CBlockLogListCtrl)
		MESSAGE_HANDLER(WM_MOUSEMOVE,   OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKA, OnHeaderItemClick)
		NOTIFY_CODE_HANDLER(HDN_ITEMCLICKW, OnHeaderItemClick)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CBlockLogListCtrl>,1)
		CHAIN_MSG_MAP(CBlockLogListCtrlBase)
	END_MSG_MAP()

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
		HICON hIcon = NULL;
		COLORREF dwTextColor = RGB(0, 0, 0);

		//CMemoryDC memdc(dc,rc);

		// 绘制背景
		if ((UINT)nSelected == lpDrawItem->itemID)
		{
			dc.FillSolidRect(rc, RGB( 185, 219, 255));
		}
		else
		{
			dc.FillSolidRect(rc, RGB(0xFF, 0xFF, 0xFF));
		}
		//每行底部的分隔线
		CRect rcDivRow = rc;
		rcDivRow.top = rcDivRow.bottom - 1;
		dc.FillSolidRect(rcDivRow, RGB(246, 246, 246));

		ItemData * pItemData = (ItemData *)GetItemData(nItemID);

		if (pItemData == NULL)
			return;

		int nLeft = rc.left;
		int nWidth = 0;

		for (int i=0; i<pItemData->subItems.size(); i++)
		{
			nWidth = GetColumnWidth(i);
			RECT rcSubItem = { nLeft, rc.top, nLeft+nWidth, rc.bottom };

			if ((pItemData->subItems[i].nFormat & FORMAT_BUTTON) !=0) //子项是一个按钮
			{
				//CDC memdc;
				//memdc.CreateCompatibleDC(dc);
				//HBITMAP hOldBmp = (HBITMAP)::SelectObject(memdc, m_bmpBtnNormal);

				CRect rect;
				rect.left = nLeft + (nWidth - m_nBtnWidth)/2;
				rect.right = rect.left + m_nBtnWidth;
				rect.top = rc.top + (rc.Height() - m_nBtnHeight)/2;
				rect.bottom = rect.top + m_nBtnHeight;

				_draw_bmp(dc,rect,m_bmpBtnNormal);

				/*dc.BitBlt(pItemData->subItems[i].rtExt.left,
					pItemData->subItems[i].rtExt.top,
					m_nBtnWidth,
					m_nBtnHeight,
					memdc,
					0,0,SRCCOPY);*/
				//::SelectObject(memdc,hOldBmp);
			}
			//文字
			UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

			if ((pItemData->subItems[i].nFormat & FORMAT_ALIGN_CENTER) == 1)
			{
				nFormat |= DT_CENTER;
			}
			else
			{
				nFormat |= DT_LEFT;
			}

			CString strText = pItemData->subItems[i].strText;

			if (strText && strText.GetLength() > 0)
			{
				COLORREF crOldText = dc.SetTextColor(pItemData->subItems[i].crText);
				HFONT hOldFont = dc.SelectFont(GetFont(pItemData->subItems[i].bTextBold));

				dc.DrawText(strText.GetBuffer(), strText.GetLength(), &rcSubItem, nFormat);
				dc.SetTextColor(crOldText);

				::DeleteObject(dc.SelectFont(hOldFont));
			}

			nLeft += nWidth;
		}
		//dc.BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),memdc,0,0,SRCCOPY);
	}

	void MeasureItem(LPMEASUREITEMSTRUCT pMeasure)
	{
		if(m_nRowHeight > 0)
		{
			pMeasure->itemHeight = m_nRowHeight;
		}
		else
		{
			COwnerDraw<CBlockLogListCtrl>::MeasureItem(pMeasure);
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
		int nRetCode = CBlockLogListCtrlBase::InsertColumn(
			nCol, lpszColumnHeading, nFormat, nWidth, nSubItem, iImage, iOrder);

		return nRetCode;
	}

	int InsertColumn(int nCol, const LVCOLUMN* pColumn)
	{
		int nRetCode = CBlockLogListCtrlBase::InsertColumn(nCol, pColumn);

		return nRetCode;
	}

	int InsertItem(ItemData& pItem)
	{
		m_Data.push_back(pItem);
		CBlockLogListCtrlBase::InsertItem(m_Data.size()-1,L"");
		return m_Data.size();
	}

	//注：基类的GetItemText(...)不可用。因为数据并没有设置到基类中去。
	BOOL SetSubItemText(int nRow,int nSub,CString strText)
	{
		if(m_Data.size() > nRow)
		{
			if(m_Data[nRow].subItems.size() > nSub)
			{
				m_Data[nRow].subItems[nSub].strText = strText;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL GetSubItemText(int nRow,int nSub,CString& strText)
	{
		if(m_Data.size() > nRow)
		{
			if(m_Data[nRow].subItems.size() > nSub)
			{
				strText = m_Data[nRow].subItems[nSub].strText;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL SetSubItemFormat(int nRow,int nSub,int nFormat)
	{
		if(m_Data.size() > nRow)
		{
			if(m_Data[nRow].subItems.size() > nSub)
			{
				m_Data[nRow].subItems[nSub].nFormat = nFormat;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL SetSubItemColor(int nRow,int nSub,COLORREF crText)
	{
		if(m_Data.size() > nRow)
		{
			if(m_Data[nRow].subItems.size() > nSub)
			{
				m_Data[nRow].subItems[nSub].crText = crText;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL SetSubItemTextBold(int nRow,int nSub,BOOL bBold)
	{
		if(m_Data.size() > nRow)
		{
			if(m_Data[nRow].subItems.size() > nSub)
			{
				m_Data[nRow].subItems[nSub].bTextBold = bBold;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL DeleteItem(LPDELETEITEMSTRUCT lpItem)
	{
		return TRUE;
	}

	BOOL DeleteAllItems()
	{
		m_Data.clear();
		return CBlockLogListCtrlBase::DeleteAllItems();
	}

	DWORD_PTR GetItemData(int nRow)
	{
		if(m_Data.size() > nRow)
		{
			return (DWORD_PTR)&m_Data[nRow];
		}

		return NULL;
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

	LRESULT OnHeaderItemClick( int nId, LPNMHDR pnmh, BOOL& bHandle)
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

	LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		int nItem,nSubItem;

		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

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
		BOOL retval = FALSE;
		nItem = -1;
		int nColumnCount = GetColumnCount();


		int nItemT = HitTest(pt, NULL);
		int nSubItemT = 0;
		BOOL bFind = FALSE;
		if (nItemT != -1)
		{
			int nLeft = 0;
			int nWidth = 0;
			SCROLLINFO hInfo;
			hInfo.cbSize = sizeof(SCROLLINFO);
			hInfo.fMask = SIF_ALL;
			GetScrollInfo(SB_HORZ,&hInfo);
			if(hInfo.nPos > 0)
			{
				//这里，一个水平滚动条的位置，正好是一个像素。
				nLeft -= hInfo.nPos;
			}
			for(nSubItemT=0; nSubItemT<m_Data[nItemT].subItems.size(); nSubItemT++)
			{
				nWidth = GetColumnWidth(nSubItemT);
				if(m_Data[nItemT].subItems[nSubItemT].nFormat&FORMAT_BUTTON)
				{
					//只要x轴在范围内就可以了，以后再考虑y轴吧
					int nMin = nWidth>m_nBtnWidth ? nLeft+(nWidth-m_nBtnWidth)/2 : nLeft;
					int nMax = nWidth>m_nBtnWidth ? nMin + m_nBtnWidth : nMin+nWidth;
					if(pt.x>=nMin && pt.x<=nMax)
					{
						bFind = TRUE;
						break;
					}
				}
				nLeft += nWidth;
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
		}
		else
		{
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
		font.DeleteObject();

		m_bmpBtnNormal.LoadBitmap(IDB_KWS_SUBITEM_NORAML);
		
		BITMAP bmpInfo;
		m_bmpBtnNormal.GetBitmap(&bmpInfo);
		m_nBtnWidth = bmpInfo.bmWidth;
		m_nBtnHeight = bmpInfo.bmHeight;
	}
	void Uninit()
	{
		m_Data.clear();
	}

protected:
	ListItemData	m_Data;
	CFont			m_font;
	CBitmap			m_bmpBtnNormal;
	int				m_nDwonBtn;
	int				m_nBtnWidth;
	int				m_nBtnHeight;
	int				m_nRowHeight;
	CWHHeaderCtrl	m_wndHeader;
};

// -------------------------------------------------------------------------
// $Log: $

#endif /* __CREPARILISTCTRL_H__ */

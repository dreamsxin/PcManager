

///////////////////////////////////////////////////////////////////////////////
// CBkListBoxBaseT - 一个简单的基于ownerdraw的可变高度的单选的listbox

#define	LB_REFRESH			(LB_MSGMAX-1)
#define	LB_SETREDRAW		(LB_MSGMAX+2)
#define LB_ENSUREVISIBLE	(LB_MSGMAX+3)

typedef CWinTraits<WS_CLIPCHILDREN | WS_CHILD |WS_VSCROLL | LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS, 0> CBkListBoxBaseTraits;

//CListBox
class CBkListBoxBase :
	  public CWindowImpl<CBkListBoxBase, CWindow, CBkListBoxBaseTraits>
{
public:

	typedef	CWindowImpl<CBkListBoxBase, CWindow, CBkListBoxBaseTraits>		TBase;

	#define		DEFAULT_SEL_VALUE		-1 
	#define		MARGIN_BOTTOM			5
	// Attributes
	static LPCTSTR GetWndClassName()
	{
		return _T("BKSIMPLELIST");
	}

	// Constructors
	CBkListBoxBase() : TBase()
	{
		_Init();
	}

	CBkListBoxBase& operator =(HWND hWnd)
	{
		_Init();

		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
		DWORD dwStyle = 0, DWORD dwExStyle = 0,
		ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		return TBase::Create(hWndParent, rect.m_lpRect, szWindowName, dwStyle, dwExStyle, MenuOrID.m_hMenu, lpCreateParam);
	}

	// for entire listbox
	int GetCount() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETCOUNT, 0, 0L);
	}

	VOID SetBkColor(COLORREF clr)
	{
		m_clrBG = clr;
	}

	int SetCount(int cItems)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_SETCOUNT, cItems, 0L);
	}

	int GetTopIndex() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETTOPINDEX, 0, 0L);
	}

	int SetTopIndex(int nIndex)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_SETTOPINDEX, nIndex, 0L);
	}

	// for single-selection listboxes
	int GetCurSel() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);
		return (int)::SendMessage(m_hWnd, LB_GETCURSEL, 0, 0L);
	}

	int SetCurSel(int nSelect)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT((GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) == 0);
		return (int)::SendMessage(m_hWnd, LB_SETCURSEL, nSelect, 0L);
	}

	// for listbox items
	DWORD_PTR GetItemData(int nIndex) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (DWORD_PTR)::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0L);
	}

	int SetItemData(int nIndex, DWORD_PTR dwItemData)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
	}

	int SetItemDataPtr(int nIndex, void* pData)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return SetItemData(nIndex, (DWORD_PTR)pData);
	}

	int GetItemRect(int nIndex, LPRECT lpRect) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)lpRect);
	}

	int GetItemHeight(int nIndex) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_GETITEMHEIGHT, nIndex, 0L);
	}

	int SetItemHeight(int nIndex, UINT cyItemHeight)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));
	}

	// Settable only attributes
	void SetColumnWidth(int cxWidth)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LB_SETCOLUMNWIDTH, cxWidth, 0L);
	}

	void ResetContent()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LB_RESETCONTENT, 0, 0L);
	}

	UINT ItemFromPoint(POINT pt, BOOL& bOutside) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		DWORD dw = (DWORD)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
		bOutside = (BOOL)HIWORD(dw);
		return (UINT)LOWORD(dw);
	}

	// manipulating listbox items
	int AddString(LPCTSTR lpszItem)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)lpszItem);
	}

	int DeleteString(UINT nIndex)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_DELETESTRING, nIndex, 0L);
	}

	int InsertString(int nIndex, LPCTSTR lpszItem)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem );
		return 0;
	}

	VOID ReDrawItem( int nIndex)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		::SendMessage(m_hWnd, LB_REFRESH, nIndex, 0L);
	}

	VOID Referesh( BOOL bRecalLayout = FALSE)
	{
		ATLASSERT(::IsWindow(m_hWnd));

		::SendMessage(m_hWnd, LB_REFRESH, (WPARAM)bRecalLayout, (LPARAM)TRUE);
		::SetWindowPos( m_hWnd, NULL, 0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	}

	// 此方法会 封闭/打开 计算高度和重绘的方法
	BOOL SetRedraw2(BOOL bRedraw)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LB_SETREDRAW, (BOOL)bRedraw, NULL);
	}

	BOOL EnsureVisible(int nItem, BOOL bPartialOK)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, LB_ENSUREVISIBLE, nItem, MAKELPARAM(bPartialOK, 0));
	}

	void RefreshIntern( BOOL bReCalc = TRUE, BOOL bReDraw = TRUE, BOOL bResetScroll = FALSE )
	{
		_RefreshIntern(bReCalc, bReDraw, bResetScroll );
	}

public:
	
	BEGIN_MSG_MAP_EX(CBkListBoxBase)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SIZE(OnSize)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		MSG_WM_PAINT(OnPaint)
		MESSAGE_HANDLER_EX(LB_DELETESTRING,_OnDeleteString)
		MESSAGE_HANDLER_EX(LB_ADDSTRING,_OnAddString)
		MESSAGE_HANDLER_EX(LB_INSERTSTRING,_OnInsertString)
		MESSAGE_HANDLER_EX(LB_RESETCONTENT,_OnResetContent)
		MESSAGE_HANDLER_EX(LB_GETCURSEL,_OnGetCursel)
		MESSAGE_HANDLER_EX(LB_SETCURSEL,_OnSetCursel)
		MESSAGE_HANDLER_EX(LB_GETCOUNT,_OnGetCount)
		MESSAGE_HANDLER_EX(LB_SETCOUNT,_OnSetCount)
		MESSAGE_HANDLER_EX(LB_GETITEMHEIGHT,_OnGetItemHeight)
		MESSAGE_HANDLER_EX(LB_SETITEMHEIGHT,_OnSetItemHeight)
		MESSAGE_HANDLER_EX(LB_GETITEMRECT,_OnGetItemRect)
		MESSAGE_HANDLER_EX(LB_ITEMFROMPOINT,_OnItemFromPoint)
		MESSAGE_HANDLER_EX(LB_SETCOLUMNWIDTH,_OnSetColumnWidth)
		MESSAGE_HANDLER_EX(LB_SETITEMDATA,_OnSetItemData)
		MESSAGE_HANDLER_EX(LB_GETITEMDATA,_OnGetItemData)
		MESSAGE_HANDLER_EX(LB_REFRESH,_OnRefresh)
		MESSAGE_HANDLER_EX(LB_SETREDRAW,_OnSetRedraw)
		MESSAGE_HANDLER_EX(LB_ENSUREVISIBLE,_OnEnsureVisible)
		MSG_WM_VSCROLL(_OnVScroll)
	END_MSG_MAP()
	

protected:

#define FIX_SCROLL_POS(sbInfo)				if (sbInfo.nPos < sbInfo.nMin)sbInfo.nPos = sbInfo.nMin;\
											else if (sbInfo.nPos > sbInfo.nMax-(int)sbInfo.nPage )\
											sbInfo.nPos = sbInfo.nMax-sbInfo.nPage;

	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		if ( TBase::GetStyle() & WS_VSCROLL )
		{
			SCROLLINFO	sbInfo = {0};

			sbInfo.cbSize = sizeof(SCROLLINFO);
			sbInfo.fMask  = SIF_ALL;

			TBase::GetScrollInfo(SB_VERT,&sbInfo);

			sbInfo.nPos -= zDelta;

			FIX_SCROLL_POS(sbInfo);

			TBase::SetScrollPos(SB_VERT,sbInfo.nPos); 

			m_nScrollOffset = 0-sbInfo.nPos;
			_RefreshIntern();
		}

		return FALSE;
	}
	
	void _OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
	{
		SCROLLINFO	sbInfo = {0};

		sbInfo.cbSize = sizeof(SCROLLINFO);
		sbInfo.fMask  = SIF_ALL;
		
		TBase::GetScrollInfo(SB_VERT,&sbInfo);

		switch(nSBCode) 
		{ 
		case SB_PAGEDOWN:
		case SB_LINEDOWN: 
			{
				sbInfo.nPos += sbInfo.nPage/4;

				if (sbInfo.nPos < sbInfo.nMin)
					sbInfo.nPos = sbInfo.nMin;
				else if (sbInfo.nPos > sbInfo.nMax-(int)sbInfo.nPage )
					sbInfo.nPos =sbInfo.nMax-sbInfo.nPage;

				TBase::SetScrollPos(SB_VERT,sbInfo.nPos); 
				
				m_nScrollOffset = 0-sbInfo.nPos;
				_RefreshIntern();
			}
			break; 
		case SB_PAGEUP:
		case SB_LINEUP: 
			{
				sbInfo.nPos -= sbInfo.nPage/4;

				if (sbInfo.nPos < sbInfo.nMin)
					sbInfo.nPos = sbInfo.nMin;
				else if (sbInfo.nPos >= sbInfo.nMax)
					sbInfo.nPos = sbInfo.nMax;

				TBase::SetScrollPos(SB_VERT,sbInfo.nPos);

				m_nScrollOffset = 0-sbInfo.nPos;
				_RefreshIntern();
			}
			break; 
		case SB_THUMBTRACK:
			{
				sbInfo.nPos = nPos;

				if (sbInfo.nPos < sbInfo.nMin)
					sbInfo.nPos = sbInfo.nMin;
				else if (sbInfo.nPos >= sbInfo.nMax)
					sbInfo.nPos = sbInfo.nMax;

				TBase::SetScrollPos(SB_VERT,sbInfo.nPos);

				m_nScrollOffset = 0-sbInfo.nPos;
				_RefreshIntern();
			}
			break;
		} 
		SetMsgHandled(FALSE);

	}
	LRESULT _OnEnsureVisible(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int		nIndex		= (int)wParam;
		BOOL	bRefresh	= FALSE;
		if ( TBase::GetStyle() & WS_VSCROLL )
		{
			if (nIndex< (int)m_listData.GetCount())
			{
				List_Intern_Data&	data		= m_listData[nIndex];
				LONG				nYOffset	= -m_nScrollOffset;
				CRect				rc;

				TBase::GetWindowRect(&rc);
				if ( data.rcItem.top < 0 )
				{
					m_nScrollOffset = -(data.rcItem.top-m_nScrollOffset);
					bRefresh		= TRUE;
				}
				else if ( data.rcItem.bottom > rc.Height() )
				{
					m_nScrollOffset = rc.Height()-(data.rcItem.bottom-m_nScrollOffset);
					bRefresh		= TRUE;
				}

				if (bRefresh)
				{
					_RefreshIntern();
				}
			}
		}

		return S_OK;
	}


	LRESULT _OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int		nIndex	= (int)wParam;
		BOOL	bAll	= (BOOL)lParam;
		BOOL	bRecal	= (BOOL)wParam;

		if (bAll)
		{
			if (bRecal)
			{
				m_bNeedRecalLayout = TRUE;
				_RecalcLayout();
			}

			m_bNeedRedraw = TRUE;
			_RePaint(NULL);
		}
		else
		{
			_RePaintItem(NULL,nIndex);
		}

		return S_OK;
	}

	LRESULT _OnGetItemData(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int nIndex = (int)wParam;

		if (nIndex< (int)m_listData.GetCount())
			return (LRESULT)m_listData[nIndex].lpParam;
		else
			return (LRESULT)0;
	}

	LRESULT _OnSetItemData(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int			nIndex = (int)wParam;
		DWORD_PTR	pdata  = (DWORD_PTR)lParam;

		if (nIndex<(int)m_listData.GetCount())
		{
			m_listData[nIndex].lpParam = pdata;
			return (LRESULT)nIndex;
		}
		else
			return (LRESULT)0;
	}

	VOID OnPaint(CDCHandle dc)
	{
		if ( m_bNeedRecalLayout )
			_RecalcLayout();

		_RePaint(dc.m_hDC);
		ValidateRect(NULL);
	}

	LRESULT _OnDeleteString(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		UINT	nIndex = (UINT)wParam;

		if (nIndex < m_listData.GetCount())
		{
			m_listData.RemoveAt( (size_t)nIndex,1);

			if (nIndex==m_nCurSel)
				m_nCurSel = DEFAULT_SEL_VALUE;

			_RefreshIntern();
		}

		return S_OK;
	}

	LRESULT _OnInsertString(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		UINT		nIndex		= (UINT)wParam;
		LPCTSTR		lpstrText	= (LPCTSTR)lParam;

		if (nIndex <= m_listData.GetCount())
		{
			List_Intern_Data	datax;

			datax.lpParam	= NULL;
			datax.nHeight	= 0;
			datax.rcItem	= CRect(0,0,0,0);
			datax.strText	= lpstrText;

			m_listData.InsertAt(nIndex,datax,1);

			if (nIndex==m_nCurSel)
				m_nCurSel++;

			_RefreshIntern();

			return nIndex;
		}
		else
			return (LRESULT)-1;
	}

	LRESULT _OnAddString(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LPCTSTR				lpstr	= (LPCTSTR)lParam;
		List_Intern_Data	dataNew;

		dataNew.nHeight		= 0;
		dataNew.strText		= lpstr;
		m_listData.Add(dataNew);

		_RefreshIntern();

		return S_OK;
	}
	LRESULT _OnResetContent(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		m_nCurSel = DEFAULT_SEL_VALUE;
		m_listData.RemoveAll();
		_RefreshIntern(TRUE,TRUE,TRUE);
		return S_OK;
	}

	LRESULT _OnGetCursel(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return (LRESULT)m_nCurSel;
	}
	LRESULT _OnSetCursel(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		m_nCurSel = (int)wParam;
		return (LRESULT)m_nCurSel;
	}

	LRESULT _OnGetCount(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return (int)m_listData.GetCount();
	}

	LRESULT _OnSetCount(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ATLASSERT(m_listData.GetCount()==0);

		int		nNewCount = (int)wParam;

		for ( int i=0; i<nNewCount; i++)
		{
			List_Intern_Data	data;
			m_listData.Add(data);
		}
		
		_RefreshIntern();

		return S_OK;
	}

	LRESULT _OnSetRedraw(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		BOOL	bOld = m_bCanRedraw;
		m_bCanRedraw = (BOOL)wParam;
		return (LRESULT)bOld;
	}

	LRESULT _OnGetItemHeight(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int nIndex = (int)wParam;

		if ( nIndex >= (int)m_listData.GetCount() )
			return (LRESULT)0;
		else
			return (LRESULT)m_listData[nIndex].nHeight;
	}

	LRESULT _OnSetItemHeight(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int nIndex	= (int)wParam;

		int nHeight	= (int)lParam;
		if (nIndex < 0 ||
			nHeight < 0)
			return 0;

		if (nIndex >= m_listData.GetCount())
			return 0;

		m_listData[nIndex].nHeight	= nHeight;
		
		_RefreshIntern(TRUE,TRUE);

		return S_OK;
	}

	LRESULT _OnGetItemRect(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int			nIndex	= (int)wParam;
		LPRECT		lprc	= (LPRECT)lParam;

		if ( nIndex < (int)m_listData.GetCount() && lprc != NULL )
		{
			lprc->left		= m_listData[nIndex].rcItem.left;
			lprc->right		= m_listData[nIndex].rcItem.right;
			lprc->bottom	= m_listData[nIndex].rcItem.bottom;
			lprc->top		= m_listData[nIndex].rcItem.top;
		}

		return (LRESULT)nIndex;
	}

	LRESULT _OnItemFromPoint(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int ptx = LOWORD(lParam);
		int pty = HIWORD(lParam);

		for ( int i = 0; i < (int)m_listData.GetCount(); i++ )
		{
			List_Intern_Data&	data = m_listData[i];

			if ( data.rcItem.PtInRect( CPoint(ptx, pty) ) )
			{
				return (LRESULT)i;
			}
		}
		
		return (LRESULT)DEFAULT_SEL_VALUE;
	}

	LRESULT _OnSetColumnWidth(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		m_nColumnWidth = (int)wParam;

		for (int i=0; i < (int)m_listData.GetCount(); i++)
			m_listData[i].rcItem.right	= m_listData[i].rcItem.left + m_nColumnWidth;

		return S_OK;
	}

private:
	VOID _RecalcLayout()
	{
		if (!m_bNeedRecalLayout)
			return;

		CRect	rc;

		TBase::GetClientRect(&rc);

		if (rc.Width()==0||rc.Height()==0)
			return;

		int	nTop			= m_nScrollOffset;
		int	nTotalHeight	= 0;
		for ( int i=0; i<(int)m_listData.GetCount(); i++)
		{
			List_Intern_Data&	data = m_listData[i];
			MEASUREITEMSTRUCT	measure = {0};


			measure.itemHeight	= 0;
			measure.itemID		= i;

			MeasureItem(&measure);


			data.nHeight = measure.itemHeight;
			data.rcItem.left	= 0;
			data.rcItem.top		= nTop;

			if (m_nColumnWidth!=-1)
				data.rcItem.right	= m_nColumnWidth;
			else
				data.rcItem.right	= rc.Width();

			data.rcItem.bottom	= nTop+data.nHeight;

			nTop += data.nHeight;

			nTotalHeight += data.nHeight;
		}

		nTotalHeight += MARGIN_BOTTOM;
		if ( nTotalHeight <= rc.Height() )
		{
			if ( TBase::GetStyle() & WS_VSCROLL )
			{
				SCROLLINFO		sbInfo = {0};
				CRect			rcWin;

				sbInfo.cbSize = sizeof(SCROLLINFO);
				sbInfo.fMask  = SIF_ALL;

				TBase::GetWindowRect(&rcWin);
				TBase::GetScrollInfo(SB_VERT,&sbInfo);
				sbInfo.nPos = 0;
				TBase::SetScrollInfo(SB_VERT,&sbInfo,FALSE);
				m_nScrollOffset = 0;

				TBase::ModifyStyle(WS_VSCROLL,0);
				TBase::SetWindowPos(NULL,rcWin.left,rcWin.right,rcWin.Width(),rcWin.Height(),
					SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOREDRAW|SWP_NOZORDER|SWP_FRAMECHANGED);
			}
		}
		else
		{
			if ( !(TBase::GetStyle() & WS_VSCROLL) )
			{
				SCROLLINFO		sbInfo = {0};
				CRect			rcWin;

				sbInfo.cbSize = sizeof(SCROLLINFO);
				sbInfo.fMask = SIF_ALL;

				TBase::GetWindowRect(&rcWin);
				TBase::ModifyStyle(0,WS_VSCROLL);

				TBase::GetScrollInfo(SB_VERT,&sbInfo);
				sbInfo.fMask = SIF_ALL;
				sbInfo.nPos = 0;
				sbInfo.nMin = 0;
				sbInfo.nMax = nTotalHeight;
				sbInfo.nPage= rcWin.Height();
				TBase::SetScrollInfo(SB_VERT,&sbInfo,TRUE);
				m_nScrollOffset = 0;

				TBase::SetWindowPos(NULL,rcWin.left,rcWin.right,rcWin.Width(),rcWin.Height(),
					SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOREDRAW|SWP_NOZORDER|SWP_FRAMECHANGED);
			}
			else
			{
				SCROLLINFO		sbInfo = {0};
				CRect			rcWin;

				sbInfo.cbSize = sizeof(SCROLLINFO);
				sbInfo.fMask = SIF_ALL;

				TBase::GetWindowRect(&rcWin);
				TBase::ModifyStyle(0,WS_VSCROLL);

				TBase::GetScrollInfo(SB_VERT,&sbInfo);

				if (sbInfo.nMin != 0 || sbInfo.nPage != rcWin.Height() || sbInfo.nMax!= nTotalHeight
					|| sbInfo.nPos != (-m_nScrollOffset) )
				{
					sbInfo.fMask = SIF_ALL;
					sbInfo.nPos = (-m_nScrollOffset);
					sbInfo.nMin = 0;
					sbInfo.nMax = nTotalHeight;
					sbInfo.nPage= rcWin.Height();
					TBase::SetScrollInfo(SB_VERT,&sbInfo,TRUE);

				}
			}
		}

		TBase::GetClientRect(rc);

		m_bNeedRecalLayout = FALSE;
	}

	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT){}

	virtual VOID DrawItem(LPDRAWITEMSTRUCT){}

	VOID _ResetScroll()
	{
		m_nScrollOffset = 0;
	}

	VOID _RefreshIntern(BOOL bReCalc = TRUE, BOOL bReDraw = TRUE, BOOL bResetScroll = FALSE)
	{
		if (!m_bCanRedraw)
		{
			m_bNeedRecalLayout	= bReCalc;
			m_bNeedRedraw		= bReDraw;
			return;
		}

		if (bResetScroll)
			_ResetScroll();

		if (bReCalc)
		{
			m_bNeedRecalLayout = TRUE;
			_RecalcLayout();
		}

		if (bReDraw)
		{
			m_bNeedRedraw = TRUE;
			_RePaint(NULL);
		}
	}

	BOOL OnEraseBkgnd(CDCHandle dc)
	{
		return TRUE;
	}

	void OnSize(UINT nType, CSize /*size*/)
	{
		m_bNeedRecalLayout	= TRUE;
		_RecalcLayout();

		m_bNeedRedraw		= TRUE;

		SetMsgHandled(FALSE);
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		SetMsgHandled(FALSE);
		return 0;
	}


	VOID _RePaintItem(HDC hdc, int nIndex)
	{
		CRect	rc;
		TBase::GetClientRect(&rc);

		if (rc.Width() == 0 || rc.Height() == 0 )
			return;

		if (nIndex>=(int)m_listData.GetCount())
			return;

		_CreateMemDC(hdc);

		BOOL	bReleaseDC = FALSE;
		if (hdc==NULL)
		{
			hdc = TBase::GetWindowDC();
			bReleaseDC = TRUE;
		}
		HBITMAP bmpTmp = m_dcMem.SelectBitmap(m_bmpMem);

		if (TRUE)
		{
			List_Intern_Data&	data		= m_listData[nIndex];

			// 强制刷新
			if ( TRUE )
			{
				DRAWITEMSTRUCT		drawData	= {0};
				drawData.hDC		= m_dcMem.m_hDC;
				drawData.itemID		= nIndex;
				drawData.rcItem		= data.rcItem;
				drawData.itemData	= data.lpParam;
				DrawItem(&drawData);
			}	

			::BitBlt(hdc,0,0,rc.Width(),rc.Height(),m_dcMem.m_hDC,0,0,SRCCOPY);
		}

		m_dcMem.SelectBitmap(bmpTmp);

		if (bReleaseDC)
			TBase::ReleaseDC(hdc);
	}

	VOID _RePaint(HDC hdc)
	{
		CRect	rc;
		TBase::GetClientRect(&rc);

		if (rc.Width() == 0 || rc.Height() == 0 )
			return;

		_CreateMemDC(hdc);

		BOOL	bReleaseDC = FALSE;
		if (hdc==NULL)
		{
			hdc = TBase::GetWindowDC();
			bReleaseDC = TRUE;
		}
		HBITMAP bmpTmp = m_dcMem.SelectBitmap(m_bmpMem);

		if (!m_bNeedRedraw)
		{
			::BitBlt(hdc,0,0,rc.Width(),rc.Height(),m_dcMem.m_hDC,0,0,SRCCOPY);
		}
		else
		{
			m_dcMem.FillSolidRect(0,0,rc.Width(),rc.Height(),m_clrBG);
			for ( int i=0; i<(int)m_listData.GetCount(); i++)
			{
				List_Intern_Data&	data		= m_listData[i];

				// 只绘制显示区域内的
				if ( data.rcItem.bottom > 0 && data.rcItem.top < rc.Height() )
				{
					DRAWITEMSTRUCT		drawData	= {0};

					drawData.hDC		= m_dcMem.m_hDC;
					drawData.itemID		= i;
					drawData.rcItem		= data.rcItem;
					drawData.itemData	= data.lpParam;

					DrawItem(&drawData);
				}	
			}
			m_bNeedRedraw = FALSE;
			::BitBlt(hdc,0,0,rc.Width(),rc.Height(),m_dcMem.m_hDC,0,0,SRCCOPY);
		}

		m_dcMem.SelectBitmap(bmpTmp);
		m_bNeedRedraw = FALSE;

		if (bReleaseDC)
			TBase::ReleaseDC(hdc);
	}

	VOID _CreateMemDC(HDC hdc)
	{

		BOOL	bNeedRecreate = FALSE;
		CRect	rc;
		TBase::GetClientRect(&rc);
		

		if (!m_dcMem.m_hDC)
		{
			bNeedRecreate = TRUE;
		}
		else
		{
			CSize sz;
			BITMAP	bitmap;
			m_bmpMem.GetBitmap(&bitmap);
			if ( bitmap.bmWidth != rc.Width() || bitmap.bmHeight != rc.Height() )
			{
				bNeedRecreate = TRUE;
			}
		}

		if ( bNeedRecreate )
		{
			BOOL	bReleaseDC = FALSE;
			HWND	hWndDesk   = ::GetDesktopWindow();
			if (hdc==NULL)
			{
			//	hdc = TBase::GetWindowDC();

				hdc = ::GetDC(hWndDesk);
				bReleaseDC = TRUE;
			}

			if (m_dcMem.m_hDC)
				m_dcMem.DeleteDC();
			if (m_bmpMem.m_hBitmap)
				m_bmpMem.DeleteObject();


			m_dcMem.Attach( ::CreateCompatibleDC(hdc) );
			m_bmpMem.CreateCompatibleBitmap( hdc, rc.Width(), rc.Height() );
			m_bNeedRedraw = TRUE;

			if (bReleaseDC)
			{
				::ReleaseDC(hWndDesk,hdc);
				//TBase::ReleaseDC(hdc);
			}
		}

	}

	VOID _Init()
	{
		m_bNeedRedraw		= TRUE;
		m_bNeedRecalLayout	= TRUE;
		m_nColumnWidth		= -1;
		m_nCurSel			= DEFAULT_SEL_VALUE;
		m_nScrollOffset		= 0;
		m_bCanRedraw		= TRUE;
		m_clrBG				= RGB(0xFB,0xFC,0xFD);
	}

private:
	

	class List_Intern_Data
	{
	public:
		List_Intern_Data()
		{
			nHeight	= 0;
			lpParam	= 0;
		};

		int			nHeight;	// 高度
		DWORD_PTR	lpParam;	// set进来的参数 
		CString		strText;	// text标题，兼容windows的函数
		CRect		rcItem;		// item区域
	};

	CAtlArray<List_Intern_Data>	m_listData;
	CDC								m_dcMem;
	CBitmap							m_bmpMem;
	BOOL							m_bNeedRedraw;
	LONG							m_nScrollOffset;
	BOOL							m_bNeedRecalLayout;
	int								m_nCurSel;
	int								m_nColumnWidth;
	BOOL							m_bCanRedraw;
	COLORREF						m_clrBG;
};


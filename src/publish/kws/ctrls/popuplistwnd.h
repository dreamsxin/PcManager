////////////////////////////////////////////////////////////////////////////////
//      
//      Kingsoft File for kpfwfrm routines file
//      
//      File      : popuplistwnd.h
//      Comment   : 弹出列表控件
//					
//      Create at : 2008-12-29
//      Create by : chenguicheng
//      
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlwin.h>
#include <vector>
#include <atlstr.h>
#include <atlframe.h>
#include <atlmisc.h>

typedef CWinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN , 0>								CPopUpWinTraits;
typedef std::vector<HBITMAP>																HBITMAPVEC;
typedef std::vector<ATL::CString>															TEXTVEC;

#define POPUP_LIST_ITEM_HEGIHT		23
#define POPUP_LIST_PADDING_LEFT		5
#define POPUP_LIST_PADDING_TOP		5
#define  WM_USER_POPUPLIST			(WM_USER + 1236)


enum{

	enum_PLT_COMBOBOX	= 0,
	enum_PLT_NORMAL		= 1,

};


class KPopUpListWnd : public CWindowImpl< KPopUpListWnd, CWindow, CPopUpWinTraits >,
					public CDoubleBufferImpl<KPopUpListWnd>
{
public:

	typedef CWindowImpl< KPopUpListWnd,CWindow,CPopUpWinTraits > theBaseClass;

	DECLARE_WND_CLASS( _T("POPUP_LIST_WND") )

	BEGIN_MSG_MAP(KPopUpListWnd)
		CHAIN_MSG_MAP(CDoubleBufferImpl<KPopUpListWnd>)
		MESSAGE_HANDLER( WM_CREATE,			OnCreate )
		MESSAGE_HANDLER( WM_DESTROY,		OnDestroy )
		MESSAGE_HANDLER( WM_LBUTTONDOWN,	OnLButtonDown )
		MESSAGE_HANDLER( WM_RBUTTONDOWN,	OnRButtonDown )
		MESSAGE_HANDLER( WM_MOUSEMOVE,		OnMouseMove)
		MESSAGE_HANDLER( WM_ACTIVATEAPP,	OnActivateApp )
		MESSAGE_HANDLER( WM_KILLFOCUS,		OnKillFocus)

	END_MSG_MAP()

	KPopUpListWnd()
	: m_hParentWnd(NULL),
	  m_hMsgWnd(NULL),
	  m_nCurIndex(-1),
	  m_nPreIndex(-1),
	  m_nItemHeight(POPUP_LIST_ITEM_HEGIHT),
	  m_clrSel(RGB(0xda, 0xf8, 0xd2)),
	  m_nType( enum_PLT_COMBOBOX )
	{
		
	};

	~KPopUpListWnd()
	{
		
	};

	void SetType( int nType )
	{
		m_nType = nType;
	}


	HWND			m_hParentWnd;

	HFONT GetNormalFont()
	{
		CFontHandle hFont;
		LOGFONT lf;
		RtlZeroMemory(&lf, sizeof(LOGFONT));
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = 12 ;
		lf.lfWeight = 400;
		_tcscpy(lf.lfFaceName, TEXT("宋体") );
		hFont.CreateFontIndirect(&lf);

		return hFont;
	}


	BOOL CreatePopUpList( HWND hMsgWnd, HWND hParentWnd, HBITMAPVEC& vecBmp, TEXTVEC& vecText, COLORREF crMask )
	{
		HWND hWnd = NULL;
		DWORD dwStyle = WS_POPUP;
		DWORD dwStyleEx = WS_EX_TOOLWINDOW | WS_EX_TOPMOST ;
		SetImageVec(vecBmp);
		SetTextVec(vecText);
		m_rectWnd		= CRect( 0, 0, 180, 98 );
		hWnd			= Create( hParentWnd, m_rectWnd, NULL, dwStyle, dwStyleEx );
		m_hParentWnd	= hParentWnd;
		m_clrMask		= crMask;
		m_nItemCnt		= vecText.size();
		m_hMsgWnd		= hMsgWnd;

		if ( vecText.size() > 0 )
		{
			
			int nWidth = ( vecText[0].GetLength() - 2 ) * 12;

			m_rectWnd.right += nWidth;
			m_rectWnd.bottom = m_rectWnd.top + GetItemHeight() * m_vecText.size();

			
			SetWindowPos(HWND_TOPMOST,
				m_rectWnd.left,
				m_rectWnd.top,
				m_rectWnd.right,
				m_rectWnd.bottom,
				SWP_SHOWWINDOW  );
				
		}

		ATLASSERT( hWnd );
		ShowWindow(SW_HIDE);
		return TRUE;
	}

	void SetItemWidth( int nWidth )
	{
		m_rectWnd.right = m_rectWnd.left + nWidth;
	}

	// xPox yPox 是相对TreeListCtrl位置
	virtual BOOL PopUp( int xPos, int yPos )
	{
		ATLASSERT(m_hWnd);
		CRect rectParent;

		MoveWindow( CRect(xPos - m_rectWnd.Width(), yPos, xPos , yPos + m_rectWnd.Height() ), TRUE );
		ShowWindow( SW_SHOWNORMAL );
		SetCapture();

		return TRUE;
	}

	virtual BOOL HideWnd()
	{
		ATLASSERT(m_hWnd);
		ShowWindow(SW_HIDE);
		ReleaseCapture();

		return TRUE;
	}

	BOOL ResetInfo()
	{
		m_nCurIndex = -1;
		m_nPreIndex = -1;

		return TRUE;
	}

	void SetItemHeight( int nHeight )
	{
		m_nItemHeight = nHeight;
	}

	int GetItemHeight()
	{
		return m_nItemHeight;
	}

	void DoPaint(CDCHandle dc)
	{
		int xPos = POPUP_LIST_PADDING_LEFT;
		int yPos = POPUP_LIST_PADDING_TOP;

		//draw bg
		DrawBkGnd( dc, xPos, yPos, m_clrSel );

		//draw bmp
		DrawBmpIcon( dc, xPos, yPos );

		//draw text
		DrawItemText( dc, xPos, yPos );

		switch( m_nType )
		{
		case enum_PLT_COMBOBOX:
			DrawBorderComboBox( dc );
			break;
		case enum_PLT_NORMAL:
			DrawBorderNormal( dc );
			break;
		}

		return;
	}

	int GetCurIndex()
	{
		return m_nCurIndex;
	}


protected:

	virtual void DrawBkGnd( CDCHandle& dc, int& xPos, int& yPos, COLORREF clrSel )
	{
		dc.FillSolidRect(m_rectWnd, RGB(0xFF, 0xFF, 0xFF));
		CRect rectSel;
		if ( m_nCurIndex != -1 )
		{
			rectSel.top = xPos + m_nCurIndex * m_nItemHeight;
			rectSel.left = yPos;
			rectSel.bottom = rectSel.top + m_nItemHeight;
			rectSel.right = rectSel.left + m_rectWnd.Width() - 2 * POPUP_LIST_PADDING_LEFT;
			dc.FillSolidRect( rectSel, clrSel );
		}
	}

	virtual void DrawBmpIcon( CDCHandle& dc, int& xPos, int& yPos )
	{
		if ( m_vecBmp.size() == 0 )
			return;

		xPos = POPUP_LIST_PADDING_LEFT;
		yPos = POPUP_LIST_PADDING_TOP;

		for ( int i = 0; i < m_vecBmp.size(); i++ )
		{
			HBITMAP hBmp = m_vecBmp[i];
			if (!hBmp)
				continue;

			CDC dcMem;
			dcMem.CreateCompatibleDC(dc);
			HBITMAP hOldBmp = dcMem.SelectBitmap( hBmp );
			BITMAP bmp;

			::GetObject(hBmp, sizeof(BITMAP), &bmp);
			dc.TransparentBlt( xPos, yPos + 2, bmp.bmWidth, bmp.bmHeight, dcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, m_clrMask );
			yPos += m_nItemHeight;
			dcMem.SelectBitmap( hOldBmp );
		}
	}

	virtual void DrawBorderNormal( CDCHandle &dc )
	{

	}

	virtual void DrawBorderComboBox( CDCHandle &dc )
	{
		dc.Draw3dRect( m_rectWnd, RGB(0xF1, 0xEF, 0xE2), RGB(0x71,0x6F,0x64));
		dc.Draw3dRect( m_rectWnd.left + 1,
			m_rectWnd.top + 1,
			m_rectWnd.right - 2,
			m_rectWnd.bottom - 2,
			RGB(0xFF, 0xFF, 0xFF), RGB(0xAC,0xA8,0x99));
		dc.Draw3dRect( m_rectWnd.left + 2,
			m_rectWnd.top + 2,
			m_rectWnd.right - 4,
			m_rectWnd.bottom - 4,
			RGB(0xEC, 0xE9, 0xD8), RGB(0xEC, 0xE9, 0xD8));
	}

	virtual void DrawItemText( CDCHandle &dc, int& xPos, int& yPos )
	{
		CFont font( GetNormalFont() );
		HFONT hOldFont = dc.SelectFont( font );

		xPos += m_nItemHeight;
		yPos = POPUP_LIST_PADDING_LEFT + 4;
		dc.SetBkMode( TRANSPARENT );
		for ( int i = 0; i < m_vecText.size(); i ++ )
		{
			dc.TextOut( xPos, yPos, m_vecText[i], m_vecText[i].GetLength() );
			yPos += m_nItemHeight;
		}

		if ( hOldFont )
		{
			dc.SelectFont( hOldFont );
		}
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		for ( int i = 0; i < m_vecBmp.size(); i++ )
		{
			HBITMAP hBmp = m_vecBmp[i];
			if (!hBmp)
				continue;

			::DeleteObject( (HGDIOBJ)hBmp );
		}
		return TRUE;
	}

	LRESULT OnActivateApp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		if( wParam == FALSE )
			ShowWindow( SW_HIDE );
		return TRUE;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		ResetInfo();
		HideWnd();

		if ( m_rectWnd.PtInRect(CPoint(xPos, yPos)))
		{
			
			m_nCurIndex= HitTest( xPos, yPos);

			if ( m_hMsgWnd && m_nCurIndex != -1)
				::SendMessage( m_hMsgWnd, WM_USER_POPUPLIST, (WPARAM)m_nCurIndex, (LPARAM)(LPCTSTR)m_vecText[m_nCurIndex] );
		}

		return TRUE;
	}

	LRESULT OnRButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		HideWnd();
		ResetInfo();
		return TRUE;
	}

	LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		ShowWindow( SW_HIDE );
		return TRUE;
	}

	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if( m_rectWnd.PtInRect(CPoint(xPos, yPos)) )
		{
			m_nCurIndex = HitTest( xPos, yPos);

			if ( m_nCurIndex != m_nPreIndex )
			{
				m_nPreIndex = m_nCurIndex;
				Invalidate(TRUE);
			}
		}

		return TRUE;
	}

	// owner function
	void SetImageVec( const HBITMAPVEC& vecBmp )
	{
		m_vecBmp.clear();
		m_vecBmp = vecBmp;
	}

	void SetTextVec( const TEXTVEC& vecText )
	{
		m_vecText.clear();
		m_vecText = vecText;
	}

	int HitTest(int xPos, int yPos )
	{
		int nIndex = -1;

		yPos -= POPUP_LIST_PADDING_TOP;
		nIndex = yPos / m_nItemHeight;

		if ( nIndex >= m_nItemCnt || nIndex < 0 )
		{
			nIndex = -1;
		}

		return nIndex;
	}

	HWND GetMsgHandle()
	{
		return m_hMsgWnd;
	}

	TEXTVEC			m_vecText;
	CRect			m_rectWnd;
	HBITMAPVEC		m_vecBmp;
	COLORREF		m_clrMask;
	COLORREF		m_clrSel;
	
	int				m_nCurIndex;
	int				m_nItemCnt;
	int				m_nPreIndex;
	HWND			m_hMsgWnd;
	int				m_nItemHeight;
	int				m_nType;

};
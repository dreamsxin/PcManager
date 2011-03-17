////////////////////////////////////////////////////////////////////////////////
//      
//      Kingsoft File for WTL GUI routines file
//      
//      File      : tracktooltip.h
//      Comment   : 跟随鼠标的tooltip
//      
//      Create at : 2009-03-26
//      Create by : chenguicheng
//      
////////////////////////////////////////////////////////////////////////////////

#pragma once


#define TT_PADDING_LEFT		7
#define TT_PADDING_TOP		7
#define TT_PADDING_BOTTOM	7
#define TT_PADDING_RIGHT	7

#define	TIMER_OUT_ID		500
#define TIMER_ID_TIP		1024		
#include "../shp/shp_misc.h"


class  KTrackTooltip : public CWindowImpl< KTrackTooltip >
{
public:
	KTrackTooltip():
	m_bTTVisble(FALSE),
	m_nIndex(-1),
	m_bRelay(FALSE),
	m_nTimeout(TIMER_OUT_ID)
	{

	}

	~KTrackTooltip()
	{
		UnInit();
	}

	BEGIN_MSG_MAP(KTrackTooltip)
		MESSAGE_HANDLER(WM_PAINT,				OnPaint)
		MESSAGE_HANDLER(WM_CREATE,				OnCreate)
		MESSAGE_HANDLER(WM_TIMER,				OnTimer)
	END_MSG_MAP()

	static DWORD GetWndStyle( DWORD dwStyle )
	{
		return WS_POPUP;
	}

	static DWORD GetWndExStyle( DWORD dwStyleEx )
	{
		return WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return TRUE;
	}

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( wParam == TIMER_ID_TIP )
		{
			KillTimer( TIMER_ID_TIP );
			if( m_bTTVisble == FALSE )
			{
				ShowWindow( SW_HIDE );
			}
		}
		return TRUE;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CPaintDC dc( m_hWnd );

		CRect rect;
		GetClientRect( rect );
		HFONT hOldFont = dc.SelectFont( m_hFont );
		dc.SetBkMode( TRANSPARENT );

		dc.FillSolidRect( rect, RGB(0xFF, 0xFF, 0xE1 ) );
		CBrush brush( ::CreateSolidBrush(RGB(0x00, 0x00, 0x00)));
		dc.FrameRect( rect, brush );
		
		//dc.DrawText( m_strText, m_strText.GetLength(), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		rect.left += TT_PADDING_LEFT * 2;
		rect.top += TT_PADDING_TOP;
		rect.right -= TT_PADDING_RIGHT;
		rect.left -= TT_PADDING_BOTTOM;

		draw_text_in_rect( dc, m_strText, m_strText.GetLength(), rect, 2 );

		if( hOldFont )
			dc.SelectFont( hOldFont );

		return 0;
	}

	void SetRelay( BOOL bRelay, int nTimer )
	{
		m_bRelay = bRelay;
		m_nTimeout = nTimer;
	}


	void Init( HWND hParent )
	{
		if( IsWindow() )
			return;

		Create( hParent );
		ShowWindow( SW_HIDE );
		m_strText = TEXT("  ");

		LOGFONT lf;
		RtlZeroMemory(&lf, sizeof(LOGFONT));
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = 12 ;
		lf.lfWeight = 400;
		lf.lfUnderline = FALSE;
		_tcscpy(lf.lfFaceName, TEXT("宋体") );
		m_hFont.CreateFontIndirect(&lf);

		return;
	}

	void UnInit()
	{
		if ( IsWindow() )
		{
			DestroyWindow( );
		}
	}

	BOOL ActiveTooltip( BOOL bActive )
	{		
		if ( bActive == TRUE && m_bTTVisble == TRUE )
		{
			return TRUE;
		}
		
		if ( IsWindow() )
		{
			if( bActive == TRUE )
			{
				SetWindowPos( NULL, 0, 0 , 0, 0, SWP_NOSIZE | SWP_NOMOVE |SWP_NOREPOSITION | SWP_SHOWWINDOW | SWP_NOACTIVATE );
				m_bTTVisble = TRUE;
			}
			else
			{
				if( m_bRelay )
					SetTimer( TIMER_ID_TIP, m_nTimeout, NULL );
				else
					ShowWindow( SW_HIDE );
				m_bTTVisble = FALSE;
			}
		}

		return TRUE;
	}

	// nIndex 用于btn类型优化。Heartbeat 里面不能优化，nIndex = -1
	BOOL ShowText( CPoint& point, LPCTSTR pszShowText, int nIndex )
	{
		if( IsWindow() )
		{
			if( abs(point.x - m_pt.x) > 2 || abs( point.y - m_pt.y) > 2 )
			{
				SetWindowPos( NULL, point.x + 16, point.y + 16 , 0, 0, SWP_NOSIZE | SWP_NOREPOSITION | SWP_SHOWWINDOW | SWP_NOACTIVATE );
				m_pt = point;
			}

			if( nIndex != m_nIndex || nIndex == -1 )
			{	
				UpdateTipText( pszShowText );
				m_nIndex = nIndex;
			}
		}

		return TRUE;
	}

	BOOL UpdateTipText( LPCTSTR pszShowText )
	{
		if( !pszShowText )
			return FALSE;
		
		m_strText = pszShowText;

		ActiveTooltip( TRUE );

		CDC dc (::GetDC( m_hWnd ) );
		HFONT hOldFont = dc.SelectFont( m_hFont );
		SIZE size;
		
		dc.GetTextExtent( m_strText, -1, &size );
		if( size.cx > 174 )
		{
			size.cx = 174;
			size.cy += 15;
		}

		if( hOldFont )
			dc.SelectFont( hOldFont );

		SetWindowPos( NULL, 0, 0, size.cx + TT_PADDING_LEFT + TT_PADDING_RIGHT,
			size.cy + TT_PADDING_BOTTOM + TT_PADDING_TOP, SWP_NOMOVE | SWP_NOREPOSITION | SWP_SHOWWINDOW | SWP_NOACTIVATE);

		Invalidate();

	
		return TRUE;
	}


private:
	BOOL						m_bTTVisble;
	ATL::CString				m_strText;
	CFont						m_hFont;
	CPoint						m_pt;

	int							m_nIndex;	//用于优化效率
	BOOL						m_bRelay;
	int							m_nTimeout;


};
////////////////////////////////////////////////////////////////////////////////
//      
//      File for kpfwfrm
//      
//      File      : shpwndbase.h
//      Version   : 1.0
//      Comment   : shp在网镖界面应用基础窗口
//      
//      Create at : 2009-03-10
//		Create by : chenguicheng  
//      
///////////////////////////////////////////////////////////////////////////////


#pragma  once 
#include <atlwin.h>
#include "shp/shpparser.h"
#include <atlframe.h>

class KShpWndBase: public CWindowImpl<KShpWndBase>,
	public CDoubleBufferImpl<KShpWndBase>
{
public:

	KShpWndBase( )
		:m_bMouseOver(FALSE)
	{}

	~KShpWndBase(){};

	BEGIN_MSG_MAP(KShpWndBase)

		CHAIN_MSG_MAP(CDoubleBufferImpl<KShpWndBase>)

		MESSAGE_HANDLER(WM_MOUSEMOVE,	OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE,	OnMouseLeave)
		MESSAGE_HANDLER(WM_LBUTTONUP,	OnLButtonUp)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		REFLECT_NOTIFICATIONS();

	END_MSG_MAP()

	static DWORD GetWndStyle( DWORD dwStyle )
	{
		if ( dwStyle )
			return dwStyle;
		else
			return WS_CHILDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;
	}

	static DWORD GetWndExStyle( DWORD dwExtyle )
	{
		if ( dwExtyle )
			return dwExtyle;
		else
			return WS_EX_CONTROLPARENT;
	}

	BOOL IsMouseOver()
	{
		return m_bMouseOver;
	}

	LPCTSTR HitTest( HDC hdc, UINT uMsg, int xPos, int yPos )
	{
		return m_shpParser.hit_test( hdc, uMsg, xPos, yPos );
	}


	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		CPoint					mousePoint(xPos, yPos);

		HDC hdc = ::GetDC( m_hWnd );
		LPCTSTR lp_str_id = NULL;
		lp_str_id = m_shpParser.hit_test( hdc, uMsg, xPos, yPos );

		if ( mousePoint == m_ptOld )
		{
			if (hdc)
				::ReleaseDC( m_hWnd, hdc );
			return TRUE;
		}
		else
		{
			m_ptOld = mousePoint;
		}

		MouseMoveTest( lp_str_id, xPos, yPos );

		if (hdc)
			::ReleaseDC( m_hWnd, hdc );

		if ( m_bMouseOver == FALSE )
		{
			m_bMouseOver = TRUE;
			StartTrackMouseLeave();
		}

		//bHandled = FALSE;

		return TRUE;
	}

	

	virtual LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		HDC hdc = ::GetDC( m_hWnd );
		m_shpParser.hit_test( hdc, uMsg, xPos, yPos );
		m_bMouseOver = FALSE;

		DoMouseLeave( xPos, yPos );

		Invalidate(FALSE);

		if (hdc)
			::ReleaseDC( m_hWnd, hdc );

		return TRUE;
	}

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		HDC hdc = ::GetDC( m_hWnd );
		LPCTSTR lp_str_id = NULL;
		lp_str_id = m_shpParser.hit_test( hdc, uMsg, xPos, yPos );

		if (hdc)
			::ReleaseDC( m_hWnd, hdc );

		if ( lp_str_id )
		{
			ProcessBtnUp( lp_str_id );
		}


		return TRUE;
	}


	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		HDC hdc = ::GetDC( m_hWnd );

		m_shpParser.hit_test( hdc, uMsg, xPos, yPos );

		if (hdc)
			::ReleaseDC( m_hWnd, hdc );

		return TRUE;
	}


	void ParserText( LPCTSTR lpText )
	{
		assert( lpText );
		if ( lpText )
		{
			m_shpParser.prase_text( lpText );
		}
	}

	void SetItemText( LPCTSTR strId, LPCTSTR strText, BOOL bVisible = TRUE )
	{
		m_shpParser.set_item_text( strId, strText, bVisible ? 1 : 0 );
	}

	void SetItemState( LPCTSTR strId, int nState )
	{
		m_shpParser.set_item_state( strId, nState );
	}


	void SetItemTextNCV( LPCTSTR strId, LPCTSTR strText )
	{
		m_shpParser.set_item_text_nochange_visible( strId, strText );
	}

	void SetLineColor( LPCTSTR strId, COLORREF clr )
	{
		m_shpParser.set_line_clr( strId, clr);
	}

	void SetTextColor( LPCTSTR strId, COLORREF clr )
	{
		m_shpParser.set_item_text_clr(strId, clr );
	}

	LPCTSTR GetItemText( LPCTSTR strId )
	{
		return m_shpParser.get_item_text( strId );
	}

	int GetItemLeft( LPCTSTR strId )
	{
		return m_shpParser.get_item_left( strId );
	}

	void SetItemLeft( LPCTSTR strId, int nLeft )
	{
		m_shpParser.set_item_left( strId, nLeft );
	}

	int GetItemWidth( LPCTSTR strId )
	{
		return m_shpParser.get_item_width( strId );
	}

	void SetItemWidth( LPCTSTR strId, int nWidth )
	{
		m_shpParser.set_item_width( strId, nWidth );
	}

	LPCTSTR GetLinkUrl( LPCTSTR strId )
	{
		return m_shpParser.get_link_url( strId );
	}

	void SetBmpSrc( LPCTSTR strId, LPCTSTR strText, BOOL bVisible = TRUE )
	{
		m_shpParser.set_bmp_src( strId, strText, bVisible ? 1 : 0);
	}

	void SetVisible( LPCTSTR strId, BOOL bVisible )
	{
		m_shpParser.set_item_visible( strId, bVisible ? 1 :0 );
	}

	void SetItemTop( LPCTSTR strId, int nTop )
	{
		m_shpParser.set_item_top( strId, nTop );
	}

	int GetItemTop( LPCTSTR strId )
	{
		return m_shpParser.get_item_top( strId );
	}

	void SetDisplay( LPCTSTR strId, BOOL bDisplay )
	{
		m_shpParser.set_item_display( strId, bDisplay ? 1 :0 );
	}

	void SetHyperLinkEnable( LPCTSTR strId, BOOL bEnable )
	{
		m_shpParser.set_hyperlink_enable( strId, bEnable ? 1 :0 );
	}

	void SetBmpHandle( LPCTSTR strId, HBITMAP hbmp, BOOL bVisible = TRUE )
	{
		m_shpParser.set_bmp_handle( strId, hbmp, bVisible ? 1 : 0 );
	}

	HBITMAP GetBmpHandle( LPCTSTR strId )
	{
		return m_shpParser.get_bmp_handle( strId );
	}

	BOOL DeleteBmp( LPCTSTR strId )
	{
		return m_shpParser.delete_bmp( strId );
	}

	void SetShapeFill( LPCTSTR strId, BOOL bFill )
	{
		return m_shpParser.set_shape_fill( strId, bFill != FALSE ? 1 : 0 );
	}

	void SetShapeBg( LPCTSTR strId, COLORREF clrBg )
	{
		return m_shpParser.set_shape_bg( strId, clrBg );
	}

	BOOL GetShapeFill( LPCTSTR strId )
	{
		return m_shpParser.get_shape_fill( strId ) == 1;
	}

	BOOL GetBtnCheck( LPCTSTR strId )
	{
		return m_shpParser.get_btn_check( strId ) == 1;
	}

	void  SetSpeTextClr(  LPCTSTR strId, COLORREF clrSpe )
	{
		m_shpParser.set_spe_clr( strId, clrSpe);
	}

	void SetSpenWord( LPCTSTR strId, ATL::CString strSpeWord )
	{
		m_shpParser.set_spe_word( strId, strSpeWord );
	}

	void GetItemRect( LPCTSTR strId, LPRECT pRect )
	{
		m_shpParser.get_item_rect( strId, pRect );
		return;
	}

	void SetBtnCheck( LPCTSTR strId, BOOL bCheck )
	{
		m_shpParser.set_btn_check( strId, bCheck != FALSE ?1:0, 1 );
	}

	void SetBtnText( LPCTSTR strId, LPCTSTR pszText )
	{
		m_shpParser.set_btn_text( strId, pszText );
	}

	void ReInit()
	{
		m_shpParser.re_init();
	}

	void DoPaint(CDCHandle dc)
	{
		DrawBkGnd( dc );
		DrawWnd( dc );

	}

	virtual LRESULT ProcessBtnUp( LPCTSTR pszClickedId )
	{
		ASSERT(FALSE);  //must be implemented in a derived class
		return FALSE;
	}

	virtual void DrawBkGnd( CDCHandle dc )
	{
		ASSERT(FALSE); //must be implemented in a derived class
		return ;
	}

	virtual void DrawWnd( CDCHandle dc )
	{
		ASSERT(FALSE); //must be implemented in a derived class
		return ;
	}

	// may be not implemented
	virtual void MouseMoveTest( LPCTSTR lp_str_id, int xPos, int yPos )
	{
		return;
	}

	virtual void DoMouseLeave( int xPos, int yPos)
	{
		return;
	}

	void InvalidateItem( LPCTSTR pszItem )
	{
		CRect rect;
		GetItemRect( pszItem, &rect );
		InvalidateRect( &rect );
	}

	//注意不要Id循环
	void MapIdToId( LPCTSTR pszId, LPCTSTR pszBtnId )
	{
		ASSERT( _tcsicmp( pszId, pszBtnId) != 0 );
		ProcessBtnUp( pszBtnId );
	}

	

protected:

	BOOL StartTrackMouseLeave()
	{
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		return _TrackMouseEvent(&tme);
	}


protected:

	BOOL				m_bMouseOver;
	shp_shpparser		m_shpParser;
	CPoint				m_ptOld;
	

};
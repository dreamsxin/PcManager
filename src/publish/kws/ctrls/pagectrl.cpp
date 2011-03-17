#include "stdafx.h"
#include "pagectrl.h"

CPageCtrl::CPageCtrl()
:m_nCurPage(1),
m_nPadding(8),
m_nItemCnt(0),
m_nMaxPage(8),
m_nPrePage(0),
m_hNotify(NULL)
{

}

CPageCtrl::~CPageCtrl()
{

}

int CPageCtrl::GetCurPage()
{
	return m_nCurPage;
}

void CPageCtrl::ReSetCurPage()
{
	m_nPrePage = m_nCurPage;
	m_nCurPage = 1;
}

int	 CPageCtrl::GetPrePage()
{
	return m_nPrePage;
}

void CPageCtrl::SetNotifyHwnd( HWND hNotify )
{
	m_hNotify = hNotify;
}

void CPageCtrl::SetCnt( int nCnt, int nOnePageCnt )
{
	//ReSetCurPage();//hub

	m_nItemCnt = nCnt;

	if( nOnePageCnt == 0 )
		nOnePageCnt = 1;

	m_nMaxPage = nCnt / nOnePageCnt;

	if( nCnt % nOnePageCnt != 0 )
		m_nMaxPage ++;

	if( IsWindow() )
		Invalidate();
}

LRESULT CPageCtrl::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	CPoint	point(xPos, yPos);
	int nFlag = HitTestBtn( point );
	int nNum = HitTestNum( point );

	if( nFlag != en_NONE || nNum != 0 )
	{
		if( m_hCurBtn )
			SetCursor( m_hCurBtn );
	}

	return TRUE;
}

LRESULT CPageCtrl::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LOGFONT lf;
	CFontHandle font = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	font.GetLogFont(&lf);
	_tcscpy( lf.lfFaceName, TEXT("宋体") );
	lf.lfHeight = -12;
	m_fontNormal = ::CreateFontIndirect(&lf);

	lf.lfWeight = FW_NORMAL;
	lf.lfUnderline = TRUE;
	m_fontLink = ::CreateFontIndirect(&lf);

	lf.lfWeight = FW_BOLD;
	lf.lfUnderline = FALSE;
	m_fontBold = ::CreateFontIndirect(&lf);

	m_hCurBtn = ::LoadCursor( NULL, IDC_HAND );

	return TRUE;
}

void CPageCtrl::DoPaint( CDCHandle dc )
{
	CRect rcClient;
	GetClientRect( &rcClient );
	CString strFirst( TEXT("返回首页") );
	CString strPre( TEXT("上一页") );
	CString strNext( TEXT("下一页") );
	CSize size;
	
	dc.FillSolidRect( rcClient, RGB(0xFB, 0xFC, 0xFD) );

	dc.GetTextExtent( strNext, -1, &size );
	m_rcNextLink.top		= ( rcClient.Height() - size.cy ) / 2 + rcClient.top;
	m_rcNextLink.bottom		= m_rcNextLink.top + size.cy;
	m_rcNextLink.right		= rcClient.right;
	m_rcNextLink.left		= m_rcNextLink.right - size.cx;

	HFONT hOldFont = dc.SelectFont( m_fontLink );
	int nMode = dc.SetBkMode( TRANSPARENT );
	COLORREF clrText = dc.SetTextColor( RGB(0x7C, 0x9F, 0xBD) );

	if( m_nCurPage == m_nMaxPage )
		dc.SetTextColor( RGB(0xCC, 0xCC, 0xCC) );
	else
		dc.SetTextColor( RGB(0x7C, 0x9F, 0xBD) );

	dc.TextOut( m_rcNextLink.left, m_rcNextLink.top, strNext, -1 );
	
	m_rcNextBtn.right = m_rcNextLink.left - m_nPadding;
	m_rcNextBtn.left = m_rcNextBtn.right - 6;
	m_rcNextBtn.top = ( rcClient.Height() - 11 ) / 2 + rcClient.top;
	m_rcNextBtn.bottom = m_rcNextBtn.top + 11;
	DrawBtnFlag( dc, m_rcNextBtn, FALSE );

	m_rcText.right = m_rcNextBtn.left - m_nPadding;
	m_rcText.left = m_rcText.right - 94;
	m_rcText.top = m_rcNextBtn.top;
	m_rcText.bottom = m_rcNextBtn.bottom;

	CPoint point;
	dc.SetTextColor( RGB(0x7C, 0x9F, 0xBD) );
	DrawPageNum( dc, m_rcText );

	m_rcPreBtn.top = m_rcNextBtn.top;
	m_rcPreBtn.bottom = m_rcNextBtn.bottom;
	m_rcPreBtn.right = m_rcText.left;
	m_rcPreBtn.left = m_rcPreBtn.right - 6;
	DrawBtnFlag( dc, m_rcPreBtn, TRUE );


	dc.GetTextExtent( strPre, -1, &size );
	m_rcPreLink.top			= m_rcNextLink.top;	
	m_rcPreLink.bottom		= m_rcNextLink.bottom;
	m_rcPreLink.right		= m_rcPreBtn.left - m_nPadding;
	m_rcPreLink.left		= m_rcPreLink.right - size.cx;

	if( m_nCurPage == 1 )
		dc.SetTextColor( RGB(0xCC, 0xCC, 0xCC) );
	else
		dc.SetTextColor( RGB(0x7C, 0x9F, 0xBD) );

	dc.TextOut( m_rcPreLink.left, m_rcPreLink.top, strPre, -1 );

	dc.GetTextExtent( strFirst, -1, &size );
	m_rcFirstLink.top			= m_rcNextLink.top;	
	m_rcFirstLink.bottom		= m_rcNextLink.bottom;
	m_rcFirstLink.right			= m_rcPreLink.left - m_nPadding;
	m_rcFirstLink.left			= m_rcFirstLink.right - size.cx;

	dc.TextOut( m_rcFirstLink.left, m_rcFirstLink.top, strFirst, -1 );

	if( hOldFont )
		dc.SelectFont( hOldFont );
	dc.SetTextColor( clrText );
	dc.SetBkMode( nMode );
}

int  CPageCtrl::GetMaxPage()
{
	return m_nMaxPage;
}

int CPageCtrl::DrawPageNum( CDCHandle& dc, CRect& rcText )
{
	int nRight = rcText.right;
	int nTop = rcText.top;
	CSize size;

	m_vecHitTest.clear();

	if( m_nMaxPage <= 7 )
	{
		for ( int i = m_nMaxPage; i >= 1; i-- )
		{
			_DrawOneNum( dc, nRight, nTop, i  );
		}	
	}
	else
	{
		if( m_nCurPage <= 5 )
		{
			_DrawOneNum( dc, nRight, nTop, m_nMaxPage);
			_DrawText( dc, nRight, nTop, TEXT("..") );

			for ( int i = 5; i >= 1; i-- )
			{
				_DrawOneNum( dc, nRight, nTop, i );
			}
		}
		else if ( m_nMaxPage - m_nCurPage < 4 )
		{
			for ( int i = m_nMaxPage; i >= m_nMaxPage - 4 ; i-- )
			{
				_DrawOneNum( dc, nRight, nTop, i );
			}

			_DrawText( dc, nRight, nTop, TEXT("..") );
			_DrawOneNum( dc, nRight, nTop, 1 );
		}
		else
		{
			_DrawOneNum( dc, nRight, nTop, m_nMaxPage );
			_DrawText( dc, nRight, nTop, TEXT("..") );

			for ( int i = m_nCurPage + 1; i >= m_nCurPage - 1; i-- )
			{
				_DrawOneNum( dc, nRight, nTop, i );
			}

			_DrawText( dc, nRight, nTop, TEXT(".."));
			_DrawOneNum( dc, nRight, nTop, 1 );
		}
	}

	rcText.left = nRight;

	return 0;
}

void CPageCtrl::_DrawText( CDCHandle& dc, int& nRight, int nTop, LPCTSTR pszText )
{
	CSize size;
	HFONT hOldFont = dc.SelectFont( m_fontNormal );
	dc.GetTextExtent( pszText, -1, &size );
	nRight -= size.cx;

	CRect rtText;
	rtText.left = nRight;
	rtText.right = rtText.left + size.cx;
	rtText.top = nTop;
	rtText.bottom = rtText.top + size.cy;
	
	dc.DrawText( pszText, -1, &rtText, DT_VCENTER | DT_LEFT | DT_SINGLELINE );
	nRight -= m_nPadding ;

	if( hOldFont )
		dc.SelectFont( hOldFont );

	return;
}

void CPageCtrl::_DrawOneNum( CDCHandle& dc, int& nRight, int nTop, int nNum )
{
	HFONT hOldFont = NULL;
	COLORREF clrText = CLR_INVALID; 
	
	if( nNum == m_nCurPage )
	{
		hOldFont = dc.SelectFont( m_fontBold );
		clrText = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
	}
	
	CString strNum;
	strNum.Format( TEXT("%d"), nNum );
	CSize size;
	dc.GetTextExtent( strNum, -1, &size );
	nRight -= size.cx;

	CRect rtText;
	rtText.left = nRight;
	rtText.right = rtText.left + size.cx;
	rtText.top = nTop;
	rtText.bottom = rtText.top + size.cy;

	dc.DrawText( strNum, -1, &rtText, DT_VCENTER | DT_LEFT | DT_SINGLELINE );
	rtText.left -= 2;
	rtText.right += 2;

	HIT_TEST_ITEM item;
	item.nPage = nNum;
	item.rect = rtText;
	m_vecHitTest.push_back( item );

	nRight -= m_nPadding;

	if( hOldFont )
		dc.SelectFont( hOldFont );
	if( clrText != CLR_INVALID  )
		dc.SetTextColor( clrText );

	return;
}

void CPageCtrl::DrawBtnFlag( CDCHandle& dc, CRect& rtBtn, BOOL bLeft )
{
	POINT point[3];
	if( bLeft )
	{
		point[0].x = rtBtn.right;
		point[0].y = rtBtn.top;

		point[1].x = rtBtn.right;
		point[1].y = rtBtn.bottom;

		point[2].x = rtBtn.left;
		point[2].y = ( rtBtn.top + rtBtn.bottom ) / 2;
	}
	else
	{
		point[0].x = rtBtn.left;
		point[0].y = rtBtn.top;

		point[1].x = rtBtn.left;
		point[1].y = rtBtn.bottom;

		point[2].x = rtBtn.right;
		point[2].y = ( rtBtn.top + rtBtn.bottom ) / 2;
	}

	HRGN  h3Pt = CreatePolygonRgn( point, 3, ALTERNATE );
	CBrush brush( CreateSolidBrush( RGB(0x7C, 0x9F, 0xBD) ) );
	if( h3Pt )
		dc.FillRgn( h3Pt, brush );

	if( h3Pt )
		::DeleteObject( (HGDIOBJ)h3Pt );
}

LRESULT CPageCtrl::OnLButtonUp(UINT , WPARAM , LPARAM lParam, BOOL& bHandled)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	CPoint	point(xPos, yPos);
	int nFlag = HitTestBtn( point );
	int nNum = HitTestNum( point );

	if( nFlag != en_NONE || nNum != 0 )
	{
		if( m_hCurBtn )
			SetCursor( m_hCurBtn );
	}

	return TRUE;
}

LRESULT CPageCtrl::OnLButtonDown(UINT , WPARAM , LPARAM lParam, BOOL& bHandled)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);

	CPoint	point(xPos, yPos);
	int nFlag = HitTestBtn( point );
	int nNum = 0;

	int nPrePage = m_nCurPage;

	switch( nFlag )
	{
	case en_FIRST_LINK:
		m_nPrePage = m_nCurPage;
		m_nCurPage = 1;
		break;
	case en_PRE_LINK:
		if( m_nCurPage > 1)
		{
			m_nPrePage = m_nCurPage;
			m_nCurPage--;
		}
		break;
	case en_NEXT_LINK:
		if( m_nCurPage < m_nMaxPage )
		{
			m_nPrePage = m_nCurPage;
			m_nCurPage ++;
		}
		break;
	case en_PRE_BTN:
		if( m_nCurPage > 1)
		{
			m_nPrePage = m_nCurPage;
			m_nCurPage--;
		}
		break;
	case en_NEXT_BTN:
		if( m_nCurPage < m_nMaxPage )
		{
			m_nPrePage = m_nCurPage;
			m_nCurPage ++;
		}	
		break;
	default:
		{
			nNum = HitTestNum( point );
			m_nPrePage = m_nCurPage;
			if( nNum != 0 )
				m_nCurPage = nNum;
		}
		break;
	}

	if( (nFlag != en_NONE || nNum != 0 ) && nPrePage != m_nCurPage )
	{
		if( m_hNotify && ::IsWindow( m_hNotify ) )
			::SendMessage( m_hNotify, WM_MSG_PAGE_CHANGE, m_nCurPage, NULL );

		if( m_hCurBtn )
			SetCursor( m_hCurBtn );
		Invalidate();
	}

	return TRUE;
}

int	HitTest( CPoint point )
{
	return 0;
}

int CPageCtrl::HitTestBtn( CPoint point )
{
	if( m_rcFirstLink.PtInRect( point )  && m_nCurPage != 1 )
		return en_FIRST_LINK;
	else if ( m_rcPreLink.PtInRect( point ) && m_nCurPage != 1 )
		return en_PRE_LINK;
	else if ( m_rcNextLink.PtInRect(point) && m_nCurPage != m_nMaxPage )
		return en_NEXT_LINK;
	else if ( m_rcPreBtn.PtInRect(point) && m_nCurPage != 1)
		return en_PRE_BTN;
	else if ( m_rcNextBtn.PtInRect( point ) && m_nCurPage != m_nMaxPage)
		return en_NEXT_BTN;

	return en_NONE;
}

int CPageCtrl::HitTestNum( CPoint point )
{

	int nNum = 0;

	for ( int i = 0; i < m_vecHitTest.size(); i++ )
	{
		if( m_vecHitTest[i].rect.PtInRect(point) )
		{
			nNum = m_vecHitTest[i].nPage;
			break;
		}
	}

	return nNum;
}
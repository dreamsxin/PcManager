////////////////////////////////////////////////////////////////////////////////
//      
//      File for kws
//      
//      File      : headerctrl.h
//      Version   : 1.0
//      Comment   : 可以自定义高度headerctrl
//      
//      Create at : 2009-09-29
//		Create by : chenguicheng  
//      
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <atlctrls.h>
#include <vector>


inline void DrawBmp( HDC hdc, CRect& rect, HBITMAP hbmp )
{
	CBitmapHandle hBmp( hbmp );
	CDC ComDC;
	ComDC.CreateCompatibleDC( hdc );
	BITMAP bmp;
	hBmp.GetBitmap( &bmp );

	HBITMAP hOldBmp = ComDC.SelectBitmap( hBmp );

	if ( bmp.bmWidth == rect.Width() && bmp.bmHeight == rect.Height() )
	{
		::BitBlt( hdc,
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			ComDC,
			0,
			0,
			SRCCOPY );
	}
	else
	{
		::StretchBlt(   hdc,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			ComDC,
			0,
			0,
			bmp.bmWidth,
			bmp.bmHeight,
			SRCCOPY );
	}

	ComDC.SelectBitmap( hOldBmp );
	ComDC.DeleteDC();
}


typedef struct tagHeaderItem 
{
	ATL::CString	strText;
	int				nFlag;
} HeaderItem;

class KHeaderCtrl : public CWindowImpl< KHeaderCtrl >
{
public:

	KHeaderCtrl( )
		:m_nHeight(20)
	{
		m_clrHeader1 = RGB(0x46, 0xD1, 0x20);
		m_clrHeader2 = RGB(0xD7, 0xF6, 0xCC);
		m_clrText	 = RGB(0,0,0);
	};

	~KHeaderCtrl()
	{
		m_vecHDItem.clear();
	};

	BEGIN_MSG_MAP(KHeaderCtrl)
		MESSAGE_HANDLER(HDM_LAYOUT ,	OnLayOut)
		MESSAGE_HANDLER(WM_PAINT,		OnPaint)
	END_MSG_MAP()

	LRESULT OnLayOut(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		DefWindowProc( uMsg, wParam, lParam );
		HD_LAYOUT &hdl = *( HD_LAYOUT* )lParam; 
		RECT *prc = hdl.prc; 
		WINDOWPOS *pwpos = hdl.pwpos; 

		pwpos->cy = m_nHeight;
		prc->top = m_nHeight;
		Invalidate();

		return TRUE;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int i = 0;

		CPaintDC dc(m_hWnd);

		ATLASSERT(::IsWindow(m_hWnd));
		int nItemCnt = (int)::SendMessage(m_hWnd, HDM_GETITEMCOUNT, 0, 0L);

		CRect ItemRect;
		for( int i = 0; i < nItemCnt; i++ )
		{
			BOOL bRet = ::SendMessage(m_hWnd, HDM_GETITEMRECT, i, (LPARAM)&ItemRect);
			if ( !bRet )
				continue;

			if ( !m_bmpHeaderBg.IsNull())
			{
				DrawBmp( dc, ItemRect, m_bmpHeaderBg );

				CPen pen,pen2;
				HPEN hOldPen;
				pen.CreatePen( PS_SOLID, 1, m_clrHeader1 );
				hOldPen = dc.SelectPen( pen );
				dc.MoveTo( ItemRect.right - 2, ItemRect.top + 3 );
				dc.LineTo( ItemRect.right - 2, ItemRect.bottom - 3 );
				dc.SelectPen( hOldPen );
			}
			else
			{
				FillGradientLine( dc, ItemRect.left, 
					ItemRect.top, 
					ItemRect.right,
					ItemRect.bottom, 
					RGB(192, 192, 192));

				CPen pen;
				HPEN hOldPen;
				pen.CreatePen( PS_SOLID, 2, RGB(128,128, 128) );
				hOldPen = dc.SelectPen( pen );
				dc.MoveTo( ItemRect.right, ItemRect.top +3 );
				dc.LineTo( ItemRect.right, ItemRect.bottom - 3 );
				dc.SelectPen( hOldPen );
			}

			if ( i < m_vecHDItem.size() )
			{
				HeaderItem headerItem = m_vecHDItem[i];
				
				HFONT hOldFont = NULL;
				if( !m_font.IsNull())
					hOldFont = dc.SelectFont( m_font );

				dc.SetBkMode( TRANSPARENT );
				COLORREF clrOld = dc.SetTextColor(m_clrText);
				dc.DrawText( (LPTSTR)(LPCTSTR)headerItem.strText, 
					headerItem.strText.GetLength(), 
					&ItemRect, 
					headerItem.nFlag | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS 
					);

				if( hOldFont)
					dc.SelectFont( hOldFont );
				dc.SetTextColor(clrOld);
			}		
		}

		//绘制非Item部分
		CRect rtClient;
		GetClientRect( &rtClient );
		rtClient.left = ItemRect.right;
		if ( !m_bmpHeaderBg.IsNull())
		{
			DrawBmp( dc, rtClient, m_bmpHeaderBg );
		}
		else
		{
			FillGradientLine( dc, rtClient.left, 
				rtClient.top, 
				rtClient.right,
				rtClient.bottom, 
				RGB(192, 192, 192));
		}

		rtClient.left = 0;
		CBrush brush( ::CreateSolidBrush( RGB(0xFF, 0xFF, 0xFF)) );
		dc.FrameRect( &rtClient, brush );

		return TRUE;
	}

	void SetHeaderHeight( int nHeight )
	{
		m_nHeight = nHeight;
	}

	void SetHeaderBg( HBITMAP hbmp, COLORREF clr1, COLORREF clr2 )
	{
		m_bmpHeaderBg.Attach( hbmp );
		m_clrHeader1 = clr1;
		m_clrHeader2 = clr2;
	}

	void SetHeaderTextColor(COLORREF clr)
	{
		m_clrText = clr;
	}
	void AddHDItem( HeaderItem& item )
	{
		m_vecHDItem.push_back( item );
	}

	size_t GetItemSize()
	{
		return m_vecHDItem.size();
	}

	void GetItemSize( HDC dc, int nIndex, LPSIZE pSize )
	{
		::GetTextExtentPoint32( dc, 
			m_vecHDItem[nIndex].strText, 
			m_vecHDItem[nIndex].strText.GetLength(),
			pSize );
	}

	void SetHeaderFont( HFONT hFont )
	{
		m_font.Attach( hFont );
	}

protected:

	int								m_nHeight;
	CBitmap							m_bmpHeaderBg;
	COLORREF						m_clrHeader1;
	COLORREF						m_clrHeader2;
	COLORREF						m_clrText;
	std::vector< HeaderItem >		m_vecHDItem;
	CFontHandle						m_font;

private:

	void FillGradientLine( HDC hDC, int nPosX, int nPosY, int right ,int nBottom, 
		COLORREF clrStart, COLORREF clrEnd = RGB(0xFF,0xFF,0xFF),
		DWORD dwMode = GRADIENT_FILL_RECT_V)
	{
		static TRIVERTEX        vert[2] ;
		static GRADIENT_RECT    gRect;

		vert [0] .x      = nPosX;
		vert [0] .y      = nPosY;
		vert [0] .Red    = MAKEWORD(0x00, GetRValue( clrEnd ));
		vert [0] .Green  = MAKEWORD(0x00, GetGValue( clrEnd ));
		vert [0] .Blue   = MAKEWORD(0x00, GetBValue( clrEnd ));
		vert [0] .Alpha  = 0x0000;

		vert [1] .x      = right;
		vert [1] .y      = nBottom; 
		vert [1] .Red    = MAKEWORD(0x00, GetRValue( clrStart ));
		vert [1] .Green  = MAKEWORD(0x00, GetGValue( clrStart ));
		vert [1] .Blue   = MAKEWORD(0x00, GetBValue( clrStart ));
		vert [1] .Alpha  = 0x0000;

		gRect.UpperLeft  = 1;
		gRect.LowerRight = 0;
		::GradientFill( hDC, vert, 2, &gRect, 1, dwMode );
	}
};
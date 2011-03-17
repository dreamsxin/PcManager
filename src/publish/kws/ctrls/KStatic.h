#ifndef _K_STATIC_H_
#define _K_STATIC_H_
#pragma once
#include <string>


class KStatic : public CWindowImpl<KStatic, CWindow> 
{
public:
	KStatic()
	{
		m_crText        = GetSysColor(COLOR_BTNTEXT);
		m_crBack        = GetSysColor(COLOR_BTNFACE);
		m_bTransparent  = false;
		m_bBackGround	= false;

		LOGFONT lf;

		::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf);

		_tcscpy( lf.lfFaceName, TEXT("ËÎÌו") );
		CreateFont(lf, false);  
	}

	~KStatic()
	{
		if (m_Font.m_hFont != NULL)
		{
			m_Font.DeleteObject();  
		}
	}

	void SubclassWindow(HWND hWnd)
	{
		CWindowImpl<KStatic, CWindow>::SubclassWindow(hWnd);
		if (::IsWindow(hWnd))
		{
			TCHAR szTmp[MAX_PATH] = _T("");
			GetWindowText(szTmp, MAX_PATH);
			m_Text = szTmp;
			//SetWindowText(_T(""));
		}
	}

	void SetBackColor(const COLORREF crBack, const bool bRedraw)
	{
		m_crBack = crBack;

		if (bRedraw)
		{
			RedrawWindow();
		}
	}

	void SetTextColor(const COLORREF crText, const bool bRedraw)
	{
		m_crText = crText;

		if (bRedraw)
		{
			RedrawWindow();
		}
	}

	void SetText(LPCTSTR Text, const bool bRedraw = true)
	{
		m_Text = Text;  

		if (bRedraw)
		{
			if (!m_bBackGround)
			{
				CRect rc; 
				GetClientRect(rc);  
				CWindow wndParent = GetParent();

				ClientToScreen(&rc);
				wndParent.ScreenToClient(&rc);
				wndParent.InvalidateRect(&rc);
				wndParent.UpdateWindow();
			}

			RedrawWindow();
		}
	}

	void SetText(int nID, const bool bRedraw = true)
	{
		CString strText;
		strText.LoadString(nID);

		SetText(strText, bRedraw);
	}

	void SetFontName(LPCTSTR FontName, const bool bRedraw)
	{
		LOGFONT lf;

		m_Font.GetLogFont(&lf);

		lstrcpy(lf.lfFaceName, FontName);

		lf.lfCharSet = DEFAULT_CHARSET;

		CreateFont(lf, bRedraw);
	}

	void SetFontSize(const int nSize, const bool bRedraw)
	{
		if (nSize > 0)
		{
			LOGFONT lf;

			m_Font.GetLogFont(&lf);

			HDC hDC         = GetDC();
			int nLogPixY    = GetDeviceCaps(hDC, LOGPIXELSY);
			ReleaseDC(hDC);

			lf.lfHeight = -MulDiv(nSize, nLogPixY, 72); //Standard conversion

			CreateFont(lf, bRedraw);
		}
	}

	void SetFontPx(const int nPx, const bool bRedraw)
	{
		if (nPx > 0)
		{
			LOGFONT lf;

			m_Font.GetLogFont(&lf);


			lf.lfHeight = -nPx;

			CreateFont(lf, bRedraw);
		}
	}

	void SetBold(const bool bBold, const bool bRedraw)
	{
		LOGFONT lf;

		m_Font.GetLogFont(&lf);

		lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

		CreateFont(lf, bRedraw);
	}

	void SetUnderline(const bool bUnderline, const bool bRedraw)
	{
		LOGFONT lf;

		m_Font.GetLogFont(&lf);

		lf.lfUnderline = bUnderline;

		CreateFont(lf, bRedraw);
	}

	void SetItalic(const bool bItalic, const bool bRedraw)
	{
		LOGFONT lf;

		m_Font.GetLogFont(&lf);

		lf.lfItalic = bItalic;

		CreateFont(lf, bRedraw);
	}

	void SetTransparent(const bool bTransparent, const bool bRedraw)
	{
		m_bTransparent      = bTransparent;
		DWORD   dwStyle     = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		DWORD   dwNewStyle  = bTransparent ? dwStyle | WS_EX_TRANSPARENT : dwStyle & ~WS_EX_TRANSPARENT;

		::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewStyle);   

		if (bRedraw)
		{
			RedrawWindow();
		}
	}

	void SetBackGround(HBITMAP hBitmap, CPoint ptBmp)
	{
		m_bmpBackGround = hBitmap;
		m_ptBmp = ptBmp;
		m_bBackGround = true;
	}

	HFONT GetFont() const
	{
		return m_Font;
	}

protected:
	bool CreateFont(const LOGFONT& lf, const bool bRedraw)
	{
		if (m_Font.m_hFont != NULL)
		{
			m_Font.DeleteObject();
		}

		bool bCreated = (m_Font.CreateFontIndirect(&lf) != NULL);

		ATLASSERT(bCreated);

		if (bRedraw)
		{
			RedrawWindow();
		}

		return bCreated;
	}

	BEGIN_MSG_MAP(KStatic)	
		MESSAGE_HANDLER(WM_PAINT,		OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND,	OnEraseBackground)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		ATLASSERT(m_hWnd);

		CRect       rc; 
		CPaintDC    dcMem(m_hWnd);  
		int         nStrLen = m_Text.GetLength();

		GetClientRect(rc);  

		if (!m_bTransparent)
		{       
			::SetBkColor(dcMem, m_crBack);
			BOOL bResult = ::ExtTextOut(dcMem, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
			ATLASSERT(bResult);
		}
		else if (m_bBackGround)
		{
			CDC dcBg = CreateCompatibleDC( dcMem );
			HBITMAP hOldBmp = dcBg.SelectBitmap(m_bmpBackGround);
			dcMem.BitBlt( 0, 0, rc.Width(), rc.Height(), dcBg, m_ptBmp.x, m_ptBmp.y, SRCCOPY);
			dcBg.SelectBitmap(hOldBmp);
		}

		if (nStrLen > 0)
		{
			DWORD   dwFlags = DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_PATH_ELLIPSIS | DT_NOPREFIX;
			DWORD   dwStyle = GetStyle();

			if (dwStyle & SS_RIGHT)
			{
				dwFlags = DT_RIGHT;
			}
			else if (dwStyle & SS_CENTER)
			{
				dwFlags = DT_CENTER;
			}

			if (dwStyle & SS_CENTERIMAGE)
			{
				dwFlags |= DT_VCENTER;
				dwFlags |= DT_SINGLELINE; 
			}       

			COLORREF    crOldBk     = 0xFFFFFFFF;
			UINT        nOldMode    = -1;

			if (!m_bTransparent)
			{   
				crOldBk = dcMem.SetBkColor(m_crBack);
			}
			else
			{
				nOldMode = dcMem.SetBkMode(TRANSPARENT);                
			}
			COLORREF    crOldText   = dcMem.SetTextColor(m_crText);
			HFONT       hOldFont    = dcMem.SelectFont(m_Font.m_hFont);         

			dcMem.DrawText(m_Text, nStrLen, rc, dwFlags);   

			dcMem.SelectFont(hOldFont);
			dcMem.SetTextColor(crOldText);  

			if (!m_bTransparent)
			{
				dcMem.SetBkColor(crOldBk);
			}
			else
			{
				dcMem.SetBkMode(nOldMode);
			}
		}


		return TRUE;
	}

	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;
		return bHandled;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (m_Font.m_hFont != NULL)
		{
			m_Font.DeleteObject();  
		}
		return 0;
	}

	bool		m_bTransparent;
	COLORREF	m_crText;	
	COLORREF	m_crBack;
	CFont		m_Font;
	CString		m_Text;
	bool		m_bBackGround;
	CBitmap		m_bmpBackGround;
	CPoint		m_ptBmp;
};


#endif	// #ifndef _K_STATIC_H_

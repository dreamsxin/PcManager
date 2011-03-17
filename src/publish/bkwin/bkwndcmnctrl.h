//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndcmnctrl.h
// Description: BkWindow Definition
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkwnd.h"
#include "bkimage.h"
#include "bktheme.h"
#include "bkwndnotify.h"




//////////////////////////////////////////////////////////////////////////
// Memory gdiplus Image Draw
//
// Usage: <memimage mempointer=xx />
//
class CBkMemoryImage : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkMemoryImage, "memimage")

		CBkMemoryImage()
	{
		m_gdiPlusImage = NULL;
		m_nAutoSize = 1;
	}
public:

	// Do nothing
	void OnPaint(CDCHandle dc)
	{
		if (m_gdiPlusImage==NULL)
			return;


		if (1 == m_nAutoSize)
		{
			Gdiplus::Graphics	gh(dc.m_hDC);
			gh.DrawImage( m_gdiPlusImage, Gdiplus::Rect(m_rcWindow.left,m_rcWindow.top,m_rcWindow.Width(),m_rcWindow.Height()),
				0,0, m_gdiPlusImage->GetWidth(), m_gdiPlusImage->GetHeight(), Gdiplus::UnitPixel );
		}else if (0 == m_nAutoSize)
		{
			Gdiplus::Graphics	gh(dc.m_hDC);
			int nWidth = m_gdiPlusImage->GetWidth(),
				nHeight = m_gdiPlusImage->GetHeight();

			gh.DrawImage(m_gdiPlusImage, m_rcWindow.left,m_rcWindow.top);
		}

	}

	HRESULT OnAttributeChange(CStringA& strValue, BOOL bLoading)
	{
		m_gdiPlusImage = (Gdiplus::Image*)IntToPtr(atoi( strValue ));
		//m_hIcon = (HICON)IntToPtr(atoi(strValue));
		return S_OK;
	}

	HRESULT OnDrawImageSize(CStringA& strValue, BOOL bLoading)
	{
		if (FALSE == strValue.IsEmpty())
			m_nAutoSize = StrToIntA(strValue);
		return S_OK;
	}

protected:
	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		BKWIN_END_MSG_MAP()

		BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_CUSTOM_ATTRIBUTE("mempointer", OnAttributeChange)
		BKWIN_CUSTOM_ATTRIBUTE("autosize",   OnDrawImageSize)
		BKWIN_DECLARE_ATTRIBUTES_END()

protected:
	Gdiplus::Image*	m_gdiPlusImage;
	int			m_nAutoSize;//是否根据给定的矩形区域自适应大小，默认自适应，0表示根据IMAGE的实际大小绘制
};

//////////////////////////////////////////////////////////////////////////
// Spacing Control
// Only leave a space, faster than "div" (... a little)
//
// Usage: <spacing width=xx />
//
class CBkSpacing : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkSpacing, "spacing")

public:

	// Do nothing
	void OnPaint(CDCHandle dc)
	{
	}

protected:
	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// New line
// Panel control process return thing, so define a blank class
//
// Usage: <br />
//
class CBkReturn : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkReturn, "br")
};

//////////////////////////////////////////////////////////////////////////
// Static Control
// 
// Usage: <text>inner text example</text>
//
class CBkStatic : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkStatic, "text")
};

//////////////////////////////////////////////////////////////////////////
// Link Control
// Only For Header Drag Test
// Usage: <link>inner text example</link>
//
class CBkLink : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkLink, "link")
};

//////////////////////////////////////////////////////////////////////////
// Button Control
// Use id attribute to process click event
// 
// Usage: <button id=xx>inner text example</button>
//
class CBkButton : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkButton, "button")
public:

	virtual BOOL NeedRedrawWhenStateChange()
	{
		return TRUE;
	}
};

//////////////////////////////////////////////////////////////////////////
// Image Control
// Use src attribute specify a resource id
// 
// Usage: <img src=xx />
//
class CBkImageWnd : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkImageWnd, "img")
public:
	CBkImageWnd()
		: m_nSubImageID(-1)
		, m_pSkin(NULL)
	{

	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (!CBkWindow::Load(pTiXmlElem))
			return FALSE;
		// 
		//         m_pSkin = BkSkin::GetSkin(m_strSkin);
		//         if (m_pSkin && m_pSkin->IsClass(CBkImageSkin::GetClassName()))
		//             return TRUE;
		// 
		// //         if (m_imgSrc.M_HOBJECT)
		// //             return TRUE;

		return TRUE;
	}

	void OnPaint(CDCHandle dc)
	{
		if (m_pSkin)
			m_pSkin->Draw(dc, m_rcWindow, m_nSubImageID);
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		__super::OnNcCalcSize(bCalcValidRects, lParam);

		LPSIZE pSize = (LPSIZE)lParam;

		if (m_pSkin)
		{
			SIZE sizeImage;
			sizeImage = m_pSkin->GetSkinSize();

			if (sizeImage.cx)
				pSize->cx = sizeImage.cx;
			if (sizeImage.cy)
				pSize->cy = sizeImage.cy;

			return TRUE;
		}

		return 0;
	}

protected:
	//     CBkImageSkin m_imgSrc;
	//     int m_nSubImageWidth;
	int m_nSubImageID;
	CBkSkinBase *m_pSkin;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, TRUE)
		//         BKWIN_CHAIN_ATTRIBUTE(m_imgSrc, TRUE)
		//         BKWIN_UINT_ATTRIBUTE("src", m_imgSrc, TRUE)
		//         BKWIN_INT_ATTRIBUTE("subwidth", m_nSubImageWidth, TRUE)
		BKWIN_INT_ATTRIBUTE("sub", m_nSubImageID, FALSE)
		BKWIN_DECLARE_ATTRIBUTES_END()

		BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Progress Control
// Use id attribute to process click event
// 
// Usage: <button id=xx>inner text example</button>
//
class CBkProgress : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkProgress, "progress")
public:
	CBkProgress()
		: m_dwMinValue(0)
		, m_dwMaxValue(0)
		, m_dwValue(0)
		, m_bShowPercent(FALSE)
		, m_pSkinBg(NULL)
		, m_pSkinPos(NULL)
	{

	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (!CBkWindow::Load(pTiXmlElem))
			return FALSE;

		//         CBkSkinBase *pSkin = NULL;
		//         pSkin = BkSkin::GetSkin(m_strSkinBg);
		// 
		//         if (pSkin)
		//         {
		//             if (pSkin->IsClass(CBkSkinImgHorzExtend::GetClassName()))
		//                 m_pSkinBg = (CBkSkinImgHorzExtend *)pSkin;
		//             else
		//                 m_pSkinBg = NULL;
		//         }
		// 
		//         pSkin = BkSkin::GetSkin(m_strSkinPos);
		// 
		//         if (pSkin)
		//         {
		//             if (pSkin->IsClass(CBkSkinImgHorzExtend::GetClassName()))
		//                 m_pSkinPos = (CBkSkinImgHorzExtend *)pSkin;
		//             else
		//                 m_pSkinPos = NULL;
		//         }
		// 
		return TRUE;
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		__super::OnNcCalcSize(bCalcValidRects, lParam);

		LPSIZE pSize = (LPSIZE)lParam;

		if (m_pSkinBg)
		{
			SIZE sizeBg = m_pSkinBg->GetSkinSize();

			pSize->cy = sizeBg.cy;

			return TRUE;
		}

		return TRUE;
	}

	void OnPaint(CDCHandle dc)
	{
		BkDCPaint bkdc;
		CPen penFrame;
		CString strPercent;
		double dPos = 0;

		BeforePaint(dc, bkdc);

		penFrame.CreatePen(
			PS_SOLID, 
			1, 
			RGB(0x70, 0x70, 0x70)
			);

		HPEN hpenOld = dc.SelectPen(penFrame);

		if (m_pSkinBg)
		{
			m_pSkinBg->Draw(dc, m_rcWindow, BkWndState_Normal);
		}
		else
		{
			dc.RoundRect(m_rcWindow, CPoint(2, 2));
		}

		if (m_dwMaxValue == m_dwMinValue)
		{
			dPos = 0;
		}
		else
		{
			if (m_dwMaxValue < m_dwMinValue)
			{
				DWORD dwTemp = m_dwMaxValue;
				m_dwMaxValue = m_dwMinValue;
				m_dwMinValue = dwTemp;
			}

			if (m_dwValue < m_dwMinValue)
			{
				m_dwValue = m_dwMinValue;
			}
			else if (m_dwValue > m_dwMaxValue)
			{
				m_dwValue = m_dwMaxValue;
			}

			dPos = (double)(m_dwValue - m_dwMinValue) / (double)(m_dwMaxValue - m_dwMinValue);
		}

		if (m_bShowPercent)
		{
			strPercent.Format(_T("%d%%"), (int)(dPos * 100));
			dc.DrawText(strPercent, strPercent.GetLength(), m_rcWindow, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

		CRect rcPosBar = m_rcWindow;

		if (m_pSkinPos)
		{
			SIZE sizePosBar = m_pSkinPos->GetSkinSize();

			rcPosBar.DeflateRect(2, (m_rcWindow.Height() - sizePosBar.cy) / 2);
			rcPosBar.right = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos);

			m_pSkinPos->Draw(dc, rcPosBar, BkWndState_Normal);
		}
		else
		{
			rcPosBar.DeflateRect(2, 2);
			rcPosBar.right = rcPosBar.left + (LONG)(rcPosBar.Width() * dPos);

			dc.RoundRect(rcPosBar, CPoint(2, 2));

			dc.FillSolidRect(rcPosBar, RGB(66, 97, 144));
		}

		dc.SelectPen(hpenOld);

		if (m_bShowPercent)
		{
			CRgn rgnClipOld, rgnClip;
			rgnClip.CreateRectRgnIndirect(rcPosBar);
			dc.GetClipRgn(rgnClipOld);
			dc.SelectClipRgn(rgnClip);

			if (CLR_INVALID == m_style.m_crBg)
				dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
			else
				dc.SetTextColor(m_style.m_crBg);

			dc.DrawText(strPercent, strPercent.GetLength(), m_rcWindow, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			dc.SetTextColor(m_style.m_crText);
			dc.SelectClipRgn(rgnClipOld);
		}

		AfterPaint(dc, bkdc);
	}

protected:
	DWORD m_dwMinValue;
	DWORD m_dwMaxValue;
	DWORD m_dwValue;
	BOOL m_bShowPercent;
	//     CStringA m_strSkinBg;
	//     CStringA m_strSkinPos;
	CBkSkinBase *m_pSkinBg;
	CBkSkinBase *m_pSkinPos;

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		BKWIN_END_MSG_MAP()

		BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_SKIN_ATTRIBUTE("bgskin", m_pSkinBg, TRUE)
		BKWIN_SKIN_ATTRIBUTE("posskin", m_pSkinPos, TRUE)
		//         BKWIN_STRING_ATTRIBUTE("bgskin", m_strSkinBg, TRUE)
		//         BKWIN_STRING_ATTRIBUTE("posskin", m_strSkinPos, TRUE)
		BKWIN_DWORD_ATTRIBUTE("min", m_dwMinValue, FALSE)
		BKWIN_DWORD_ATTRIBUTE("max", m_dwMaxValue, FALSE)
		BKWIN_DWORD_ATTRIBUTE("value", m_dwValue, FALSE)
		BKWIN_UINT_ATTRIBUTE("showpercent", m_bShowPercent, FALSE)
		BKWIN_DECLARE_ATTRIBUTES_END()
};

//////////////////////////////////////////////////////////////////////////
// Image Button Control
// 
// Usage: <imgbtn src=xx />
//
class CBkImageBtnWnd : public CBkImageWnd
{
	BKOBJ_DECLARE_CLASS_NAME(CBkImageBtnWnd, "imgbtn")
public:
	CBkImageBtnWnd()
	{

	}

	virtual BOOL NeedRedrawWhenStateChange()
	{
		return TRUE;
	}

	//     virtual BOOL Load(TiXmlElement* pTiXmlElem)
	//     {
	//         if (!CBkWindow::Load(pTiXmlElem))
	//             return FALSE;
	// 
	//         if (m_imgSrc.M_HOBJECT)
	//             return TRUE;
	// 
	//         return FALSE;
	//     }

	void OnPaint(CDCHandle dc)
	{
		//DWORD dwState = GetState();

		if (m_pSkin)
		{
			m_pSkin->Draw(
				dc, m_rcWindow, 
				IIF_STATE4(GetState(), 0, 1, 2, 3)
				);
		}
	}

	//     LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	//     {
	//         LPSIZE pSize = (LPSIZE)lParam;
	// 
	//         if (m_imgSrc.M_HOBJECT)
	//         {
	//             SIZE sizeImage;
	// 
	//             m_imgSrc.GetImageSize(sizeImage);
	// 
	//             pSize->cx = (m_imgSrc.GetSubImageWidth() != 0) ? m_imgSrc.GetSubImageWidth() : sizeImage.cx;
	//             pSize->cy = sizeImage.cy;
	// 
	//             return TRUE;
	//         }
	// 
	//         return FALSE;
	//     }

protected:

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		//         MSG_WM_NCCALCSIZE(OnNcCalcSize)
		BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Real Window Control
// Binding a real window
// 
// Usage: <realwnd id=xx />
//
class CBkRealWnd : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkRealWnd, "realwnd")
public:
	CBkRealWnd()
		//         : m_uRealWinDlgResID(0)
	{

	}

	virtual BOOL NeedRedrawWhenStateChange()
	{
		return FALSE;
	}

	void ShowRealWindow(BOOL bShow)
	{
		::ShowWindow(_GetRealWindow(), bShow ? SW_SHOW : SW_HIDE);
	}

	void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
	{
		CRect rcOldWnd = m_rcWindow;

		__super::OnWindowPosChanged(lpWndPos);

		if (rcOldWnd != m_rcWindow)
		{
			HWND hWndReal = _GetRealWindow();
			::SetWindowPos(hWndReal, HWND_TOP, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(), SWP_NOREDRAW);

			BKNMREALWNDRESIZED nms;
			nms.hdr.code = BKNM_REALWND_RESIZED;
			nms.hdr.hwndFrom = m_hWndContainer;
			nms.hdr.idFrom = ::GetDlgCtrlID(m_hWndContainer);
			nms.uItemID = GetCmdID();
			nms.rcWnd = m_rcWindow;

			LRESULT lRet = ::SendMessage(m_hWndContainer, WM_NOTIFY, (LPARAM)nms.hdr.idFrom, (WPARAM)&nms);

			if (::IsWindowVisible(hWndReal))
			{
				CRect rcUpdate = m_rcWindow;
				rcUpdate.MoveToXY(0, 0);
				//::RedrawWindow(_GetRealWindow(), rcUpdate, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				//                 ::RedrawWindow(hWndReal, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW);
				::InvalidateRect(hWndReal, NULL, TRUE);
				::SetWindowPos(hWndReal, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
			}
		}
	}

	void OnShowWindow(BOOL bShow, UINT nStatus)
	{
		__super::OnShowWindow(bShow, nStatus);

		if (bShow)
		{
			// 如果父窗口或者父窗口的父窗口隐藏（主要是由于Tab切换），则不显示真实窗口
			CBkWindow* pWnd = this;

			while (TRUE)
			{
				pWnd = BkWnds::GetWindow(pWnd->GetParent());

				if (NULL == pWnd)
					break;

				if (!pWnd->IsVisible())
					return;
			}
		}

		ShowRealWindow(bShow);
	}

	void OnDestroy()
	{
		HWND hWndReal = _GetRealWindow();
		if (hWndReal && ::IsWindow(hWndReal))
			::ShowWindow(hWndReal, SW_HIDE);
	}

	// Do nothing
	void OnPaint(CDCHandle dc)
	{
	}

protected:

	HWND _GetRealWindow()
	{
		return ::GetDlgItem(m_hWndContainer, m_uCmdID);
	}

	//     UINT m_uRealWinDlgResID;

	//     BKWIN_DECLARE_ATTRIBUTES_BEGIN()
	//         BKWIN_UINT_ATTRIBUTE("ctrlid", m_uRealWinDlgResID, FALSE)
	//     BKWIN_DECLARE_ATTRIBUTES_END()

	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Line Control
// Simple HTML "HR" tag
//
// Usage: <hr style=solid size=1 crbg=.../>
//
class CBkLine : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkLine, "hr")

public:

	CBkLine()
		: m_nPenStyle(PS_SOLID)
		, m_nLineSize(1)
	{
	}

	// Do nothing
	void OnPaint(CDCHandle dc)
	{
		CPen pen;

		pen.CreatePen(m_nPenStyle, m_nLineSize, m_style.m_crBg);

		HPEN hpenOld = dc.SelectPen(pen);

		dc.MoveTo(m_rcWindow.left, m_rcWindow.top/* + m_rcWindow.Height() / 2*/);
		dc.LineTo(m_rcWindow.right, m_rcWindow.bottom/*top + m_rcWindow.Height() / 2*/);

		dc.SelectPen(hpenOld);
	}

protected:
	int m_nPenStyle;
	int m_nLineSize;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_INT_ATTRIBUTE("size", m_nLineSize, FALSE)
		BKWIN_ENUM_ATTRIBUTE("style", int, FALSE)
		BKWIN_ENUM_VALUE("solid", PS_SOLID)             // default
		BKWIN_ENUM_VALUE("dash", PS_DASH)               /* -------  */ 
		BKWIN_ENUM_VALUE("dot", PS_DOT)                 /* .......  */ 
		BKWIN_ENUM_VALUE("dashdot", PS_DASHDOT)         /* _._._._  */ 
		BKWIN_ENUM_VALUE("dashdotdot", PS_DASHDOTDOT)   /* _.._.._  */ 
		BKWIN_ENUM_END(m_nPenStyle)
		BKWIN_DECLARE_ATTRIBUTES_END()

		BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Check Box
//
// Usage: <check state=1>This is a check-box</check>
//
class CBkCheckBox : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkCheckBox, "check")

		enum {
			CheckBoxSize = 13,
			CheckBoxSpacing = 4,
	};

public:

	CBkCheckBox()
		: m_theme()
	{
	}

	void OnPaint(CDCHandle dc)
	{
		CRect rcCheckBox = m_rcWindow;
		rcCheckBox.right = rcCheckBox.left + CheckBoxSize;

		m_theme.OpenTheme(m_hWndContainer);

		if (m_theme.IsValid())
		{
			m_theme.DrawBackground(dc, _GetDrawState(), rcCheckBox);
		}
		else
		{
			dc.DrawFrameControl(rcCheckBox, DFC_BUTTON, _GetDrawState());
		}

		m_rcWindow.left += CheckBoxSize + CheckBoxSpacing;

		m_style.m_nTextAlign &= ~DT_BOTTOM;
		m_style.m_nTextAlign |= DT_VCENTER;

		__super::OnPaint(dc);

		m_rcWindow.left -= CheckBoxSize + CheckBoxSpacing;
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		LPSIZE pSize = (LPSIZE)lParam;

		__super::OnNcCalcSize(bCalcValidRects, lParam);

		pSize->cx += CheckBoxSize + CheckBoxSpacing;
		pSize->cy = max(pSize->cy, CheckBoxSize);

		return FALSE;
	}

protected:

	CBkCheckBoxTheme m_theme;

	int m_nState;

	UINT _GetDrawState()
	{
		DWORD dwState = GetState();
		UINT uState = m_theme.IsValid() ? 0 : DFCS_BUTTONCHECK | DFCS_TRANSPARENT;

		if (m_theme.IsValid())
		{
			if (dwState & BkWndState_Check)
			{
				if (dwState & BkWndState_Disable)
					uState = CBS_CHECKEDDISABLED;
				else if (dwState & BkWndState_PushDown)
					uState = CBS_CHECKEDPRESSED;
				else if (dwState & BkWndState_Hover)
					uState = CBS_CHECKEDHOT;
				else
					uState = CBS_CHECKEDNORMAL;
			}
			else
			{
				if (dwState & BkWndState_Disable)
					uState = CBS_UNCHECKEDDISABLED;
				else if (dwState & BkWndState_PushDown)
					uState = CBS_UNCHECKEDPRESSED;
				else if (dwState & BkWndState_Hover)
					uState = CBS_UNCHECKEDHOT;
				else
					uState = CBS_UNCHECKEDNORMAL;
			}
		}
		else
		{
			uState = DFCS_BUTTONCHECK | DFCS_TRANSPARENT;
			if (dwState & BkWndState_Disable)
				uState |= DFCS_INACTIVE;
			if (dwState & BkWndState_Hover)
				uState |= DFCS_HOT;
			if (dwState & BkWndState_PushDown)
				uState |= DFCS_PUSHED;
			if (dwState & BkWndState_Check)
				uState |= DFCS_CHECKED;
		}

		return uState;
	}

	virtual BOOL NeedRedrawWhenStateChange()
	{
		return TRUE;
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (IsChecked())
			ModifyState(0, BkWndState_Check);
		else
			ModifyState(BkWndState_Check, 0);
	}

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_UINT_ATTRIBUTE("check", m_nState, FALSE)
		BKWIN_DECLARE_ATTRIBUTES_END()

		BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Icon Control
// Use src attribute specify a resource id
// 
// Usage: <icon src=xx />
//
class CBkIconWnd : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkIconWnd, "icon")
public:
	CBkIconWnd()
		: m_uResID(-1)
		, m_nSize(16)
		, m_bOemIcon(FALSE)
	{

	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (!CBkWindow::Load(pTiXmlElem))
			return FALSE;

		_ReloadIcon();

		return TRUE;
	}

	void OnPaint(CDCHandle dc)
	{
		if (m_uResIDCurrent != m_uResID)
			_ReloadIcon();

		if (NULL == m_theIcon.m_hIcon)
		{
			if (m_hIcoAttach)
			{
				CIconHandle ico(m_hIcoAttach);

				ico.DrawIconEx(dc, m_rcWindow.TopLeft(), m_rcWindow.Size());
			}
		}
		else
			m_theIcon.DrawIconEx(dc, m_rcWindow.TopLeft(), m_rcWindow.Size());
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		LPSIZE pSize = (LPSIZE)lParam;

		pSize->cx = m_nSize;
		pSize->cy = m_nSize;

		return TRUE;
	}

	void AttachIcon(HICON hIcon)
	{
		m_hIcoAttach = hIcon;
	}

protected:

	void _ReloadIcon()
	{
		if (m_theIcon.m_hIcon)
			m_theIcon.DestroyIcon();

		if (m_bOemIcon)
			m_theIcon.LoadOEMIcon(MAKEINTRESOURCE(m_uResID));
		else
			m_theIcon.LoadIcon(MAKEINTRESOURCE(m_uResID), m_nSize, m_nSize);

		m_uResIDCurrent = m_uResID;
	}

	CIcon m_theIcon;
	UINT m_uResID;
	UINT m_uResIDCurrent;
	int m_nSize;
	BOOL m_bOemIcon;
	HICON m_hIcoAttach;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_UINT_ATTRIBUTE("src", m_uResID, FALSE)
		BKWIN_UINT_ATTRIBUTE("oem", m_bOemIcon, FALSE)
		BKWIN_INT_ATTRIBUTE("size", m_nSize, FALSE)
		BKWIN_DECLARE_ATTRIBUTES_END()

		BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// Radio Box
//
// Usage: <radio state=1>This is a check-box</radio>
//
class CBkRadioBox : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkRadioBox, "radio")

		enum {
			RadioBoxSize = 12,
			RadioBoxSpacing = 2,
	};

public:

	CBkRadioBox()
		: m_theme()
	{
	}

	~CBkRadioBox()
	{
		BkWnds::UnregisterRadioGroup(this, m_strGroup);
	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (!CBkWindow::Load(pTiXmlElem))
			return FALSE;

		return BkWnds::RegisterRadioGroup(this, m_strGroup);;
	}

	void OnPaint(CDCHandle dc)
	{
		CRect rcCheckBox(0, 0, RadioBoxSize, RadioBoxSize);

		rcCheckBox.MoveToXY(m_rcWindow.left, m_rcWindow.top + (m_rcWindow.Height() - RadioBoxSize) / 2);

		m_theme.OpenTheme(m_hWndContainer);

		if (m_theme.IsValid())
		{
			m_theme.DrawBackground(dc, _GetDrawState(), rcCheckBox);
		}
		else
		{
			dc.DrawFrameControl(rcCheckBox, DFC_BUTTON, _GetDrawState());
		}

		m_rcWindow.left += RadioBoxSize + RadioBoxSpacing;

		m_style.m_nTextAlign &= ~DT_BOTTOM;
		m_style.m_nTextAlign |= DT_VCENTER | DT_SINGLELINE;

		__super::OnPaint(dc);

		m_rcWindow.left -= RadioBoxSize + RadioBoxSpacing;
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		LPSIZE pSize = (LPSIZE)lParam;

		__super::OnNcCalcSize(bCalcValidRects, lParam);

		pSize->cx += RadioBoxSize + RadioBoxSpacing;
		pSize->cy = max(pSize->cy, RadioBoxSize);

		return FALSE;
	}

protected:

	CBkRadioBoxTheme m_theme;

	int m_nState;
	CStringA m_strGroup;

	UINT _GetDrawState()
	{
		DWORD dwState = GetState();
		UINT uState = m_theme.IsValid() ? 0 : DFCS_BUTTONRADIO | DFCS_TRANSPARENT;

		if (m_theme.IsValid())
		{
			if (dwState & BkWndState_Check)
			{
				if (dwState & BkWndState_Disable)
					uState = RBS_CHECKEDDISABLED;
				else if (dwState & BkWndState_PushDown)
					uState = RBS_CHECKEDPRESSED;
				else if (dwState & BkWndState_Hover)
					uState = RBS_CHECKEDHOT;
				else
					uState = RBS_CHECKEDNORMAL;
			}
			else
			{
				if (dwState & BkWndState_Disable)
					uState = RBS_UNCHECKEDDISABLED;
				else if (dwState & BkWndState_PushDown)
					uState = RBS_UNCHECKEDPRESSED;
				else if (dwState & BkWndState_Hover)
					uState = RBS_UNCHECKEDHOT;
				else
					uState = RBS_UNCHECKEDNORMAL;
			}
		}
		else
		{
			uState = DFCS_BUTTONRADIO | DFCS_TRANSPARENT;
			if (dwState & BkWndState_Disable)
				uState |= DFCS_INACTIVE;
			if (dwState & BkWndState_Hover)
				uState |= DFCS_HOT;
			if (dwState & BkWndState_PushDown)
				uState |= DFCS_PUSHED;
			if (dwState & BkWndState_Check)
				uState |= DFCS_CHECKED;
		}

		return uState;
	}

	virtual BOOL NeedRedrawWhenStateChange()
	{
		return TRUE;
	}

	void OnLButtonUp(UINT nFlags, CPoint point)
	{
		BkWnds::SelectRadio(this, m_strGroup, NULL);
		//ModifyState(BkWndState_Check, 0);
	}

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_UINT_ATTRIBUTE("check", m_nState, FALSE)
		BKWIN_STRING_ATTRIBUTE("group", m_strGroup, FALSE)
		BKWIN_DECLARE_ATTRIBUTES_END()

		BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		BKWIN_END_MSG_MAP()
};


//////////////////////////////////////////////////////////////////////////
// ImageListH Control add by zbl
// Use src attribute specify a resource id
// 
// Usage: <imglisth src=xx />
//
class CBkImageListHWnd : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkImageListHWnd, "imglisth")
public:
	CBkImageListHWnd()
		: m_nSubImageID(-1)
		, m_pSkin(NULL)
		, m_nImageCount(0)
	{
		m_mapSkin.RemoveAll();
		m_mapSkinSub.RemoveAll();
	}

	virtual BOOL Load(TiXmlElement* pTiXmlElem)
	{
		if (!CBkWindow::Load(pTiXmlElem))
			return FALSE;

		CStringA strSkin, strSkinValue;
		for (int i = 0; i < m_nImageCount; i++)
		{
			strSkin.Format("skin%d", i);
			m_mapSkin[strSkin] = NULL;
			//
			strSkin.Format("sub%d", i);
			m_mapSkinSub[strSkin] = -1;
		}

		for (TiXmlAttribute *pAttrib = pTiXmlElem->FirstAttribute(); NULL != pAttrib; pAttrib = pAttrib->Next())
		{
			strSkin = pAttrib->Name();
			if (0 == strSkin.CompareNoCase("skinlist"))
			{
				MapSkinList(pAttrib->Value());
			}else if (0 == strSkin.CompareNoCase("sublist"))
			{
				MapSkinSubList(pAttrib->Value());
			}

		}

		return TRUE;
	}

	void OnPaint(CDCHandle dc)
	{
		CRect rc = m_rcWindow;
		CStringA strKey = "";
		for (int i = 0; i < m_nImageCount; i++)
		{
			CBkSkinBase* pSkin = NULL;
			strKey.Format("skin%d", i);
			pSkin = m_mapSkin[strKey];
			if (NULL != pSkin)
			{
				strKey.Format("sub%d", i);
				pSkin->Draw(dc, rc, m_mapSkinSub[strKey]);
				SIZE size = pSkin->GetSkinSize();
				rc.OffsetRect(size.cx, 0);
			}
		}
	}

	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		__super::OnNcCalcSize(bCalcValidRects, lParam);

		LPSIZE pSize = (LPSIZE)lParam;

		if (m_pSkin)
		{
			SIZE sizeImage;
			sizeImage = m_pSkin->GetSkinSize();

			if (sizeImage.cx)
				pSize->cx = sizeImage.cx;
			if (sizeImage.cy)
				pSize->cy = sizeImage.cy;

			return TRUE;
		}

		return 0;
	}

	void InitSkinList(CStringA SkinList, CSimpleArray<CStringA>& arrList)
	{
		CStringA strList = SkinList, strTmp;
		arrList.RemoveAll();
		strList.Append(",");
		int nIndex = -1;
		nIndex = strList.Find(",");
		while(nIndex >= 0)
		{
			strTmp = strList.Left(nIndex);
			if (strTmp.GetLength() > 0)
				arrList.Add(strTmp);
			strList = strList.Mid(nIndex+1);
			nIndex = strList.Find(",");
		}
	}

	void MapSkinList(CStringA strList)
	{
		CSimpleArray<CStringA> arrList;
		InitSkinList(strList, arrList);
		CStringA strSkinValue;
		for (int i = 0; i < arrList.GetSize(); i++)
		{
			CStringA strSkin = arrList[i];
			int nIndex = strSkin.Find("=");
			if (nIndex > 0)
			{
				strSkin = strSkin.Left(nIndex);
				strSkinValue = arrList[i].Mid(nIndex+1);
				m_mapSkin[strSkin] = BkSkin::GetSkin(strSkinValue);
			}
		}
	}

	void MapSkinSubList(CStringA strList)
	{
		CSimpleArray<CStringA> arrList;
		InitSkinList(strList, arrList);
		CStringA strSkinValue;
		for (int i = 0; i < arrList.GetSize(); i++)
		{
			CStringA strSkin = arrList[i];
			int nIndex = strSkin.Find("=");
			if (nIndex > 0)
			{
				strSkin = strSkin.Left(nIndex);
				strSkinValue = arrList[i].Mid(nIndex+1);
				m_mapSkinSub[strSkin] = StrToIntA(strSkinValue);

			}
		}
	}
	HRESULT OnAttributeSkinListChange(CStringA& strValue, BOOL bLoading)
	{//skin0="img0",skin1="img1",skin2="img2"
		if (FALSE == strValue.IsEmpty())
		{
			m_strSkinList = strValue;
			MapSkinList(m_strSkinList);
		}

		return TRUE;
	}

	HRESULT OnAttributeSubSinListChange(CStringA& strValue, BOOL bLoading)
	{//skin0sub="0",skin1sub="1",skin2sub="2"
		if (FALSE == strValue.IsEmpty())
		{
			m_strSubList = strValue;
			
			MapSkinSubList(m_strSubList);
		}

		return TRUE;
	}

protected:
	int m_nSubImageID;
	CBkSkinBase *m_pSkin;
	//
	int m_nImageCount;
	CStringA m_strSkinList;
	CStringA m_strSubList;
	//CSimpleMap<CStringA, CBkSkinBase*>	m_mapSkin;
	CAtlMap<CStringA, int>				m_mapSkinSub;
	CAtlMap<CStringA, CBkSkinBase*>		m_mapSkin;

	BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_SKIN_ATTRIBUTE("skin", m_pSkin, TRUE)
		BKWIN_INT_ATTRIBUTE("sub", m_nSubImageID, FALSE)
		BKWIN_INT_ATTRIBUTE("imagecount", m_nImageCount, FALSE)
		BKWIN_CUSTOM_ATTRIBUTE("skinlist", OnAttributeSkinListChange)
		BKWIN_CUSTOM_ATTRIBUTE("sublist", OnAttributeSubSinListChange)
		BKWIN_DECLARE_ATTRIBUTES_END()

		BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		BKWIN_END_MSG_MAP()
};

//////////////////////////////////////////////////////////////////////////
// icon draw from file add by zbl
//
// Usage: <memimage mempointer=xx />
//
class CIconCache
{
public:
	static CIconCache* GetIconCachePtr()
	{
		static CIconCache cache;
		return &cache;
	}
	HICON GetIcon(CStringA strPath)
	{
		CAtlMap<CStringA, HICON>::CPair* ptr = m_mapIcon.Lookup(strPath);
		if (NULL != ptr)
			return ptr->m_value;

		if (TRUE == PathFileExistsA(strPath))
		{
// 			if (0 == strPath.Right(4).CompareNoCase(".ico"))
// 			{
// 				if (NULL != hIcon)
// 					m_mapIcon[strPath] = hIcon;
// 			}
// 			else
			{
				SHFILEINFOA fileInfo = {0};
				if (::SHGetFileInfoA(strPath, 0, &fileInfo, sizeof(fileInfo), SHGFI_ICON | SHGFI_LARGEICON |SHGFI_DISPLAYNAME) &&
					NULL != fileInfo.hIcon)
				{
					m_mapIcon[strPath] = fileInfo.hIcon;
					return fileInfo.hIcon;
				}
			}
		}
		return NULL;
	}

	HICON GetIconSmall(CStringA strPath)
	{
		CAtlMap<CStringA, HICON>::CPair* ptr = m_mapISmall.Lookup(strPath);
		if (NULL != ptr)
			return ptr->m_value;

		if (TRUE == PathFileExistsA(strPath))
		{
			// 			if (0 == strPath.Right(4).CompareNoCase(".ico"))
			// 			{
			// 				if (NULL != hIcon)
			// 					m_mapIcon[strPath] = hIcon;
			// 			}
			// 			else
			{
				SHFILEINFOA fileInfo = {0};
		
				if (::SHGetFileInfoA(strPath, 0, &fileInfo, sizeof(fileInfo), SHGFI_ICON | SHGFI_SMALLICON |SHGFI_DISPLAYNAME) &&
					NULL != fileInfo.hIcon)
				{
					m_mapISmall[strPath] = fileInfo.hIcon;
					return fileInfo.hIcon;
				}
			}
		}
		return NULL;
	}

	~CIconCache(){}
private:
	CAtlMap<CStringA, HICON> m_mapIcon;
	CAtlMap<CStringA, HICON> m_mapISmall;
	CIconCache(){

		m_mapIcon.RemoveAll();
		m_mapISmall.RemoveAll();
	}
};
class CBkDrawFileIcon : public CBkWindow
{
	BKOBJ_DECLARE_CLASS_NAME(CBkDrawFileIcon, "iconfrommodule")
		CBkDrawFileIcon()
	{
		m_strFilePath.Empty();
		m_hIcon = NULL;
		m_nIsSmallIcon = FALSE;
	}
public:

	// Do nothing
	void OnPaint(CDCHandle dc)
	{

		HICON hIcon = NULL;
		if (NULL != m_hIcon)
		{
			hIcon = m_hIcon;
		}
		else
		{
			if (TRUE == PathFileExistsA(m_strFilePath))
			{
				if (m_nIsSmallIcon)
					hIcon = CIconCache::GetIconCachePtr()->GetIconSmall(m_strFilePath);
				else
					hIcon = CIconCache::GetIconCachePtr()->GetIcon(m_strFilePath);;
			}
		}
		
		if (NULL != hIcon)
		{
			::DrawIconEx(dc.m_hDC, m_rcWindow.left, m_rcWindow.top, hIcon, m_rcWindow.Width(), m_rcWindow.Height(),  0, NULL, DI_NORMAL);
		}

	}


	HRESULT OnIconFile(CStringA& strValue, BOOL bLoading)
	{
		if (FALSE == strValue.IsEmpty())
			m_strFilePath = strValue;
		return S_OK;
	}

	HRESULT OnMemIcon(CStringA& strValue, BOOL bLoading)
	{
		if (FALSE == strValue.IsEmpty())
		{
			DWORD dwValue = StrToIntA(strValue);
			m_hIcon = (HICON)UlongToPtr(dwValue);
		}
			
		return S_OK;

	}

	HRESULT OnSetIconMode(CStringA& strValue, BOOL bLoading)
	{
		if (FALSE == strValue.IsEmpty())
			m_nIsSmallIcon = StrToIntA(strValue);
		
		return S_OK;
	}

protected:
	BKWIN_BEGIN_MSG_MAP()
		MSG_WM_PAINT(OnPaint)
		BKWIN_END_MSG_MAP()

		BKWIN_DECLARE_ATTRIBUTES_BEGIN()
		BKWIN_CUSTOM_ATTRIBUTE("srcfile",   OnIconFile)
		BKWIN_CUSTOM_ATTRIBUTE("iconhandle",   OnMemIcon)
		BKWIN_CUSTOM_ATTRIBUTE("issmall",   OnSetIconMode)
		BKWIN_DECLARE_ATTRIBUTES_END()

protected:
	CStringA	   m_strFilePath;
	HICON		   m_hIcon;
	int		       m_nIsSmallIcon;
};
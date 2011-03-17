//////////////////////////////////////////////////////////////////////////
//   File Name: bkwndpanel.h
// Description: CBkPanel Definition, Virtual Container of BkWindows
//     Creator: Zhang Xiaoxuan
//     Version: 2009.04.28 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "bkwndcmnctrl.h"
#include "bkwndrichtxt.h"

class CBkPanel : public CBkContainerWnd
{
    BKOBJ_DECLARE_CLASS_NAME(CBkPanel, "div")

public:
    CBkPanel()
        : m_bOnlyDrawChild(FALSE)
    {
    }

protected:

    CAtlList<CBkWindow *> m_lstWndChild;
    BOOL m_bOnlyDrawChild;

protected:

    CBkWindow* _CreateBkWindowByName(LPCSTR lpszName);

    // Vert-Align Process
    // Called after this line composed
    void _ProcessLineVAlign(POSITION posLineStart, POSITION posLineEnd, LONG lLineTop, LONG lLineBottom)
    {
        CRect rcChild;

        if (m_uVAlign == VAlign_Top)
            return;

        POSITION posLine = posLineStart;
        int nNewTop = 0;

        while (posLine != posLineEnd)
        {
            CBkWindow *pBkWndLine = m_lstWndChild.GetNext(posLine);

            pBkWndLine->GetRect(rcChild);

            if (m_uVAlign == VAlign_Middle)
                nNewTop = lLineTop + (lLineBottom - lLineTop - rcChild.Height()) / 2;
            else if(m_uVAlign == VAlign_Bottom)
                nNewTop = lLineBottom - rcChild.Height();

            rcChild.MoveToY(nNewTop);
            pBkWndLine->Move(rcChild);
        }
    }

    // Composing .....
    void _ComposingPanel(LPWINDOWPOS lpWndPos)
    {
        POSITION pos = m_lstWndChild.GetHeadPosition(), posPrev = NULL, posThisLineStart = NULL;
        WINDOWPOS WndPos;
        CRect rcChild;

        LONG lNextLineTop = lpWndPos->y + m_style.m_nMarginY + m_style.m_nLineSpacing;
        LONG lMaxRight = m_rcWindow.right;
        LONG lMaxBottom = m_rcWindow.top;
        BOOL bFirstInBlock = TRUE, bNewLine = FALSE;

        memcpy(&WndPos, lpWndPos, sizeof(WINDOWPOS));

        WndPos.x += m_style.m_nMarginX;
        WndPos.y += m_style.m_nMarginY;
        WndPos.cx = ((GetPositionType() & SizeX_Specify) ? m_lSpecifyWidth : WndPos.cx) - m_style.m_nMarginX * 2;
        WndPos.cy -= m_style.m_nMarginY * 2;

        while (pos != NULL)
        {
            posPrev = pos;

            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

            if (!pBkWndChild->IsVisible())
                continue;

            if (Position_Float & pBkWndChild->GetPositionType())
                continue;

            if (bFirstInBlock)
                posThisLineStart = posPrev;

            // Return
            if (pBkWndChild->IsClass(CBkReturn::GetClassName()))
            {
                _ProcessLineVAlign(posThisLineStart, posPrev, WndPos.y, lNextLineTop);
                posThisLineStart = posPrev;

                WndPos.x = lpWndPos->x + m_style.m_nMarginX;
                WndPos.y = lNextLineTop + m_style.m_nLineSpacing;
                WndPos.cx = ((GetPositionType() & SizeX_Specify) ? m_lSpecifyWidth : lpWndPos->cx) - m_style.m_nMarginX * 2;
                WndPos.cy = lpWndPos->y + lpWndPos->cy - WndPos.y - m_style.m_nMarginY;

                bFirstInBlock = FALSE;

                continue;
            }

            if (WndPos.cx == 0)
            {
                bNewLine = TRUE;
            }
            else
            {
                pBkWndChild->BkSendMessage(WM_WINDOWPOSCHANGED, NULL, (LPARAM)&WndPos);
                pBkWndChild->GetRect(rcChild);

                bNewLine = FALSE;

                // Is this line full?
                if ((rcChild.right > WndPos.x + WndPos.cx || rcChild.right <= rcChild.left) && !bFirstInBlock)
                {
                    bNewLine = TRUE;
                    _ProcessLineVAlign(posThisLineStart, posPrev, WndPos.y, lNextLineTop);
                    posThisLineStart = posPrev;
                }
            }

            bFirstInBlock = FALSE;

            if (bNewLine)
            {
                // Reposition this control to next line
                WndPos.x = lpWndPos->x + m_style.m_nMarginX;
                WndPos.y = lNextLineTop + m_style.m_nLineSpacing;
                WndPos.cx = ((GetPositionType() & SizeX_Specify) ? m_lSpecifyWidth : lpWndPos->cx) - m_style.m_nMarginX * 2;
                WndPos.cy = lpWndPos->y + lpWndPos->cy - WndPos.y - m_style.m_nMarginY;

                pBkWndChild->BkSendMessage(WM_WINDOWPOSCHANGED, NULL, (LPARAM)&WndPos);
                pBkWndChild->GetRect(rcChild);
            }

            // Next control position
            WndPos.x += rcChild.Width() + m_style.m_nSpacing;
            WndPos.cx -= rcChild.Width() + m_style.m_nSpacing;

            if (lNextLineTop < rcChild.bottom)
            {
                lNextLineTop = rcChild.bottom;
            }

            if (lMaxRight < rcChild.right + m_style.m_nMarginX)
            {
                lMaxRight = rcChild.right + m_style.m_nMarginX;
            }
        }

        // Process last line
        _ProcessLineVAlign(posThisLineStart, NULL, WndPos.y, lNextLineTop);

        switch (GetPositionType() & SizeX_Mask)
        {
        case SizeX_FitParent:
            m_rcWindow.right = lpWndPos->x + lpWndPos->cx;
            break;
        case SizeX_FitContent:
            m_rcWindow.right = lMaxRight;
            break;
        case SizeX_Specify:
            m_rcWindow.right = lpWndPos->x + m_lSpecifyWidth;
            break;
        }

        switch (GetPositionType() & SizeY_Mask)
        {
        case SizeY_FitParent:
            m_rcWindow.bottom = lpWndPos->y + lpWndPos->cy;
            break;
        case SizeY_FitContent:
            m_rcWindow.bottom = lNextLineTop + m_style.m_nMarginY;
            break;
        case SizeY_Specify:
            m_rcWindow.bottom = lpWndPos->y + m_lSpecifyHeight;
            break;
        }

        // Process float children here
        pos = m_lstWndChild.GetHeadPosition();

        while (pos != NULL)
        {
            posPrev = pos;

            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

            if (!pBkWndChild->IsVisible())
                continue;

            if (0 == (Position_Float & pBkWndChild->GetPositionType()))
                continue;

            memcpy(&WndPos, lpWndPos, sizeof(WINDOWPOS));

            WndPos.x += m_style.m_nMarginX;
            WndPos.y += m_style.m_nMarginY;
            WndPos.cx = ((GetPositionType() & SizeX_Specify) ? m_lSpecifyWidth : WndPos.cx) - m_style.m_nMarginX * 2;
            WndPos.cy -= m_style.m_nMarginY * 2;

            pBkWndChild->BkSendMessage(WM_WINDOWPOSCHANGED, NULL, (LPARAM)&WndPos);
            pBkWndChild->GetRect(rcChild);

            if (Align_Right & pBkWndChild->GetAlign())
            {
                WndPos.x = WndPos.x + WndPos.cx - rcChild.Width();
            }
            else if (Align_Center & pBkWndChild->GetAlign())
            {
                WndPos.x = WndPos.x + (WndPos.cx - rcChild.Width()) / 2;
            }

            if (VAlign_Bottom & pBkWndChild->GetAlign())
            {
                WndPos.y = WndPos.y + WndPos.cy - rcChild.Height();
            }
            else if (VAlign_Middle & pBkWndChild->GetAlign())
            {
                WndPos.y = WndPos.y + (WndPos.cy - rcChild.Height()) / 2;
            }

            WndPos.cx = rcChild.Width();
            WndPos.cy = rcChild.Height();

            pBkWndChild->BkSendMessage(WM_WINDOWPOSCHANGED, NULL, (LPARAM)&WndPos);

        }
    }

public:

    BOOL LoadChilds(TiXmlElement* pTiXmlChildElem)
    {
        BkSendMessage(WM_DESTROY);

        BOOL bVisible = IsVisible(TRUE);

        for (TiXmlElement* pXmlChild = pTiXmlChildElem; NULL != pXmlChild; pXmlChild = pXmlChild->NextSiblingElement())
        {
            CBkWindow *pNewChildWindow = _CreateBkWindowByName(pXmlChild->Value());
            if (!pNewChildWindow)
                continue;

//             int nID = 0;
//             pXmlChild->Attribute("id", &nID);

            pNewChildWindow->SetParent(m_hBkWnd);
            pNewChildWindow->SetContainer(m_hWndContainer);
            pNewChildWindow->Load(pXmlChild);

//             if (pNewChildWindow->IsVisible())
//                 pNewChildWindow->BkSendMessage(WM_SHOWWINDOW, bVisible);

            m_lstWndChild.AddTail(pNewChildWindow);
        }

        return TRUE;
    }

    // Create children
    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
        if (!CBkWindow::Load(pTiXmlElem))
            return FALSE;

        return LoadChilds(pTiXmlElem->FirstChildElement());
    }

    // Hittest children
    virtual HBKWND BkGetHWNDFromPoint(POINT ptHitTest, BOOL bOnlyText)
    {
        if (m_rcWindow.PtInRect(ptHitTest))
        {
            if (m_bOnlyDrawChild)
                return m_hBkWnd;

            HBKWND hBkWndChild = NULL;

            POSITION pos = m_lstWndChild.GetTailPosition();

            while (pos != NULL)
            {
                CBkWindow *pBkWndChild = m_lstWndChild.GetPrev(pos);
                if (!pBkWndChild->IsVisible() || pBkWndChild->IsTransparent())
                    continue;

                if (Position_Float == (pBkWndChild->GetPositionType() & Position_Float))
                {
                    hBkWndChild = pBkWndChild->BkGetHWNDFromPoint(ptHitTest, bOnlyText);

                    if (hBkWndChild)
                        break;
                }
            }

            if (hBkWndChild)
                return hBkWndChild;

            pos = m_lstWndChild.GetTailPosition();

            while (pos != NULL)
            {
                CBkWindow *pBkWndChild = m_lstWndChild.GetPrev(pos);
                if (!pBkWndChild->IsVisible() || pBkWndChild->IsTransparent())
                    continue;

                if (0 == (pBkWndChild->GetPositionType() & Position_Float))
                {
                    hBkWndChild = pBkWndChild->BkGetHWNDFromPoint(ptHitTest, bOnlyText);

                    if (hBkWndChild)
                        break;
                }
            }

            if (hBkWndChild)
                return hBkWndChild;
            else
                return m_hBkWnd;
        }
        else
            return NULL;
    }

    void SetContainer(HWND hWndContainer)
    {
        __super::SetContainer(hWndContainer);

        POSITION pos = m_lstWndChild.GetHeadPosition();

        while (pos != NULL)
        {
            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

            if (pBkWndChild)
            {
                pBkWndChild->SetContainer(hWndContainer);
            }
        }
    }

    CBkWindow* FindChildByCmdID(UINT uCmdID)
    {
        CBkWindow *pChildFind = NULL;

        POSITION pos = m_lstWndChild.GetHeadPosition();

        while (pos != NULL)
        {
            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

            if (pBkWndChild->GetCmdID() == uCmdID)
                return pBkWndChild;
        }

        pos = m_lstWndChild.GetHeadPosition();

        while (pos != NULL)
        {
            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

            if (pBkWndChild->IsContainer())
            {
                pChildFind = ((CBkContainerWnd *)pBkWndChild)->FindChildByCmdID(uCmdID);
                if (pChildFind)
                    return pChildFind;
            }
        }

        return NULL;
    }

    virtual BOOL RedrawRegion(CDCHandle& dc, CRgn& rgn)
    {
        if (__super::RedrawRegion(dc, rgn))
        {
            POSITION pos = m_lstWndChild.GetHeadPosition();
            BkDCPaint BkDC;

            BeforePaint(dc, BkDC);

            while (pos != NULL)
            {
                CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

                if (pBkWndChild && pBkWndChild->IsVisible())
                {
                    pBkWndChild->RedrawRegion(dc, rgn);
                }
            }

            AfterPaint(dc, BkDC);

            return TRUE;
        }

        return FALSE;
    }

    void ShowAllRealWindowChilds(BOOL bShow);

    void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
    {
        // Calc default panel size
        CBkWindow::OnWindowPosChanged(lpWndPos);

        _ComposingPanel(lpWndPos);

    }

    int OnCreate(LPCREATESTRUCT /*lpCreateStruct*/)
    {
        POSITION pos = m_lstWndChild.GetHeadPosition();

        while (pos != NULL)
        {
            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

            pBkWndChild->OnCreate(NULL);
        }

        return TRUE;
    }

    void OnDestroy()
    {
        POSITION pos = m_lstWndChild.GetHeadPosition();

        while (pos != NULL)
        {
            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

            pBkWndChild->BkSendMessage(WM_DESTROY);

            delete pBkWndChild;
        }

        m_lstWndChild.RemoveAll();
    }

    BOOL OnEraseBkgnd(CDCHandle dc)
    {
//         CBkWindow::OnEraseBkgnd(dc);

        POSITION pos = m_lstWndChild.GetHeadPosition();

        BOOL bDisabled = IsDisabled(), bIsChildDisabled = FALSE;

        while (pos != NULL)
        {
            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);
            if (!pBkWndChild->IsVisible())
                continue;

            if (bDisabled)
            {
                bIsChildDisabled = pBkWndChild->IsDisabled();

                if (!bIsChildDisabled)
                    pBkWndChild->ModifyState(BkWndState_Disable, 0);
            }

            pBkWndChild->BkSendMessage(WM_ERASEBKGND, (WPARAM)(HDC)dc);

            if (bDisabled && !bIsChildDisabled)
            {
                pBkWndChild->ModifyState(0, BkWndState_Disable);
            }
        }

        return TRUE;
    }

    void OnPaint(CDCHandle dc)
    {
        return;

        HFONT hftOld = NULL, hftDraw = NULL;
        COLORREF crOld = CLR_INVALID, crOldBg = CLR_INVALID, crDraw = CLR_INVALID;
        CRect rcText = m_rcWindow;
        int nOldBkMode = OPAQUE;

        if (!m_style.m_strSkinName.IsEmpty())
            nOldBkMode = dc.SetBkMode(TRANSPARENT);
        if (CLR_INVALID != m_style.m_crBg)
            crOldBg = dc.SetBkColor(m_style.m_crBg);

        if (m_style.m_ftText)
            hftDraw = m_style.m_ftText;

        if (m_style.m_crText != CLR_INVALID)
            crDraw = m_style.m_crText;

        if (BkWndState_Hover == (GetState() & BkWndState_Hover))
        {
            if (m_style.m_ftHover)
                hftDraw = m_style.m_ftHover;

            if (m_style.m_crHoverText != CLR_INVALID)
                crDraw = m_style.m_crHoverText;
        }

        if (hftDraw)
            hftOld = dc.SelectFont(hftDraw);

        if (crDraw != CLR_INVALID)
            crOld = dc.SetTextColor(crDraw);
//         int nOldBkMode = OPAQUE;
//         COLORREF crOldBg = CLR_INVALID;
//         COLORREF crOldText = CLR_INVALID;
// 
//         if (!m_style.m_strSkinName.IsEmpty())
//             nOldBkMode = dc.SetBkMode(TRANSPARENT);
//         if (CLR_INVALID != m_style.m_crBg)
//             crOldBg = dc.SetBkColor(m_style.m_crBg);
//         if (CLR_INVALID != m_style.m_crText)
//             crOldText = dc.SetTextColor(m_style.m_crText);

        POSITION pos = m_lstWndChild.GetHeadPosition();

        BOOL bDisabled = IsDisabled(), bIsChildDisabled = FALSE;

        while (pos != NULL)
        {
            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

            if (!pBkWndChild->IsVisible())
                continue;

            if (bDisabled)
            {
                bIsChildDisabled = pBkWndChild->IsDisabled();

                if (!bIsChildDisabled)
                    pBkWndChild->ModifyState(BkWndState_Disable, 0);
            }

            pBkWndChild->BkSendMessage(WM_PAINT, (WPARAM)(HDC)dc);

            if (bDisabled && !bIsChildDisabled)
            {
                pBkWndChild->ModifyState(0, BkWndState_Disable);
            }
        }

        if (hftDraw)
            dc.SelectFont(hftOld);

        if (crDraw != CLR_INVALID)
            dc.SetTextColor(crOld);

        if (!m_style.m_strSkinName.IsEmpty())
            dc.SetBkMode(nOldBkMode);
        if (CLR_INVALID != m_style.m_crBg)
            dc.SetBkColor(crOldBg);
    }

    void OnMouseMove(UINT nFlags, CPoint point)
    {
        POSITION pos = m_lstWndChild.GetHeadPosition();
        CRect rcChild;

        while (pos != NULL)
        {
            CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);
            if (!pBkWndChild->IsVisible())
                continue;

            pBkWndChild->GetRect(rcChild);

            if (rcChild.PtInRect(point))
            {
                pBkWndChild->BkSendMessage(WM_MOUSEMOVE, (WPARAM)nFlags, (LPARAM)MAKELONG(point.x, point.y));
            }
        }
    }

    void OnShowWindow(BOOL bShow, UINT nStatus)
    {
        __super::OnShowWindow(bShow, nStatus);

        ShowAllRealWindowChilds(bShow);
    }

protected:

    BKWIN_DECLARE_ATTRIBUTES_BEGIN()
        BKWIN_INT_ATTRIBUTE("onlydrawchild", m_bOnlyDrawChild, FALSE)
    BKWIN_DECLARE_ATTRIBUTES_END()

    BKWIN_BEGIN_MSG_MAP()
        MSG_WM_PAINT(OnPaint)
//         MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_SHOWWINDOW(OnShowWindow)
    BKWIN_END_MSG_MAP()
};

#include "bkwnddlg.h"
#include "bkwnddlgfile.h"
#include "bkwndtabctrl.h"

inline CBkWindow* CBkPanel::_CreateBkWindowByName(LPCSTR lpszName)
{
    CBkWindow *pNewWindow = NULL;

//     pNewWindow = CBkPanel::CheckAndNew(lpszName);
//     if (pNewWindow)
//         return pNewWindow;

    pNewWindow = CBkDialog::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

	pNewWindow = CBkDialogFile::CheckAndNew(lpszName);
	if (pNewWindow)
		return pNewWindow;

    pNewWindow = CBkStatic::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkButton::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

//     pNewWindow = CBkSpacing::CheckAndNew(lpszName);
//     if (pNewWindow)
//         return pNewWindow;

    pNewWindow = CBkImageWnd::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

//     pNewWindow = CBkReturn::CheckAndNew(lpszName);
//     if (pNewWindow)
//         return pNewWindow;

    pNewWindow = CBkProgress::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkImageBtnWnd::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkRealWnd::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkLine::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkCheckBox::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkIconWnd::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkTabCtrl::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkRadioBox::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkRichText::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

    pNewWindow = CBkLink::CheckAndNew(lpszName);
    if (pNewWindow)
        return pNewWindow;

	pNewWindow = CBkMemoryImage::CheckAndNew(lpszName);
	if (pNewWindow)
		return pNewWindow;

	pNewWindow = CBkImageListHWnd::CheckAndNew(lpszName);
	if (pNewWindow)
		return pNewWindow;

	pNewWindow = CBkDrawFileIcon::CheckAndNew(lpszName);
	if (pNewWindow)
		return pNewWindow;

    return NULL;
}

inline void CBkPanel::ShowAllRealWindowChilds(BOOL bShow)
{
    POSITION pos = m_lstWndChild.GetHeadPosition();

    BOOL bVisible = IsVisible(TRUE);

    while (pos != NULL)
    {
        CBkWindow *pBkWndChild = m_lstWndChild.GetNext(pos);

        BOOL bChildVisible = bVisible && pBkWndChild->IsVisible();

//         if (!pBkWndChild->IsVisible())
//             continue;

        if (pBkWndChild->IsClass(CBkRealWnd::GetClassName()))
        {
            ((CBkRealWnd *)pBkWndChild)->ShowRealWindow(bShow && bChildVisible);
        }
        else if (pBkWndChild->IsClass(CBkPanel::GetClassName()) || pBkWndChild->IsClass(CBkDialog::GetClassName()) || pBkWndChild->IsClass(CBkDialogFile::GetClassName()) )
        {
            ((CBkPanel *)pBkWndChild)->ShowAllRealWindowChilds(bShow && bChildVisible);
        }
        else if (pBkWndChild->IsClass(CBkTabCtrl::GetClassName()))
        {
            ((CBkTabCtrl *)pBkWndChild)->ShowAllRealWindowChilds(bShow && bChildVisible);
        }
    }
}

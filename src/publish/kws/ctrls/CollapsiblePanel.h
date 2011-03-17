#if !defined(AFX_COLLAPSIBLEPANEL_H__20031129_797C_31F4_B029_0080AD509054__INCLUDED_)
#define AFX_COLLAPSIBLEPANEL_H__20031129_797C_31F4_B029_0080AD509054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __cplusplus
  #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLCTRLS_H__
  #error CollapsiblePanel.h requires atlctrls.h to be included first
#endif

#ifndef __ATLGDIX_H__
  #error This control requires my atlgdix.h to be included first
#endif

#if (_WIN32_IE < 0x0400)
  #error This control requires _WIN32_IE >= 0x0400
#endif

#include <zmouse.h>
#include "ScanResultListPanel.h"

// Control notifications
#define CPN_EXPANDING     1
#define CPN_EXPANDED      2
#define		MSG_CHANGE_SCAN_RESULT_CHECK_STAT		(WM_USER + 500)

// Extended Panel style
#define CPS_EX_NOANIMATE       0x00000001
#define CPS_EX_FLATSTYLE       0x00000002
#define CPS_EX_DEEPDISABLE     0x00000004
#define CPS_EX_SHAREIMAGELIST  0x00000008
#define CPS_EX_OWNERDRAW       0x00000010
#define CPS_EX_NOEXPANDBUTTON  0x00000020
#define CPS_EX_EXPANDCLICK     0x00000040
#define CPS_EX_NOSCROLLBAR     0x00000080
#define CPS_EX_SELHIGHLIGHT    0x00000100


#define MSG_REARRANGE_PANELS   (WM_USER + 101)


template< class T, class TBase = CWindow, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CCollapsiblePanelImpl : 
   public CWindowImpl< T >
{
public:
   enum { ANIMATETIMER_ID = 22 };    // Happens to be my lucky number!
   enum { IMAGE_GAP = 5 };           // Gap between button-base and icon-base

   typedef struct tagPANEL
   {
		BOOL	bCheck;
		CString strType;
		CString strItemName;
		CString strLevel;
		CString strAction;
		CString strEngine;
		COLORREF clrLevel;
		UINT	uBkIndex;
		int			nType;
		DWORD		dwID;
		int			nLevel;
		int			nAction;
		BOOL		bReboot;
		BOOL		bSetWhite;
		CWindow*	pWnd;

		HWND hWnd;                     // Handle to view
		int iImage;                    // Image Index into ImageList
		bool bEnabled;                 // Is panel enabled?
		bool bExpanded;                // Is panel expanded?
		COLORREF clrText;              // Text color. If CLR_INVALID then use default
		COLORREF clrBar1;              // Bar color. If CLR_INVALID then use default
		COLORREF clrBar2;              // Bar color. If CLR_INVALID then use default
		HBITMAP hbmpButton;            // Bitmap representation of button
		SIZE szChild;                  // Preferred size of child pane
		DWORD dwAnimateStopTime;       // Time when animation should stop
		int cy;                        // Current child height
		SIZE szCheckBox;
		CRect rtTrust;
		RECT  rtText;
		BOOL  bShowTrust;
   } PANEL;

   typedef struct tagPANELTHEME
   {
      COLORREF clrBack;
      COLORREF clrTextActive;
      COLORREF clrTextInactive;
      COLORREF clrTextSel;
      COLORREF clrBarActive1;
      COLORREF clrBarActive2;
      COLORREF clrBarInactive1;
      COLORREF clrBarInactive2;
      COLORREF clrBarSel1;
      COLORREF clrBarSel2;
      COLORREF clrBorder;
      int iArc;
   } PANELTHEME;

   CListArray<PANEL> m_aPanels;		 // Collection of panels
   int m_iCurSel;                    // Last known selected panel
   CFont m_fntNormal;                // Normal font
   CFont m_fntBold;                  // Bold font
   CFont m_ftnLink;
   DWORD m_dwExtStyle;               // Extended style flags
   PANELTHEME m_Theme;               // Color configuration
   CIconHandle m_iconCollapse;       // Icon for expander button
   CIconHandle m_iconExpand;         // Icon for expander button
   CBitmapHandle m_bmpCollapse;
   CBitmapHandle m_bmpExpand;
   bool m_bOwnCollapseIcons;         // We own expand icons?
   CImageList m_Images;              // Images for icons
   CImageList m_ImagesGrey;          // Images for grey icons
   SIZE m_szMargin;                  // Margins between window and panels
   int m_cyPadding;                  // Padding between panels
   int m_cyButton;                   // Height of button (including icon)
   int m_cyBar;                      // Height of bar (colored text/caption)
   WORD m_cxLastWidth;               // Last known width of window
   WORD m_cyLastHeight;              // Last known height of window
   DWORD m_dwAnimateTime;            // How long time to allow animation (in ms)
   UINT m_uMsgMouseWheel;            // Mouse wheel registered message
   bool		m_bRePos;
   UINT		m_msgCheck;
   UINT		m_msgTrust;
   BOOL		m_bShowEngName;
   int		m_nFlag;//hub  识别扫完和扫描过程中的区别
   int		m_nScrollCurPos;//hub
   CToolTipCtrl	m_wndToolTip;
   BOOL		m_bScaning;
   long		xLite;
   // Operations

   // yykingking 打钩的消息搞成变化的
   CCollapsiblePanelImpl()
   {
	   m_msgCheck		= MSG_CHANGE_SCAN_RESULT_CHECK_STAT;
	   m_msgTrust		= -1;
	   m_bShowEngName	= TRUE;
	   m_nFlag			= -1;	//hub 初始化
	   m_nScrollCurPos  = 0;	//初始化
	   m_bScaning		= FALSE;
	   xLite = 0;
   }

   void SetScaning( BOOL bFalse )
   {
	   m_bScaning = bFalse;
   }

   VOID SetCheckNotifyMsg(UINT uMsg)
   {
	   m_msgCheck = uMsg;
   }

   void SetScriptTrustMsg( UINT uMsg )
   {
		m_msgTrust = uMsg;
   }

   VOID SetShowEngineName(BOOL bShow)
   {
	   m_bShowEngName = bShow;
   }

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
      if( bRet ) _Init();
      return bRet;
   }
   // Call this in your WTL PreTranslateMessage handler do properly
   // manage tabbing and other dialog-specific things...
   BOOL PreTranslateMessage(MSG* pMsg)
   {
      if( m_iCurSel < 0 ) return FALSE;
      return ::IsDialogMessage(m_aPanels[m_iCurSel].hWnd, pMsg);
   }
   int GetCurSel() const
   {
      return m_iCurSel;
   }
   void SetCurSel(int iSel)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_iCurSel = iSel;
      _RecalcLayout();
   }
   void DeleteAllItems()
   {
      ATLASSERT(::IsWindow(m_hWnd));
      for( int i = 0; i < m_aPanels.GetSize(); i++ ) 
	  {
         if( m_aPanels[i].hbmpButton != NULL ) 
			 ::DeleteObject(m_aPanels[i].hbmpButton);
      }
      m_aPanels.RemoveAll();
      m_iCurSel = -1;
   }
   int GetItemCount() const
   {
      return m_aPanels.GetSize();
   }
   int InsertItem(
	   int nIndex,
	   UINT uBkIndex,
	   int	nType,
	   DWORD dwID,
	   int nLevel,
	   int nAction,
	   CWindow* pWnd, 
	   LPCTSTR pstrType, 
	   LPCTSTR pstrItem, 
	   LPCTSTR psEngine,
	   LPCTSTR pstrLevel,
	   LPCTSTR pstrAction,
	   BOOL bShowTrust,
	   int iImage = -1, COLORREF clrLevel = CLR_INVALID)
   {
      ATLASSERT(::IsWindow(pWnd->m_hWnd));
      ATLASSERT(iImage==-1 || !m_Images.IsNull()); // Need to assign ImageList first
      //ATLASSERT(((DWORD)::GetWindowLong(hWndView, GWL_STYLE) & WS_CLIPSIBLINGS)!=0); // Must have CLIPSIBLINGS
      // Assign new parent to panel
      ::SetParent(pWnd->m_hWnd, m_hWnd);
      // Make sure the panel is resized to the preferred size
      // before it's added so we can know its size when expanding it!
      RECT rc;
      ::GetWindowRect(pWnd->m_hWnd, &rc);
      ATLASSERT(rc.right-rc.left>0 && rc.bottom-rc.top>0);
      // Add the item in the list
      PANEL panel = { 0 };
      panel.hWnd = pWnd->m_hWnd;
	 
	  panel.strType = pstrType;
	  panel.strItemName = pstrItem;
	  panel.strLevel = pstrLevel;
	  panel.strAction = pstrAction;
	  panel.strEngine = psEngine;
	  panel.pWnd = pWnd;

	  panel.uBkIndex = uBkIndex;
	  panel.bSetWhite = false;
	  panel.nType = nType;
	  panel.dwID = dwID;
	  panel.nLevel = nLevel;
	  panel.nAction = nAction;
	  panel.bReboot = false;
	  panel.bCheck = true;
      panel.bExpanded = false;
      panel.bEnabled = true;
	  panel.bShowTrust = bShowTrust;
      panel.clrLevel = clrLevel;
	  panel.clrText = CLR_INVALID;
      panel.clrBar1 = CLR_INVALID;
      panel.clrBar2 = CLR_INVALID;
      panel.iImage = iImage;
      panel.szChild.cx = rc.right - rc.left;
	  panel.szChild.cy = rc.bottom - rc.top;
	  panel.cy = panel.bExpanded ? panel.szChild.cy : 0;
      if (nIndex == -1)
      {
		  m_aPanels.Add(panel);
      }
	  else
		m_aPanels.InsertAt(nIndex, panel);

      ::ShowWindow(pWnd->m_hWnd, SW_HIDE );
	  
      return m_aPanels.GetSize() - 1;
   }
   void RePaintPanel(int nIndex)//hub
   {
	   BOOL b = GetItemExpanded(nIndex);
	   if (b)//需要重绘
	   {
		   ::InvalidateRect(m_aPanels[nIndex].hWnd, NULL, TRUE);
	   }
	   return;
   }
   BOOL GetItemExpanded(int iIndex) const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      if( iIndex < 0 || iIndex >= m_aPanels.GetSize() ) return FALSE;
      return m_aPanels[iIndex].bExpanded == true;
   }
   BOOL SetItemExpanded(int iIndex, BOOL bExpanded, BOOL bAnimate = FALSE)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      if( iIndex < 0 || iIndex >= m_aPanels.GetSize() ) return FALSE;
      if( m_aPanels[iIndex].bExpanded == (bExpanded == TRUE) ) return TRUE;
      // Send notification
      ::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CPN_EXPANDING), (LPARAM) m_hWnd);
      // Set panel attributes
      PANEL& panel = m_aPanels[iIndex];
      panel.bExpanded = bExpanded == TRUE;
      DWORD dwTick = ::GetTickCount();
      if( bAnimate && (m_dwExtStyle & CPS_EX_NOANIMATE) == 0 ) dwTick += m_dwAnimateTime;
      panel.dwAnimateStopTime = dwTick;
      // Display panel now so animation can be shown

	  //::ShowWindow(panel.hWnd, SW_SHOWNOACTIVATE );
	  if( bExpanded )
	  {
		  RECT rc = GetPanelRect(iIndex, FALSE);
		  ::SetWindowPos( panel.hWnd, NULL, 
			  rc.left, rc.top, rc.right - rc.left, bAnimate ? 0 : rc.bottom - rc.top, 
			  SWP_NOZORDER | ( bAnimate ? SW_SHOWNOACTIVATE  :  SWP_SHOWWINDOW ) );		 

	  }

	  //panel.cy = panel.bExpanded ? panel.szChild.cy : 0;
      // Start the show
	  if( bAnimate )
	  {
		  SetTimer(ANIMATETIMER_ID, 30L);  // 30 ms timer
		  SendMessage(WM_TIMER, ANIMATETIMER_ID);
	  }
	  else{
		  panel.cy = panel.bExpanded ? panel.szChild.cy : 0;
         _RecalcLayout();
         _RearrangePanels();
         _RecalcScrollBar();
      }
      return TRUE;
   }
   BOOL GetItemEnabled(int iIndex) const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      if( iIndex < 0 || iIndex >= m_aPanels.GetSize() ) return FALSE;
      return m_aPanels[iIndex].bEnabled == true;
   }

   BOOL SetItemReboot(int nIndex)
   {
	   ATLASSERT(::IsWindow(m_hWnd));
	   if( nIndex < 0 || nIndex >= m_aPanels.GetSize() ) 
		   return FALSE;
	   m_aPanels[nIndex].bReboot = true;
		
	   CRect rc = GetPanelRect(nIndex, TRUE);
	   InvalidateRect(rc);
	   return TRUE;
   }
   BOOL SetItemEnabled(int iIndex, BOOL bEnabled)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      if( iIndex < 0 || iIndex >= m_aPanels.GetSize() ) return FALSE;
      m_aPanels[iIndex].bEnabled = bEnabled == TRUE;
      if( (m_dwExtStyle & CPS_EX_DEEPDISABLE) != 0 ) ::EnableWindow(m_aPanels[iIndex].hWnd, bEnabled);
      _RecalcLayout();
      return TRUE;
   }
   BOOL SetItemColors(int iIndex, COLORREF clrText, COLORREF clrBar1 = CLR_INVALID, COLORREF clrBar2 = CLR_INVALID)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      if( iIndex < 0 || iIndex >= m_aPanels.GetSize() ) return FALSE;
      m_aPanels[iIndex].clrText = clrText;
      m_aPanels[iIndex].clrBar1 = clrBar1;
      m_aPanels[iIndex].clrBar2 = clrBar2;
      _RecalcLayout();
      return TRUE;
   }
   DWORD SetExtendedPanelStyle(DWORD dwStyle)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      DWORD dwOldStyle = m_dwExtStyle;
      m_dwExtStyle = dwStyle;
      Invalidate();
      return dwOldStyle;
   }
   void SetAnimateTime(DWORD dwAnimateTime)
   {
      m_dwAnimateTime = dwAnimateTime;
   }

   void SetExpandBitmaps( HBITMAP hExpand, HBITMAP hCollapse)
   {
	   m_bmpExpand.Attach( hExpand );
	   m_bmpCollapse.Attach( hCollapse );
   }


   CImageList SetImageList(HIMAGELIST hImages, int nImageList)
   {
      CImageList hOldImages;
      switch( nImageList ) {
      case LVSIL_NORMAL:
         hOldImages = m_Images;
         m_Images = hImages;
         break;
      case LVSIL_STATE:
         hOldImages = m_ImagesGrey;
         m_ImagesGrey = hImages;
         break;
      default:
         ATLASSERT(false);
      }
      _RecalcLayout();
      return hOldImages;
   }
   void SetMargins(int cx, int cy)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_szMargin.cx = cx;
      m_szMargin.cy = cy;
      _RecalcLayout();
   }
   int GetPadding() const
   {
      return m_cyPadding;
   }
   void SetPadding(int iPadding)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_cyPadding = iPadding;
      _RearrangePanels();
      _RecalcScrollBar();
   }
   PANELTHEME GetPanelTheme() const
   {
      return m_Theme;
   }
   void SetPanelTheme(const PANELTHEME Theme)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_Theme = Theme;
      _RecalcLayout();
   }
   RECT GetPanelRect(int iIndex, BOOL bIncludeButton) const
   {
      ATLASSERT(::IsWindow(m_hWnd));
      RECT rc = { 0 };
      if( iIndex < 0 || iIndex >= m_aPanels.GetSize() ) return rc;
      GetClientRect(&rc);
      ::OffsetRect(&rc, 0, -GetScrollPos(SB_VERT));
      ::InflateRect(&rc, -m_szMargin.cx, -m_szMargin.cy);
      for( int i = 0; i <= iIndex; i++ ) {
         if( m_aPanels[i].iImage >= 0 ) rc.top += m_cyButton - m_cyBar;
         rc.top += m_cyBar;
         if( i == iIndex ) {
            rc.bottom = rc.top + m_aPanels[i].cy;
            if( bIncludeButton ) rc.top -= m_cyButton;
            if( !bIncludeButton && (GetStyle() & WS_BORDER) != 0 ) ::InflateRect(&rc, -1, -1);
            return rc;
         }
         rc.top += m_aPanels[i].cy + m_cyPadding;
      }
      ATLASSERT(false);
      return rc;
   }

   void GetItem(int nIndex, PANEL& panel)
   {
	   if( nIndex < 0 || nIndex >= m_aPanels.GetSize() ) 
		   return;
		
	   CScanResultListPanel* pPanelWnd = (CScanResultListPanel*)m_aPanels[nIndex].pWnd;
	   ATLASSERT( pPanelWnd );
	   if( pPanelWnd )
			m_aPanels[nIndex].bSetWhite = pPanelWnd->IsSetWhite();
	   panel = m_aPanels[nIndex];
   }

   // Implementation
   void	_InitTip()
   {
	   m_wndToolTip.Create(m_hWnd);
	   m_wndToolTip.Activate(TRUE);	   
   }
   void _Init()
   {
      ATLASSERT(::IsWindow(m_hWnd));
	  ATLASSERT(GetStyle() & WS_CLIPCHILDREN);

      m_iCurSel = -1;
      m_dwExtStyle = 0;
      m_cxLastWidth = 0;
      m_cyLastHeight = 0;

      CFontHandle font = GetFont();
      if( font.IsNull() ) 
		  font = AtlGetDefaultGuiFont();
      LOGFONT lf;
      font.GetLogFont(&lf);
	  _tcscpy( lf.lfFaceName, TEXT("宋体") );
	  lf.lfHeight = -12;
	  lf.lfWeight = FW_NORMAL;

      if( !m_fntNormal.IsNull() ) 
		  m_fntNormal.DeleteObject();
      m_fntNormal.CreateFontIndirect(&lf);
      SetFont(m_fntNormal);

	  lf.lfWeight = FW_BOLD;
	  if( !m_fntBold.IsNull() ) 
		  m_fntBold.DeleteObject();
	  m_fntBold.CreateFontIndirect(&lf);


	  lf.lfWeight = FW_NORMAL;
	  lf.lfUnderline = TRUE;
	  if( !m_ftnLink.IsNull() ) 
		  m_ftnLink.DeleteObject();
	  m_ftnLink.CreateFontIndirect(&lf);

      m_szMargin.cx = 0;
      m_szMargin.cy = 0;
      m_cyPadding = 1;
      m_cyButton = 29;
      m_cyBar = 29;

      m_dwAnimateTime = 250;

      m_Theme.clrBack = RGB(255,255,255);
      m_Theme.clrTextActive = RGB(0,0,128);
      m_Theme.clrTextInactive = RGB(120,120,120);
      m_Theme.clrTextSel = RGB(0,0,128);
      m_Theme.clrBarActive1 = RGB(235,239,243);
      m_Theme.clrBarActive2 = RGB(214,215,224);
      m_Theme.clrBarInactive1 = RGB(245,245,245);
      m_Theme.clrBarInactive2 = RGB(184,184,184);
      m_Theme.clrBarSel1 = RGB(174,230,250);
      m_Theme.clrBarSel2 = RGB(128,128,128);
      m_Theme.clrBorder = RGB(0,0,0);
      m_Theme.iArc = 0;

	  // load list images
	  m_Images.CreateFromImage( IDB_LISTITEMS, 16, 0, RGB( 255, 0, 255 ), IMAGE_BITMAP, LR_CREATEDIBSECTION );

      m_uMsgMouseWheel = ::RegisterWindowMessage(MSH_MOUSEWHEEL);
	  _InitTip();
   }
   void _RecalcLayout()
   {
      // No need to do this before first WM_SIZE is seen
      if( m_cxLastWidth == 0 ) return;
      // Need to recreate all buttons because view changed
      for( int i = 0; i < m_aPanels.GetSize(); i++ ) {
         if( m_aPanels[i].hbmpButton != NULL ) {
            ::DeleteObject(m_aPanels[i].hbmpButton);
            m_aPanels[i].hbmpButton = NULL;
         }
      }
      // Calculate button size
      SIZE szIcons = { 0 };
      if( !m_Images.IsNull() ) 
		  m_Images.GetIconSize(szIcons);
      m_cyButton = max(m_cyBar, szIcons.cy + IMAGE_GAP);
      // Repaint
      Invalidate();
   }
   void _RecalcScrollBarProcess()//hub  扫描过程中查看
   {
	   // Check that we can manipulate the scrollbar
	   if( (m_dwExtStyle & CPS_EX_NOSCROLLBAR) != 0 ) return;
	   int iPos = GetScrollPos(SB_VERT);
	   RECT rc = GetPanelRect(GetItemCount() - 1, TRUE);

	   int iNewPos = iPos;
	   RECT rcCur = GetPanelRect(m_iCurSel, TRUE);
	   if (rcCur.top < 0)
	   {
		   iNewPos -= 0 - rcCur.top;
	   }
	   else if (rcCur.bottom > m_cyLastHeight) 
	   {
		   iNewPos += (rcCur.bottom - m_cyLastHeight > rcCur.top ? rcCur.top - 1 : rcCur.bottom - m_cyLastHeight);
	   }
	   // Update the scrollbar based on the total height
	   SCROLLINFO si = { 0 };
	   si.cbSize = sizeof(si);
	   si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	   si.nMin = 0;
	   si.nMax = rc.bottom + m_szMargin.cy + iPos;
	   si.nPage = m_cyLastHeight;
	   si.nPos = m_nScrollCurPos;
	   SetScrollInfo(SB_VERT, &si, TRUE);
	   if( (UINT) iPos > si.nMax - si.nPage || iNewPos != iPos) 
		   _RearrangePanels(-1, true);
   }
   void _RecalcScrollBar()
   {
      // Check that we can manipulate the scrollbar
      if( (m_dwExtStyle & CPS_EX_NOSCROLLBAR) != 0 ) return;
      // Scrollbar support by Anatoly Ivasyuk.
      // Get the location of the last panel in the list
      int iPos = GetScrollPos(SB_VERT);
      RECT rc = GetPanelRect(GetItemCount() - 1, TRUE);

	  int iNewPos = iPos;
	  RECT rcCur = GetPanelRect(m_iCurSel, TRUE);
	  if (rcCur.top < 0)
	  {
		  iNewPos -= 0 - rcCur.top;
	  }
	  else if (rcCur.bottom > m_cyLastHeight) 
	  {
		  iNewPos += (rcCur.bottom - m_cyLastHeight > rcCur.top ? rcCur.top - 1 : rcCur.bottom - m_cyLastHeight);
	  }
      // Update the scrollbar based on the total height
      SCROLLINFO si = { 0 };
      si.cbSize = sizeof(si);
      si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
      si.nMin = 0;
      si.nMax = rc.bottom + m_szMargin.cy + iPos;
      si.nPage = m_cyLastHeight;
	  si.nPos = iNewPos;
      SetScrollInfo(SB_VERT, &si, TRUE);
      if( (UINT) iPos > si.nMax - si.nPage || iNewPos != iPos) 
		  _RearrangePanels(-1, true);
   }

   LRESULT OnRearrangePanels(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
	   if (m_bRePos)
	   {
		   _RearrangePanels(-1, true);
	   }
	   else
		   return FALSE;

		return TRUE;
   }

   void _RearrangePanels( int iStart = -1, bool bRedraw = false)
   {
      for( int i = 0; i < m_aPanels.GetSize(); i++ ) 
	  {
         
		 if( m_aPanels[i].bExpanded || iStart == i )
		 {
			 RECT rc = GetPanelRect(i, FALSE);
			::SetWindowPos(m_aPanels[i].hWnd, NULL, 
							rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 
							SWP_NOZORDER | SWP_NOACTIVATE);
		}
		 if (bRedraw)
			 ::InvalidateRect(m_aPanels[i].hWnd, NULL, TRUE);
      }
      Invalidate();
   }
#if defined(_INC_MATH)
   double _CosineInterpolate(double y1, double y2, double mu) const
   {
#ifndef PI
      #define PI 3.1415926535897932384626433832795029L
#endif // PI
      double mu2 = (1.0 - cos(mu * PI)) / 2.0;
      return y1 * (1.0 - mu2) + y2 * mu2;
   }
#endif // _INC_MATH
   void _CreateButton(CDCHandle dc, int iIndex, RECT rcItem)
   {
	   CBitmapHandle bmpBar;
	   if( iIndex == m_iCurSel && (m_dwExtStyle & CPS_EX_SELHIGHLIGHT) != 0 )
		   bmpBar = BkBmpPool::GetBitmap(IDB_SCAN_RESULT_LIST_BAR_SEL);
	   else 
		   bmpBar = BkBmpPool::GetBitmap(IDB_SCAN_RESULT_LIST_BAR);

	   BITMAP bmpInfo = { 0 };
	   bmpBar.GetBitmap(&bmpInfo);

      // Create bitmap for button-background
      CDC dcMem;
      dcMem.CreateCompatibleDC(dc);
      CBitmapHandle bmp;
      bmp.CreateCompatibleBitmap(dc, rcItem.right - rcItem.left, bmpInfo.bmHeight);
      HBITMAP hOldBmp = dcMem.SelectBitmap(bmp);
      // Clear background
      dcMem.FillSolidRect(&rcItem, m_Theme.clrBack);

      // Draw stuff

      {
		  CDC dcCompat;
		  dcCompat.CreateCompatibleDC();

		  HBITMAP hOldBmp2 = dcCompat.SelectBitmap(bmpBar);
		  dcMem.StretchBlt(0, 0, rcItem.right - rcItem.left, bmpInfo.bmHeight, dcCompat, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
  
         // Done
		 dcCompat.SelectBitmap(hOldBmp2);

      }
      dcMem.SelectBitmap(hOldBmp);

	  PANEL& panel = m_aPanels[iIndex];
	  if (panel.hbmpButton)
	  {
		  ::DeleteObject(panel.hbmpButton);
		  panel.hbmpButton = NULL;
	  }
      panel.hbmpButton = bmp.Detach();
   }
   LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
	   POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	   
	   for( int i = 0; i < m_aPanels.GetSize(); i++ )
	   {		  
		   if(m_wndToolTip.IsWindow())
		   {
			   CRect rc(m_aPanels[i].rtText);
			   
			   if (rc.PtInRect(pt))
			   {
				   if(m_wndToolTip.IsWindow())
				   {
					   MSG msgRelay = { m_hWnd, uMsg, wParam, lParam };
					   m_wndToolTip.RelayEvent(&msgRelay);
				   }
			   }
			}		  
	   }

	   return 0;
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CCollapsiblePanelImpl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	  MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
      MESSAGE_HANDLER(WM_TIMER, OnTimer)
      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
      MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDown)
      MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
      MESSAGE_HANDLER(m_uMsgMouseWheel, OnMouseWheel)
      MESSAGE_HANDLER(WM_PARENTNOTIFY, OnMouseActivate)
	  MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
	  MESSAGE_HANDLER(MSG_REARRANGE_PANELS, OnRearrangePanels)
	  MESSAGE_HANDLER(WM_SETCURSOR,			OnSetCursor)
	  MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	  //CHAIN_MSG_MAP(CDoubleBufferWindowImpl< T >)
   END_MSG_MAP()


   LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
   {
	   UINT uCode = LOWORD(lParam);
	   UINT uMouseMsg = HIWORD(lParam);
	   CPoint point;
	   GetCursorPos( &point );
	   ScreenToClient( &point );

	   if( uMouseMsg == 0 || uCode != HTCLIENT )
	   {
		   bHandled = FALSE;
		   return FALSE;
	   }

	   BOOL bFind = FALSE;
	   if( uMouseMsg >= WM_MOUSEFIRST && uMouseMsg <= WM_MOUSELAST )
	   {
		   for( int i = 0; i < m_aPanels.GetSize(); i++ )
		   {
			   PANEL& panel = m_aPanels[i];
			   if( panel.bShowTrust && panel.rtTrust.PtInRect( point ) &&  panel.bEnabled && !m_bScaning )
			   {
				   SetCursor( LoadCursor(NULL, IDC_HAND ));
				   bFind = TRUE;
				   break;
			   }
		   } 
	   }

	   if( !bFind )
			bHandled = FALSE;
	   
	   return TRUE;
   }

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc();
      _Init();
      return lRes;
   }

   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      DeleteAllItems();
      if( m_bOwnCollapseIcons ) {
         if( !m_iconCollapse.IsNull() ) m_iconCollapse.DestroyIcon();
         if( !m_iconExpand.IsNull() ) m_iconExpand.DestroyIcon();
      }
      if( (m_dwExtStyle & CPS_EX_SHAREIMAGELIST) == 0 ) {
         if( m_Images.IsNull() ) m_Images.Destroy();
         if( m_ImagesGrey.IsNull() ) m_ImagesGrey.Destroy();
      }
      bHandled = FALSE;
	  if ( m_wndToolTip.IsWindow() )
		  m_wndToolTip.DestroyWindow();
	  else
		  m_wndToolTip.m_hWnd = NULL;
      return 0;
   }
   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
   {
      if( m_cxLastWidth != LOWORD(lParam) ) {
         m_cxLastWidth = LOWORD(lParam);
         _RecalcLayout();
         _RearrangePanels();
         _RecalcScrollBar();
      }
      if( m_cyLastHeight != HIWORD(lParam) ) {
         m_cyLastHeight = HIWORD(lParam);
         _RecalcScrollBar();
      }
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
   {
      bHandled = FALSE;
      if( wParam != ANIMATETIMER_ID ) return 0;
      // Process movement of any animating panel
      bool bStillAnimating = false;
      DWORD dwTick = ::GetTickCount();
	  int nIndex = 0;
      for( int i = 0; i < m_aPanels.GetSize(); i++ ) 
	  {
         if( m_aPanels[i].dwAnimateStopTime > 0 ) 
		 {
			nIndex = i;
            PANEL& panel = m_aPanels[i];
            if( dwTick >= panel.dwAnimateStopTime ) 
			{
               // Done animating this panel
               panel.dwAnimateStopTime = 0;
               panel.cy = panel.bExpanded ? panel.szChild.cy : 0;
               // Show/hide child window
               ::ShowWindow(panel.hWnd, panel.bExpanded ? SW_SHOWNOACTIVATE : SW_HIDE);
               // Send notification
               ::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), CPN_EXPANDED), (LPARAM) m_hWnd);
            }
            else 
            {
#if defined(_INC_MATH)
               double mu;
               double dblTimeDiff = panel.dwAnimateStopTime - dwTick;
               if( dblTimeDiff <= 0.0 || m_dwAnimateTime == 0 ) mu = 1.0;
               else mu = dblTimeDiff / (double) m_dwAnimateTime;
               int iHeight = (int) _CosineInterpolate(0, panel.szChild.cy, mu);
               panel.cy = panel.bExpanded ? panel.szChild.cy - iHeight : iHeight;
#else  // _INC_MATH
               int iDiff = panel.bExpanded ? panel.szChild.cy - panel.cy : panel.cy;
               iDiff /= 2;
               panel.cy = panel.bExpanded ? panel.szChild.cy - iDiff : iDiff;
#endif // _INC_MATH
               bStillAnimating = true;
            }
         }
      }
      _RearrangePanels( nIndex );
      if( !bStillAnimating ) 
      {
         _RecalcLayout();
         _RecalcScrollBar( );
         KillTimer(ANIMATETIMER_ID);
      }
      return 0;
   }
   LRESULT OnVScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Scrollbar support added by Anatoly Ivasyuk
      // Get all the vertial scroll bar information
      SCROLLINFO si = { 0 };
      si.cbSize = sizeof(si);
      si.fMask  = SIF_ALL;
      GetScrollInfo(SB_VERT, &si);  
      // Save the position for comparison later on
      int yPos = si.nPos;  
	  bool bRedraw = false;
      switch( LOWORD(wParam) ) 
	  {
      case SB_TOP:
         // User clicked the HOME keyboard key
         si.nPos = si.nMin;
         break;           
      case SB_BOTTOM:
         // User clicked the END keyboard key
         si.nPos = si.nMax;
         break;          
      case SB_LINEUP:
         // User clicked the top arrow
		  bRedraw = true;
		  {
			  RECT rc;
			  int nAdd = 0;
			  for( int i = 0; i < m_aPanels.GetSize(); i++ ) 
			  {
				  rc = GetPanelRect(i, TRUE);
				  int nTop = rc.top;
				  if (nTop >= m_szMargin.cy)
				  {
					  rc = GetPanelRect(i - 1, TRUE);

					  nAdd = rc.bottom - rc.top - nTop;
					  break;
				  }
			  }
			  si.nPos -= nAdd + m_cyPadding;
		  }
         break;           
      case SB_LINEDOWN:
         // User clicked the bottom arrow
		  bRedraw = true;
		  {
			  RECT rc;
			  int nAdd = 0;
			  for( int i = 0; i < m_aPanels.GetSize(); i++ ) 
			  {
				  rc = GetPanelRect(i, TRUE);
				  if (rc.top == m_szMargin.cy)
				  {
					  nAdd = rc.bottom - rc.top + m_cyPadding;
					  break;
				  }
				  else if (rc.top > m_szMargin.cy && rc.top - m_cyPadding > 0)
				  {
					  nAdd = rc.top - m_cyPadding;
					  break;
				  }
			  }

			  if (si.nMax >=  si.nPos + nAdd + m_cyPadding)
			  {
				  si.nPos += nAdd;
			  }

		  }
         break;           
      case SB_PAGEUP:
         // User clicked the shaft above the scroll box
		  bRedraw = true;
		  {
			  RECT rc;
			  int nAdd = 0;
			  for( int i = 0; i < m_aPanels.GetSize(); i++ ) 
			  {
				  rc = GetPanelRect(i, TRUE);
				  int nTop = rc.top;
				  if (nTop >= m_szMargin.cy)
				  {
					  rc = GetPanelRect(i - 1, TRUE);

					  nAdd = rc.bottom - rc.top - nTop;
					  break;
				  }
			  }
			  si.nPos -= nAdd + m_cyPadding;
		  }
         break;           
      case SB_PAGEDOWN:
         // User clicked the shaft below the scroll box
		  bRedraw = true;
		  {
			  RECT rc;
			  int nAdd = 0;
			  for( int i = 0; i < m_aPanels.GetSize(); i++ ) 
			  {
				  rc = GetPanelRect(i, TRUE);
				  if (rc.top == m_szMargin.cy)
				  {
					  nAdd = rc.bottom - rc.top + m_cyPadding;
					  break;
				  }
				  else if (rc.top > m_szMargin.cy && rc.top - m_cyPadding > 0)
				  {
					  nAdd = rc.top - m_cyPadding;
					  break;
				  }
			  }

			  if (si.nMax >=  si.nPos + nAdd + m_cyPadding)
			  {
				  si.nPos += nAdd;
			  }

		  }
         break;           
      case SB_THUMBTRACK:
		  bRedraw = true;
         // User dragged the scroll box
         si.nPos = si.nTrackPos;
         break;           
      default:
         break; 
      }
      // Set the position and then retrieve it.  Due to adjustments
      //   by Windows it may not be the same as the value set.
      si.fMask = SIF_POS;
      SetScrollInfo(SB_VERT, &si, TRUE);
      GetScrollInfo(SB_VERT, &si);
      if( si.nPos != yPos ) 
	  {                   
		  m_bRePos = false;
         _RecalcLayout();
		 PostMessage(WM_PAINT);
		 PostMessage(MSG_REARRANGE_PANELS);
         Invalidate();
      } 
	  m_nScrollCurPos = si.nPos;//hub
      return 0;
   }
   LRESULT OnMouseWheel(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      int zDelta = (int) (short) HIWORD(wParam);

	  SendMessage(WM_VSCROLL, MAKEWPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0), NULL);
      return 0;
   }
   LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
	  SetFocus();
      POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      for( int i = 0; i < m_aPanels.GetSize(); i++ ) 
	  {
        // Limit click area to button (or just header)
        RECT rc = GetPanelRect(i, TRUE);
		RECT rcCheckBox = rc;
		SIZE szIcon;
		m_Images.GetIconSize(szIcon);

	    rcCheckBox.left = rcCheckBox.left + m_aPanels[i].szCheckBox.cx;
		rcCheckBox.right = rcCheckBox.left + szIcon.cx;
		rcCheckBox.top = rcCheckBox.top + m_aPanels[i].szCheckBox.cy;
		rcCheckBox.bottom = rcCheckBox.top + szIcon.cy;

		if (::PtInRect(&rcCheckBox, pt) && m_aPanels[i].bEnabled)
		{
			m_aPanels[i].bCheck = !m_aPanels[i].bCheck;
			InvalidateRect( &rcCheckBox );
			SendMessage(GetParent().GetParent(), m_msgCheck, i, m_aPanels[i].bCheck );
		}
		else if ( m_aPanels[i].rtTrust.PtInRect(pt) && m_aPanels[i].bEnabled && !m_bScaning )
		{
			SendMessage( GetParent().GetParent(), m_msgTrust, i, (LPARAM)(LPCTSTR)(m_aPanels[i].strItemName) );
		}
		else
		{
			rc.top += m_cyButton - m_cyBar;
			rc.bottom = rc.top + m_cyBar;
			if( (m_dwExtStyle & CPS_EX_EXPANDCLICK) != 0 ) 
				::SetRect(&rc, rc.right - 30, rc.top + 3, rc.right, rc.bottom - 3);
			if( ::PtInRect(&rc, pt) ) 
			{
				 m_iCurSel = i;
				 SetItemExpanded(i, !GetItemExpanded(i), TRUE );
				 if( (m_dwExtStyle & CPS_EX_SELHIGHLIGHT) != 0 ) 
					 _RecalcLayout();
				 break;
			}
		}
      }
      return 0;
   }
   LRESULT OnMouseActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      LRESULT lRes = DefWindowProc();
      POINT pt = { 0 };
      ::GetCursorPos(&pt);
      ScreenToClient(&pt);
      for( int i = 0; i < m_aPanels.GetSize(); i++ )
	  {
         RECT rc = GetPanelRect(i, FALSE);
         if( ::PtInRect(&rc, pt) ) 
		 {
            //if( GetItemEnabled(i)) //&& m_iCurSel != i ) 
			{
               m_iCurSel = i;
               if( (m_dwExtStyle & CPS_EX_SELHIGHLIGHT) != 0 ) 
			   {
				   _RecalcLayout();
				   _RecalcScrollBar();
			   }
               ::SetFocus(m_aPanels[m_iCurSel].hWnd);
            }
            break;
         }
      }
      return lRes;
   }
  
   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
	   CDCHandle	dc( (HDC)wParam );
	   CDC			MemDc;
	   CBitmap		MemBmp;

	   CRect rcClient;
	   GetClientRect(&rcClient);
	   CRect rcWnd;
	   GetWindowRect( &rcWnd );
	   int nWidth = rcWnd.Width() - rcClient.Width();

	   MemDc.CreateCompatibleDC(NULL);
	   MemBmp.CreateCompatibleBitmap(dc, rcClient.Width(), rcClient.Height());

	   HBITMAP hOldBmp = MemDc.SelectBitmap(MemBmp);
	   MemDc.FillSolidRect( &rcClient, RGB(0xff, 0xff, 0xff) );


	   ::InflateRect(&rcClient, -m_szMargin.cx, -m_szMargin.cy);

	   for( int i = 0; i < m_aPanels.GetSize(); i++ )
	   {
		   RECT rcItem = GetPanelRect(i, TRUE);
		   rcItem.right += nWidth;
		   RECT rcButton = rcItem;
		   rcButton.bottom = rcButton.top + m_cyButton;
		   RECT rcDummy = { 0 };
		   if( !::IntersectRect(&rcDummy, &rcClient, &rcButton) ) 
			   continue;

		   m_bRePos = true;

		   PANEL& panel = m_aPanels[i];
		   RECT rcBar = { rcButton.left, rcButton.bottom - m_cyBar, rcButton.right, rcButton.bottom };
		   if( panel.hbmpButton == NULL ) 
			   _CreateButton(MemDc.m_hDC, i, rcBar);
		   ATLASSERT(panel.hbmpButton != NULL);
		   // Paint button
		   CDC dcBmp;
		   dcBmp.CreateCompatibleDC(MemDc);
		   HBITMAP hOldBmp2 = dcBmp.SelectBitmap(panel.hbmpButton);
		   MemDc.BitBlt(rcBar.left, rcBar.top >= 0 ? rcBar.top : 0, rcBar.right - rcBar.left, rcBar.bottom, dcBmp, 0, rcBar.top >= 0 ? 0 : 0 - rcBar.top, SRCCOPY);
		   dcBmp.SelectBitmap(hOldBmp2);

		   // Rect for text
		   RECT rcText = rcButton;
		   rcText.left += 0;
		   rcText.right -= 4;
		   rcText.top = rcText.bottom - m_cyBar;
		   // Add expander
		   if (m_nFlag == 0)//hub  未结束查看
		   {
			   if( (m_dwExtStyle & CPS_EX_NOEXPANDBUTTON) == 0 ) 
			   {
				   hOldBmp2 = dcBmp.SelectBitmap( panel.bExpanded ? m_bmpCollapse : m_bmpExpand );
				   MemDc.TransparentBlt(rcText.left + 8, 
					   rcBar.top + (rcBar.bottom - rcBar.top - 15) / 2, 
					   15, 15,
					   dcBmp, 
					   0, 0,
					   15, 15,
					   RGB(0xFF, 0x00, 0xFF));
				   rcText.right -= 26;
			   }			   
		   }
		   else//结束查看
		   {
			   if( (m_dwExtStyle & CPS_EX_NOEXPANDBUTTON) == 0 ) 
			   {
				   hOldBmp2 = dcBmp.SelectBitmap( panel.bExpanded ? m_bmpCollapse : m_bmpExpand );
				   MemDc.TransparentBlt(rcBar.right - 44, 
					   rcBar.top + (rcBar.bottom - rcBar.top - 15) / 2, 
					   15, 15,
					   dcBmp, 
					   0, 0,
					   15, 15,
					   RGB(0xFF, 0x00, 0xFF));
				   rcText.right -= 26;
			   }
		   }
		  
		   
		   // Add icon
		   if( panel.iImage >= 0 && !m_Images.IsNull() ) {
			   SIZE szIcon;
			   m_Images.GetIconSize(szIcon);
			   POINT pt = { rcItem.left + 2, rcItem.top + 2 };
			   if( !panel.bEnabled && !m_ImagesGrey.IsNull() ) {
				   m_ImagesGrey.Draw(MemDc, panel.iImage, pt, ILD_TRANSPARENT);
			   }
			   else {
				   m_Images.Draw(MemDc, panel.iImage, pt, ILD_TRANSPARENT);
			   }
			   rcText.left += szIcon.cx;
		   }

		   CSize sizeIcon;
		   m_Images.GetIconSize( sizeIcon );
 		   POINT pt = { rcText.left + 8, rcText.top + (rcText.bottom - rcText.top - sizeIcon.cy) / 2 };

		   if (m_nFlag != 0)//hub 扫描结束
		   {
			   if (panel.bEnabled)
			   {
				   m_Images.Draw(MemDc, (int)panel.bCheck > 0 ? 3 : 2, pt, ILD_TRANSPARENT );
			   }
			   else
				   m_Images.Draw(MemDc, (int)panel.bCheck > 0 ? 4 : 2, pt, ILD_TRANSPARENT );
		   }		  

		   m_aPanels[i].szCheckBox.cx = pt.x - rcButton.left;
		   m_aPanels[i].szCheckBox.cy = pt.y - rcButton.top; 

		   rcText.left += pt.x + sizeIcon.cx;
		   rcText.right -= 25;

		   // Print text
		   COLORREF clrText = RGB(0, 0, 0);
		   if( clrText == CLR_INVALID ) 
			   clrText = m_Theme.clrTextActive;
		   if( i == m_iCurSel && (m_dwExtStyle & CPS_EX_SELHIGHLIGHT) != 0 ) clrText = m_Theme.clrTextSel;
		   if( !m_aPanels[i].bEnabled ) 
			   clrText = m_Theme.clrTextInactive;


		   UINT nOldMode = MemDc.SetBkMode(TRANSPARENT);                
		   COLORREF crOldText = MemDc.SetTextColor(clrText);
		   HFONT hOldFont = MemDc.SelectFont(m_fntBold);
		   CString strTmp = _T("[");
		   CSize szText;
		   MemDc.GetTextExtent(strTmp, strTmp.GetLength(), &szText);
		   MemDc.DrawText(strTmp, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
		   MemDc.SelectFont(hOldFont);


		   hOldFont = MemDc.SelectFont(m_fntNormal);

		   rcText.left += szText.cx + 3;
		   strTmp = panel.strType;
		   MemDc.GetTextExtent(strTmp, strTmp.GetLength(), &szText);
		   MemDc.DrawText(strTmp, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
		   MemDc.SelectFont(hOldFont);


		   hOldFont = MemDc.SelectFont(m_fntBold);

		   rcText.left += szText.cx + 3;
		   strTmp = _T("]");
		   MemDc.GetTextExtent(strTmp, strTmp.GetLength(), &szText);
		   MemDc.DrawText(strTmp, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);		   
		   rcText.left += szText.cx + 8;
		   strTmp = panel.strItemName;
		   CRect rcItemName = rcText;
		   MemDc.GetTextExtent(strTmp, strTmp.GetLength(), &szText);
		   rcItemName.right = rcItemName.left + szText.cx;
		   MemDc.SelectFont(hOldFont);
		   
		   MemDc.SetTextColor(crOldText);
		   
		   rcText.left = rcText.right - 74;
		   rcText.right += 8;
// 		   if( panel.bShowTrust )//去掉右上角的信任
// 		   {
// 			    if( panel.bEnabled && !m_bScaning )
// 					crOldText = MemDc.SetTextColor( RGB(28, 116, 4) );
// 				else
// 					crOldText = MemDc.SetTextColor( RGB(0xCC, 0xCC, 0xCC) );
// 
// 				strTmp = TEXT("信任");
// 				hOldFont = MemDc.SelectFont(m_ftnLink);
// 				MemDc.GetTextExtent(strTmp, strTmp.GetLength(), &szText);
// 				panel.rtTrust = rcText;
// 				panel.rtTrust.left = panel.rtTrust.right - szText.cx;
// 				panel.rtTrust.top = ( rcText.top + rcText.bottom ) / 2 - szText.cy / 2;
// 				panel.rtTrust.bottom = panel.rtTrust.top + szText.cy;
// 				MemDc.DrawText(strTmp, -1, &(panel.rtTrust), DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_RIGHT );
// 
// 				MemDc.SetTextColor(RGB(4, 82, 144));
// 				rcText.right -= szText.cx;
// 				strTmp = panel.strAction;
// 				strTmp += TEXT("/");
// 				hOldFont = MemDc.SelectFont(m_fntNormal);
// 				MemDc.DrawText(strTmp, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_RIGHT );
// 		   }
// 		   else
		   {
			    strTmp = panel.strAction;
				rcText.right -= 14;
				crOldText = MemDc.SetTextColor(RGB(4, 82, 144));
				hOldFont = MemDc.SelectFont(m_fntNormal);
				MemDc.DrawText(strTmp, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_RIGHT );
				xLite = rcText.left;
		   }

		   if (!panel.bReboot)
		   {
			   MemDc.SetTextColor(crOldText);
			   crOldText = MemDc.SetTextColor(RGB(0, 0, 0));
			   strTmp = _T("");
			   MemDc.GetTextExtent(strTmp, strTmp.GetLength(), &szText);
			   rcText.left = rcText.left - szText.cx;
			   MemDc.DrawText(strTmp, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
		   }
		   MemDc.SelectFont(hOldFont);


		   MemDc.SetTextColor(crOldText);
		   crOldText = MemDc.SetTextColor(panel.clrLevel);
		   hOldFont = MemDc.SelectFont(m_fntBold);

		   //等级
		   strTmp = panel.strLevel;
		   MemDc.GetTextExtent(strTmp, strTmp.GetLength(), &szText);
		   rcText.right = rcText.left;
		   rcText.left = rcText.right - 80;
		   MemDc.DrawText(strTmp, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER);

		   if ( m_bShowEngName )
		   {
			   //引擎
			   MemDc.SetTextColor(crOldText);
			   hOldFont = MemDc.SelectFont(m_fntNormal);
			   strTmp = panel.strEngine;
			   MemDc.GetTextExtent(strTmp, strTmp.GetLength(), &szText);
			   rcText.right = rcText.left;
			   rcText.left = rcText.right - 90;
			   MemDc.DrawText(strTmp, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER);
		   }


		   MemDc.SetTextColor(crOldText);
		   crOldText = MemDc.SetTextColor(RGB(45, 61, 76));
		   strTmp = panel.strItemName;
		   if (rcItemName.right > rcText.left - 8)
			   rcItemName.right = rcText.left - 8;
		   MemDc.DrawText(strTmp, -1, &rcItemName, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
		   panel.rtText = rcItemName;
		   CRect rc(rcItemName);
		   m_wndToolTip.AddTool(m_hWnd,(LPCTSTR)strTmp,rc,1);

		   MemDc.SetTextColor(crOldText);  
		   MemDc.SelectFont(hOldFont);
		   MemDc.SetBkMode(nOldMode);

		   // Optional border around child
		   if( GetStyle() & WS_BORDER )
		   {
			   RECT rcChild = rcItem;
			   rcChild.top += m_cyButton;
			   CPen pen;
			   pen.CreatePen(PS_SOLID, 1, m_Theme.clrBorder);
			   HPEN hOldPen = MemDc.SelectPen(pen);
			   HBRUSH hOldBrush = MemDc.SelectStockBrush(NULL_BRUSH);
			   MemDc.Rectangle(&rcChild);
			   MemDc.SelectBrush(hOldBrush);
			   MemDc.SelectPen(hOldPen);
		   }
	   }


	   dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), MemDc, 0, 0, SRCCOPY);
	   MemDc.SelectBitmap(hOldBmp);

	   return TRUE;
   }

   void CheckAll(bool bCheck = true)
   {
	   for( int i = 0; i < m_aPanels.GetSize(); i++ )
	   {
		   if (m_aPanels[i].bEnabled)
		   {
			   m_aPanels[i].bCheck = bCheck;
		   }
	   }
	   Invalidate();
   }

   void SetItemCheck( int nIndex, BOOL bCheck )
   {
		if( nIndex < m_aPanels.GetSize() )
			m_aPanels[nIndex].bCheck = bCheck;

		Invalidate();
   }

   bool IsCheckAll()
   {
	   bool bRet = true;
	   for( int i = 0; i < m_aPanels.GetSize(); i++ )
	   {
		   if (!m_aPanels[i].bCheck)
		   {
			   bRet = false;
			   break;
		   }
	   }

	   return bRet;
   }

   bool IsMoreOneCheck()
   {
	   bool bRet = true;
	   for( int i = 0; i < m_aPanels.GetSize(); i++ )
	   {
		   if (m_aPanels[i].bCheck)
		   {
			   bRet = true;
			   break;
		   }
	   }

	   return bRet;
   }
};

class CCollapsiblePanelCtrl : public CCollapsiblePanelImpl<CCollapsiblePanelCtrl>
{
public:
   DECLARE_WND_CLASS(_T("WTL_CollapsiblePanel"))
};


#endif // !defined(AFX_COLLAPSIBLEPANEL_H__20031129_797C_31F4_B029_0080AD509054__INCLUDED_)


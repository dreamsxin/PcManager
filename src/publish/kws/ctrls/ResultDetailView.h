// CollapsibleTestView.h : interface of the CCollapsibleTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLAPSIBLETESTVIEW_H__2B5D4682_F2E0_4514_8C8B_9823859F866A__INCLUDED_)
#define AFX_COLLAPSIBLETESTVIEW_H__2B5D4682_F2E0_4514_8C8B_9823859F866A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "atlgdix.h"
#include "CollapsiblePanel.h"
#include "ScanResultListPanel.h"
#include "scan/KLogic.h"
#include "ListTypes.h"
#include <wtlhelper/whwindow.h>


class CResultDetailView : public CCollapsiblePanelImpl<CResultDetailView>
{
public:
   DECLARE_WND_CLASS(_T("ResultDetailView"))

   typedef CCollapsiblePanelImpl<CResultDetailView> parentClass;
   CListArray<CScanResultListPanel*> m_Panels;    // Collection of panels

   BEGIN_MSG_MAP(CResultDetailView)
	  MESSAGE_HANDLER(WM_MSG_FIX_STATE, OnFixState)
	  MESSAGE_HANDLER(WM_MSG_SETWHITE,  OnSetWhite)
      MESSAGE_HANDLER(WM_CREATE,		OnCreate)
	  MESSAGE_HANDLER(WM_DESTROY,		OnDestroy)
	  MESSAGE_HANDLER(MSG_APP_VIRUS_DEL_LIST,  OnDeList)
	  MESSAGE_HANDLER(WM_MSG_ISSCANING,  IsScaning)
	  MESSAGE_HANDLER(WM_MSG_XLITE,		 GetlXite)
      CHAIN_MSG_MAP( parentClass )
   END_MSG_MAP()

   CResultDetailView()
   :m_Fixing( FALSE )
   {
	   
   }

   CFont						m_BoldFont;
   CFont						m_NormalFont;
   CFont						m_LinkFont;
   CBrush						m_brush;
   BOOL							m_Fixing;
   KLogic						m_Logic;

   void setFlag(int nFlag)//hub  设置扫描过程查看和扫描结束查看的的标志
   {
		m_nFlag = nFlag;
   }

   void SetFixing( BOOL bFixing )
   {
		m_Fixing = bFixing;

		for ( int i = 0; i < m_Panels.GetSize(); i++)
		{
			CScanResultListPanel* pPanel = m_Panels[i];
			if( pPanel && pPanel->IsWindow() && pPanel->IsWindowVisible() )
				pPanel->Invalidate();
		}
		
   }

   LRESULT OnFixState(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
		return m_Fixing;
   }
   BOOL IsScaning(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
	   return m_bScaning;
   }
   long GetlXite(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)//判断是否存在scroll
   {	  
	  return xLite;
   }
   
   LRESULT OnSetWhite(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
	   SendMessage( GetParent().GetParent(), MSG_APP_SETWHITE, NULL, NULL );
	   return TRUE;
   }
   LRESULT OnDeList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)//hub
   {
	   //找到要删除的对应项
	   CScanResultListPanel* pPanelWnd = (CScanResultListPanel*)wParam;
	   CString str = L"";
	   bool bTrustScript = (bool)lParam;
	   int nIndex = -1;
	   for( int i = 0; i < m_Panels.GetSize(); i++ )
	   {
		   //找到是哪个panel
		   CScanResultListPanel* pPanel = m_Panels[i];	
		   if (pPanelWnd == pPanel)
		   {			   
			   nIndex = i;
			   if (bTrustScript == true)//信任脚本
			   {
				   str = m_aPanels[i].strItemName;
			   }
			   break;
		   }
	   }
	   
	   if (nIndex != -1)
		   SendMessage( GetParent().GetParent(), MSG_APP_VIRUS_DEL_LIST_2, (WPARAM)nIndex, (LPARAM)(LPCTSTR)str );
	   return TRUE;
   }

   LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
      LRESULT lRes = parentClass::OnCreate(uMsg, wParam, lParam, bHandled);

	  SetExtendedPanelStyle(CPS_EX_SELHIGHLIGHT);

	  //SetExpandBitmaps(IDI_EXPAND, IDI_COLLAPSE);
	  SetExpandBitmaps( BkBmpPool::GetBitmap(IDB_EXPAND), BkBmpPool::GetBitmap(IDB_COLLPEND));

	  m_brush.CreateSolidBrush(RGB(255, 255, 255));
	  LOGFONT lf;
	  CFontHandle font = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
	  font.GetLogFont(&lf);
	  _tcscpy( lf.lfFaceName, TEXT("宋体") );
	  lf.lfHeight = -12;
	  m_NormalFont = ::CreateFontIndirect(&lf);

	  lf.lfWeight = FW_BOLD;
	  m_BoldFont = ::CreateFontIndirect(&lf);

	  lf.lfWeight = FW_NORMAL;
	  lf.lfUnderline = TRUE;
	  m_LinkFont = ::CreateFontIndirect(&lf);

      return lRes;
   }

   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
	   for( int i = 0; i < m_Panels.GetSize(); i++ ) 
	   {
		   if (m_Panels[i])
		   {
			   m_Panels[i]->DestroyWindow();
			   delete m_Panels[i];
		   }
	   }

	   bHandled = FALSE;
	   return 0;
   }

   bool Add(int nIndex, const S_FIX_ITEM_EX& Item)
   {
		bool	bRet		= false;
		
		CString strSrcItemName = Item.strItemName.c_str();
		CString strFile = Item.strFile.c_str();
		CString strSubKey = Item.strSubKey.c_str();

		CString strType;
		m_Logic.GetTypeStringEx(Item.emType,Item.emLevel, strSrcItemName,strType);

		CString strLevel;
		COLORREF clrLevel = m_Logic.GetLevelString(Item.emLevel, strLevel);

		CString strAction;
		m_Logic.GetActionString(Item.ActionType, strAction);

		
		CString strItemName;
		m_Logic.GetItemNameString(Item.emType,strSrcItemName,strFile,strSubKey,strItemName);

		CString strEngine;
		m_Logic.GetEnginString( Item.emType, Item.scanEngineType, strEngine );

		CScanResultListPanel* pPanel = new CScanResultListPanel;
		CRect rc;
		GetClientRect(rc);
		pPanel->SetResHandle( m_NormalFont, m_BoldFont, m_LinkFont, m_brush );
		pPanel->Create(m_hWnd, rc);
		pPanel->SetDetail(Item);

		int nItem = 0;
		BOOL bVirus = m_Logic.IsRealVirus( Item.emType, Item.emLevel );

		nItem = InsertItem(
			nIndex,
			Item.uIndex,
			Item.emType,
			Item.dwID,
			Item.emLevel,
			Item.ActionType,
			pPanel, 
			strType, 
			bVirus ? strFile: strItemName, 
			strEngine,
			strLevel, 
			strAction, 
			Item.bScriptCanBeTrusted,
			-1, 
			clrLevel
			);

		if (nIndex == -1)
		{
			m_Panels.Add(pPanel);
		}
		else
			m_Panels.InsertAt(nIndex, pPanel);


		bRet = true;
		return bRet;
   }

   void DeleteItem(int nIndex)
   {
	   if( nIndex < 0 || nIndex >= m_aPanels.GetSize() ) 
		   return;

	   if( m_aPanels[nIndex].hbmpButton != NULL ) 
		   ::DeleteObject(m_aPanels[nIndex].hbmpButton);
	   m_aPanels.RemoveAt(nIndex);

	   if (m_Panels[nIndex] && m_Panels[nIndex]->IsWindow())
	   {
		   m_Panels[nIndex]->DestroyWindow();
		   delete m_Panels[nIndex];
		   m_Panels.RemoveAt(nIndex);
	   }

	   _RecalcLayout();
	   _RearrangePanels();
	   _RecalcScrollBar();
   }

   void DeleteAll()
   {
		DeleteAllItems();
		for( int i = 0; i < m_Panels.GetSize(); i++ ) 
		{
			if (m_Panels[i])
			{
				if (m_Panels[i]->IsWindow())
					m_Panels[i]->DestroyWindow();
				delete m_Panels[i];
				m_Panels[i] = NULL;
			}
		}

		m_Panels.RemoveAll();
   }
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLLAPSIBLETESTVIEW_H__2B5D4682_F2E0_4514_8C8B_9823859F866A__INCLUDED_)

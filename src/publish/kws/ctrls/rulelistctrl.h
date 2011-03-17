////////////////////////////////////////////////////////////////////////////////
//      
//      File for kwsplugin 
//      
//      File      : rulelistctrl.h
//      Version   : 1.0
//      Comment   : 网盾木马查杀日志查看器
//      
//      Create at : 2010-09-08
//		Create by : chenguicheng  
//      
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "whitelistctrl.h"

class KRuleListCtrl : public CWhiteListCtrl
					, COwnerDraw<KRuleListCtrl>
{
public:

	int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/)
	{
		// all items are equal
		return 0;
	}

	BOOL DeleteItem(LPDELETEITEMSTRUCT lpItem)
	{
		return TRUE;
	}

	BOOL DeleteItem( int nIndex )
	{
		return CWhiteListCtrlBase::DeleteItem( nIndex );
	}

	LRESULT OnDrawItem(UINT , WPARAM , LPARAM lParam, BOOL& bHandled)
	{
		LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
		return 0;
	}

	void MeasureItem(LPMEASUREITEMSTRUCT pMeasure)
	{
		if( GetHeight() > 0)
		{
			pMeasure->itemHeight = GetHeight();
		}
		else
		{
			COwnerDraw<KRuleListCtrl>::MeasureItem(pMeasure);
		}
	}

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItem)
	{
		CDCHandle dc(lpDrawItem->hDC);
		CRect rc(lpDrawItem->rcItem);
		int nSelected = GetSelectedIndex();
		int nColumnCount = GetColumnCount();
		int nItemID = lpDrawItem->itemID;
		COLORREF dwTextColor = 0;

		// 绘制背景
		if ((UINT)nSelected == lpDrawItem->itemID)
			dc.FillSolidRect(rc, RGB(0xB9,0xDB,0xFF));
		else
			dc.FillSolidRect(rc, RGB(0xFF, 0xFF, 0xFF));
		//每行底部的分隔线
		CRect rcDivRow = rc;
		rcDivRow.top = rcDivRow.bottom - 1;
		dc.FillSolidRect(rcDivRow, RGB( 0xEA, 0xEA, 0xEA));

		CRect rect;
		int nState = 0;
		HFONT hOldFont = NULL;
		int nFmt = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;

		for(int i = 0 ; i < nColumnCount; i++)
		{
			GetSubItemRect( nItemID, i, LVIR_BOUNDS, &rect );
			CString str;
			rect.right = rect.left + GetColumnWidth(i);

			switch( i )
			{
				//case 0:
			case 0:
				hOldFont = dc.SelectFont( m_font );
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				nFmt |= DT_LEFT;
				rect.left += 5;
				break;
			case 1:
// 				hOldFont = dc.SelectFont( m_fontLink );
// 				dwTextColor = dc.SetTextColor( RGB(48, 100, 165) );
				hOldFont = dc.SelectFont( m_font );
				dwTextColor = dc.SetTextColor( RGB(0x00, 0x00, 0x00) );
				nFmt |= DT_CENTER;
				break;
			}

			GetItemText( nItemID, i, str );
			dc.DrawText( str, -1, &rect, nFmt );

			if( hOldFont )
				dc.SelectFont( hOldFont );

			dc.SetTextColor( dwTextColor );	
		}	
	}
public:
	BEGIN_MSG_MAP(KRuleListCtrl)
		CHAIN_MSG_MAP_ALT(COwnerDraw<KRuleListCtrl>,1)
		CHAIN_MSG_MAP(CWhiteListCtrl)		
	END_MSG_MAP()
};
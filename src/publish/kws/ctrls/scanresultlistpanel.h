// ScanResultListPanel.h : interface of the CScanResultListPanel class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "kws/KAVPublic.h"
#include "scan/KLogic.h"
#include "beikesafemsgbox.h"
#include "KStatic.h"
#include "ListTypes.h"
#include "PlugViewDetailDlg.h"
#include "safemon/safetrayshell.h"
#define URL_WITDH_IN_PANEL			300	//网址显示的宽度，超出此长度将进行截断
#define URL_WIDTH_IN_COMMONDDLG		120
#define FILE_WIDTH_IN_COMMONDDLG	200
#define WM_MSG_FIX_STATE			( WM_APP+ 23 )
#define WM_MSG_SETWHITE				( WM_APP+ 24 )
#define WM_MSG_ISSCANING			( WM_APP+ 25 )
#define WM_MSG_XLITE				( WM_APP+ 26 )

typedef struct tagViewInfo{
	CString strUrl;
	CString strFile;
	CString strPos;
	CString strLink;
}VIEWINFO;

extern VIEWINFO gInfo;

class CScanResultListPanel : 
	public CWindowImpl<CScanResultListPanel>
{
public:
	

	typedef enum _EM_LINK_TYPE
	{
		em_link_url,
		em_link_file
	}EM_LINK_TYPE;

	typedef struct _DETAIL_ITEM
	{
		CString		strText;
		COLORREF	clrText;
		CRect		rc;
		bool		bBold;
		bool		bLink;
		bool		bTooltips;
		CString		strData;
		CString		strVirPath;
		DWORD		dwFormat;
		EM_LINK_TYPE emLinkType;
		bool		bTrustScript;
		_DETAIL_ITEM()
		{
			strText		= _T("");
			strData		= _T("");
			strVirPath	= _T("");
			bTrustScript = false;
		}
	} DETAIL_ITEM;

	typedef struct _COMPRESS_CONFIG
	{
		DWORD dwFormat;	//给文字添加额外的格式
		int nWidth;		//文字固定宽度
	}COMPRESS_CONFIG;


	CSimpleArray<DETAIL_ITEM>		m_DetailItemList;
	CFontHandle						m_BoldFont;
	CFontHandle						m_NormalFont;
	CFontHandle						m_LinkFont;
	CBrushHandle					m_brush;
	HCURSOR						m_hCursor;
	CToolTipCtrl				m_ttToolTip;
	BOOL						m_bSetWhite;
	CString						m_sPosInfo;			//hub
	CString						m_sUrlInfo;			//hub
	CString						m_sFileInfo;		//hub
	CString						m_sLinkInfo;

	CScanResultListPanel()
	{
		m_hCursor = NULL;
		m_bSetWhite = FALSE;
		m_sPosInfo = _T("");
		m_sUrlInfo = _T("");
		m_sFileInfo = _T("");
	}

	~CScanResultListPanel()
	{
		if(m_hCursor != NULL)
			::DestroyCursor(m_hCursor);
	}

	BOOL IsSetWhite()
	{
		return m_bSetWhite;
	}

	BEGIN_MSG_MAP(CScanResultListPanel)
		MESSAGE_HANDLER(WM_CREATE, OnInitDialog)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColor)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColor)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	static DWORD GetWndStyle(DWORD dwStyle)
	{
		return WS_CHILD;
	}
	static DWORD GetWndExStyle(DWORD dwExStyle)
	{
		return 0;
	}

	void SetResHandle( HFONT hNorFont, HFONT hBoldFont, HFONT hLinkFont, HBRUSH hBrush )
	{
		m_BoldFont = hBoldFont;
		m_NormalFont = hNorFont;
		m_LinkFont = hLinkFont;
		m_brush = hBrush;
	}

	BOOL IsFixing( )
	{
		return ::SendMessage( GetParent(), WM_MSG_FIX_STATE, NULL, NULL );
	}
	//hub
	BOOL IsScaning()
	{
		return ::SendMessage( GetParent(), WM_MSG_ISSCANING, NULL, NULL );
	}
	long GetXlite()
	{
		return ::SendMessage( GetParent(), WM_MSG_XLITE, NULL, NULL );
	}
	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_hCursor = ::LoadCursor(NULL, IDC_HAND);

		m_ttToolTip.Create(m_hWnd);
		m_ttToolTip.Activate(TRUE);

		return TRUE;
	}

	LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CDCHandle dc = (HDC)wParam;
		dc.SetBkMode(TRANSPARENT);

		return (LRESULT)m_brush.m_hBrush;
	}

	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CDCHandle		dc( (HDC)wParam );
		CDC				MemDc;
		CBitmap			MemBmp;

		CRect rcClient;
		GetClientRect(&rcClient);

		BOOL bFixing = IsFixing();
		BOOL bScaning = IsScaning();		

		MemDc.CreateCompatibleDC(NULL);
		MemBmp.CreateCompatibleBitmap(dc, rcClient.Width(), rcClient.Height());

		HBITMAP hOldBmp = MemDc.SelectBitmap(MemBmp);
		MemDc.FillSolidRect( &rcClient, RGB(0xff, 0xff, 0xff) );


		for (int i = 0; i < m_DetailItemList.GetSize(); i++)
		{
			DWORD dwFlags = DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX;
			UINT nOldMode = MemDc.SetBkMode(TRANSPARENT);                

			COLORREF crOldText = MemDc.SetTextColor(m_DetailItemList[i].clrText);
			HFONT hFont = NULL;
			if (m_DetailItemList[i].bLink)
			{
				if( bFixing )
					MemDc.SetTextColor( RGB(0xCC, 0xCC, 0xCC));
				hFont = m_LinkFont.m_hFont;
			}
			else
			{
				if (m_DetailItemList[i].bBold)
				{
					hFont = m_BoldFont.m_hFont;
				}
				else
					hFont = m_NormalFont.m_hFont;
			}

			HFONT hOldFont = MemDc.SelectFont(hFont);    

			dwFlags |= m_DetailItemList[i].dwFormat;
			if (m_DetailItemList[i].strText == _T("信任") && bScaning)
			{	
				continue;
			}
			if (m_DetailItemList[i].strText == _T("信任"))
			{	
				long xlite = GetXlite();
				m_DetailItemList[i].rc.right = xlite + 70;	
				m_DetailItemList[i].rc.left = m_DetailItemList[i].rc.right - 24;					
			}

			MemDc.DrawText(m_DetailItemList[i].strText, -1, m_DetailItemList[i].rc, dwFlags);   

			MemDc.SelectFont(hOldFont);
			MemDc.SetTextColor(crOldText);  
			MemDc.SetBkMode(nOldMode);			
		}


		dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), MemDc, 0, 0, SRCCOPY);
		MemDc.SelectBitmap(hOldBmp);

		rcClient.top = rcClient.bottom - 1;
		dc.FillSolidRect(  &rcClient, RGB(0xCC, 0xCC, 0xCC) );

		return TRUE;
	}

	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };


		for( int i = 0; i < m_DetailItemList.GetSize(); i++ ) 
		{
			if (m_DetailItemList[i].bLink && ::PtInRect(&m_DetailItemList[i].rc, pt) && !IsFixing())
			{
				if( m_DetailItemList[i].strText == TEXT("查看详情") )
					ViewInfo( pt );
				else
					SetWhite(m_DetailItemList[i]);
			}
		}

		return 0;
	}
	
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

		for( int i = 0; i < m_DetailItemList.GetSize(); i++ ) 
		{
			if (m_DetailItemList[i].bLink && ::PtInRect(&m_DetailItemList[i].rc, pt) && !IsFixing())
			{
				::SetCursor(m_hCursor);
			}

			if(m_DetailItemList[i].bTooltips && ::PtInRect(&m_DetailItemList[i].rc, pt))
			{
				if(m_ttToolTip.IsWindow())
				{
					MSG msgRelay = { m_hWnd, uMsg, wParam, lParam };
					m_ttToolTip.RelayEvent(&msgRelay);
				}
			}
		}

		return 0;
	}
	
	
	void ViewInfo( POINT& pt)
	{
		
		gInfo.strFile		= m_sFileInfo;
		gInfo.strUrl		= m_sUrlInfo;
		gInfo.strPos		= m_sPosInfo;
		gInfo.strLink		= m_sLinkInfo;

		GetParent().GetParent().GetParent().PostMessage( MSG_APP_VIRUS_VIEW_INFO, (WPARAM)&gInfo, NULL );

		return;
	}

	void SetWhite(DETAIL_ITEM& Item)
	{
		S_TRUST_LIST TrustList;
		EM_TRUST_ITEM_TYPE emItemType;

		if(Item.emLinkType == em_link_url)
		{
			emItemType = enum_TRUST_ITEM_TYPE_URL;
		}
		else
		{
			emItemType = enum_TRUST_ITEM_TYPE_FILE;
		}
		
		if (Item.strText == _T("添加为信任"))
		{
			CString strMsg;
			CString strCompress;
			CompressString(Item.strData,strCompress,URL_WIDTH_IN_COMMONDDLG,Item.emLinkType);

			CBkSafeMsgBox2 msgbox;
			msgbox.AddButton(L"取消", IDCANCEL);
			msgbox.AddButton(L"添加信任", IDOK);
			
			if(Item.emLinkType == em_link_url)
			{
				strMsg.Format(_T("您确定添加网址 %s 为信任\r\n\r\n项吗？(添加后将不再处理此项)"), strCompress);
			}
			else
			{
				strMsg.Format(_T("您确定添加文件 %s 为信任\r\n\r\n项吗？(添加后将不再处理此项)"), strCompress);
			}
			
			UINT_PTR uRet = msgbox.ShowMutlLineMsg(strMsg, NULL, MB_BK_CUSTOM_BUTTON | MB_ICONWARNING);

			if( IDOK != uRet )
				return;

			TrustList.operation = enum_TRUST_LIST_ADD;
			Item.strText = _T("取消信任");
			TrustList.itemType = emItemType;
			TrustList.vecItemList.push_back((LPCTSTR)Item.strData);

			//if( Item.emLinkType == em_link_file )
			//	AddFileToBk( Item.strData );

			m_bSetWhite = TRUE;
		}
		else if (Item.strText == _T("取消信任"))
		{
			TrustList.operation = enum_TRUST_LIST_DELETE;
			Item.strText = _T("添加为信任");
			TrustList.itemType = emItemType;
			TrustList.vecItemList.push_back((LPCTSTR)Item.strData);

			//if( Item.emLinkType == em_link_file )
			//	DelFileFromBk( Item.strData );

			m_bSetWhite = FALSE;
		}
		else if (Item.strText == _T("信任"))
		{
			//“信任”的功能
			CBkSafeMsgBox2 msgbox;
			msgbox.AddButton(L"确定", IDOK);
			msgbox.AddButton(L"取消", IDCANCEL);
			CRect rc(0, 0, 250, 52);
			UINT_PTR uRet = msgbox.ShowPanelMsg(BkString::Get(IDS_VIRSCAN_2726), &rc, NULL, MB_BK_CUSTOM_BUTTON | MB_ICONWARNING, NULL, NULL);			
			if( IDOK != uRet )
				return;
			else
			{
				//选择了确定   在异常项目中删除此项目 MSG_APP_VIRUS_DEL_LIST
				GetParent().PostMessage( MSG_APP_VIRUS_DEL_LIST, (WPARAM)this, (LPARAM)Item.bTrustScript );
				
				if (Item.bTrustScript != true)//文件信任
				{						
					TrustList.operation = enum_TRUST_LIST_ADD;
					TrustList.itemType = emItemType;
					if (Item.strVirPath == _T(""))
					{
						TrustList.vecItemList.push_back((LPCTSTR)Item.strData);
					}
					else//病毒
					{
						TrustList.vecItemList.push_back((LPCTSTR)Item.strVirPath);
					}
				//	m_bSetWhite = TRUE;
				}
			}
		}


// 		TrustList.itemType = emItemType;
// 		TrustList.vecItemList.push_back((LPCTSTR)Item.strData);

		KLogic Logic;
		HRESULT lRetCode = Logic.SetUserTrustList(TrustList);
		CSafeMonitorTrayShell::WhiteListLibUpdated();

		SendMessage( GetParent(), WM_MSG_SETWHITE, NULL, NULL );

		InvalidateRect(Item.rc);
	}

	/*
	void AddFileToBk( LPCTSTR pszFile )
	{
		CWhiteList wl;
		CWhiteListItem wli;
		wli.m_filepath = pszFile;
		wl.Initialize();
		wl.AddWhiteItem(wli);
		wl.Uninitialize();

		CBkUtility util;

		util.Initialize();
		util.WhiteListUpdate();
		util.Uninitialize();

		CSafeMonitorTrayShell::WhiteListLibUpdated();
	}

	void DelFileFromBk( LPCTSTR pszFile )
	{
		CWhiteList wl;
		CAtlArray<CWhiteListItem> wliArray;
		wl.Initialize();

		CWhiteListItem wli;
		wli.m_filepath = pszFile;
		wliArray.Add(wli);

		if ( wliArray.GetCount() > 0 )
			wl.DeleteWhiteItem(wliArray);

		wl.Uninitialize();

		CBkUtility util;

		util.Initialize();
		util.WhiteListUpdate();
		util.Uninitialize();

		CSafeMonitorTrayShell::WhiteListLibUpdated();
	}
	*/

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if ( m_ttToolTip.IsWindow() )
			m_ttToolTip.DestroyWindow();
		else
			m_ttToolTip.m_hWnd = NULL;

		return 0;
	}

	bool CreateDetailItem(
		const CString& strText,
		const CPoint& ptStart,
		int	nClientWidth,
		bool bBold,
		CPoint& ptEnd,
		COMPRESS_CONFIG* pCompressConf = NULL)
	{
		bool bRet = false;
		//CString strConvert = strText;

		//if(pCompressConf)
		//{
			//CompressString(strText,strConvert,*pCompressConf);
		//}

		{
			CClientDC dcClient(m_hWnd);
			HFONT hOldFont = dcClient.SelectFont(bBold ? m_BoldFont : m_NormalFont);
			CSize sizeExtent;
			dcClient.GetTextExtent(strText, -1, &sizeExtent);


			int nRest = nClientWidth - ptStart.x;
			//int nRow = sizeExtent.cx / nRest;
			//nRow += sizeExtent.cx % nRest > 0 ? 1 : 0;
			//int nAdd = sizeExtent.cy * nRow;
			
			CRect rc(ptStart.x, ptStart.y, ptStart.x + nRest, ptEnd.y);			
			if(pCompressConf)
			{
				dcClient.DrawText(strText, -1, rc, DT_LEFT | DT_SINGLELINE | DT_EDITCONTROL | DT_NOPREFIX | DT_CALCRECT);
			}
			else
			{
				dcClient.DrawText(strText, -1, rc, DT_LEFT | DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX | DT_CALCRECT);
			}
			dcClient.SelectFont(hOldFont);

			DETAIL_ITEM Item;
			if(pCompressConf)
			{
				rc.right = rc.left + pCompressConf->nWidth;

				Item.bTooltips = TRUE;
				Item.dwFormat = pCompressConf->dwFormat;
				//最后一个参数是控件id，这里好像没什么用，随便传个1
				m_ttToolTip.AddTool(m_hWnd,(LPCTSTR)strText,rc,1);
			}
			else
			{
				Item.bTooltips = FALSE;
				Item.dwFormat = 0;
			}			
			Item.bBold = bBold;
			Item.rc = rc;
			Item.bLink = false;
			Item.strText = strText;
			Item.clrText = RGB(0, 0, 0);			
			m_DetailItemList.Add(Item);

			ptEnd.x = rc.right;
			ptEnd.y = rc.bottom;
		}


		bRet = true;
		return bRet;
	}
	bool CreateDetailItemSetWhite(
		const CString& strText,
		const CString& strData,
		const CPoint& ptStart,
		int	nClientWidth,
		CPoint& ptEnd,
		EM_LINK_TYPE emType,
		CString strVirPath = _T(""),
		bool bTrustScript = false)
	{
		bool bRet = false;


		{
			CClientDC dcClient(m_hWnd);
			HFONT hOldFont = dcClient.SelectFont(m_LinkFont);
			CSize sizeExtent;
			dcClient.GetTextExtent(strText, -1, &sizeExtent);
			dcClient.SelectFont(hOldFont);


			CRect rc;
			int nRest = nClientWidth - ptStart.x;
			if (nRest < sizeExtent.cx)
			{
				rc.left = nClientWidth - sizeExtent.cx;
				rc.right = nClientWidth;
				rc.top = ptEnd.y;
				rc.bottom = rc.top + sizeExtent.cy;
			}
			else
			{
				rc.left = ptStart.x;
				rc.right = rc.left + sizeExtent.cx;
				rc.top = ptStart.y;
				rc.bottom = rc.top + sizeExtent.cy;
			}
			ptEnd.x = rc.right;
			ptEnd.y = rc.bottom;			
			
			DETAIL_ITEM Item;
			Item.dwFormat = 0;
			Item.bBold = false;
			Item.rc = rc;			
			Item.bLink = true;
			Item.emLinkType = emType;
			Item.strText = strText;
			Item.clrText = RGB(28, 116, 4);
			Item.strData = strData;
			Item.strVirPath = strVirPath;//病毒之用
			Item.bTrustScript = bTrustScript;
			m_DetailItemList.Add(Item);
		}

		bRet = true;
		return bRet;
	}

	bool CreateDetailCategoryWithSingleItem(
		const CString& strTitle,
		const CString& strItem,
		CPoint& ptStart,
		int	nClientWidth,
		int nWordsInter,
		CPoint& ptEnd,
		bool bSetWhite = false,
		COMPRESS_CONFIG* pConfig = NULL,
		EM_LINK_TYPE emLinkType = em_link_url,
		bool bSetTrustStat = false,//hub add
		CString strVirPath = _T(""),
		bool bTrustScript = false//脚本信任
		)
	{
		bool bRet = false;


		CPoint ptNext;
		bRet = CreateDetailItem(
			strTitle,
			ptStart,
			nClientWidth,
			false,
			ptNext);
		KAV_PROCESS_ERROR(bRet);

		ptStart.x = ptNext.x + nWordsInter;
		bRet = CreateDetailItem(
			strItem,
			ptStart,
			nClientWidth,
			false,
			ptNext,
			pConfig);
		//KAV_PROCESS_ERROR(bRet);
		//hub
		if (strTitle == _T("位置："))
		{
			CPoint ptStart(ptNext.x + nWordsInter * 3, ptStart.y);
			bRet = CreateDetailItemSetWhite(
				_T("查看详情"),
				strItem,
				ptStart,
				nClientWidth,
				ptNext,
				emLinkType
				);
		}
		if (bSetTrustStat)
		{
			CPoint ptStart(ptNext.x + nWordsInter * 3, ptStart.y);
			bRet = CreateDetailItemSetWhite(
				_T("信任"),
				strItem,
				ptStart,
				nClientWidth,
				ptNext,
				emLinkType,
				strVirPath,
				bTrustScript);
		}

		if (bSetWhite)
		{
			CPoint ptNewStart(ptNext.x + nWordsInter * 3, ptStart.y);
			if(pConfig)
			{
				ptNewStart.x = ptStart.x + pConfig->nWidth + nWordsInter;
			}
			bRet = CreateDetailItemSetWhite(
				_T("添加为信任"),
				strItem,
				ptNewStart,
				nClientWidth,
				ptNext,
				emLinkType);
			//KAV_PROCESS_ERROR(bRet);
		}

		ptEnd = ptNext;


		bRet = true;
Exit0:
		return bRet;
	}

	bool CreateDetailCategoryWithMultiItem(
		const CString& strTitle,
		const std::vector<std::wstring>& strItemList,
		CPoint& ptStart,
		int	nClientWidth,
		int nWordsInter,
		CPoint& ptEnd,
		bool bSetWhite = false,
		COMPRESS_CONFIG* pConfig = NULL,
		EM_LINK_TYPE emLinkType = em_link_url)		//只有在bSetWhite为true时，emLinkType才有效
	{
		bool bRet = false;


		CPoint ptNext;
		bRet = CreateDetailItem(
			strTitle,
			ptStart,
			nClientWidth,
			false,
			ptNext);
		KAV_PROCESS_ERROR(bRet);


		{
			ptStart.x = ptNext.x + nWordsInter;
			CString strItem;
			for (int i = 0; i < strItemList.size(); i++)
			{
				strItem = strItemList[i].c_str();
				KStatic* pItem = NULL;
				bRet = CreateDetailItem(
					strItem,
					ptStart,
					nClientWidth,
					false,
					ptNext,
					pConfig);
				//KAV_PROCESS_ERROR(bRet);


				if (bSetWhite)
				{
					CPoint ptNewStart(ptNext.x + nWordsInter, ptStart.y);
					if(pConfig)
					{
						ptNewStart.x = ptStart.x + pConfig->nWidth + nWordsInter;												
					}				
					
					bRet = CreateDetailItemSetWhite(
						_T("添加为信任"),
						strItem,
						ptNewStart,
						nClientWidth,
						ptNext,
						emLinkType);
					//KAV_PROCESS_ERROR(bRet);
				}
				

				ptStart.y = ptNext.y;
			}
		}


		ptEnd = ptNext;


		bRet = true;
Exit0:
		return bRet;
	}

	//缩进字符串，对于过长的URL和文件名，按规定进行缩进。
	void CompressString(const CString& strOrig,CString& strDes,int nWidth,EM_LINK_TYPE emLinkType)
	{
		strDes = strOrig;

		CClientDC dcClient(m_hWnd);
		HFONT hOldFont = dcClient.SelectFont(m_NormalFont);
		CSize sizeExtent;
		dcClient.GetTextExtent(strOrig,-1,&sizeExtent);

		if(sizeExtent.cx<nWidth)
			goto Exit0 ;

		

		if(emLinkType == em_link_url)	//尾部截断型
		{
			CString strCompress = strOrig;
			int nPosRight = strCompress.GetLength()-1;
			int nPosLeft = 0;
			int nPos = nPosRight/2;	

			while((nPosLeft != nPosRight) && (1 != nPosLeft-nPosRight) && (-1 != nPosLeft-nPosRight))
			{
				dcClient.GetTextExtent(strOrig.Left(nPos-3)+L"...",-1,&sizeExtent);

				if(sizeExtent.cx < nWidth)
				{
					nPosLeft = nPos;
					nPos = nPosRight - (nPosRight - nPos)/2;
				}
				else if(sizeExtent.cx > nWidth)
				{
					nPosRight = nPos;
					nPos = nPosLeft + (nPos - nPosLeft)/2;
				}
				else
				{
					nPosLeft = nPos;
					nPosRight = nPos;
				}

			}
			strDes = strOrig.Left(nPosLeft-3)+L"...";
		}
		else if(emLinkType == em_link_file)		//直接取文件名
		{
			int nStart = strOrig.ReverseFind('\\');
			CString strTemp;
			if(nStart>=0)
			{
				strTemp = strOrig.Right(strOrig.GetLength() - nStart -1);
				//strTemp = strOrig;
			}
			else
			{
				strTemp = strOrig;
			}
			CompressString(strTemp,strDes,FILE_WIDTH_IN_COMMONDDLG,em_link_url);
		}

Exit0:
		dcClient.SelectFont(hOldFont);
		return ;
	}

	BOOL CanTrust(EM_POINT_LEVEL emLevel)
	{
		BOOL bRet = FALSE;
		switch(emLevel)
		{
		case enum_POINT_LEVEL_RISK:
		case enum_POINT_LEVEL_UNKNOWN:
		case enum_POINT_LEVEL_DOUBT:
		case enum_POINT_LEVEL_SAFE:
		case enum_POINT_LEVEL_NORMAL:
		case enum_POINT_LEVEL_ABNORMAL:
		case enum_POINT_LEVEL_SYS_FILE_ABNORMAL:
		case enum_POINT_LEVEL_NET_DETECT_FAILED:
			bRet = TRUE;
			break;
		}
		return bRet;
	}

	BOOL IsRealVirus( EM_SCAN_TYPE nType, EM_POINT_LEVEL nLevel )
	{
		BOOL bRet = FALSE;

		if( nType == enum_Startup_Point || nType == enum_BK_TROJAN_Point )
		{
			switch( nLevel)
			{
			case enum_POINT_LEVEL_VIRUS:
			case enum_POINT_LEVEL_TROJAN:
			case enum_POINT_LEVEL_BACKDOOR:
			case enum_POINT_LEVEL_WORM:
			case enum_POINT_LEVEL_ROOTKIT:
			case enum_POINT_LEVEL_DANGER:
				bRet = TRUE;
				break;
			default:
				bRet = FALSE;
			}
		}

		return bRet;
	}

	void SetDetail(const S_FIX_ITEM_EX& Item)
	{		
		if (m_DetailItemList.GetSize())
			return;

		const CPoint ptBegin(25, 10);
		const CPoint ptEnd(20, 10);
		const CPoint ptWordsInter(5, 0);
		CPoint ptStart;
		CPoint ptNext;
		KLogic logic;

		CRect rcClient;
		GetWindowRect(rcClient);
		ScreenToClient(rcClient);

		int nClientHeight = ptStart.y + ptEnd.y;

		CString strSrcDesc = Item.strDescript.c_str();
		CString strItemName = Item.strItemName.c_str();
		bool bSetTrustStat = false;//hub
		bool bTrustScript = false;

		switch (Item.emType)
		{
		case enum_System_Abnormal_Point:
		case enum_System_Abnormal_Point_Browser:
		case enum_Browser_Home:
		case enum_DeskTop_Icon:
		case enum_Ie_Link:
		case enum_Sys_Icon:
		case enum_Ie_Setting:
		case enum_Search_engine:
		case enum_Browser_Link:
		case enum_Abnormal_Url:
		case enum_Http_Protocol:
		case enum_Other_Sys_Abnormal_Point:
		case enum_Danger_Plugin://hub
			{
				// 描述
				if (Item.bScriptCanBeTrusted)
				{
					bSetTrustStat = true;
					bTrustScript = true;
				}
				CString strItem;				
				logic.GetDescStringEx(Item.emType,Item.emLevel, Item.ActionType, strItemName,strSrcDesc,strItem);
				ptStart = ptBegin;
				CreateDetailCategoryWithSingleItem(
					_T("描述："),
					strItem,
					ptStart,
					rcClient.Width() - ptEnd.x,
					ptWordsInter.x,
					ptNext,
					false,
					NULL,
					em_link_url,
					bSetTrustStat,
					L"",
					bTrustScript);

				// 网址
				for (int i = 0; i < Item.MaliciousInfo.vecItem.size(); i++)
				{
					if (Item.MaliciousInfo.vecItem[i].strType == _T("Url") 
						&& Item.MaliciousInfo.vecItem[i].vecValue.size())
					{
						ptStart.x = ptBegin.x;
						ptStart.y = ptNext.y + ptEnd.y;
						COMPRESS_CONFIG config;
						config.nWidth = URL_WITDH_IN_PANEL;
						config.dwFormat = DT_END_ELLIPSIS;						
						CreateDetailCategoryWithMultiItem(
							_T("网址："),
							Item.MaliciousInfo.vecItem[i].vecValue,
							ptStart,
							rcClient.Width() - ptEnd.x,
							ptWordsInter.x,
							ptNext,
							true,
							&config,
							em_link_url);

						//hub  网址
						wstring sItemUrlInfo;
						for (int j = 0; j < Item.MaliciousInfo.vecItem[i].vecValue.size(); j++)
						{
							sItemUrlInfo = Item.MaliciousInfo.vecItem[i].vecValue[j];
							m_sUrlInfo += sItemUrlInfo.c_str();
							m_sUrlInfo += "\r\n";
						}												
						
					}
				}
				
				// 文件  
				//文件和位置  调换了位置  hub
				for (int i = 0; i < Item.MaliciousInfo.vecItem.size(); i++)
				{
					if (Item.MaliciousInfo.vecItem[i].strType == _T("FilePath") 
						&& Item.MaliciousInfo.vecItem[i].vecValue.size())
					{
						ptStart.x = ptBegin.x;
						ptStart.y = ptNext.y + ptEnd.y;
						COMPRESS_CONFIG config;
						config.nWidth = URL_WITDH_IN_PANEL;
						config.dwFormat = DT_PATH_ELLIPSIS;
						CreateDetailCategoryWithMultiItem(
							_T("文件："),
							Item.MaliciousInfo.vecItem[i].vecValue,
							ptStart,
							rcClient.Width() - ptEnd.x,
							ptWordsInter.x,
							ptNext,
							false,
							&config,
							em_link_file);

						//hub  文件
						wstring sItemFileInfo;
						for (int j = 0; j < Item.MaliciousInfo.vecItem[i].vecValue.size(); j++)
						{
							sItemFileInfo = Item.MaliciousInfo.vecItem[i].vecValue[j];
							m_sFileInfo += sItemFileInfo.c_str();
							m_sFileInfo += "\r\n";
						}												
						
					}
				}

				// 位置	
				for (int i = 0; i < Item.MaliciousInfo.vecItem.size(); i++)
				{					
					if (Item.MaliciousInfo.vecItem[i].strType == _T("Location") 
						&& Item.MaliciousInfo.vecItem[i].vecValue.size())
					{
						ptStart.x = ptBegin.x;
						ptStart.y = ptNext.y + ptEnd.y;
						wstring sItemPosInfo;						
						sItemPosInfo = Item.MaliciousInfo.vecItem[i].vecValue[0];//hub
						strItem = sItemPosInfo.c_str();
						COMPRESS_CONFIG config;
						config.nWidth = URL_WITDH_IN_PANEL;
						config.dwFormat = DT_PATH_ELLIPSIS;
						CreateDetailCategoryWithSingleItem(//hub
							_T("位置："),
							strItem,
							ptStart,
							rcClient.Width() - ptEnd.x,
							ptWordsInter.x,
							ptNext,
							false,
							&config);
						break;//hub
					}
				}
				//hub  位置				
				for (int i = 0; i < Item.MaliciousInfo.vecItem.size(); i++)
				{		
					if (Item.MaliciousInfo.vecItem[i].strType == _T("Location") 
						&& Item.MaliciousInfo.vecItem[i].vecValue.size())
					{
						wstring sItemPosInfo;
						for (int j = 0; j < Item.MaliciousInfo.vecItem[i].vecValue.size(); j++)
						{
							sItemPosInfo = Item.MaliciousInfo.vecItem[i].vecValue[j];//hub
							if( sItemPosInfo.size() > 1 && sItemPosInfo.at( 1 ) == TEXT(':') )
							{	
								m_sLinkInfo += sItemPosInfo.c_str();
								m_sLinkInfo += _T("\r\n");
							}
							else
							{
								m_sPosInfo += sItemPosInfo.c_str();
								m_sPosInfo += _T("\r\n");
							}
							
						}						
					}					
				}	

				nClientHeight += ptNext.y;
			}
			break;
		case enum_AV_Point:
			goto Exit0;
			break;
		case enum_BK_TROJAN_Point:
		case enum_Startup_Point:

		default:
			{
				CString strItem;
				logic.GetDescStringEx(Item.emType,Item.emLevel, Item.ActionType, strItemName,strSrcDesc,strItem);
				ptStart = ptBegin;
				ptNext = ptBegin;
				// 文件
				//文件和位置  调换了位置  hub
				if (!Item.strFile.empty())
				{
					CString strFile = Item.strFile.c_str();					
					//只有是启动点，文件存在,并且风险等级合适的项，才可以“添加为信任”
					if( PathFileExists(strFile)/*&& CanTrust(Item.emLevel)*/)//先判断此文件是否存在
					{
// 						COMPRESS_CONFIG config;
// 						config.nWidth = URL_WITDH_IN_PANEL;
// 						config.dwFormat = DT_PATH_ELLIPSIS;
						bSetTrustStat = true;
					}
					if(IsRealVirus( Item.emType, Item.emLevel) )
					{
						CString strVirusName;
						logic.AliPayVirus( Item.strItemName.c_str(), strVirusName );						
						CreateDetailCategoryWithSingleItem(
							_T("病毒名："),
							strVirusName,
							ptStart,
							rcClient.Width() - ptEnd.x,
							ptWordsInter.x,
							ptNext,
							false,
							NULL,
							em_link_file,
							bSetTrustStat,
							strFile);
						m_sFileInfo = strFile;
						m_sFileInfo += "\r\n";
					}
					else
					{
						CreateDetailCategoryWithSingleItem(
							_T("文件："),
							strFile,
							ptStart,
							rcClient.Width() - ptEnd.x,
							ptWordsInter.x,
							ptNext,
							false,
							NULL,
							em_link_file,
							bSetTrustStat);
						m_sFileInfo = strFile;
						m_sFileInfo += "\r\n";
					}
				}
				
				// 描述
				if (bSetTrustStat == false)
				{
					if (Item.bScriptCanBeTrusted)//信任脚本的
					{
						bSetTrustStat = true;
						bTrustScript = true;
					}
				}
				else
					bSetTrustStat = false;

				ptStart.x = ptBegin.x;
				ptStart.y = ptNext.y + ptEnd.y;

				CreateDetailCategoryWithSingleItem(
					_T("描述："),
					strItem,
					ptStart,
					rcClient.Width() - ptEnd.x,
					ptWordsInter.x,
					ptNext,
					false,
					NULL,
					em_link_url,
					bSetTrustStat,
					L"",
					bSetTrustStat);


				// 位置
				if (!Item.strLocation.empty() )
				{
					if (Item.strSubKey.empty())
					{
						strItem = Item.strLocation.c_str();						
					}
					else
						strItem = (Item.strLocation + _T("\\[") + Item.strSubKey + _T("]")).c_str();

					if( Item.strLocation.size() > 1 && Item.strLocation.at( 1 ) == TEXT(':') )
					{	
						m_sLinkInfo = Item.strLocation.c_str();
						m_sLinkInfo += _T("\r\n");
					}
					else
					{
						m_sPosInfo = strItem;//hub
						m_sPosInfo += "\r\n";
					}
					
					
					//如果位置跟文件相同则去掉一个
					if( strItem != Item.strFile.c_str() )
					{
						COMPRESS_CONFIG config;
						ptStart.x = ptBegin.x;
						ptStart.y = ptNext.y + ptEnd.y;
						config.nWidth = URL_WITDH_IN_PANEL;
						config.dwFormat = DT_PATH_ELLIPSIS;
						CreateDetailCategoryWithSingleItem(
							_T("位置："),
							strItem,
							ptStart,
							rcClient.Width() - ptEnd.x,
							ptWordsInter.x,
							ptNext,
							false,
							&config);
					}
				}
				
				nClientHeight += ptNext.y;
			}
			break;
		}	// switch (Item.emType)


		rcClient.bottom = rcClient.top + nClientHeight;
		MoveWindow(rcClient);

Exit0:
		return;
	}
};

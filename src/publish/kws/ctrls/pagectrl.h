#pragma once
#include <vector>

struct HIT_TEST_ITEM 
{
	int		nPage;
	CRect	rect;
};

#define WM_MSG_PAGE_CHANGE		(WM_USER+1232)
#define COUNT_PRE_PAGE			80

class CPageCtrl : public CDoubleBufferWindowImpl<CPageCtrl>
{
public:
	BEGIN_MSG_MAP(CPageCtrl)
		MESSAGE_HANDLER( WM_MOUSEMOVE,		OnMouseMove )
		MESSAGE_HANDLER( WM_CREATE,			OnCreate )
		MESSAGE_HANDLER( WM_LBUTTONDOWN,	OnLButtonDown )
		MESSAGE_HANDLER( WM_LBUTTONUP,		OnLButtonUp )
		CHAIN_MSG_MAP(CDoubleBufferImpl<CPageCtrl>)
	END_MSG_MAP()

	CPageCtrl();
	~CPageCtrl();

	enum{
		en_NONE = 0,
		en_FIRST_LINK,
		en_PRE_LINK,
		en_NEXT_LINK,
		en_PRE_BTN,
		en_NEXT_BTN,
		en_NUMBER,
	};

	void DoPaint( CDCHandle dc );
	void SetCnt( int nCnt, int nOnePageCnt );
	void SetNotifyHwnd( HWND hNotify );
	void ReSetCurPage();
	int  GetCurPage();
	int	 GetPrePage();
	int  GetMaxPage();

protected:

	LRESULT OnLButtonDown(UINT , WPARAM , LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT , WPARAM , LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT , WPARAM , LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT , WPARAM , LPARAM lParam, BOOL& bHandled);
	int		HitTestBtn( CPoint point );
	int		HitTestNum( CPoint point );
	int		HitTest( CPoint point );
	void	DrawBtnFlag( CDCHandle& dc, CRect& rtBtn, BOOL bLeft );
	int		DrawPageNum( CDCHandle& dc, CRect& rcText );
	void	_DrawOneNum( CDCHandle& dc, int& nRight, int nTop, int nNum );
	void	_DrawText( CDCHandle& dc, int& nRight, int nTop, LPCTSTR pszText );

	CRect m_rcFirstLink;
	CRect m_rcPreLink;
	CRect m_rcNextLink;
	CRect m_rcPreBtn;
	CRect m_rcNextBtn;
	CRect m_rcText;

	CFont	m_fontLink;
	CFont	m_fontNormal;
	CFont	m_fontBold;
	HCURSOR m_hCurBtn;
	int		m_nCurPage;
	int		m_nPrePage;
	int		m_nPadding;
	int		m_nItemCnt;
	int		m_nMaxPage;
	HWND	m_hNotify;
	std::vector<HIT_TEST_ITEM> m_vecHitTest;

};
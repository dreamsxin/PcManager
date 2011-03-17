////////////////////////////////////////////////////////////////////////////////
//      
//      Kingsoft File for shp routines file
//      
//      File      : shp_link.h
//      Comment   : simple html parser bar(progress bar) object class
//					
//      Create at : 2010-06-25
//      Create by : chenguicheng
//      
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "shpsupport.h"

#define SHP_BAR_ITEM_KEY						TEXT("bar")

// bar_type=0 背景和增长条都是3个图片组成
// bar_type=1 背景是3个图片，增长条(bar)是一个图片拉伸的
// bar_type=2 背景是3个图片，增长条(bar)是一个图片平铺
// bar_type=3 不使用图片的进度条，背景，增长条都是纯色
// 这些图片都支持透明颜色 mask_clr

#define SHP_BAR_ITEM_BAR_TYPE_KEY					TEXT("bar_type")

#define SHP_BAR_ITEM_BG_LEFT_SRC_KEY				TEXT("bg_left_src")
#define SHP_BAR_ITEM_BG_RIGHT_SRC_KEY				TEXT("bg_right_src")
#define SHP_BAR_ITEM_BG_SRC_KEY						TEXT("bg_src")
#define SHP_BAR_ITEM_BAR_SRC_KEY					TEXT("bar_src")
#define SHP_BAR_ITEM_BAR_LEFT_SRC_KEY				TEXT("bar_left_src")
#define SHP_BAR_ITEM_BAR_RIGHT_SRC_KEY				TEXT("bar_right_src")

#define SHP_BAR_ITEM_FONT_FACE_KEY				TEXT("font_face")
#define SHP_BAR_ITEM_FONT_SIZE_KEY				TEXT("font_size")
#define SHP_BAR_ITEM_FONT_WEIGHT_KEY			TEXT("font_weight")
#define SHP_BAR_ITEM_CLR_MASK_KEY				TEXT("mask_clr")
#define SHP_BAR_ITEM_SHOW_TEXT_KEY				TEXT("show_text")
#define SHP_BAR_ITEM_BG_CLR_KEY					TEXT("bg_clr")
#define SHP_BAR_ITEM_BG_BORDER_CLR_KEY			TEXT("bg_border_clr")
#define SHP_BAR_ITEM_BAR_CLR_KEY				TEXT("bar_clr")
#define SHP_BAR_ITEM_BAR_BORDER_CLR_KEY			TEXT("bar_border_clr")

#define SHP_BAR_ITEM_P_LEFT_KEY					TEXT("p_left")
#define SHP_BAR_ITEM_P_TOP_KEY					TEXT("p_top")
#define SHP_BAR_ITEM_P_BOTTOM_KEY				TEXT("p_bottom")
#define SHP_BAR_ITEM_P_RIGHT_KEY				TEXT("p_right")
#define SHP_BAR_ITEM_POS_KEY					TEXT("pos")


class shp_bar_item : public shp_dom_item
{
public:
	shp_bar_item()
		:m_i_bar_type(0),
		m_clr_mask( CLR_INVALID ),
		m_i_font_size(12),
		m_i_font_weight(400),
		m_i_p_left(0),
		m_i_p_right(0),
		m_i_p_top(0),
		m_i_p_bottom(0),
		m_i_pos(0),
		m_bg_clr(RGB(0xFF, 0x00, 0x00)),
		m_bg_border_clr(RGB(0xFF, 0xFF, 0x00)),
		m_bar_clr(RGB(0xFF, 0x00, 0xFF)),
		m_bar_border_clr(RGB(0x00, 0xFF, 0x00))
	{
		
	}

	~shp_bar_item()
	{
		
	}

	void set_pos( int i_pos )
	{
		m_i_pos = i_pos;
	}

	void set_show_text( int i_show_text )
	{
		m_i_show_text = i_show_text;
	}

	void set_bar_bmp_obj( HBITMAP bmp_bg, HBITMAP bmp_bg_left, HBITMAP bmp_bg_right,
							HBITMAP bmp_bar, HBITMAP bmp_bar_left, HBITMAP bmp_bar_right )
	{
		m_bmp_bg_left.Attach( bmp_bg_left );
		m_bmp_bg_right.Attach( bmp_bg_right );
		m_bmp_bg.Attach( bmp_bg );
		m_bmp_bar_left.Attach( bmp_bar_left );
		m_bmp_bar_right.Attach( bmp_bar_right );
		m_bmp_bar.Attach( bmp_bar );
	}

	//set_attribute( p, m )
	BEGIN_KEY_MAP()
		//to add new attribute map( key map to member )
		ADD_INT_KEY( SHP_BAR_ITEM_BAR_TYPE_KEY,			m_i_bar_type)
		ADD_STR_KEY( SHP_BAR_ITEM_BG_LEFT_SRC_KEY,		m_str_bg_left)
		ADD_STR_KEY( SHP_BAR_ITEM_BG_RIGHT_SRC_KEY,		m_str_bg_right)
		ADD_STR_KEY( SHP_BAR_ITEM_BG_SRC_KEY,			m_str_bg)
		ADD_STR_KEY( SHP_BAR_ITEM_BAR_SRC_KEY,			m_str_bar)
		ADD_STR_KEY( SHP_BAR_ITEM_BAR_LEFT_SRC_KEY,		m_str_bar_left)
		ADD_STR_KEY( SHP_BAR_ITEM_BAR_RIGHT_SRC_KEY,	m_str_bar_right)

		ADD_STR_KEY( SHP_BAR_ITEM_FONT_FACE_KEY,			m_str_font_face)
		ADD_INT_KEY( SHP_BAR_ITEM_FONT_SIZE_KEY,			m_i_font_size)
		ADD_INT_KEY( SHP_BAR_ITEM_FONT_WEIGHT_KEY,			m_i_font_weight)

		ADD_CLR_KEY( SHP_BAR_ITEM_CLR_MASK_KEY,				m_clr_mask)
		ADD_INT_KEY( SHP_BAR_ITEM_SHOW_TEXT_KEY,			m_i_show_text)

		ADD_CLR_KEY( SHP_BAR_ITEM_BG_CLR_KEY,				m_bg_clr)
		ADD_CLR_KEY( SHP_BAR_ITEM_BG_BORDER_CLR_KEY,		m_bg_border_clr)
		ADD_CLR_KEY( SHP_BAR_ITEM_BAR_CLR_KEY,				m_bar_clr)
		ADD_CLR_KEY( SHP_BAR_ITEM_BAR_BORDER_CLR_KEY,		m_bar_border_clr)


		ADD_INT_KEY( SHP_BAR_ITEM_P_LEFT_KEY,			m_i_p_left)
		ADD_INT_KEY( SHP_BAR_ITEM_P_TOP_KEY,			m_i_p_top)
		ADD_INT_KEY( SHP_BAR_ITEM_P_BOTTOM_KEY,			m_i_p_bottom)
		ADD_INT_KEY( SHP_BAR_ITEM_P_RIGHT_KEY,			m_i_p_right)
		ADD_INT_KEY( SHP_BAR_ITEM_POS_KEY,				m_i_pos )

		BASE_CLASS_PROCESS(shp_item_win32)
	END_KEY_MAP()


	void draw_item( HDC hdc, CRect* parent_rect, CRect *left_rect )
	{
		if ( !m_b_init )
		{
			_init_rect(parent_rect, left_rect);

			if ( m_i_bar_type >= 0  && m_i_bar_type <= 2 )
			{
				_init_bmp();
			}

			if( m_i_show_text )
				_init_font();

			m_b_init = true;
		}

		if ( get_visible() )
		{
			CRect rt_bar;
			rt_bar.left = m_draw_rect.left + m_i_p_left;
			rt_bar.top = m_draw_rect.top + m_i_p_top;
			rt_bar.right = m_draw_rect.right - m_i_p_right;
			rt_bar.bottom  = m_draw_rect.bottom - m_i_p_bottom;
			rt_bar.right = rt_bar.left + rt_bar.Width() * m_i_pos / 100;

			switch( m_i_bar_type )
			{
			case 0:
				draw_bar_33( hdc, rt_bar );
				break;
			case 1:
				draw_bar_31( hdc, rt_bar );
				break;
			case 2:
				draw_bar_32( hdc, rt_bar );
				break;
			case 3:
				draw_bar_nobmp( hdc, rt_bar );
				break;
			}
		}
	}


	//不处理hit_test 返回 NO_PROCESS
	LPCTSTR hit_test( HDC hdc, UINT umsg, int x, int y )
	{
		return NO_PROCESS;
	}

protected:

	void draw_bar_33( HDC hdc, CRect& rt_bar )
	{
		if( m_clr_mask != CLR_INVALID )
		{
			_draw_strech_bmp_mask( hdc, m_draw_rect, m_bmp_bg, m_bmp_bg_left, m_bmp_bg_right, m_clr_mask );
			_draw_strech_bmp_mask( hdc, rt_bar, m_bmp_bar, m_bmp_bar_left, m_bmp_bar_right, m_clr_mask );
		}
		else
		{
			_draw_strech_bmp( hdc, m_draw_rect, m_bmp_bg, m_bmp_bg_left, m_bmp_bg_right );
			_draw_strech_bmp( hdc, rt_bar, m_bmp_bar, m_bmp_bar_left, m_bmp_bar_right );
		}
		
		if( m_i_show_text )
			draw_text( hdc );

	}

	void draw_bar_31( HDC hdc, CRect& rt_bar )
	{
		if( m_clr_mask != CLR_INVALID )
		{
			_draw_strech_bmp_mask( hdc, m_draw_rect, m_bmp_bg, m_bmp_bg_left, m_bmp_bg_right, m_clr_mask );
			_draw_bmp_mask( hdc, rt_bar, m_bmp_bar, m_clr_mask );
		}
		else
		{
			_draw_strech_bmp( hdc, m_draw_rect, m_bmp_bg, m_bmp_bg_left, m_bmp_bg_right );
			_draw_bmp( hdc, rt_bar, m_bmp_bar );
		}
		
		if( m_i_show_text )
			draw_text( hdc );
	}

	void draw_bar_32( HDC hdc, CRect& rt_bar )
	{
		if( m_clr_mask != CLR_INVALID )
		{
			_draw_strech_bmp_mask( hdc, m_draw_rect, m_bmp_bg, m_bmp_bg_left, m_bmp_bg_right, m_clr_mask );
			draw_rect_with_bmp( hdc, rt_bar, m_bmp_bar );
		}
		else
		{
			_draw_strech_bmp( hdc, m_draw_rect, m_bmp_bg, m_bmp_bg_left, m_bmp_bg_right );
			draw_rect_with_bmp( hdc, rt_bar, m_bmp_bar );
		}
		
		if( m_i_show_text )
			draw_text( hdc );
	}

	void draw_bar_nobmp( HDC hdc, CRect& rt_bar )
	{
		CDCHandle dc( hdc );
		dc.FillSolidRect( m_draw_rect, m_bg_clr );
		CBrush brush_bg( ::CreateSolidBrush( m_bg_border_clr) );
		dc.FrameRect( m_draw_rect, brush_bg );

		dc.FillSolidRect( rt_bar, m_bar_clr );
		CBrush brush_bar( ::CreateSolidBrush( m_bar_border_clr) );
		dc.FrameRect( rt_bar, brush_bar );

		if( m_i_show_text )
			draw_text( hdc );
	}

	void draw_text( HDC hdc )
	{
		CDCHandle dc( hdc );
		shpstr str_text;
		str_text.Format( TEXT("%d%%"), m_i_pos );
		int nMode = dc.SetBkMode( TRANSPARENT );
		HFONT hOldFont = dc.SelectFont( m_h_font );
		shpclr clr_text = dc.SetTextColor( m_clr_text );
		dc.DrawText( str_text, -1, m_draw_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		dc.SetTextColor( clr_text );
		dc.SetBkMode( nMode );
		if( hOldFont )
			dc.SelectFont( hOldFont );
	}

	void _init_rect( CRect *p_rect, CRect *left_rect )
	{
		draw_rect_zero();

		if( p_rect )
		{
			m_draw_rect.left	= p_rect->left + m_i_left;
			m_draw_rect.top		= p_rect->top + m_i_top;
			m_draw_rect.right	= m_draw_rect.left + m_i_width;
			m_draw_rect.bottom	= m_draw_rect.top + m_i_height;
		}
	}
	

	void _init_font()
	{
		if ( m_str_font_face.GetLength() > 0 )
		{
			LOGFONT lf;
			memset(&lf, 0, sizeof(LOGFONT));
			lf.lfHeight = m_i_font_size ;
			lf.lfWeight = m_i_font_weight;
			_tcscpy( lf.lfFaceName, m_str_font_face );

			if (!m_h_font.IsNull())
			{
				m_h_font.DeleteObject();
			}

			m_h_font.CreateFontIndirect( &lf );
		}
	}

	CRect* get_draw_rect()
	{
		return &m_draw_rect;
	}

	void _init_bmp()
	{
		if ( m_str_bar.GetLength() > 0 )
			loadbmp( m_str_bar, m_bmp_bar );
		
		if ( m_str_bar_left.GetLength() > 0 )
			loadbmp( m_str_bar_left, m_bmp_bar_left );

		if ( m_str_bar_right.GetLength() > 0 )
			loadbmp( m_str_bar_right, m_bmp_bar_right);


		if ( m_str_bg.GetLength() > 0 )
			loadbmp( m_str_bg, m_bmp_bg );

		if ( m_str_bg_left.GetLength() > 0 )
			loadbmp( m_str_bg_left, m_bmp_bg_left );

		if ( m_str_bg_right.GetLength() > 0 )
			loadbmp( m_str_bg_right, m_bmp_bg_right );
	}

	void loadbmp( shpstr& str_bmp, CBitmap& bmp )
	{
		shpstr str_path( shp_path_hlp::Instance().get_img_path());
		str_path += str_bmp;
		HANDLE handle = NULL;
		handle = ::LoadImage(_AtlBaseModule.GetResourceInstance(), 
			str_path, IMAGE_BITMAP, 
			0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE);
		DWORD dwError = 0;
		if( !handle)
			dwError = GetLastError();
		assert( handle);
		bmp.Attach((HBITMAP) handle );
	}


private:

	//属性
	shpclr		m_bg_clr;
	shpclr		m_bg_border_clr;
	shpclr		m_bar_clr;
	shpclr		m_bar_border_clr;


	int			m_i_bar_type;
	shpclr		m_clr_mask;
	int			m_i_show_text;

	shpstr		m_str_font_face;
	int			m_i_font_size;
	int			m_i_font_weight;
	
	shpstr		m_str_bg_left;
	shpstr		m_str_bg_right;
	shpstr		m_str_bg;
	shpstr		m_str_bar;
	shpstr		m_str_bar_left;
	shpstr		m_str_bar_right;

	int			m_i_p_left;
	int			m_i_p_right;
	int			m_i_p_top;
	int			m_i_p_bottom;
	int			m_i_pos;

	CFont		m_h_font;
	CBitmap		m_bmp_bg_left;
	CBitmap		m_bmp_bg_right;
	CBitmap		m_bmp_bg;
	CBitmap		m_bmp_bar_left;
	CBitmap		m_bmp_bar_right;
	CBitmap		m_bmp_bar;
};

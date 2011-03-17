////////////////////////////////////////////////////////////////////////////////
//      
//      Kingsoft File for shp routines file
//      
//      File      : shpsupport.h
//      Comment   : simple html parser object base class
//					
//      Create at : 2008-09-04
//      Create by : chenguicheng
//      
////////////////////////////////////////////////////////////////////////////////

#pragma  once

#include <atlpath.h>
#include "shpitem.h"
#include "shpbase.h"


#define NO_PROCESS			NULL

//////////////////////////////////////////////////////////////////////////
// 该路径必须 “\\”结尾
//////////////////////////////////////////////////////////////////////////

class shp_path_hlp
{
public:
	shp_path_hlp()
	{

	}

	~shp_path_hlp()
	{

	}

	static shp_path_hlp& Instance()
	{
		static shp_path_hlp _obj;
		return _obj;
	}

	void set_img_path( LPCTSTR lpszPath )
	{
		TCHAR tszPath[MAX_PATH*2] = {0};
		GetModuleFileName( ModuleHelper::GetModuleInstance(), tszPath, MAX_PATH * 2 );
		CPath path( tszPath );
		path.RemoveFileSpec();
		path.Append( lpszPath );
	
		m_str_image_path = (LPCTSTR)path;
	}

	shpstr get_img_path( )
	{
		return m_str_image_path;
	}

private:
	shpstr m_str_image_path;
};

//////////////////////////////////
//用一个类抽象画函数实现(未完成)
//////////////////////////////////

//支持win32 API 画元素
class shp_item_win32 : public shp_item
{
public:
	virtual ~shp_item_win32()
	{
	};

	BEGIN_KEY_MAP()
		//to add new attribute map( key map to member )
		BASE_CLASS_PROCESS(shp_item)
	END_KEY_MAP()

	virtual void draw_item( HDC hdc, CRect* parent_rect, CRect *left_rect ) = 0;

	virtual CRect* get_draw_rect() = 0;

	//不处理hit_test 返回 false
	virtual LPCTSTR hit_test( HDC hdc, UINT umsg, int x, int y ) = 0;

};

typedef shp_tree_item< shp_item_win32 >		shp_dom_item;


//支持agg画元素

class shp_item_agg : public shp_item
{
	virtual void draw_item() = 0;
};

//完成 tree_item 定义
//可以使用下面方法定义新的元素
/*
class shp_div_item :public shp_item_win32
{
	virtual void draw_item(HDC hdc)
	{
		return;
	}
};

typedef tree_item< shp_div_item >		shp_div_obj; //定义最基本元素，基础没有变化
*/


//typedef shp_obj< ,div_item >  shp_div_obj; // 产生一个新的div元素
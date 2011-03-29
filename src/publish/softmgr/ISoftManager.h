/**
* @file    ISoftManager.h
* @brief   软件管理
* @author  liveck
* @date    2010/04/07 21:50
*/

#pragma once

typedef void (*GetInfoCallback)(void* soft,wchar_t* name,wchar_t* value);

static void GetInfoUseMap(void* soft,wchar_t* name,wchar_t* value)
{
	CAtlMap<CString,CString>* p =(CAtlMap<CString,CString>*)soft;
	p->SetAt(name,value);
}

typedef void (*GetCatlogCallback)(void* cat,wchar_t* name);

static void GetCatlogUseArr(void* cat,wchar_t* name)
{
	CAtlArray<CString>* p =(CAtlArray<CString>*)cat;
	p->Add(name);
}


// NECESS	-  necessary  必要的, 必需的, 必然的

struct NECESS_SOFT				// 必备里面软件的属性
{
public:
	NECESS_SOFT()
	{
		nSortIndex = 0;
	}
	bool operator < (NECESS_SOFT& cmp)
	{
		return nSortIndex<cmp.nSortIndex;
	}

	CString		strSoftID;		// 软件ID
	DWORD		nSortIndex;		// 排序
};

struct NECESS_TYPE
{
public:
	NECESS_TYPE()
	{
		nSortIndex = 0;
	}
	bool operator < (NECESS_TYPE& cmp)
	{
		return nSortIndex<cmp.nSortIndex;
	}

	CString		strTypeName;				// 分类名称
	DWORD		nSortIndex;					// 排序
	CSimpleArray<NECESS_SOFT>	softList;	// 必备软件
};

struct NECESS_GROUP
{
public:
	NECESS_GROUP()
	{
		nSortIndex = 0;
	}

	bool operator < (NECESS_GROUP& cmp)
	{
		return nSortIndex<cmp.nSortIndex;
	}

	CString		strGroupName;				// 组名字
	DWORD		nSortIndex;					// 组排序
	CSimpleArray<NECESS_TYPE>	typeList;	// 必备的type列表
};


struct ONE_KEY_SOFT				// 一键装机软件的属性
{
public:
	ONE_KEY_SOFT()
	{
		nSortIndex	= 0;
		bCheck		= FALSE;
	}
	bool operator < (ONE_KEY_SOFT& cmp)
	{
		return nSortIndex<cmp.nSortIndex;
	}
	CString		strSoftID;		// 软件ID
	DWORD		nSortIndex;		// 排序
	BOOL		bCheck;
	CString		strShowName;	// 显示名称
};
struct ONE_KEY_TYPE
{
public:
	ONE_KEY_TYPE()
	{
		nSortIndex = 0;
	}
	bool operator < (ONE_KEY_TYPE& cmp)
	{
		return nSortIndex<cmp.nSortIndex;
	}
	CString		strTypeName;				// 分类名称
	DWORD		nSortIndex;					// 排序
	CSimpleArray<ONE_KEY_SOFT>	softList;	// 必备软件
};
struct ONE_KEY_GROUP
{
public:
	ONE_KEY_GROUP()
	{
		nSortIndex = 0;
	}
	bool operator < (ONE_KEY_GROUP& cmp)
	{
		return nSortIndex<cmp.nSortIndex;
	}
	CString		strGroupName;				// 组名字
	DWORD		nSortIndex;					// 组排序
	CSimpleArray<ONE_KEY_TYPE>	typeList;	// 必备的type列表
};
class __declspec(uuid("2442A51E-7C0F-4225-BFEF-2B8B1EA3B8C5")) ISoftManager
{
public:
	/**
	* 加载软件库
	* @return   int
	* @param CString lib_file
	* @param bool reload
	*/
	virtual int LoadSoftDB(CString lib_file) = 0;

	/**
	* 清空软件库
	* @return   int
	*/
	virtual int FreeSoftDB() = 0;

	/**
	* 软件数量
	* @return   int
	* @param CString cat
	*/
	virtual int GetSoftCount(CString cat) = 0;

	/**
	* 根据id取软件信息
	* @return   int
	* @param CString id
	* @param CAtlMap<CString
	* @param CString> & soft
	*/
	virtual int GetSoftById(CString id, GetInfoCallback func ,void* pSoft) = 0;

	/**
	* 获取装机必备软件信息
	* @return   void*
	* @param CString cat
	*/
	virtual void* GetLast50Soft() = 0;

	/**
	* 获取装机必备软件信息
	* @return   void*
	* @param CString cat
	*/
	virtual void* GetEssentialSoft(CString cat,CString rank=L"") = 0;

	/**
	* 根据cat分类信息获取软件信息
	* 
	* @return   void*
	* @param CString cat cat为""时，取得全部软件信息
	* @param CString rank 排序热度依据
	*/
	virtual void* GetAllSoft(CString cat,CString rank=L"") = 0;

	/**
	* 获取所有装机必备软件信息
	*/
	virtual void* GetAllNecessSoft() = 0;


	/**
	* 根据关键词搜索
	* @return   void*
	* @param CString cat
	* @param CString rank 排序热度依据
	*/
	virtual void* SearchSoft(CString word,CString rank="") = 0;

	/**
	* GetAllSoft SearchSolft 之后用这个取得具体的信息
	* @return   int
	* @param void * pos
	* @param CAtlMap<CString,CString> & soft
	*/
	virtual int GetNextSoft(void* pos, GetInfoCallback func ,void* soft) = 0;

	
	/**
	* 获取完软件信息用这个释放pos，否则会内存泄露
	* @return   int
	* @param void * pos
	*/
	virtual int FinalizeGet(void* pos) = 0;

	/**
	* 加载热度文件
	* @return   CString rank_name 默认热度名称
	* @param CString lib_file 热度文件路径
	*/
	virtual CString LoadRank(CString lib_file) = 0;


	/**
	* 释放热度库
	* @return   int
	* @param CString rank_name
	*/
	virtual int FreeRank(CString rank_name) = 0;


	/**
	* 获取软件分类
	* @return   int 分类数量
	* @param CAtlArray<CString> & result
	*/
	virtual int GetCatalog(GetCatlogCallback func,void* para) = 0;

	/**
	* 猜测卸载一句话描述
	* @return   CString
	* @param CString name
	*/
	virtual CString GetUnistallInfo(CString name, GetInfoCallback func ,void* soft) = 0;
	

	///用于差量更新
	virtual int Combine(CString diff_file)=0;

	//获取类型
	virtual int GetCategroy(GetInfoCallback func ,void* pParam) = 0;


	// 加载装机必备的库
	virtual BOOL LoadNecessDat(LPCTSTR strLibFile) = 0;

	// 获取装机必备数组
	virtual BOOL GetNecessSoftInfo( CSimpleArray<NECESS_GROUP>** ppInfo ) = 0;

	virtual BOOL GetOneKeySoftInfo( CSimpleArray<ONE_KEY_GROUP>** ppInfo) = 0;
	// 释放装机必备数组内存
	virtual VOID FreeNecessInfo() = 0;

	// 把头部信息刷新进入数据库
	virtual void UpdateCache() = 0;

	//判断库文件跟缓存文件是否版本不一样
	virtual BOOL IsLibUpdate() = 0;
};
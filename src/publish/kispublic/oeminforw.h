// ----------------------------------------------------------------------------- 
//	FileName	: oeminforw.h
//	Author		: Zqy
//	Create On	: 2007-11-10 18:17:57
//	Description	: 
//
// ----------------------------------------------------------------------------- 
#ifndef _OEMINFORW_H_
#define _OEMINFORW_H_
// ----------------------------------------------------------------------------- 
#include <vector>


#pragma pack(4)
#pragma warning( push )
#pragma warning( disable : 4200 )


enum FDRecType
{
	rtStart	  = 0x100,	//第一个记录	
	rtGroup	  = 0x101,	//开始一个分组
	rtIntData = 0x102,	//整型			FDIntData
	rtStrData = 0x103,	//字符串类型	FDStrData
	rtEnd	  = 0xFFF,	//表示结束
};

struct FDPackInfoHeader
{
	WORD wType;
	WORD wTotalSize;
};
struct FDRecHeader
{
	WORD type;
	WORD len;
};

struct FDStart
{
	int filever;		//FeatureData本身的版本号
						//2008年 12
	int datas;			//有多少个FeatureData
	int reserved[4];	//保留，为0
	int groups;			//有多少个分组
	int offset[0];		//每个分组记录(FDGroup)的记录头在文件中的偏移量
};

//GROUPID包含在ID中
#define FDGROUPID_MASK 0xFFFF0000

struct FDGroup
{
	int groupid;	//分组号
	int fdcnt;		//本组中的FeatureData个数
	int offset[0];	//本组中每一个值的记录头(FDRecHeader)在文件中的偏移量
};

struct FDIntData
{
	DWORD id;
	DWORD val;
};

struct FDStrData
{
	DWORD id;
	DWORD len; //包括结束的0
	WCHAR val[0];
};

struct FDEnd
{
	DWORD reserved;
};

#pragma warning( pop )
#pragma pack()

#define MAX_GROUP 4

class KFeatureReader
{
	BYTE* m_buf;
	BYTE* m_end;	//rtEnd记录头所在

	FDGroup* m_groups[MAX_GROUP];
	FDStart* m_FDStart;
	
	FDRecHeader* Find(int id);
	FDGroup* FindGroup(int id);
	BOOL ValidRange(void* p, int size = 0);
public:
	KFeatureReader();
	~KFeatureReader();

	FDStart* GetFDStart();
	int QueryInt(int id);
	LPCWSTR QueryStr(int id);
	HRESULT Load(LPCWSTR lpcwFile);

protected:
	HRESULT Clear();
};

class KFeatureWriter
{
	struct GROUP : std::vector<FDRecHeader*>
	{
		BOOL used;
		int datasize;	//本分组中数据大小
	};

	BYTE m_bufStart[sizeof(FDStart) + sizeof(int) * MAX_GROUP];
	FDStart* m_start;
	GROUP  m_groups[MAX_GROUP];
	GROUP* m_grp;

	BOOL m_failed;
	BOOL CheckID(int id);
public:
	KFeatureWriter();
	~KFeatureWriter();
	
	void Over();

	KFeatureWriter& Start();
	KFeatureWriter& StartGroup(int grpid);
	KFeatureWriter& AddFeature(int id, int Val);
	KFeatureWriter& AddFeature(int id, LPCWSTR lpcwVal);
	KFeatureWriter& EndGroup(int grpid);

	HRESULT End();

	HRESULT Write(LPCWSTR lpcwPath);
};


// ----------------------------------------------------------------------------- 
#endif //_OEMINFORW_H_

#pragma once

typedef int int32;
typedef unsigned int uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;

enum ProcessState{
	ProcessState_Idle,
	ProcessState_StartConnectToServer,
	ProcessState_OpenRequest,
	ProcessState_EndRequest,
	ProcessState_EndRequestDone,
	ProcessState_BeginReceiveData,	// 开始获取数据
	ProcessState_ReceiveData,
	ProcessState_ReceivedData,		// 数据已经接受, 等待处理 
	ProcessState_PartFinished,		// 部分完成
	ProcessState_Finished,
	ProcessState_UserCanceled,
	ProcessState_Failed,
};

enum DLERRCode{
	DLERR_SUCCESS = 0,
	DLERR_INTERNAL,				//internal stat 
	DLLER_SERVER_FILENOTFOUND,	//服务器的文件未找到
	DLERR_CREATEFILE,			//创建文件失败
	DLERR_WRITEFILE,			//写入文件失败
	DLLER_NETWORK,				//网络错误
	DLLER_INTERNAL_WAIT_OBJ,	//内部错误等待其他对象
	DLLER_INTERNAL_CHECKMD5,	//内部错误 计算md5
	DLLER_USER_CANCEL,			//用户取消
	DLLER_SEGENT_ERROR,			//达到最大尝试次数，仍然有块下载出错
};

struct RemoteFileInfo{
	DWORD nStatusCode;
	BOOL bRandomAccess;
	int64 fileSize;
	int64 fileDownloaded;
	
	void Reset()
	{
		nStatusCode = 0;
		bRandomAccess = FALSE;
		fileSize = fileDownloaded = 0;
	}
	void Reset(INT64 _fileSize, INT64 _fileDownloaded, BOOL _bRandomAccess)
	{
		fileSize = _fileSize;
		fileDownloaded = _fileDownloaded;
		bRandomAccess = _bRandomAccess;
	}
};

class IDownStat 
{
public:
	virtual int64 Downloaded() = 0;
	virtual int64 Speed() = 0;
	virtual int64 AverageSpeed() = 0;
	virtual DWORD TimeUsed() = 0;
};


class IHttpAsyncObserver;
class IDownload;
class IHttpAsyncObserver 
{
public:
	virtual BOOL OnHttpAsyncEvent( IDownload* pDownload, ProcessState state, LPARAM lParam ) = 0;
};

class __declspec(uuid("4E65061C-EA4B-4543-A27C-FAA11AA50FC5")) IDownload
{
public:
	// 
	virtual void Release() = 0;
	// Create 
	virtual VOID SetObserver(IHttpAsyncObserver *pObserver) = 0;
	virtual LPVOID SetUserData(LPVOID lpUserData) = 0;
	virtual void SetDownloadInfo(LPCTSTR szUrl, LPCTSTR szFilePath) = 0;
	
	// Control 
	virtual BOOL Fetch(INT nCorrurent=0) = 0;
	virtual BOOL Start(INT nCorrurent=0) = 0;
	virtual void Stop() = 0;
	virtual void Wait() = 0;	// 等待结束 !!
	
	// After Done 
	virtual DLERRCode LastError() = 0;
	virtual LPVOID GetUserData() = 0;

	virtual RemoteFileInfo *GetRemoteFileInfo() = 0;
	virtual IDownStat *GetDownloadStat() = 0;
	virtual void GetDownloadInfo(CString &strUrl, CString &strFilePath) = 0;
};

//#define DOWNLOAD_TEMP_FILE_SUFFIX "_kt"
//#define DOWNLOAD_TEMP_FILE_INFO_SUFFIX "_kti"
#define DOWNLOAD_TEMP_FILE_SUFFIX ".fd"
#define DOWNLOAD_TEMP_FILE_INFO_SUFFIX ".fd.cfg"

EXTERN_C HRESULT WINAPI CreateDownloadObject( REFIID riid, void** ppvObj );

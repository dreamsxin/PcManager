//
#ifndef _KWS_IPC_H_
#define _KWS_IPC_H_

#include <aclapi.h>
#include <tchar.h>
#include <map>
#include <string>
using namespace std;

extern class IPCServer *g_pIPCServer;

#define PAGE_SIZE			0x1000
#define HANDLE_SIZE			0x100

#define IPC_ERR_OK			0  // OK
#define IPC_ERR_UNKNOWN		1  // 
#define IPC_ERR_INVALIDARG	2  // 无效参数
#define IPC_ERR_DATASIZE	3  // 数据包太大, 我们比较挫, 不支持大数据
#define IPC_ERR_OBJECT		4  // 服务端未启动, 或打开通信对象失败
#define IPC_ERR_TIMEOUT		5  // 等待超时

typedef DWORD (WINAPI *LPIPC_CALL_ROUTINE)(LPVOID lpParameter);

typedef struct _IPC_CALL_STUB
{
	class IPCServer *pServer;
	LPIPC_CALL_ROUTINE lpCallRoutine;
	HANDLE hParamMapping;
	HANDLE hMutexClientEnter;
	HANDLE hEventServerProcess;
	HANDLE hEventClientLeave;
	bool bBlock;
	int nHandleOrder;
	unsigned uParamSize;
	void *pParamBuffer;

} IPC_CALL_STUB, *PIPC_CALL_STUB;

typedef map<const basic_string<TCHAR>, IPC_CALL_STUB> IPC_CALL;
typedef IPC_CALL::iterator IPC_CALL_ITERATOR;
typedef pair<IPC_CALL::iterator, bool> IPC_CALL_INSERT_PAIR;
typedef pair<const basic_string<TCHAR>, IPC_CALL_STUB> IPC_CALL_PAIR;

typedef map<int, basic_string<TCHAR>> IPC_ORDER;
typedef IPC_ORDER::iterator IPC_ORDER_ITERATOR;
typedef pair<int, basic_string<TCHAR>> IPC_ORDER_PAIR;

class IPCServer
{
public:
	IPCServer();
	~IPCServer();

	int Init();
	int Uninit();

	int Start();
	int AddCall(const basic_string<TCHAR> call, LPIPC_CALL_ROUTINE lpCallRoutine, unsigned uParamSize = PAGE_SIZE, bool bBlock = true);
	int SetBuffer(void *pMapBuffer, void *pParamBuffer, unsigned uParamSize);
	int GetBuffer(void *pMapBuffer, void *pParamBuffer, unsigned &uParamSize);
	int ReleaseBuffer(void *pParamBuffer);

private:
	static unsigned __stdcall _Listen(void *p);
	void _Lesten();
	void _NotifyStop();
	bool SetObjectToLowIntegrity(HANDLE hObject, SE_OBJECT_TYPE type = SE_KERNEL_OBJECT);

	int SuperaddHandle(HANDLE hHandle);

private:
	IPC_CALL	m_ipcCall;
	IPC_ORDER	m_ipcOrder;
	HANDLE		m_hHandles[HANDLE_SIZE];
	
	HANDLE		m_hListenThread;
	HANDLE		m_hStopListenEvent;
	DWORD		m_dwListenCount;
};

//***************************************************************************//
//***************************************************************************//

class IPCClient
{
public:
	int RunCall(const basic_string<TCHAR> call, void *pParamBuffer, unsigned &uParamSize, DWORD dwTimeout = 5000);

private:
	unsigned ToBuffer(void *pBuffer, void *pParamBuffer, unsigned uParamSize);

	unsigned FromBuffer(void *pBuffer, void *pParamBuffer, unsigned &uParamSize);
};

#endif
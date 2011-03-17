
#pragma once

#ifndef VIRTUAL_ENG
#define VIRTUAL_ENG virtual
#endif

class IModuleKillService
{
public:
	VIRTUAL_ENG	BOOL AddFile(LPCTSTR lpFile) = 0;
	VIRTUAL_ENG BOOL AddFolder( LPCTSTR lpFolder) = 0;
	VIRTUAL_ENG BOOL AddRegKey(HKEY hRoot, LPCTSTR lpKey) = 0;
	VIRTUAL_ENG BOOL AddRegValue(HKEY hRoot, LPCTSTR lpKey, LPCTSTR lpValue) = 0;
	VIRTUAL_ENG	BOOL DelFile(LPCTSTR lpFile) = 0;
	VIRTUAL_ENG BOOL DelFolder( LPCTSTR lpFolder) = 0;
	VIRTUAL_ENG BOOL DelRegKey(HKEY hRoot, LPCTSTR lpKey) = 0;
	VIRTUAL_ENG BOOL DelRegValue(HKEY hRoot, LPCTSTR lpKey, LPCTSTR lpValue) = 0;
};

class IModuleKillClient
{
public:
	VIRTUAL_ENG BOOL Init(IModuleKillService* pService) = 0;
	VIRTUAL_ENG BOOL QuickScan() = 0;
	VIRTUAL_ENG BOOL ScanDetail() = 0;
	VIRTUAL_ENG BOOL Remove() = 0;
	VIRTUAL_ENG BOOL UnInit() = 0;
};

class IModuleKillClientNull : public IModuleKillClient
{
public:
	IModuleKillClientNull(){m_pService=NULL;}
	VIRTUAL_ENG BOOL Init(IModuleKillService* pService){m_pService=pService;return TRUE;}
	VIRTUAL_ENG BOOL QuickScan(){return FALSE;}
	VIRTUAL_ENG BOOL ScanDetail(){return FALSE;}
	VIRTUAL_ENG BOOL Remove(){return FALSE;}
	VIRTUAL_ENG BOOL UnInit(){return TRUE;}
protected:
	IModuleKillService* m_pService;
};

#define VER_EXTEND_MOD	1

typedef IModuleKillClient* (WINAPI *PFNNewKillClient)(DWORD ,LPCTSTR);
typedef VOID (WINAPI *PFNFreeKillClient)(IModuleKillClient*);

#define EXPORT_NEW_CLIENT	"a"
#define EXPORT_DEL_CLIENT	"b"


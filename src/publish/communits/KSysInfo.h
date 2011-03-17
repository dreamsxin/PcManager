//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

#define REG_CHECK_NAME					L"KSysInfo2"

#define BOOT_INI_OPERATION_SECTION		L"operating systems"

//////////////////////////////////////////////////////////////////////////

#define PRODUCT_UNDEFINED                       0x00000000

#define PRODUCT_ULTIMATE                        0x00000001
#define PRODUCT_HOME_BASIC                      0x00000002
#define PRODUCT_HOME_PREMIUM                    0x00000003
#define PRODUCT_ENTERPRISE                      0x00000004
#define PRODUCT_HOME_BASIC_N                    0x00000005
#define PRODUCT_BUSINESS                        0x00000006
#define PRODUCT_STANDARD_SERVER                 0x00000007
#define PRODUCT_DATACENTER_SERVER               0x00000008
#define PRODUCT_SMALLBUSINESS_SERVER            0x00000009
#define PRODUCT_ENTERPRISE_SERVER               0x0000000A
#define PRODUCT_STARTER                         0x0000000B
#define PRODUCT_DATACENTER_SERVER_CORE          0x0000000C
#define PRODUCT_STANDARD_SERVER_CORE            0x0000000D
#define PRODUCT_ENTERPRISE_SERVER_CORE          0x0000000E
#define PRODUCT_ENTERPRISE_SERVER_IA64          0x0000000F
#define PRODUCT_BUSINESS_N                      0x00000010
#define PRODUCT_WEB_SERVER                      0x00000011
#define PRODUCT_CLUSTER_SERVER                  0x00000012
#define PRODUCT_HOME_SERVER                     0x00000013
#define PRODUCT_STORAGE_EXPRESS_SERVER          0x00000014
#define PRODUCT_STORAGE_STANDARD_SERVER         0x00000015
#define PRODUCT_STORAGE_WORKGROUP_SERVER        0x00000016
#define PRODUCT_STORAGE_ENTERPRISE_SERVER       0x00000017
#define PRODUCT_SERVER_FOR_SMALLBUSINESS        0x00000018
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM    0x00000019

#define PRODUCT_UNLICENSED                      0xABCDABCD

typedef BOOL (WINAPI *PFN_GetProductInfo)(
	DWORD dwOSMajorVersion,
	DWORD dwOSMinorVersion,
	DWORD dwSpMajorVersion,
	DWORD dwSpMinorVersion,
	PDWORD pdwReturnedProductType);

//////////////////////////////////////////////////////////////////////////

typedef enum
{
	WIN_XP_HOME			= 0x1,
	WIN_XP_PRO			= 0x2,
	WIN_XP_OEM_FPP		= 0x4,
	WIN_XP_OEM_VLK		= 0x8,
	WIN_XP_OEM_OEM		= 0x10,
	WIN_XP_MULTI_BOOT	= 0x20,
	WIN_XP_C_DRIVE		= 0x40,
	WIN_XP_SP1			= 0x80,
	WIN_XP_SP2			= 0x100,
	WIN_XP_SP3			= 0x200,
	WIN_XP_GRUB_BOOT	= 0x400,
	WIN7_ULTIMATE		= 0x800,
	WIN7_HOME_PREMIUM	= 0x1000,
	WIN7_HOME_BASIC		= 0x2000,
	WIN7_ENTERPRISE		= 0x4000,
	WIN7_BUSINESS		= 0x8000,
	WIN7_STARTER		= 0x10000
};

//////////////////////////////////////////////////////////////////////////

class CKSysInfo
{
public:
	//返回-1时，不上传数据
	int Get();

private:
	int IsGetOne();

	int GetOSType();

	int GetOEMType();

	int GetBootInfo();

	int GetSystemDrive();

	BYTE GetResKeyCode(
		HMODULE hModule,
		UINT unResID);
};

//////////////////////////////////////////////////////////////////////////
#ifndef _BOOT_CHECK_STRUCT_H_
#define _BOOT_CHECK_STRUCT_H_

#include <windows.h>

#pragma pack(push)
#pragma pack(4)

#define ENCRYPTION_CHAR 'H'
//-----------------------------------------------------------------------------//

//
//  File bootclean
//

typedef struct _FILE_CLEAN_HEADER 
{
	ULONG ulWindowsVersion;
	ULONG ulNumber;
} FILE_CLEAN_HEADER, *PFILE_CLEAN_HEADER;

typedef struct _FILE_CLEAN_ENTRY 
{
	WCHAR wstrFilePath[MAX_PATH];
	WCHAR wstrFileMD5[33];
} FILE_CLEAN_ENTRY, *PFILE_CLEAN_ENTRY;
//-----------------------------------------------------------------------------//

//
//  File bootreplace
//

typedef struct _FILE_REPLACE_HEADER 
{
	ULONG ulWindowsVersion;
	ULONG ulNumber;
} FILE_REPLACE_HEADER, *PFILE_REPLACE_HEADER;

typedef struct _FILE_REPLACE_ENTRY 
{
	WCHAR wstrFileFrom[MAX_PATH];
	WCHAR wstrFileFromMD5[33];
	WCHAR wstrFileTo[MAX_PATH];
} FILE_REPLACE_ENTRY, *PFILE_REPLACE_ENTRY;
//-----------------------------------------------------------------------------//

//
//  Reg bootclean
//

//
// Reasonable Limit for Values Names
//
#define BC_REG_MAX_VALUENAME_LENGTH     256

#define BC_REG_MAX_VALUE_LENGTH		512

#define BC_REG_MAX_KEYPATH_LENGTH	512

enum
{
	ActionRegKeyDelete = 0,
	ActionRegValueDelete,
	ActionRegValueReplace,
	ActionRegKeyEmpty,
};
typedef struct _REG_CLEAN_HEADER 
{
	ULONG ulWindowsVersion;
	ULONG ulNumber;
} REG_CLEAN_HEADER, *PREG_CLEAN_HEADER;

typedef struct _REG_CLEAN_ENTRY
{
	ULONG ulType;

	ULONG ulValueType;

	WCHAR wstrKeyPath[BC_REG_MAX_KEYPATH_LENGTH];
	ULONG ulKeyPathLength;

	WCHAR wstrValueName[BC_REG_MAX_VALUENAME_LENGTH];
	ULONG ulValueNameLength;

	CHAR pNewValueBuffer[BC_REG_MAX_VALUE_LENGTH];
	ULONG ulNewValueBufferLength;
} REG_CLEAN_ENTRY, *PREG_CLEAN_ENTRY;
//-----------------------------------------------------------------------------//

//
// Driver approved list
//

typedef struct _DRIVER_APPROVED_HEADER 
{
	ULONG ulWindowsVersion;
	ULONG ulNumber;
} DRIVER_APPROVED_HEADER, *PDRIVER_APPROVED_HEADER;

typedef struct _DRIVER_APPROVED_ENTRY
{
	WCHAR MD5[33]; // Consider binary, later
} DRIVER_APPROVED_ENTRY, *PDRIVER_APPROVED_ENTRY;
//-----------------------------------------------------------------------------//

//
//  Driver blocked list
//

typedef struct _DRIVER_BLOCKED_HEADER 
{
	ULONG ulWindowsVersion;
	ULONG ulNumber;
} DRIVER_BLOCKED_HEADER, *PDRIVER_BLOCKED_HEADER;

typedef struct _DRIVER_BLOCKED_ENTRY
{
	WCHAR MD5[33]; // Consider binary, later
} DRIVER_BLOCKED_ENTRY, *PDRIVER_BLOCKED_ENTRY;
//-----------------------------------------------------------------------------//

#pragma pack(pop)

#endif
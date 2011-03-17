/* -------------------------------------------------------------------------
// 文件名		:	base64/base64.h
// 创建人		:	chenghui
// 创建时间		:	2008-5-29 12:30:54
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __BASE64_BASE64_H__
#define __BASE64_BASE64_H__

#ifndef ASSERT
#define ASSERT _ASSERT
#endif

// -------------------------------------------------------------------------
DWORD Base64DecodeGetLength(DWORD size);
DWORD Base64Decode(LPBYTE desData, const char* srcData, DWORD srcLen);

DWORD Base64EncodeGetLength(DWORD size);
DWORD Base64Encode( void* dest, const void* src, DWORD size );
// -------------------------------------------------------------------------
// $Log: $

#endif /* __BASE64_BASE64_H__ */

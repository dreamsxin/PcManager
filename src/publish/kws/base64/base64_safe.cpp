/* -------------------------------------------------------------------------
// 文件名		:	base64/base64.cpp
// 创建人		:	chenghui
// 创建时间		:	2008-5-29 12:35:18
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "base64_safe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	BASE64_INPUT						3
#define	BASE64_OUTPUT						4

// -------------------------------------------------------------------------
DWORD Base64DecodeGetLength(DWORD size)
{
	return ((size + (BASE64_OUTPUT - 1) / BASE64_OUTPUT) * BASE64_INPUT);
}

DWORD Base64Decode(byte* desData, const char* srcData, DWORD srcLen)
{
	if (!desData)
	{
		ASSERT(FALSE);
		return -1;
	}

	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};

	int nValue;
	DWORD i= 0;
	DWORD j= 0;
	DWORD nCount = 0;
	while (i < srcLen)
	{
		if (*srcData != '\r' && *srcData!='\n')
		{
			nValue = DecodeTable[*srcData++] << 18;
			nValue += DecodeTable[*srcData++] << 12;
			desData[j++] =(nValue & 0x00FF0000) >> 16;
			nCount++;
			if (*srcData != '=')
			{
				nValue +=  DecodeTable[*srcData++] << 6;
				desData[j++] = (nValue & 0x0000FF00) >> 8;
				nCount++;
				if (*srcData != '=')
				{
					nValue += DecodeTable[*srcData++];
					desData[j++] = nValue & 0x000000FF;
					nCount++;
				}
			}
			i += 4;
		}
		else// 回车换行,跳过
		{
			srcData++;
			i++;
		}
	}
	return nCount;
}


DWORD Base64EncodeGetLength( DWORD size )
{
	return (((size + (BASE64_INPUT - 1)) / BASE64_INPUT) * BASE64_OUTPUT);
}

DWORD Base64Encode( void* dest, const void* src, DWORD size )
{
	if (!dest || !src)
	{
		ASSERT(FALSE);
		return -1;
	}

	///
	static const CHAR BASE64_TABLE[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";

	LPBYTE pSrc = (LPBYTE)src;
	LPBYTE pDest = (LPBYTE)dest;
	unsigned dwSrcSize = size;
	unsigned dwDestSize = 0;

	while (dwSrcSize >= 1)
	{
		unsigned dwBlockSize = __min( dwSrcSize, BASE64_INPUT );
		ASSERT( 1 <= dwBlockSize && dwBlockSize <= BASE64_INPUT );

		// Encode inputs...
		BYTE n1, n2 = 0, n3 = 0, n4 = 0;
		switch (dwBlockSize)
		{
		case 3:
			n4  =  (pSrc[ 2 ] & 0x3f);
			n3  = ((pSrc[ 2 ] & 0xc0) >> 6);
		case 2:
			n3 |= ((pSrc[ 1 ] & 0x0f) << 2);
			n2  = ((pSrc[ 1 ] & 0xf0) >> 4);
		case 1:
			n2 |= ((pSrc[ 0 ] & 0x03) << 4);
			n1  = ((pSrc[ 0 ] & 0xfc) >> 2);
			break;

		default:
			ASSERT( 0 );
		}
		pSrc += dwBlockSize;
		dwSrcSize -= dwBlockSize;

		// Validate...
		ASSERT( 0 <= n1 && n1 <= 63 );
		ASSERT( 0 <= n2 && n2 <= 63 );
		ASSERT( 0 <= n3 && n3 <= 63 );
		ASSERT( 0 <= n4 && n4 <= 63 );

		// Padding...
		switch (dwBlockSize)
		{
		case 1: n3 = 64;
		case 2: n4 = 64;
		case 3:
			break;

		default:
			ASSERT( 0 );
		}

		// 4 outputs...
		*pDest++ = BASE64_TABLE[ n1 ];
		*pDest++ = BASE64_TABLE[ n2 ];
		*pDest++ = BASE64_TABLE[ n3 ];
		*pDest++ = BASE64_TABLE[ n4 ];
		dwDestSize += BASE64_OUTPUT; //4
	}

	return dwDestSize;
}

// -------------------------------------------------------------------------
// $Log: $

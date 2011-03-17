// dummyz@126.com
// pe resource item data struct

#pragma once

#include <PshPack1.h>

//
// Icon Struct
// http://www.moon-soft.com/program/FORMAT/windows/icons.htm
//
struct _PE_ICONDIRENTRY;

typedef struct _PE_ICONDIR
{
	WORD	IdReserved;		// reserved (must be 0)
	WORD	IdType;			// resource type (1 for icons)
	WORD	IdCount;		// how many images?
//	_PE_ICONDIRENTRY	IdEntries[1];	// an entry for each image (idcount of 'em)
} PE_ICONDIR, *LPICONDIR;

typedef struct _PE_ICONDIRENTRY
{
	BYTE	bWidth;			// width, in pixels, of the image
	BYTE	bHeight;		// height, in pixels, of the image
	BYTE	bColorCount;	// number of colors in image (0 if >=8bpp)
	BYTE	bReserved;		// reserved ( must be 0)
	WORD	wPlanes;		// color planes
	WORD	wBitcount;		// bits per pixel
	DWORD	dwBytesInRes;	// how many bytes in this resource?
	WORD	wIconID;		// Ö¸Ã÷ icon µÄ id
} PE_ICONDIRENTRY;

typedef struct _PE_ICONIMAGE
{
	BITMAPINFOHEADER	Header;		// dib header
	RGBQUAD				Colors[1];	// color table
	BYTE				Xor[1];		// dib bits for xor mask
	BYTE				And[1];		// dib bits for and mask
} PE_ICONIMAGE;

#include <PopPack.h>


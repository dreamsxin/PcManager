#ifndef _RAWDISKOP_H_
#define _RAWDISKOP_H_
//-----------------------------------------------------------------------------//
LONG
DISKRawReadWholeFile(
					 PWCHAR pwstrFilePath,
					 PVOID* ppWholeFileBuffer,
					 PULONG pulBufferLength
					 );
//-----------------------------------------------------------------------------//
#endif
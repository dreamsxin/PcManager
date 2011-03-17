#ifndef _EXPORT_H_
#define _EXPORT_H_
//-----------------------------------------------------------------------------//
#define		UD_RESULT_NORMAL_FILE			0
#define		UD_RESULT_SYSTEM_FILE			1
#define		UD_RESULT_UNDELETABLE_FILE		2
//-----------------------------------------------------------------------------//
NTSTATUS
Init (
	  VOID
	  );

NTSTATUS
Uninit (
		VOID
		);

NTSTATUS
CheckFileDeletable (
					PWCHAR pwszFilePath
					);
//-----------------------------------------------------------------------------//
#endif
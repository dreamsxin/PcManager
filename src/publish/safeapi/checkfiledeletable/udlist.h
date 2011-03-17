#ifndef _UD_LIST_H_
#define _UD_LIST_H_
//-----------------------------------------------------------------------------//
NTSTATUS
InitUdList (
			VOID
			);

NTSTATUS
UninitUdList (
			  VOID
			  );

NTSTATUS
AddPath (
		 PWCHAR pwszPath
		 );

NTSTATUS
DebugDisplayUdList (
					VOID
					);

BOOL
IsInUdList (
			PWCHAR pwszPath
			);
//-----------------------------------------------------------------------------//
#endif
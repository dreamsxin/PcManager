#pragma once
struct ProcessLibOverwriteRule
{
	BOOL Overwrite_CompanyName;
	BOOL Overwrite_Desc;
	BOOL Overwrite_Path;
	BOOL Overwrite_SignName;
	BOOL Overwrite_CanClose;
	BOOL Overwrite_ProcessType;
	BOOL Overwrite_SoftID;

	ProcessLibOverwriteRule()
	{
		Overwrite_CompanyName = FALSE;
		Overwrite_Desc = FALSE;
		Overwrite_Path = FALSE;
		Overwrite_SignName = FALSE;
		Overwrite_CanClose = FALSE ;
		Overwrite_ProcessType = FALSE;
		Overwrite_SoftID = FALSE;
	}
};
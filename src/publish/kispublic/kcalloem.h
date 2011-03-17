#pragma once
#include "public.h"
#include "klocker.h"

class KisPublic
{
private:
	KisPublic();

public:
	static KisPublic* Instance(void);
	int Init(LPCWSTR lpFileName = NULL);
	int Unit(void);

	int KQueryOEMIntA(IN int nKey, IN int& nValue);
	int KQueryOEMLPCWSTR(IN int nKey, IN std::wstring& strValue);

private:
	HMODULE m_hDll;
	KisPublic static m_pKisPublic;
	ILockerMode* m_pLockerCS;
};

#include "StdAfx.h"
#include "kprocessitem.h"

KProcessItem::KProcessItem(void)
{
	m_processType = 0;
	m_softId = L"";
	m_bCanClose = TRUE;
	m_processItemId = -1;
	m_companyIndex = -1;
	m_signNameIndex = -1;
	m_pathIndex = -1;
	m_pathLevel = 0;
}

KProcessItem::~KProcessItem(void)
{
}

KProcessItem& KProcessItem::operator=( const KProcessItem& right )
{
	m_processItemId = right.ProcessItemId();
	m_desc = right.Desc();
	m_processType = right.ProcessType();
	m_softId = right.SoftId();
	m_companyIndex = right.CompanyIndex();
	m_signNameIndex = right.SignNameIndex();
	m_pathIndex = right.PathIndex();
	m_pathLevel = right.PathLevel();
	m_displayName = right.DisplayName();
	m_bCanClose = right.CanClose();
	m_processName = right.ProcessName();

	return *this;
}
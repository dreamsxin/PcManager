#include "stdafx.h"
#include "ksqliteresult.h"


#define CHECK_HRETHANDLE(hRetHandle,iter, nCol) \
assert( hRetHandle);\
iter = hRetHandle;\
assert(iter != m_retsSetList.end());\
assert(nCol<iter->size());


template<class STRTYPE>
int KSqliteResult<STRTYPE>::Init(sqlite3_stmt *stmt)
{
	int nRet = -1;
	int nColCount = 0;
	if ( stmt == NULL)
		return E_INVALIDARG;

	m_retsVecPtr.clear();
	m_retsSetList.clear();

	nColCount = sqlite3_column_count(stmt);
	if (nColCount > 0)
	{
		
		while (SQLITE_ROW == sqlite3_step(stmt))
		{
			std::vector<STRTYPE> rowValue;
			m_retsSetList.push_back( rowValue);
			m_retsSetList.back().reserve( nColCount);
			for ( int i = 0; i < nColCount; ++i)
				m_retsSetList.back().push_back((const char *)sqlite3_column_text(stmt, i));
		}

		ResultList2VectorPtrs();
	}

	

	return 0;
}

template<class STRTYPE>
void KSqliteResult<STRTYPE>::Release()
{
	delete this;
}

template<class STRTYPE>
size_t KSqliteResult<STRTYPE>::GetColCount()
{
	if ( m_retsSetList.empty())
		return 0;

	return m_retsSetList.front().size();
}

template<class STRTYPE>
size_t KSqliteResult<STRTYPE>::GetRowCount()
{
	return m_retsSetList.size();
}


template<class STRTYPE>
__int64 KSqliteResult<STRTYPE>::GetInt64Value(size_t nRow, size_t nCol)
{
	return (*m_retsVecPtr[nRow])[nCol];
}

template<class STRTYPE>
unsigned __int64 KSqliteResult<STRTYPE>::GetUnsignedInt64Value(size_t nRow, size_t nCol)
{
	return (*m_retsVecPtr[nRow])[nCol];
}

template<class STRTYPE>
const char *KSqliteResult<STRTYPE>::GetStringValue(size_t nRow, size_t nCol)
{
	return (*m_retsVecPtr[nRow])[nCol];
}

template<class STRTYPE>
void KSqliteResult<STRTYPE>::ResultList2VectorPtrs()
{
	if (m_retsSetList.empty())
		return;

	RESULTSET_LIST::iterator iter	= m_retsSetList.begin();
	RESULTSET_LIST::iterator itEnd	= m_retsSetList.end();
	m_retsVecPtr.reserve(m_retsSetList.size());
	for ( ; iter != itEnd; ++iter)
		m_retsVecPtr.push_back( &(*iter));
}
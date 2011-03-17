////////////////////////////////////////////////////////////////
//
//	Filename: 	KSqliteResult.h
//	Creator:	yangjun  <yangjun@kingsoft.com>
//	Date:		2009-7-29  21: 59
//	Comment:	
//
///////////////////////////////////////////////////////////////
#pragma once

#include "ksqliteimp.h"
#include <list>
#include <vector>

template<class STRTYPE>
class KSqliteResult: public IKSqliteResult
{
public:
	int Init( sqlite3_stmt *pStmt);

	virtual size_t GetRowCount();
	virtual size_t GetColCount();

	virtual void   Release();

	virtual __int64			 GetInt64Value(size_t nRow, size_t nCol);
	virtual unsigned __int64 GetUnsignedInt64Value(size_t nRow, size_t nCol);
	virtual const char*		 GetStringValue(size_t nRow, size_t nCol);

private:
	void ResultList2VectorPtrs();

	typedef std::list<std::vector<STRTYPE>> RESULTSET_LIST;
	typedef std::vector<std::vector<STRTYPE>*> RESULTSET_VECTOR_PTR;

	RESULTSET_LIST m_retsSetList;
	RESULTSET_VECTOR_PTR m_retsVecPtr;
};

#include "ksqliteresult_inl.h"
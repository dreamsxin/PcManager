
// dummyz@126.com

#pragma once

class CThreadLangLocal
{
public:
	CThreadLangLocal(LCID Locale)
	{
		m_OldLocale = GetThreadLocale();
		SetThreadLocale(Locale);
	}

	virtual ~CThreadLangLocal()
	{
		SetThreadLocale(m_OldLocale);
	}

protected:
	LCID m_OldLocale;
};

#define USE_CHINESE_LANG_LOCAL	\
	CThreadLangLocal thread_lang_chinese(0x0804);
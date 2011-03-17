
#pragma once

#include "tinyxml/tinyxml.h"

inline DWORD get_xml_attr(TiXmlElement* node, const char* name, DWORD defval)
{
	int a;
	return (NULL == node->Attribute(name, &a)) ? defval : (DWORD)a;
}

inline const char* get_xml_attr(TiXmlElement* node, const char* name, const char* defval)
{
	const char* a = node->Attribute(name);
	return (a == NULL) ? defval : a;
}

inline const char* get_xml_data(TiXmlElement* node, const char* defval)
{
	const char* a = node->GetText();
	return (a == NULL) ? defval : a;
}

inline const char* get_xml_child_data(TiXmlElement* node, const char* name, const char* defval)
{
	TiXmlElement* child = node->FirstChildElement(name);
	return (child == NULL) ? defval : get_xml_data(child, defval);
}

inline void set_xml_data(TiXmlElement* node, const char* data, bool cdata = false)
{
	TiXmlNode* child = node->FirstChild();
	if ( child == NULL )
	{
		TiXmlText e(data);

		e.SetCDATA(cdata);
		node->InsertEndChild(e);
	}
	else
	{
		TiXmlText* p = child->ToText();
		if ( p == NULL )
		{
			TiXmlText e(data);

			e.SetCDATA(cdata);
			node->InsertBeforeChild(child, e);
		}
		else
		{
			p->SetCDATA(cdata);
			p->SetValue(data);
		}
	}
}

#include "stdafx.h"
#include "ConfigItem.h"
#include "Exception.h"

#ifdef _LIBUTILS_USE_XML_LIBXML2
	#include "libxml/tree.h"
	#include "libxml/parser.h"
	#include <libxml/xpath.h>
	#include <libxml/xpathInternals.h>
#endif

#ifdef _LIBUTILS_USE_XML

CConfigItemList::CConfigItemList()
:vector<CConfigItem*>()
{
}

CConfigItemList::CConfigItemList(const CConfigItemList& cpy)
:vector<CConfigItem*>()
{
	for (const_iterator i=cpy.begin();i!=cpy.end();i++)
		push_back(new CConfigItem(**i));
}

CConfigItemList::~CConfigItemList()
{
	for (iterator i=begin();i!=end();i++)
		delete (*i);
}

CConfigItem::CConfigItem(void)
{
#ifdef _LIBUTILS_USE_XML_LIBXML2
	m_Node = NULL;
#endif
}

#ifdef _LIBUTILS_USE_XML_LIBXML2
CConfigItem::CConfigItem(xmlNodePtr node)
{
	m_Node = node;
}
#endif

CConfigItem::CConfigItem(const CConfigItem& cpy)
{
	m_Node = NULL;
	SetNode(cpy);
}


CConfigItem::~CConfigItem(void)
{
#ifdef _LIBUTILS_USE_XML_LIBXML2
//	if (m_Node)
//		xmlFreeNode(m_Node);
#endif
}

void CConfigItem::ParseXPath(string Path, string &First, string &Other)
{
	int pos = Path.find_first_of('/');
	if (pos>0)
	{
		First = Path.substr(0,pos);
		Other = Path.substr(pos+1);
	}
	else
	{
		First = Path;
		Other = "";
	}
}
/*
string CConfigItem::GetValue()
{
#ifdef _LIBUTILS_USE_XML_LIBXML2
	if (m_Node)
	{
		xmlNodePtr node = m_Node->children;

		if (!m_Node)
			return "";

		short type = node->type;
		const xmlChar *val = node->content;/*
		char* asciiVal = XMLString::transcode(val);
		string result = asciiVal;
		XMLString::release(&asciiVal);* /
		return (char*)val;
	}

	return "";
#endif
}


string CConfigItem::GetValue(string path)
{
#ifdef _LIBUTILS_USE_XML_LIBXML2
	CConfigItem elem = GetElement(path);
	return elem.GetValue();
#endif
}
*/

string CConfigItem::GetAttribute(string path, bool bMandatory, string defaultValue)
{
#ifdef _LIBUTILS_USE_XML_LIBXML2
	string First, Other;
	ParseXPath(path, First, Other);

	if (Other.length())
	{
		return GetElement(First).GetAttribute(Other);
	}
	else
	{
		char* val = (char*)xmlGetProp(m_Node, BAD_CAST path.c_str());

		if (!val)
		{
			if (bMandatory)
				throw CHaException(CHaException::ErrAttributeNotFound, path);
			else
				return defaultValue;
		}

		string sVal = val;
		xmlFree(val);
		return sVal;
	}
#endif
}

CConfigItem CConfigItem::GetElement(string path)
{
#ifdef _LIBUTILS_USE_XML_LIBXML2
	string First, Other;
	ParseXPath(path, First, Other);

	if (Other.length())
	{
		return GetElement(First).GetElement(Other);
	}
	else
	{
		xmlNodePtr retVal = NULL, cur = m_Node->children;

		while(cur)
		{
			if (cur->type==XML_ELEMENT_NODE && strcmp((char*)cur->name, First.c_str())==0)
			{
				if (retVal)
				{
					return NULL;
				}
				retVal = cur;
			}

			cur = cur->next;
		}

		return retVal;
	}
#endif
}

void CConfigItem::GetElementList(string path, CConfigItemList &list)
{
	if (!m_Node)
		return;

#ifdef _LIBUTILS_USE_XML_LIBXML2
	string First, Other;
	ParseXPath(path, First, Other);

	if (Other.length())
	{
		GetElement(First).GetElementList(Other, list);
	}
	else
	{
		list.clear();
		xmlNodePtr cur = m_Node->children;

		while(cur)
		{
			if (First.compare((char*)cur->name)==0)
			{
				list.push_back(new CConfigItem(cur));
			}
			cur = cur->next;
		}
	}
#endif
}

#ifdef _LIBUTILS_USE_XML_LIBXML2
	void CConfigItem::SetNode(xmlNodePtr node)
	{
		m_Node = node;
		/*  never free single node
		if (m_Node)
			xmlFreeNode(m_Node);
		m_Node = xmlCopyNode(node, true);
		*/
	}
#endif

#ifdef _LIBUTILS_USE_XML_LIBXML2
	bool CConfigItem::isNode()
	{
		return m_Node->type==XML_ELEMENT_NODE;
	};
#endif



#endif
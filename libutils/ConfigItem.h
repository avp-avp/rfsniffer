#pragma once
#pragma warning (disable: 4251)

#include "libutils.h"
#include "strutils.h"

#ifdef _LIBUTILS_USE_XML

#pragma warning (disable: 4275)
class CConfigItem;
class LIBUTILS_API CConfigItemList:
	public vector<CConfigItem*>
{
public:
	CConfigItemList();
	CConfigItemList(const CConfigItemList& cpy);

	~CConfigItemList();
};

#ifdef _LIBUTILS_USE_XML_LIBXML2
	typedef struct _xmlNode xmlNode;
	typedef xmlNode *xmlNodePtr;
#endif

class LIBUTILS_API CConfigItem
{
	friend class CXmlDocument;
#ifdef _LIBUTILS_USE_XML_LIBXML2
	xmlNodePtr m_Node;
#endif

public:
	CConfigItem(void);
	CConfigItem(const CConfigItem& cpy);
#ifdef _LIBUTILS_USE_XML_LIBXML2
	CConfigItem(xmlNodePtr node);
#endif
	virtual ~CConfigItem(void);
	
	static void ParseXPath(string Path, string &First, string &Other);
/*
	static string GetValue(const char* path);
	static string GetAttribute(const char* path);
	static CConfigItem GetElement(const char* path);
	static CConfigItemList GetElementList(const char* path);
*/
	//string GetValue();
	//string GetValue(string path);
	string GetAttribute(string path, bool bMandatory=true, string defaultValue="");
	int GetAttributeInt(string path, bool bMandatory=true, string defaultValue=""){return atoi(GetAttribute(path, bMandatory, defaultValue));};
	CConfigItem GetElement(string path);
	void GetElementList(string path, CConfigItemList &list);

#ifdef _LIBUTILS_USE_XML_LIBXML2
	bool isEmpty(){return m_Node==NULL;};
	bool isNode();
	xmlNodePtr GetNode(){return m_Node;};
	void SetNode(xmlNodePtr node);
	void SetNode(const CConfigItem& node){SetNode(node.m_Node);};
#endif
};

#endif

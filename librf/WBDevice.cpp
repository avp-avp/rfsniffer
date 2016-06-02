#include "stdafx.h"
#include "WBDevice.h"

const char *g_Topics[] =
{
	"error",
	"switch",
	"alarm",
	"pushbutton",
	"range",
	"rgb",
	"text",
	"value",
	"temperature",
	"rel_humidity",
	"",
};

CWBDevice::CWBDevice()
{

}

CWBDevice::CWBDevice(string Name, string Description)
:m_Name(Name), m_Description(Description)
{

}


CWBDevice::~CWBDevice()
{
	for_each(CControlMap, m_Controls, i)
		delete i->second;
}

#ifdef USE_CONFIG
void CWBDevice::Init(CConfigItem config)
{
	m_Name = config.GetAttribute("Name");
	m_Description = config.GetAttribute("Description");

	CConfigItemList controls;
	config.GetElementList("Control", controls);
	for_each(CConfigItemList, controls, control)
	{
		CWBControl *Control = new CWBControl;
		Control->Name = (*control)->GetAttribute("Name");
		Control->Source = (*control)->GetAttribute("Source", false);
		Control->SourceType = (*control)->GetAttribute("SourceType", false);
		Control->Readonly = atoi((*control)->GetAttribute("Readonly", false, "1")) != 0;
		string type = (*control)->GetAttribute("Type");
		Control->Type = CWBControl::Error;
		for (int i = 0; g_Topics[i][0];i++)
		{
			if (type == g_Topics[i])
			{
				Control->Type = (CWBControl::ControlType)i;
				break;
			}
		}

		m_Controls[Control->Name] = Control;
	}
}
#endif

void CWBDevice::AddControl(string Name, CWBControl::ControlType Type, bool ReadOnly, string Source, string SourceType)
{
	CWBControl *Control = new CWBControl;
	Control->Name = Name;
	Control->Source = Source;
	Control->SourceType = SourceType;
	Control->Readonly = ReadOnly;
	Control->Type = Type;
	m_Controls[Control->Name] = Control;
}


void CWBDevice::set(string Name, string Value)
{
	CControlMap::iterator i = m_Controls.find(Name);

	if (i == m_Controls.end())
		throw CHaException(CHaException::ErrBadParam, Name);

	i->second->fValue = (float)atof(Value);
	i->second->sValue = Value;
	i->second->Changed = true;
}

void CWBDevice::set(string Name, float Value)
{
	CControlMap::iterator i = m_Controls.find(Name);

	if (i == m_Controls.end())
		throw CHaException(CHaException::ErrBadParam, Name);

	i->second->fValue = Value;
	i->second->sValue = ftoa(Value);
	i->second->Changed = true;
}

void CWBDevice::CreateDeviceValues(string_map &v)
{
	string base = "/devices/" + m_Name;
	v[base + "/meta/name"] = m_Description;

	for_each(CControlMap, m_Controls, i)
	{
		v[base + "/meta/name"] = m_Description;
		v[base + "/controls/" + i->first] = i->second->sValue;
		v[base + "/controls/" + i->first +"/meta/type"] = g_Topics[i->second->Type];
		if (i->second->Readonly)
			v[base + "/controls/" + i->first + "/meta/readonly"] = "1";
	}
	//UpdateValues(v);
}

void CWBDevice::UpdateValues(string_map &v)
{
	string base = "/devices/" + m_Name;

	for_each(CControlMap, m_Controls, i)
	{
		if (i->second->Changed)
		{
			v[base + "/controls/" + i->first] = i->second->sValue;
			i->second->Changed = false;
		}
	}
}

bool CWBDevice::sourceExists(string source)
{
	for_each(CControlMap, m_Controls, i)
	{
		if (i->second->Source == source)
			return true;
	}

	return false;
}

void CWBDevice::setBySource(string source, string sourceType, string Value)
{
	if (sourceType=="X10")
		Value = Value=="ON"?"1":"0";

	for_each(CControlMap, m_Controls, i)
	{
		if (i->second->Source == source)
			set(i->first, Value);
	}
}

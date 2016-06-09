#include "stdafx.h"
#include "../libutils/logging.h"
#include "../libutils/Exception.h"
#include "MqttConnect.h"


CMqttConnection::CMqttConnection(string Server, CLog* log)
:m_isConnected(false), mosquittopp("TestMqttConnection")
{
	m_Server  = Server;
	m_Log = log;

	connect(m_Server.c_str());
	loop_start();

}

CMqttConnection::~CMqttConnection()
{
	loop_stop(true);
}


void CMqttConnection::on_connect(int rc)
{
	m_Log->Printf(1, "mqtt::on_connect(%d)", rc);
	
	if (!rc)
	{
		m_isConnected = true;
	}
}

void CMqttConnection::on_disconnect(int rc)
{
	m_isConnected = false;
	m_Log->Printf(1, "mqtt::on_disconnect(%d)", rc);
}

void CMqttConnection::on_publish(int mid)
{
	m_Log->Printf(5, "mqtt::on_publish(%d)", mid);
}

void CMqttConnection::on_message(const struct mosquitto_message *message)
{
	m_Log->Printf(1, "mqtt::on_message(%s=%s)", message->topic, message->payload);
}

void CMqttConnection::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	m_Log->Printf(5, "mqtt::on_subscribe(%d)", mid);
}

void CMqttConnection::on_unsubscribe(int mid)
{
	m_Log->Printf(5, "mqtt::on_message(%d)", mid);
}

void CMqttConnection::on_log(int level, const char *str)
{
	m_Log->Printf(10, "mqtt::on_log(%d, %s)", level, str);
}

void CMqttConnection::on_error()
{
	m_Log->Printf(1, "mqtt::on_error()");
}

void CMqttConnection::NewMessage(string message)
{
	string_vector typeValue;
	SplitString(message, ':', typeValue);

	if (typeValue.size()!=2)
		return;

	if (typeValue[0]=="RST")
	{
		string_vector parts, id, t, h;
		SplitString(typeValue[1], ' ', parts);
		SplitString(parts[0], '=', id);
		SplitString(parts[1], '=', h);
		SplitString(parts[2], '=', t);

		string name = string("RST_")+id[1];
		CWBDevice *dev = m_Devices[name];
		if (!dev)
		{
			string desc = string("RST sensor ")+id[1];
			dev = new CWBDevice(name, desc);
			dev->AddControl("Temperature", CWBControl::Temperature, true);
			dev->AddControl("Humidity", CWBControl::RelativeHumidity, true);
			CreateDevice(dev);
		}

		dev->set("Temperature", t[1]);
		dev->set("Humidity", h[1]);
		m_Log->Printf(3, "Msg from RST %s", message.c_str());
	} 
	else if (typeValue[0]=="Oregon")
	{
		// Oregon:type=1D20 id=51 ch=1 t=23.2 h=39. RSSI=-106 (-106)

		string_vector type, parts, id, ch, t, h;
		SplitString(typeValue[1], ' ', parts);
		SplitString(parts[0], '=', type);
		SplitString(parts[1], '=', id);
		SplitString(parts[2], '=', ch);
		SplitString(parts[3], '=', t);
		SplitString(parts[4], '=', h);

		//oregon_rx_1d20_68_1
		string name = string("oregon_rx_")+type[1]+"_"+id[1]+"_"+ch[1];
		CWBDevice *dev = m_Devices[name];
		if (!dev)
		{

			string desc = string("Oregon sensor [")+type[1]+"] ("+id[1]+"-"+ch[1]+")";
			dev = new CWBDevice(name, desc);
			dev->AddControl("temperature", CWBControl::Temperature, true);
			dev->AddControl("humidity", CWBControl::RelativeHumidity, true);
			CreateDevice(dev);
		}

		dev->set("temperature", t[1]);
		dev->set("humidity", h[1]);
		m_Log->Printf(3, "Msg from RST %s", message.c_str());
	} 
	else if (typeValue[0]=="X10")
	{
		CWBDevice *dev = m_Devices["X10"];
		if (!dev)
		{
			dev = new CWBDevice("X10", "X10");
			dev->AddControl("Command", CWBControl::Text, true);
			CreateDevice(dev);
		}

		dev->set("Command", typeValue[1]);
		m_Log->Printf(3, "Msg from X10 %s", message.c_str());
	}
	else if (typeValue[0]=="Raex" || typeValue[0]=="Livolo")
	{
		CWBDevice *dev = m_Devices["Remotes"];
		if (!dev)
		{
			dev = new CWBDevice("Remotes", "RF Remote controls");
			dev->AddControl("Raex", CWBControl::Text, true);
			dev->AddControl("Livolo", CWBControl::Text, true);
			//dev->AddControl("Raw", CWBControl::Text, true);
			CreateDevice(dev);
		}

		dev->set(typeValue[0], typeValue[1]);
		m_Log->Printf(3, "New RF message %s", message.c_str());
	}

	SendUpdate();
}

void CMqttConnection::SendUpdate()
{
	string_map v;

	for_each(CWBDeviceMap, m_Devices, dev)
	{
		if (dev->second)
			dev->second->UpdateValues(v);
	}

	for_each(string_map, v, i)
	{
		publish(NULL, i->first.c_str(), i->second.size(), i->second.c_str(), 0, true);
		m_Log->Printf(5, "publish %s=%s", i->first.c_str(), i->second.c_str());
	}
}

void CMqttConnection::CreateDevice(CWBDevice* dev)
{
	m_Devices[dev->getName()] = dev;
	string_map v;
	dev->CreateDeviceValues(v);
	for_each(string_map, v, i)
	{
		publish(NULL, i->first.c_str(), i->second.size(), i->second.c_str(), 0, true);
		m_Log->Printf(5, "publish %s=%s", i->first.c_str(), i->second.c_str());
	}
}

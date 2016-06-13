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
	string type, value;
	SplitPair(message, ':', type, value);

	if (type=="RST")
	{
		string_map values;
		SplitValues(value, values);
		string id=values["id"], t=values["t"], h=values["h"];

		string name = string("RST_")+id;
		CWBDevice *dev = m_Devices[name];
		if (!dev)
		{
			string desc = string("RST sensor ")+id;
			dev = new CWBDevice(name, desc);
			dev->AddControl("Temperature", CWBControl::Temperature, true);
			dev->AddControl("Humidity", CWBControl::RelativeHumidity, true);
			CreateDevice(dev);
		}

		dev->set("Temperature", t);
		dev->set("Humidity", h);
		m_Log->Printf(3, "Msg from RST %s", value.c_str());
	} 
	else if (type=="Oregon")
	{
		// Oregon:type=1D20 id=51 ch=1 t=23.2 h=39. RSSI=-106 (-106)

		string_map values;
		SplitValues(value, values);
		string sensorType=values["type"], id=values["id"], ch=values["ch"], t=values["t"], h=values["h"];

		//oregon_rx_1d20_68_1
		string name = string("oregon_rx_")+sensorType+"_"+id+"_"+ch;
		CWBDevice *dev = m_Devices[name];
		if (!dev)
		{
			string desc = string("Oregon sensor [")+sensorType+"] ("+id+"-"+ch+")";
			dev = new CWBDevice(name, desc);
			dev->AddControl("temperature", CWBControl::Temperature, true);

			if (h.length()>0)
				dev->AddControl("humidity", CWBControl::RelativeHumidity, true);
		
			CreateDevice(dev);
		}

		dev->set("temperature", t);
		if (h.length()>0)
			dev->set("humidity", h);

		m_Log->Printf(3, "Msg from Oregon %s", value.c_str());
	} 
	else if (type=="X10")
	{
		CWBDevice *dev = m_Devices["X10"];
		if (!dev)
		{
			dev = new CWBDevice("X10", "X10");
			dev->AddControl("Command", CWBControl::Text, true);
			CreateDevice(dev);
		}

		dev->set("Command", value);
		m_Log->Printf(3, "Msg from X10 %s", message.c_str());
	}
	else if (type=="Raex" || type=="Livolo")
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

		dev->set(type, value);
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

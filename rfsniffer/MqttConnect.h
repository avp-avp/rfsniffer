#include "mosquittopp.h"
#include "../librf/WBDevice.h"


class CMqttConnection
:public mosqpp::mosquittopp
{
	string m_Server;
	CLog *m_Log;
	bool m_isConnected;
	CWBDeviceMap m_Devices;

public:
	CMqttConnection(string Server, CLog* log);	
	~CMqttConnection();	
	void NewMessage(string message);

private:
    virtual void on_connect(int rc);
    virtual void on_disconnect(int rc);
    virtual void on_publish(int mid);
    virtual void on_message(const struct mosquitto_message *message);
    virtual void on_subscribe(int mid, int qos_count, const int *granted_qos);
    virtual void on_unsubscribe(int mid);
    virtual void on_log(int level, const char *str);
    virtual void on_error();

    void SendUpdate();
    void CreateDevice(CWBDevice* dev);
};

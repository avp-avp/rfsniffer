#include "mosquittopp.h"
#include "../libs/libwb/WBDevice.h"
#include "../libs/librf/RFProtocolNooLite.h"

class RFM69OOK;

class CMqttConnection
:public mosqpp::mosquittopp
{
	string m_Server;
	CLog *m_Log;
	bool m_isConnected;
	CWBDeviceMap m_Devices;
    RFM69OOK *m_RFM;
    CRFProtocolNooLite m_nooLite;

public:
	CMqttConnection(string Server, CLog* log, RFM69OOK *rfm);	
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

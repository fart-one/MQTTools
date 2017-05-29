#include <PubSubClient.h>
#include <ESP8266Wifi.h>

#ifndef MQTTools_h
#define MQTTools_h

class MQTTools	{

	public:
		MQTTools(WiFiClientSecure* wclient, PubSubClient* client, const char* mqttServer, int mqttPort, const char* mqttFingerprint, const char*  mqttUser, const char* mqttPass);
//		MQTTools(WiFiClient* wclient, PubSubClient* client, const char* mqttServer, int mqttPort, const char* mqttFingerprint, const char*  mqttUser, const char* mqttPass);
		int Connect(void);
		int Publish(String topic, String message);
		bool VerifyTLSFingerprint(void);
	private:
		const char* _mqttFingerprint;
		const char* _mqttUser;
		const char* _mqttPass;
		const char* _mqttServer;
		int _mqttPort;
		int mqttQOS = 1;
		WiFiClientSecure* _wclient;
//		WiFiClient* _wclient;
		PubSubClient* _client;
		int _mqttReconnectTime = 5; // seconds

};


#endif

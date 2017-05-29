#include <MQTTools.h>

MQTTools::MQTTools(WiFiClientSecure* wclient, PubSubClient* client, const char* mqttServer, int mqttPort, const char* mqttFingerprint, const char* mqttUser, const char* mqttPass)	{
	_mqttFingerprint = mqttFingerprint;
	_mqttUser = mqttUser;
	_mqttPass = mqttPass;
	_mqttServer = mqttServer;
	_mqttPort = mqttPort;
	_wclient = wclient;
	_client = client;
}
//MQTTools::MQTTools(WiFiClient* wclient, PubSubClient* client, const char* mqttServer, int mqttPort, const char* mqttFingerprint, const char* mqttUser, const char* mqttPass)	{
//	_mqttFingerprint = mqttFingerprint;
//	_mqttUser = mqttUser;
//	_mqttPass = mqttPass;
//	_mqttServer = mqttServer;
//	_mqttPort = mqttPort;
//	_wclient = wclient;
//	_client = client;
//}


int MQTTools::Connect(void)	{
/* 
Connect to MQTT Server. Returns:
	0 - if properly connected to MQTT
	1 - if connected to WiFi but can't connect to MQTT
	2 - if checking fingerprint failed
	3 - if not connected to WiFi
*/
	String deviceId = String(ESP.getChipId());
	if (WiFi.status() == WL_CONNECTED) {	

		// connect to socket
		while (!_wclient->connect(_mqttServer, _mqttPort)) {
			Serial.println("-!>Can't connect to socket. Will try again.");
			delay(_mqttReconnectTime * 1000);
		}

		// check fingerprint
//		if (!this->VerifyTLSFingerprint()) {
//			Serial.println("-!>TLS fingerprint failed");
//			return 2;
//		};

		// connect to MQTT Server	
		if(!_client->connected()) {
 			if (_client->connect(MQTT::Connect(deviceId).set_auth(_mqttUser, _mqttPass).set_keepalive(60))) {
			        Serial.println("--> MQTT server - connected.");
      			} else {
        			Serial.println("-!>Could not connect to MQTT server");
				return 1;
			}
		}
	} else {
		Serial.println("-!>No WiFi connection");
		return 3;
	}
	return 0;
	
}
	

int MQTTools::Publish(String topic, String message) {
	bool sent;
	if (_client->connected()) {
		Serial.println("-?> Publishing: "+message+" -> "+topic);
		//_client.set_callback(mqttCallback);
		sent=_client->publish(MQTT::Publish(topic, message).set_qos(1).set_retain());
		if(!sent)	{
			Serial.println("-!> Error while sending message: \"connection lost, or message too large\"");
			return 1;
		} else {
			Serial.println("--> Message sent correctly");
			return 0;
		}
	} else {
		Serial.println("-!>Not connected to MQTT server. Cant publish.");
		return 0;
	}
}

bool MQTTools::VerifyTLSFingerprint(void) {
	if (_wclient->verify(_mqttFingerprint, _mqttServer)) {
		Serial.println("--> TLS fingerprint matches");
		return true;
	} else {
		Serial.println("-!> Certificate doesn't match. Can't connect!");
		return false;
	}	
}



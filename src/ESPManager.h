/*
 * ESPManager.h
 *
 *  Created on: May 22, 2017
 *      Author: asl
 */

#ifndef ESPMANAGER_H_
#define ESPMANAGER_H_

#include <Esp.h>
#include <PinManager.h>
#include <SettingsManager.h>
#include <ESP8266WiFi.h>
#include <MqttManager.h>
#include <WebServerManager.h>

class ESPManager {
public:
	ESPManager();
	virtual ~ESPManager();

	void begin();
	void loop();

	void attachPubSubListener(String topic, MqttManager::TPubSubListenerFunction listenerFunction);
	void sendMessage(String topic, String message);

	void attachWebHandler(String url, HTTPMethod method, String contentType, WebServerManager::TActionFunction actionFunction, WebServerManager::TPrepareResponseFunction responseFunction);

	void registerPin(int pin, String name, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinReaderFunction readerFunction, PinManager::TPinHandlerFunction handlerFanction);
	void registerPin(int pin, String name, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinHandlerFunction handlerFanction);
	void registerPin(int pin, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinReaderFunction readerFunction, PinManager::TPinHandlerFunction handlerFanction);
	void registerPin(int pin, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinHandlerFunction handlerFanction);
	void registerPin(int pin, String name, Direction direction, PinType pinType);
	void registerPin(int pin, Direction direction, PinType pinType);

private:
	void enableWifi();
	void enableMqtt();
	void enableWebServer();
	bool connectToWifiNetwork(String id, String pwd);
	void setupAccessPoint();
	String getJsonValue(String property, String json);

	SettingsManager _settingsManager;
	PinManager _pinManager;
	WiFiClient _wifiClient;
	PubSubClient _mqttClient;
	MqttManager _mqttManager;
	ESP8266WebServer _webServer;
	WebServerManager _webServerManager;

	String _ssid = "";
	String _password = "";

	String _mqttHost = "";
	int _mqttPort = 0;
	String _mqttUser = "";
	String _mqttPassword = "";

	const char* AP_ID = "ESP8266_AP";
	const char* AP_PASSWORD = "ESP8266_AP";

	const String NETWORK_STATUS_JSON = String("{\"network\":{\"status\": \"%s\",\"ssid\": \"%s\",\"macaddress\": \"%02x:%02x:%02x:%02x:%02x:%02x\",\"mode\": \"%s\"}}");
	const String NETWORKS_JSON = String("{\"ssid\": \"%s\",\"rssi\": \"%s\",\"encryption\": \"%s\"}");
	const String URLS_JSON = String("{\"url\":\"%s\",\"method\":\"%s\"}");
	const String MQTT_STATUS_JSON = String("{\"mqtt\":{\"status\":\"%s\",\"host\":\"%s\",\"port\":\"%s\",\"user\":\"%s\",\"listeners\":[%s]}}");
	const String PINS_STATUS_JSON = String("{\"pin\":\"%d\", \"name\":\"%s\", \"direction\":\"%s\", \"type\":\"%s\", \"event\":[%s], \"value\":\"%d\"}");
	const String CHIP_JSON = String("{\"chipid\":\"%s\"}");
};

#endif /* ESPMANAGER_H_ */

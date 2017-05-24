/*
 * ESPManager.cpp
 *
 *  Created on: May 22, 2017
 *      Author: asl
 */

#include "ESPManager.h"
#include "WebServerPages.h"

ESPManager::ESPManager() :
	_settingsManager(512),
	_pinManager(),
	_wifiClient(),
	_mqttClient(_wifiClient),
	_mqttManager(_mqttClient),
	_webServer(80),
	_webServerManager(_webServer) {
}

ESPManager::~ESPManager() {

}

void ESPManager::begin() {
	_settingsManager.registerSetting("ssid", 32);
	_settingsManager.registerSetting("password", 32);
	_settingsManager.registerSetting("mqttHost", 32);
	_settingsManager.registerSetting("mqttPort", 4);
	_settingsManager.registerSetting("mqttUser", 32);
	_settingsManager.registerSetting("mqttPassword", 32);

	enableWifi();

	enableMqtt();

	enableWebServer();
}

void ESPManager::loop() {
	_pinManager.loop();
	_mqttManager.loop();
	_webServerManager.loop();
}

void ESPManager::enableWifi() {
	//delay(10000);
	_ssid = _settingsManager.loadString("ssid");
	_password = _settingsManager.loadString("password");

	if (WiFi.status() == WL_CONNECTED /*
		|| (WiFi.getMode() == WIFI_STA && WiFi.localIP() != IPAddress(0, 0, 0, 0))
		|| (WiFi.getMode() == WIFI_AP_STA && WiFi.softAPIP() != IPAddress(0, 0, 0, 0))*/) {
		return;
	}

	if (_ssid.length() == 0 || _password.length() == 0 || !connectToWifiNetwork(_ssid, _password)) {
		setupAccessPoint();
	}
}

bool ESPManager::connectToWifiNetwork(String id, String pwd) {
	WiFi.disconnect();

	WiFi.mode(WIFI_STA);
	WiFi.begin(id.c_str(), pwd.c_str());

	int attempts = 50;
	int currentAttempt = 0;
	while (WiFi.status() != WL_CONNECTED && currentAttempt < attempts) {
		currentAttempt++;
		delay(500);
	}

	bool connected = WiFi.status() == WL_CONNECTED;
	return connected;
}

void ESPManager::setupAccessPoint() {
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
	WiFi.softAP(AP_ID, AP_PASSWORD);

	delay(1000);
}

void ESPManager::enableMqtt() {
	_mqttHost = _settingsManager.loadString("mqttHost");
	_mqttPort = _settingsManager.loadInt("mqttPort");;
	_mqttUser = _settingsManager.loadString("mqttUser");;
	_mqttPassword = _settingsManager.loadString("mqttPassword");

	_mqttManager.start(_mqttHost, _mqttPort, _mqttUser, _mqttPassword);
	Serial.println("Connected=" + String((_mqttManager.isConnected() ? "yes": "no")));

	_mqttManager.attachPubSubListener("echo/topic", [](String topic, String payload) {
		Serial.println(payload);
	});
}

void ESPManager::enableWebServer() {
	WebServerManager::TPrepareResponseFunction networkStatusResponseFunction = [this](bool actionSucceed, String payload) -> String {
		int length = NETWORK_STATUS_JSON.length() + String("connected").length() + (WiFi.getMode() == WIFI_STA ? _ssid.length() : String(AP_ID).length());
		char* response = new char[length];
		byte macAddress[6];
		WiFi.macAddress(macAddress);
		sprintf(response, NETWORK_STATUS_JSON.c_str(), "connected", (WiFi.getMode() == WIFI_STA ? _ssid.c_str() : AP_ID), macAddress[5], macAddress[4], macAddress[3], macAddress[2], macAddress[1], macAddress[0], (WiFi.getMode() == WIFI_STA ? "STA" : "AP"));
		String responseString = String(response);
		delete[] response;
		return responseString;
	};

	WebServerManager::TPrepareResponseFunction mqttStatusResponseFunction = [this](bool actionSucceed, String payload) -> String {
		String listeners = "";
		String* listenersArray = _mqttManager.getListenersNames();
		int listenersCount = _mqttManager.getListenersCount();
		for (int i = 0; i < listenersCount; i++) {
			listeners += "\"" + listenersArray[i] + "\"";
			if (i < (listenersCount - 1)) {
				listeners += ",";
			}
		}
		delete[] listenersArray;

		int length = MQTT_STATUS_JSON.length() + String("not connected").length() + _mqttHost.length() + String(_mqttPort).length() + _mqttUser.length() + listeners.length();
		char* response = new char[length];
		sprintf(response, MQTT_STATUS_JSON.c_str(), (_mqttManager.isConnected() ? "connected" : "not connected"), _mqttHost.c_str(), String(_mqttPort).c_str(), _mqttUser.c_str(), listeners.c_str());
		String responseString = String(response);
		delete[] response;
		return responseString;
	};

	_webServerManager.attachWebHandler("/main.css", HTTP_GET, "text/css", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		return MAIN_CSS;
	});

	_webServerManager.attachWebHandler("/table.js", HTTP_GET, "application/javascript", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		return TABLE_JS;
	});

	_webServerManager.attachWebHandler("/wifistatus.js", HTTP_GET, "application/javascript", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		return WIFISTATUS_JS;
	});

	_webServerManager.attachWebHandler("/wifinetworks.js", HTTP_GET, "application/javascript", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		return WIFINETWORKS_JS;
	});

	_webServerManager.attachWebHandler("/mqttstatus.js", HTTP_GET, "application/javascript", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		return MQTTSTATUS_JS;
	});

	_webServerManager.attachWebHandler("/pinstatus.js", HTTP_GET, "application/javascript", [](String payload) -> bool {
		return true;
	}, [](bool actionSucceed, String payload) -> String {
		return PINSTATUS_JS;
	});

	_webServerManager.attachWebHandler("/main.js", HTTP_GET, "application/javascript", [](String payload) -> bool {
		return true;
	}, [](bool actionSucceed, String payload) -> String {
		return MAIN_JS;
	});

	_webServerManager.attachWebHandler("/", HTTP_GET, "text/html", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		return MAIN_HTML;
	});

	_webServerManager.attachWebHandler("/wifi/network/status", HTTP_GET, "application/json", [this](String payload) -> bool {
		return true;
	}, networkStatusResponseFunction);

	_webServerManager.attachWebHandler("/wifi/network/connect", HTTP_POST, "application/json", [this](String payload) -> bool {
		payload.replace("\n", "");

		String ssid = getJsonValue("ssid", payload);
		String password = getJsonValue("password", payload);

		bool connected = connectToWifiNetwork(ssid, password);
		if (!connected) {
			setupAccessPoint();
		}

		_settingsManager.save("ssid", ssid);
		_settingsManager.save("password", password);

		return connected;
	}, networkStatusResponseFunction);

	_webServerManager.attachWebHandler("/wifi/accesspoint/start", HTTP_GET, "application/json", [this](String payload) -> bool {
		setupAccessPoint();
		return true;
	}, networkStatusResponseFunction);

	_webServerManager.attachWebHandler("/mqtt/status", HTTP_GET, "application/json", [this](String payload) -> bool {
		return true;
	}, mqttStatusResponseFunction);

	_webServerManager.attachWebHandler("/mqtt/connect", HTTP_POST, "application/json", [this](String payload) -> bool {
		payload.replace("\n", "");

		_mqttHost = getJsonValue("host", payload);
		_mqttPort = (getJsonValue("port", payload)).toInt();
		_mqttUser = getJsonValue("user", payload);
		_mqttPassword = getJsonValue("password", payload);

		_mqttManager.start(_mqttHost, _mqttPort, _mqttUser, _mqttPassword);

		_settingsManager.save("mqttHost", _mqttHost);
		_settingsManager.save("mqttPort", _mqttPort);
		_settingsManager.save("mqttUser", _mqttUser);
		_settingsManager.save("mqttPassword", _mqttPassword);

		return true;
	}, mqttStatusResponseFunction);

	_webServerManager.attachWebHandler("/pin/status", HTTP_GET, "application/json", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		int* pins;
		int pinsCount;
		if (payload.length() > 0) {
			int pin = payload.toInt();
			pinsCount = 1;
			pins = new int[pinsCount];
			pins[0] = pin;
		} else {
			pinsCount = _pinManager.getRegisteredPinsCount();
			pins = _pinManager.getRegisteredPins();
		}

		String responseString = payload.length() > 0 ? "{\"pin\":" : "{\"pins\":[";

		for (int k = 0; k < pinsCount; k++) {
			int pin = pins[k];
			String pinName = _pinManager.getPinName(pin);
			String pinDirection = _pinManager.getPinDirection(pin) == IN ? "IN" : "OUT";
			String pinType = _pinManager.getPinType(pin) == DIGITAL ? "DIGITAL" : "ANALOG";
			int pinValue = _pinManager.getPinValue(pin);
			int eventsCount = _pinManager.getPinEventsCount(pin);
			PinEvent* events = _pinManager.getPinEvents(pin);
			String eventsString = "";
			for (int i = 0; i < eventsCount; i++) {
				eventsString += (events[i] == LOW_STATE ? "\"LOW\"" : (events[i] == HIGH_STATE ? "\"HIGH\"" : "\"ANY\""));
				if (i < (eventsCount - 1)) {
					eventsString += ",";
				}
			}
			int length = PINS_STATUS_JSON.length() + String(pin).length() + pinName.length() + pinDirection.length() + pinType.length() + eventsString.length() + String(pinValue).length();
			char* response = new char[length];
			sprintf(response, PINS_STATUS_JSON.c_str(), pin, pinName.c_str(), pinDirection.c_str(), pinType.c_str(), eventsString.c_str(), pinValue);
			responseString += String(response);
			if (k < (pinsCount - 1)) {
				responseString += ",";
			}
			delete[] response;
		}
		delete[] pins;

		responseString += payload.length() > 0 ? "}" : "]}";
		return responseString;
	});

	_webServerManager.attachWebHandler("/pin/set", HTTP_POST, "application/json", [this](String payload) -> bool {
		payload.replace("\n", "");

		int pin = (getJsonValue("pin", payload)).toInt();
		int value = (getJsonValue("value", payload)).toInt();
		_pinManager.setPinValue(pin, value);
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		return "{\"result\":\"success\"}";
	});

	_webServerManager.attachWebHandler("/url/mappings", HTTP_GET, "application/json", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		String response = String("{\"urls\":[");
		String* urls = _webServerManager.getRegisteredUrls();
		for (int i = 0; i < _webServerManager.getRegisteredWebHandlersCount(); i++) {
			int length = URLS_JSON.length() + 22 + urls[i].length() + 4;
			char* urlJson = new char[length];
			String url = "http://" + (WiFi.getMode() == WIFI_STA ? WiFi.localIP().toString() : WiFi.softAPIP().toString()) + urls[i];
			sprintf(urlJson, URLS_JSON.c_str(), url.c_str(), (_webServerManager.getHTTPMethod(urls[i]) == HTTP_GET ? "GET" : "POST"));
			response += String(urlJson);
			if (i < _webServerManager.getRegisteredWebHandlersCount() - 1) {
				response += String(",");
			}
			delete[] urlJson;
		}
		delete[] urls;
		response += String("]}");
		return response;
	});

	_webServerManager.attachWebHandler("/wifi/networks", HTTP_GET, "application/json", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		int n = WiFi.scanNetworks();
		String response = String("{\"networks\":[");
		for (int i = 0; i < n; i++) {
			String ssid = String(WiFi.ESP8266WiFiScanClass::SSID(i));
			String rssi = String(WiFi.ESP8266WiFiScanClass::RSSI(i));
			String encryptionType = String(WiFi.encryptionType(i) == ENC_TYPE_NONE ? "none" : "encrypted");

			int length = NETWORKS_JSON.length() + ssid.length() + rssi.length() + encryptionType.length();
			char* networkJson = new char[length];
			sprintf(networkJson, NETWORKS_JSON.c_str(), ssid.c_str(), rssi.c_str(), encryptionType.c_str());
			response += String(networkJson);
			if (i < (n - 1)) {
				response += String(",");
			}
			delete[] networkJson;
		}
		response += String("]}");
		return response;
	});

	_webServerManager.attachWebHandler("/system/chip", HTTP_GET, "application/json", [this](String payload) -> bool {
		return true;
	}, [this](bool actionSucceed, String payload) -> String {
		String chipId = String(ESP.getChipId());
		int length = CHIP_JSON.length() + chipId.length();
		char* response = new char[length];
		sprintf(response, CHIP_JSON.c_str(), chipId.c_str());
		String responseString = String(response);
		delete[] response;
		return responseString;
	});

	_webServerManager.start();
}

String ESPManager::getJsonValue(String property, String json) {
	String value = "";
	int index = json.indexOf("\"" + property + "\"");
	if (index > 0 && (index = json.indexOf(":", index)) > 0 && (index = json.indexOf("\"", index)) > 0) {
		index++;
		while (json[index] != '\"') {
			value += json[index++];
		}
	}
	return value;
}

void ESPManager::attachPubSubListener(String topic, MqttManager::TPubSubListenerFunction listenerFunction) {
	_mqttManager.attachPubSubListener(topic, listenerFunction);
}

void ESPManager::sendMessage(String topic, String message) {
	_mqttManager.sendMessage(topic, message);
}

void ESPManager::attachWebHandler(String url, HTTPMethod method, String contentType, WebServerManager::TActionFunction actionFunction, WebServerManager::TPrepareResponseFunction responseFunction) {
	_webServerManager.attachWebHandler(url, method, contentType, actionFunction, responseFunction);
}

void ESPManager::registerPin(int pin, String name, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinReaderFunction readerFunction, PinManager::TPinHandlerFunction handlerFanction) {
	_pinManager.registerPin(pin, name, direction, pinType, pinEvent, readerFunction, handlerFanction);
}

void ESPManager::registerPin(int pin, String name, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinHandlerFunction handlerFanction) {
	_pinManager.registerPin(pin, name, direction, pinType, pinEvent, handlerFanction);
}

void ESPManager::registerPin(int pin, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinReaderFunction readerFunction, PinManager::TPinHandlerFunction handlerFanction) {
	_pinManager.registerPin(pin, direction, pinType, pinEvent, readerFunction, handlerFanction);
}

void ESPManager::registerPin(int pin, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinHandlerFunction handlerFanction) {
	_pinManager.registerPin(pin, direction, pinType, pinEvent, handlerFanction);
}

void ESPManager::registerPin(int pin, String name, Direction direction, PinType pinType) {
	_pinManager.registerPin(pin, name, direction, pinType);
}

void ESPManager::registerPin(int pin, Direction direction, PinType pinType) {
	_pinManager.registerPin(pin, direction, pinType);
}

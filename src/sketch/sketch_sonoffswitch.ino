#include <Esp.h>
#include <PinManager.h>
#include <SettingsManager.h>
#include <ESP8266WiFi.h>
#include <MqttManager.h>
#include <WebServerManager.h>
#include <WebServerPages.h>

const int BUTTON = 0;
const int RELAY = 12;
const int LED = 13;

enum RelayState { RELAY_ON, RELAY_OFF };

const char* AP_ID = "ESP8266_AP";
const char* AP_PASSWORD = "ESP8266_AP";
String relayStatusUpdateTopic = String(ESP.getChipId()) + "/" + String(RELAY) + "/status/topic";

const String NETWORK_STATUS_JSON = String("{\"network\":{\"status\": \"%s\",\"ssid\": \"%s\",\"macaddress\": \"%02x:%02x:%02x:%02x:%02x:%02x\",\"mode\": \"%s\"}}");
const String NETWORKS_JSON = String("{\"ssid\": \"%s\",\"rssi\": \"%s\",\"encryption\": \"%s\"}");
const String URLS_JSON = String("{\"url\":\"%s\",\"method\":\"%s\"}");
const String MQTT_STATUS_JSON = String("{\"mqtt\":{\"status\":\"%s\",\"host\":\"%s\",\"port\":\"%s\",\"user\":\"%s\",\"listeners\":[%s]}}");
const String PINS_STATUS_JSON = String("{\"pin\":\"%d\", \"name\":\"%s\", \"direction\":\"%s\", \"type\":\"%s\", \"event\":[%s], \"value\":\"%d\"}");

SettingsManager settingsManager(512);
PinManager pinManager;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
MqttManager mqttManager(mqttClient);
ESP8266WebServer webServer(80);
WebServerManager webServerManager(webServer);

RelayState relayState = RELAY_OFF;
long switchTime = millis();

String ssid = "";
String password = "";

String mqttHost = "";
int mqttPort = 0;
String mqttUser = "";
String mqttPassword = "";

void setup() {
  Serial.begin(115200);
  
  settingsManager.registerSetting("ssid", 32);
  settingsManager.registerSetting("password", 32);
  settingsManager.registerSetting("mqttHost", 32);
  settingsManager.registerSetting("mqttPort", 4);
  settingsManager.registerSetting("mqttUser", 32);
  settingsManager.registerSetting("mqttPassword", 32);
  Serial.println("Settings are registered");
  
  pinManager.registerPin(BUTTON, "button", IN, DIGITAL, LOW_STATE, [](int pin, int value) {
    //button pressed
    Serial.println("button pressed");
    pinManager.setPinValue(LED, LOW);
  });

  pinManager.registerPin(BUTTON, "button", IN, DIGITAL, HIGH_STATE, [](int pin, int value) {
    //button released
    Serial.println("button released");
    pinManager.setPinValue(LED, HIGH);

    long currentTime = millis();
    if (currentTime - switchTime < 500) {
      return;
    }
    
    (relayState == RELAY_OFF) ? setRelayState(RELAY_ON) : setRelayState(RELAY_OFF);

    switchTime = currentTime;
  });

  pinManager.registerPin(RELAY, "relay", OUT, DIGITAL, ANY_STATE, [](int pin, int value) {
    relayState = value == HIGH ? RELAY_ON : RELAY_OFF;      
    publishRelayState();
  });
  
  pinManager.registerPin(LED, "led", OUT, DIGITAL);
  Serial.println("pins are registered");

  setRelayState(relayState);

  enableWifi();

  enableMqtt();

  enableWebServer();
}

void loop() {
  pinManager.loop();
  mqttManager.loop();
  webServerManager.loop();
}

void setRelayState(RelayState state) {
  switch (state) {
    case RELAY_ON: {
      pinManager.setPinValue(RELAY, HIGH);
      break;
    }

    case RELAY_OFF: {
      pinManager.setPinValue(RELAY, LOW);
      break;
    }
  }
}

void publishRelayState() {
  String message = relayState == RELAY_ON ? "ON" : "OFF";
  mqttManager.sendMessage(relayStatusUpdateTopic, message);
  Serial.println("publishing message '" + message + "' to " + relayStatusUpdateTopic);
}

void enableWifi() {
  //delay(10000);
  ssid = settingsManager.loadString("ssid");
  password = settingsManager.loadString("password");

  Serial.println("ssid='" + ssid + "'");
  Serial.println("password='" + password + "'");
  Serial.println("status=" + String(WiFi.status()));
  Serial.println("mode=" + String(WiFi.getMode()));
  Serial.println("localIP=" + WiFi.localIP().toString());
  Serial.println("softIP=" + WiFi.softAPIP().toString());
  

  if (WiFi.status() == WL_CONNECTED /*
        || (WiFi.getMode() == WIFI_STA && WiFi.localIP() != IPAddress(0, 0, 0, 0))
        || (WiFi.getMode() == WIFI_AP_STA && WiFi.softAPIP() != IPAddress(0, 0, 0, 0))*/) {
    Serial.println("connected to network");          
    return;
  }
  
  if (ssid.length() == 0 || password.length() == 0 || !connectToWifiNetwork(ssid, password)) {
    setupAccessPoint();
  }
}

bool connectToWifiNetwork(String id, String pwd) {
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
  Serial.println(String(connected ? "" : "Not") + " connected to " + id + " network");
  return connected;
}

void setupAccessPoint() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(IPAddress(192, 168, 1, 1), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_ID, AP_PASSWORD);

  delay(1000);
  Serial.println("Started as hotspot");
}

void enableMqtt() {
  Serial.println("Reading properties");
  
  mqttHost = settingsManager.loadString("mqttHost");
  mqttPort = settingsManager.loadInt("mqttPort");;
  mqttUser = settingsManager.loadString("mqttUser");;
  mqttPassword = settingsManager.loadString("mqttPassword");

  Serial.println("host='" + mqttHost + "'");
  Serial.println("port='" + String(mqttPort) + "'");
  Serial.println("user='" + mqttUser + "'");
  Serial.println("password='" + mqttPassword + "'");
  
  mqttManager.start(mqttHost, mqttPort, mqttUser, mqttPassword);
  Serial.println("Connected=" + String((mqttManager.isConnected() ? "yes": "no")));
  String topic = String(ESP.getChipId()) + "/command/topic";
  
  mqttManager.attachPubSubListener(topic, [](String topic, String payload) {
    Serial.println("recieved message '" + payload + "' on topic " + topic);
    if (payload == "ON") {
      setRelayState(RELAY_ON);
    } else {
      setRelayState(RELAY_OFF);;
    }
  });

  mqttManager.attachPubSubListener("echo/topic", [](String topic, String payload) {
    Serial.println(payload);
  });

}

void enableWebServer() {
  WebServerManager::TPrepareResponseFunction networkStatusResponseFunction = [](bool actionSucceed, String payload) -> String {
      int length = NETWORK_STATUS_JSON.length() + String("connected").length() + (WiFi.getMode() == WIFI_STA ? ssid.length() : String(AP_ID).length());
      char* response = new char[length];
      byte macAddress[6];
      WiFi.macAddress(macAddress);
      sprintf(response, NETWORK_STATUS_JSON.c_str(), "connected", (WiFi.getMode() == WIFI_STA ? ssid.c_str() : AP_ID), macAddress[5], macAddress[4], macAddress[3], macAddress[2], macAddress[1], macAddress[0], (WiFi.getMode() == WIFI_STA ? "STA" : "AP"));
      String responseString = String(response);
      delete[] response;
      return responseString;
    };

    WebServerManager::TPrepareResponseFunction mqttStatusResponseFunction = [](bool actionSucceed, String payload) -> String {
      String listeners = "";
      String* listenersArray = mqttManager.getListenersNames();
      int listenersCount = mqttManager.getListenersCount();
      for (int i = 0; i < listenersCount; i++) {
        listeners += "\"" + listenersArray[i] + "\"";
        if (i < (listenersCount - 1)) {
          listeners += ",";
        }
      }
      delete[] listenersArray;
      
      int length = MQTT_STATUS_JSON.length() + String("not connected").length() + mqttHost.length() + String(mqttPort).length() + mqttUser.length() + listeners.length();
      char* response = new char[length];
      sprintf(response, MQTT_STATUS_JSON.c_str(), (mqttManager.isConnected() ? "connected" : "not connected"), mqttHost.c_str(), String(mqttPort).c_str(), mqttUser.c_str(), listeners.c_str());
      String responseString = String(response);
      delete[] response;
      return responseString;
    };

  webServerManager.attachWebHandler("/main.css", HTTP_GET, "text/css", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
        return MAIN_CSS;
    });

  webServerManager.attachWebHandler("/table.js", HTTP_GET, "application/javascript", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
        return TABLE_JS;
    });

  webServerManager.attachWebHandler("/wifistatus.js", HTTP_GET, "application/javascript", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
        return WIFISTATUS_JS;
    });

  webServerManager.attachWebHandler("/wifinetworks.js", HTTP_GET, "application/javascript", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
        return WIFINETWORKS_JS;
    });
    
  webServerManager.attachWebHandler("/mqttstatus.js", HTTP_GET, "application/javascript", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
        return MQTTSTATUS_JS;
    });    

  webServerManager.attachWebHandler("/pinstatus.js", HTTP_GET, "application/javascript", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
        return PINSTATUS_JS;
    });

  webServerManager.attachWebHandler("/main.js", HTTP_GET, "application/javascript", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
        return MAIN_JS;
    });

  webServerManager.attachWebHandler("/", HTTP_GET, "text/html", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
        return MAIN_HTML;
    });

  webServerManager.attachWebHandler("/wifi/network/status", HTTP_GET, "application/json", [](String payload) -> bool {
      return true;
    }, networkStatusResponseFunction);

  webServerManager.attachWebHandler("/wifi/network/connect", HTTP_POST, "application/json", [](String payload) -> bool {
      payload.replace("\n", "");

      String ssid = getJsonValue("ssid", payload);
      String password = getJsonValue("password", payload);

      bool connected = connectToWifiNetwork(ssid, password);
      if (!connected) {
        setupAccessPoint();
      }
      
      settingsManager.save("ssid", ssid);
      settingsManager.save("password", password);

      return connected;
    }, networkStatusResponseFunction);

  webServerManager.attachWebHandler("/wifi/accesspoint/start", HTTP_GET, "application/json", [](String payload) -> bool {
      setupAccessPoint();
      return true;
    }, networkStatusResponseFunction);

  webServerManager.attachWebHandler("/mqtt/status", HTTP_GET, "application/json", [](String payload) -> bool {
      return true;
    }, mqttStatusResponseFunction);

  webServerManager.attachWebHandler("/mqtt/connect", HTTP_POST, "application/json", [](String payload) -> bool {
      payload.replace("\n", "");

      mqttHost = getJsonValue("host", payload);
      mqttPort = (getJsonValue("port", payload)).toInt();
      mqttUser = getJsonValue("user", payload);
      mqttPassword = getJsonValue("password", payload);

      mqttManager.start(mqttHost, mqttPort, mqttUser, mqttPassword);

      settingsManager.save("mqttHost", mqttHost);
      settingsManager.save("mqttPort", mqttPort);
      settingsManager.save("mqttUser", mqttUser);
      settingsManager.save("mqttPassword", mqttPassword);

      return true;
    }, mqttStatusResponseFunction);

  webServerManager.attachWebHandler("/pin/status", HTTP_GET, "application/json", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
      int* pins;
      int pinsCount;
      if (payload.length() > 0) {
        int pin = payload.toInt();
        pinsCount = 1;
        pins = new int[pinsCount];
        pins[0] = pin;
      } else {
        pinsCount = pinManager.getRegisteredPinsCount();
        pins = pinManager.getRegisteredPins();
      }

      String responseString = payload.length() > 0 ? "{\"pin\":" : "{\"pins\":[";
      
      for (int k = 0; k < pinsCount; k++) {
        int pin = pins[k];
        String pinName = pinManager.getPinName(pin);
        String pinDirection = pinManager.getPinDirection(pin) == IN ? "IN" : "OUT";
        String pinType = pinManager.getPinType(pin) == DIGITAL ? "DIGITAL" : "ANALOG";
        int pinValue = pinManager.getPinValue(pin);        
        int eventsCount = pinManager.getPinEventsCount(pin);
        PinEvent* events = pinManager.getPinEvents(pin);
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

  webServerManager.attachWebHandler("/pin/set", HTTP_POST, "application/json", [](String payload) -> bool {
      payload.replace("\n", "");

      int pin = (getJsonValue("pin", payload)).toInt();
      int value = (getJsonValue("value", payload)).toInt();
      pinManager.setPinValue(pin, value);
      return true;
    }, [](bool actionSucceed, String payload) -> String {
      return "{\"result\":\"success\"}";
    });

  webServerManager.attachWebHandler("/url/mappings", HTTP_GET, "application/json", [](String payload) -> bool {
     return true;
    }, [](bool actionSucceed, String payload) -> String {
      String response = String("{\"urls\":[");
      String* urls = webServerManager.getRegisteredUrls();
      for (int i = 0; i < webServerManager.getRegisteredWebHandlersCount(); i++) {
        int length = URLS_JSON.length() + 22 + urls[i].length() + 4;
        char* urlJson = new char[length];
        String url = "http://" + (WiFi.getMode() == WIFI_STA ? WiFi.localIP().toString() : WiFi.softAPIP().toString()) + urls[i];
        sprintf(urlJson, URLS_JSON.c_str(), url.c_str(), (webServerManager.getHTTPMethod(urls[i]) == HTTP_GET ? "GET" : "POST"));
        response += String(urlJson);
        delete[] urlJson;
      }
      delete[] urls;
      response += String("]}");
      return response;
    });

  webServerManager.attachWebHandler("/wifi/networks", HTTP_GET, "application/json", [](String payload) -> bool {
      return true;
    }, [](bool actionSucceed, String payload) -> String {
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
    webServerManager.start();
}

String getJsonValue(String property, String json) {
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


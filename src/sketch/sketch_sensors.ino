#include <ESPManager.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

#define DHTTYPE DHT22
#define DHTPIN  12
//#define DHTPIN  4

const int TEMP = 12;
const int HUM = 13;
const int LIGHT = 4;

//const int TEMP = 4;
//const int HUM = 5;

String temperatureStatusUpdateTopic = String(ESP.getChipId()) + "/" + String(TEMP) + "/status/topic";
String humidityStatusUpdateTopic = String(ESP.getChipId()) + "/" + String(HUM) + "/status/topic";
String lightStatusUpdateTopic = String(ESP.getChipId()) + "/" + String(LIGHT) + "/status/topic";

ESPManager espManager;
DHT dht(DHTPIN, DHTTYPE, 0);
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

int lightValue = 0;

void setup() {
  Serial.begin(115200);

  enableDHT();
  enableTSL2561();

  espManager.registerPin(TEMP, "temperature", IN, DIGITAL, ANY_STATE, [](int pin) -> int {
    return dht.readTemperature(false);
  }, [](int pin, int value) {
    String message = String(value);
    espManager.sendMessage(temperatureStatusUpdateTopic, message);
  });

  espManager.registerPin(HUM, "humidity", IN, DIGITAL, ANY_STATE, [](int pin) -> int {
    return dht.readHumidity();
  },
  [](int pin, int value) {
    String message = String(value);
    espManager.sendMessage(humidityStatusUpdateTopic, message);
  });

  espManager.registerPin(LIGHT, "light", IN, ANALOG, ANY_STATE, [](int pin) -> int {
    sensors_event_t event;
    tsl.getEvent(&event);
 
    if (event.light) {
      lightValue = event.light;
    }
    return lightValue;
  },
  [](int pin, int value) {
    String message = String(value);
    espManager.sendMessage(lightStatusUpdateTopic, message);
  });

  espManager.begin();
}

void loop() {
  espManager.loop();
}

void enableDHT() {
  dht.begin();
}

void enableTSL2561() {
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  tsl.begin();
}


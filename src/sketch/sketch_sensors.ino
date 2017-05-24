#include <ESPManager.h>
#include <DHT.h>

#define DHTTYPE DHT22
#define DHTPIN  12

const int TEMP = 12;
const int HUM = 13;

String temperatureStatusUpdateTopic = String(ESP.getChipId()) + "/" + String(TEMP) + "/status/topic";
String humidityStatusUpdateTopic = String(ESP.getChipId()) + "/" + String(HUM) + "/status/topic";

ESPManager espManager;
DHT dht(DHTPIN, DHTTYPE, 0);

void setup() {
  Serial.begin(115200);

  enableDHT();

  espManager.registerPin(TEMP, "temperature", IN, DIGITAL, ANY_STATE, [](int pin) -> int {
    return dht.readTemperature(false);;
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

  espManager.begin();
}

void loop() {
  espManager.loop();
}

void enableDHT() {
  dht.begin();
}


#include <ESPManager.h>

const int BUTTON = 0;
const int RELAY = 12;
const int LED = 13;

enum RelayState { RELAY_ON, RELAY_OFF };

String relayStatusUpdateTopic = String(ESP.getChipId()) + "/" + String(RELAY) + "/status/topic";

ESPManager espManager;

RelayState relayState = RELAY_OFF;
long switchTime = millis();

void setup() {
  Serial.begin(115200);
  
  espManager.registerPin(BUTTON, "button", IN, DIGITAL, LOW_STATE, [](int pin, int value) {
    //button pressed
    Serial.println("button pressed");
    espManager.setPinValue(LED, LOW);
  });

  espManager.registerPin(BUTTON, "button", IN, DIGITAL, HIGH_STATE, [](int pin, int value) {
    //button released
    Serial.println("button released");
    espManager.setPinValue(LED, HIGH);

    long currentTime = millis();
    if (currentTime - switchTime < 500) {
      return;
    }
    
    (relayState == RELAY_OFF) ? setRelayState(RELAY_ON) : setRelayState(RELAY_OFF);

    switchTime = currentTime;
  });

  espManager.registerPin(RELAY, "relay", OUT, DIGITAL, ANY_STATE, [](int pin, int value) {
    relayState = value == HIGH ? RELAY_ON : RELAY_OFF;      
    publishRelayState();
  });
  
  espManager.registerPin(LED, "led", OUT, DIGITAL);
  Serial.println("pins are registered");

  String topic = String(ESP.getChipId()) + "/command/topic";

  espManager.attachPubSubListener(topic, [](String topic, String payload) {
    Serial.println("recieved message '" + payload + "' on topic " + topic);
    if (payload == "ON") {
      setRelayState(RELAY_ON);
    } else {
      setRelayState(RELAY_OFF);;
    }
  });

  setRelayState(relayState);

  espManager.begin();
}

void loop() {
  espManager.loop();
}

void setRelayState(RelayState state) {
  switch (state) {
    case RELAY_ON: {
      espManager.setPinValue(RELAY, HIGH);
      break;
    }

    case RELAY_OFF: {
      espManager.setPinValue(RELAY, LOW);
      break;
    }
  }
}

void publishRelayState() {
  String message = relayState == RELAY_ON ? "ON" : "OFF";
  espManager.sendMessage(relayStatusUpdateTopic, message);
  Serial.println("publishing message '" + message + "' to " + relayStatusUpdateTopic);
}

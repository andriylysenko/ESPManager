/*
 * MqttManager.h
 *
 *  Created on: Apr 23, 2017
 *      Author: asl
 */

#ifndef MQTTMANAGER_H_
#define MQTTMANAGER_H_

#include <functional>
#include <Arduino.h>
#include <PubSubClient.h>

class MqttManager {
public:
	MqttManager(PubSubClient pubSubClient);
	virtual ~MqttManager();

	typedef std::function<void(String topic, String payload)> TPubSubListenerFunction;

	struct PubSubHandler {
		String _topic;
		MqttManager::TPubSubListenerFunction _listenerFunction;

		PubSubHandler* next() {
			return _next;
		}

		void addNext(PubSubHandler* handler) {
			_next = handler;
		}

		private:
		PubSubHandler* _next = nullptr;
	};

	void start(String host, int port, String user, String password);
	bool isConnected();
	void attachPubSubListener(String topic, MqttManager::TPubSubListenerFunction listenerFunction);
	int getListenersCount();
	String* getListenersNames();
	void sendMessage(String topic, String message);
	void loop();

private:
	PubSubClient _pubSubClient;

	PubSubHandler* _firstPubSubHandler;
};

#endif /* MQTTMANAGER_H_ */

/*
 * MqttManager.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: asl
 */

#include "MqttManager.h"

MqttManager::MqttManager(PubSubClient pubSubClient) {
	_pubSubClient = pubSubClient;
	_firstPubSubHandler = nullptr;
}

MqttManager::~MqttManager() {
	PubSubHandler* handler = _firstPubSubHandler;
	while (handler) {
		PubSubHandler* next = handler->next();
		delete handler;
		handler = next;
	}
}

void MqttManager::start(String host, int port, String user, String password) {
	_host = host;
	_port = port;
	_user = user;
	_password = password;

	if (_pubSubClient.connected()) {
		_pubSubClient.disconnect();
	}

	_pubSubClient.setServer(host.c_str(), port);

	_pubSubClient.setCallback([this](char* topic, byte* payload, unsigned int length) {
		String mqttTopic = String(topic);
		String p = "";
		for (int i = 0; i < length; i++) {
			p += char(payload[i]);
		}

		PubSubHandler* handler = _firstPubSubHandler;
		while (handler) {
			if (mqttTopic == handler->_topic) {
				handler->_listenerFunction(mqttTopic, p);
			}
			handler = handler->next();
		}
	});

	(user.length() > 0 && password.length() > 0) ?
		_pubSubClient.connect(String(ESP.getChipId()).c_str(), user.c_str(), password.c_str()) :
		_pubSubClient.connect(String(ESP.getChipId()).c_str());

	if (_pubSubClient.connected()) {
		PubSubHandler* handler = _firstPubSubHandler;
		while (handler) {
			_pubSubClient.subscribe(handler->_topic.c_str());
			handler = handler->next();
		}
	}
}

bool MqttManager::isConnected() {
	return _pubSubClient.connected();
}

void MqttManager::attachPubSubListener(String topic, MqttManager::TPubSubListenerFunction listenerFunction) {
	PubSubHandler* pubSubHandler = new PubSubHandler;
	pubSubHandler->_topic = topic;
	pubSubHandler->_listenerFunction = listenerFunction;

	_pubSubClient.subscribe(topic.c_str());

	if (!_firstPubSubHandler) {
		_firstPubSubHandler = pubSubHandler;
	} else {
		pubSubHandler->addNext(_firstPubSubHandler);
		_firstPubSubHandler = pubSubHandler;
	}
}

int MqttManager::getListenersCount() {
	int count = 0;
	PubSubHandler* pubSubHandler = _firstPubSubHandler;
	while (pubSubHandler) {
		count++;
		pubSubHandler = pubSubHandler->next();
	}
	return count;
}

String* MqttManager::getListenersNames() {
	String* listeners = new String[getListenersCount()];
	int i = 0;
	PubSubHandler* pubSubHandler = _firstPubSubHandler;
	while (pubSubHandler) {
		listeners[i++] = pubSubHandler->_topic;
		pubSubHandler = pubSubHandler->next();
	}
	return listeners;
}

void MqttManager::sendMessage(String topic, String message) {
	_pubSubClient.publish(topic.c_str(), message.c_str(), message.length());
}

void MqttManager::loop() {
	if (!isConnected()) {
		long now = millis();
		if (now - _lastReconnectAttempt > 1000) {
			_lastReconnectAttempt = now;
			start(_host, _port, _user, _password);
			if (isConnected()) {
				_lastReconnectAttempt = 0;
			}
		}
	}
	_pubSubClient.loop();
}


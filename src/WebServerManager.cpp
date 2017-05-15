/*
 * WebServerManager.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: asl
 */

#include "WebServerManager.h"
#include <ESP8266WiFi.h>

WebServerManager::WebServerManager(ESP8266WebServer webServer) {
	_webServer = webServer;
	_firstURLMappings = nullptr;
}

WebServerManager::~WebServerManager() {
	URLMappings* urlMappings = _firstURLMappings;
	while (urlMappings) {
		URLMappings* next = _firstURLMappings->next();
		delete urlMappings;
		urlMappings = next;
	}
}

void WebServerManager::start() {
	_webServer.begin();
}

void WebServerManager::attachWebHandler(String url, HTTPMethod method, String contentType, WebServerManager::TActionFunction actionFunction, WebServerManager::TPrepareResponseFunction responseFunction) {
	_webServer.on(url.c_str(), method, [this, contentType, actionFunction, responseFunction]() {
		String payload = "";
		if (_webServer.hasArg("payload")) {
			payload = _webServer.arg("payload");
		}
		bool success = actionFunction(payload);
		String response = responseFunction(success, payload);
		_webServer.send(200, contentType.c_str(), response.c_str());
	});

	URLMappings* urlMappings = new URLMappings;
	urlMappings->url = url;
	urlMappings->method = method;

	if (!_firstURLMappings) {
		_firstURLMappings = urlMappings;
	} else {
		urlMappings->addNext(_firstURLMappings);
		_firstURLMappings = urlMappings;
	}
}

int WebServerManager::getRegisteredWebHandlersCount() {
	int count = 0;
	URLMappings* urlMappings = _firstURLMappings;
	while (urlMappings) {
		count++;
		urlMappings = urlMappings->next();
	}
	return count;
}

String* WebServerManager::getRegisteredUrls() {
	String *urls = new String[getRegisteredWebHandlersCount()];
	int i = 0;
	URLMappings* urlMappings = _firstURLMappings;
	while (urlMappings) {
		urls[i++] = urlMappings->url;
		urlMappings = urlMappings->next();
	}
	return urls;
}

HTTPMethod WebServerManager::getHTTPMethod(String url) {
	URLMappings* urlMappings = _firstURLMappings;
	while (urlMappings) {
		if (url == urlMappings->url) {
			return urlMappings->method;
		}
		urlMappings = urlMappings->next();
	}
	return HTTP_ANY;
}

void WebServerManager::loop() {
	_webServer.handleClient();
}

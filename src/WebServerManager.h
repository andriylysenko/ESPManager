/*
 * WebServerManager.h
 *
 *  Created on: Apr 23, 2017
 *      Author: asl
 */

#ifndef WEBSERVERMANAGER_H_
#define WEBSERVERMANAGER_H_

#include <ESP8266WebServer.h>

class WebServerManager {
public:
	WebServerManager(ESP8266WebServer webServer);
	virtual ~WebServerManager();

	typedef std::function<bool(String)> TActionFunction;
	typedef std::function<String(bool, String)> TPrepareResponseFunction;

	struct URLMappings {
		String url;
		HTTPMethod method;

		URLMappings* next() {
			return _next;
		}

		void addNext(URLMappings* urlMappings) {
			_next = urlMappings;
		}

		private:
		URLMappings* _next = nullptr;
	};

	void start();
	void attachWebHandler(String url, HTTPMethod method, String contentType, WebServerManager::TActionFunction actionFunction, WebServerManager::TPrepareResponseFunction responseFunction);
	int getRegisteredWebHandlersCount();
	String* getRegisteredUrls();
	HTTPMethod getHTTPMethod(String url);
	void loop();
private:
	ESP8266WebServer _webServer;
	URLMappings* _firstURLMappings;
};

#endif /* WEBSERVERMANAGER_H_ */

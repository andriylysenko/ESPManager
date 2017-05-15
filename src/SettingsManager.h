/*
 * SettingsManager.h
 *
 *  Created on: Apr 23, 2017
 *      Author: asl
 */

#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include <Arduino.h>

class SettingsManager {
public:
	SettingsManager(int size);
	virtual ~SettingsManager();


	struct Setting {
		String _name;
		int _size;

		Setting* next() {
			return _next;
		}

		void addNext(Setting* setting) {
			_next = setting;
		}

		private:
		Setting* _next = nullptr;
	};

	void registerSetting(String name, int size);

	char* loadBytes(String name);
	String loadString(String name);
	int loadInt(String name);

	void save(String name, char* byteArray);
	void save(String name, String value);
	void save(String name, int value);

private:
	int _size;

	Setting* _head;
	Setting* _tail;

	char* readBytes(int start, int end);
	String readString(int start, int end);
	int readInt(int start);

	void writeString(int start, int end, String data);
	void writeBytes(int start, int end, char* data);
	void writeInt(int start, int data);
};

#endif /* SETTINGSMANAGER_H_ */

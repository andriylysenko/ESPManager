/*
 * SettingsManager.cpp
 *
 *  Created on: Apr 23, 2017
 *      Author: asl
 */

#include "SettingsManager.h"
#include <EEPROM.h>

SettingsManager::SettingsManager(int size) {
	_size = size;
	EEPROM.begin(_size);
	_head = nullptr;
	_tail = nullptr;
}

SettingsManager::~SettingsManager() {
	Setting* setting = _head;
	while (setting) {
		Setting* next = setting->next();
		delete setting;
		setting = next;
	}
}

void SettingsManager::registerSetting(String name, int size) {
	Setting* setting = new Setting;
	setting->_name = name;
	setting->_size = size;

	if (!_head) {
		_head = _tail = setting;
	} else {
		_tail->addNext(setting);
		_tail = setting;
	}
}

char* SettingsManager::loadBytes(String name) {
	int start = 0;
	Setting* setting = _head;
	while (setting) {
		if (setting->_name == name) {
			break;
		}
		start += setting->_size;
		setting = setting->next();
	}
	return readBytes(start, start + setting->_size);
}

String SettingsManager::loadString(String name) {
	int start = 0;
	Setting* setting = _head;
	while (setting) {
		if (setting->_name == name) {
			break;
		}
		start += setting->_size;
		setting = setting->next();
	}
	return readString(start, start + setting->_size);
}

void SettingsManager::save(String name, char* byteArray) {
	int start = 0;
	Setting* setting = _head;
	while (setting) {
		if (setting->_name == name) {
			break;
		}
		start += setting->_size;
		setting = setting->next();
	}
	writeBytes(start, start + setting->_size, byteArray);
}

void SettingsManager::save(String name, String value) {
	int start = 0;
	Setting* setting = _head;
	while (setting) {
		if (setting->_name == name) {
			break;
		}
		start += setting->_size;
		setting = setting->next();
	}
	writeString(start, start + setting->_size, value);
}

void SettingsManager::save(String name, int value) {
	int start = 0;
	Setting* setting = _head;
	while (setting) {
		if (setting->_name == name) {
			break;
		}
		start += setting->_size;
		setting = setting->next();
	}
	writeInt(start, value);
}

int SettingsManager::loadInt(String name) {
	int start = 0;
	Setting* setting = _head;
	while (setting) {
		if (setting->_name == name) {
			break;
		}
		start += setting->_size;
		setting = setting->next();
	}
	return readInt(start);
}

char* SettingsManager::readBytes(int start, int end) {
	char* result = new char[end - start];
	for (int i = start; i < end; i++) {
		result[i - start] = char(EEPROM.read(i));
	}
	return result;
}

String SettingsManager::readString(int start, int end) {
	String result = "";
	for (int i = start; i < end; i++) {
		char b = char(EEPROM.read(i));
		if (b == 0) {
			break;
		}
		result += b;
	}
	return result;
}

int SettingsManager::readInt(int start) {
	int result = (EEPROM.read(start) << 24) | (EEPROM.read(start + 1) << 16) | (EEPROM.read(start + 2) << 8) | (EEPROM.read(start + 3));
	return result;
}

void SettingsManager::writeString(int start, int end, String data) {
	for (int i = start; i < end; i++) {
		if (data.length() >= (i - start)) {
			EEPROM.write(i, data[i - start]);
		} else {
			EEPROM.write(i, 0);
		}
	}
	EEPROM.commit();
}

void SettingsManager::writeBytes(int start, int end, char* data) {
	int length = end - start;
	for (int i = start; i <= end; i++) {
		if (length >= (i - start)) {
			EEPROM.write(i, data[i - start]);
		} else {
			EEPROM.write(i, 0);
		}
	}
	EEPROM.commit();
}

void SettingsManager::writeInt(int start, int data) {
	EEPROM.write(start, (data >> 24));
	EEPROM.write(start + 1, (data >> 16));
	EEPROM.write(start + 2, (data >> 8));
	EEPROM.write(start + 3, (data));
	EEPROM.commit();
}

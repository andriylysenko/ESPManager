/*
 * PinManager.cpp
 *
 *  Created on: Apr 22, 2017
 *      Author: asl
 */

#include "PinManager.h"

PinManager::PinManager() {
	_firstPinHandler = nullptr;
}

PinManager::~PinManager() {
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		PinHandler* next = handler->next();
		delete handler;
		handler = next;
	}
}

void PinManager::registerPin(int pin, String name, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinReaderFunction readerFunction, PinManager::TPinHandlerFunction handlerFanction) {
	if (direction == IN) {
		pinMode(pin, INPUT);
	} else {
		pinMode(pin, OUTPUT);
	}

	PinHandler* pinHandler = new PinHandler;
	pinHandler->_pin = pin;
	pinHandler->_name = name;
	pinHandler->_direction = direction;
	pinHandler->_pinType = pinType;
	pinHandler->_pinEvent = pinEvent;
	pinHandler->_readerFunction = readerFunction;
	pinHandler->_handlerFunction = handlerFanction;

	if (!_firstPinHandler) {
		_firstPinHandler = pinHandler;
	} else {
		pinHandler->addNext(_firstPinHandler);
		_firstPinHandler = pinHandler;
	}
}

void PinManager::registerPin(int pin, String name, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinHandlerFunction handlerFanction) {
	registerPin(pin, name, direction, pinType, pinEvent, nullptr, handlerFanction);
}

void PinManager::registerPin(int pin, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinReaderFunction readerFunction, PinManager::TPinHandlerFunction handlerFanction) {
	registerPin(pin, "pin" + String(pin), direction, pinType, pinEvent, readerFunction, handlerFanction);
}

void PinManager::registerPin(int pin, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinHandlerFunction handlerFanction) {
	registerPin(pin, "pin" + String(pin), direction, pinType, pinEvent, handlerFanction);
}

void PinManager::registerPin(int pin, String name, Direction direction, PinType pinType) {
	registerPin(pin, name, direction, pinType, ANY_STATE, nullptr);
}

void PinManager::registerPin(int pin, Direction direction, PinType pinType) {
	registerPin(pin, direction, pinType, ANY_STATE, nullptr);
}

int PinManager::getRegisteredPinsCount() {
	int count = 0;
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		count++;
		handler = handler->next();
	}

	int *pins = new int[count];
	int n = 0;
	handler = _firstPinHandler;
	while (handler) {
		if (n == 0) {
			pins[n++] = handler->_pin;
		} else {
			int i;
			for (i = 0; i < n; i++) {
				if (pins[i] == handler->_pin) {
					break;
				}
			}
			if (i == n) {
				pins[n++] = handler->_pin;
			}
		}
		handler = handler->next();
	}
	delete[] pins;
	return n;
}

int* PinManager::getRegisteredPins() {
	int *pins = new int[getRegisteredPinsCount()];
	int n = 0;
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		if (n == 0) {
			pins[n++] = handler->_pin;
		} else {
			int i;
			for (i = 0; i < n; i++) {
				if (pins[i] == handler->_pin) {
					break;
				}
			}
			if (i == n) {
				pins[n++] = handler->_pin;
			}
		}
		handler = handler->next();
	}
	return pins;
}

int PinManager::getPinValue(int pin) {
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		if (handler->_pin == pin) {
			return handler->_value;
		}
		handler = handler->next();
	}
	return 0;
}

void PinManager::setPinValue(int pin, int value) {
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		if (handler->_pin == pin) {
			handler->handleOut(value);

			switch (handler->_pinType) {
				case DIGITAL: {
					digitalWrite(pin, value > 0 ? HIGH : LOW);
					handler->_value = value > 0 ? HIGH : LOW;
					break;
				}
				case ANALOG: {
					analogWrite(pin, value);
					handler->_value = value;
				}
			}
			return;
		}
		handler = handler->next();
	}
}

String PinManager::getPinName(int pin) {
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		if (handler->_pin == pin) {
			return handler->_name;
		}
		handler = handler->next();
	}
	return "";
}

Direction PinManager::getPinDirection(int pin) {
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		if (handler->_pin == pin) {
			return handler->_direction;
		}
		handler = handler->next();
	}
	return IN;
}

PinType PinManager::getPinType(int pin) {
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		if (handler->_pin == pin) {
			return handler->_pinType;
		}
		handler = handler->next();
	}
	return DIGITAL;
}

int PinManager::getPinEventsCount(int pin) {
	int i = 0;
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		if (handler->_pin == pin) {
			i++;
		}
		handler = handler->next();
	}
	return i;
}

PinEvent* PinManager::getPinEvents(int pin) {
	PinEvent *pinEvents = new PinEvent[getPinEventsCount(pin)];
	int i = 0;
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		if (handler->_pin == pin) {
			pinEvents[i++] = handler->_pinEvent;
		}
		handler = handler->next();
	}
	return pinEvents;
}

void PinManager::loop() {
	PinHandler* handler = _firstPinHandler;
	while (handler) {
		handler->handleIn();
		handler = handler->next();
	}
}


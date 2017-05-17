/*
 * PinManager.h
 *
 *  Created on: Apr 22, 2017
 *      Author: asl
 */

#ifndef PINMANAGER_H_
#define PINMANAGER_H_

#include <functional>
#include <Arduino.h>

enum Direction {IN, OUT};
enum PinType { DIGITAL, ANALOG };
enum PinEvent {LOW_STATE, HIGH_STATE, ANY_STATE};

class PinManager {
public:
	PinManager();
	virtual ~PinManager();

	typedef std::function<void(int, int)> TPinHandlerFunction;
	typedef std::function<int(int)> TPinReaderFunction;

	struct PinHandler {
			int _pin;
			String _name;
			Direction _direction;
			PinType _pinType;
			PinEvent _pinEvent;
			PinManager::TPinHandlerFunction _handlerFunction;
			PinManager::TPinReaderFunction _readerFunction;
			int _value;

			int read() {
				if (_readerFunction != nullptr) {
					return _readerFunction(_pin);
				}

				if (_pinType == DIGITAL) {
					return digitalRead(_pin);
				} else {
					return analogRead(_pin);
				}
			}

			bool canHandle(int value) {
				if (_handlerFunction == nullptr) {
					return false;
				}

				switch (_pinType) {
					case DIGITAL: {
						switch (_pinEvent) {
							case LOW_STATE: {
								return value == LOW && _value == HIGH;
							}
							case HIGH_STATE: {
								return value == HIGH && _value == LOW;
							}
							default: {
								return value != _value;
							}
						}
						break;
					}
					case ANALOG: {
						return value != _value;
					}
				}
				return false;
			}

			void handleIn() {
				if (_direction == OUT) {
					return;
				}

				int value = read();
				if (canHandle(value)) {
					_handlerFunction(_pin, value);
				}
				_value = value;
			}

			void handleOut(int value) {
				if (_direction == IN) {
					return;
				}

				if (canHandle(value)) {
					_handlerFunction(_pin, value);
				}
				_value = value;
			}

			PinHandler* next() {
				return _next;
			}

			void addNext(PinHandler* handler) {
				_next = handler;
			}

			private:
			PinHandler* _next = nullptr;
		};

	void registerPin(int pin, String name, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinReaderFunction readerFunction, PinManager::TPinHandlerFunction handlerFanction);
	void registerPin(int pin, String name, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinHandlerFunction handlerFanction);
	void registerPin(int pin, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinReaderFunction readerFunction, PinManager::TPinHandlerFunction handlerFanction);
	void registerPin(int pin, Direction direction, PinType pinType, PinEvent pinEvent, PinManager::TPinHandlerFunction handlerFanction);
	void registerPin(int pin, String name, Direction direction, PinType pinType);
	void registerPin(int pin, Direction direction, PinType pinType);

	int getRegisteredPinsCount();
	int* getRegisteredPins();

	int getPinValue(int pin);
	void setPinValue(int pin, int value);

	String getPinName(int pin);
	Direction getPinDirection(int pin);
	PinType getPinType(int pin);
	int getPinEventsCount(int pin);
	PinEvent* getPinEvents(int pin);

	void loop();

private:
	PinHandler* _firstPinHandler;
};

#endif /* PINMANAGER_H_ */

#include "Arduino.h"
#include "skypeLibrary.h"
Status::Status(long lastUpdate, boolean  state) {
  _state = state;
  _lastUpdate = lastUpdate;
};

boolean  Status::getState() {
  return _state;
};

long Status::getLastUpdate() {
  return _lastUpdate;
};

void Status::setState(boolean state) {
	 _state = state;
};

void Status::setLastUpdate(long lastUpdate) {
	_lastUpdate = lastUpdate;
};
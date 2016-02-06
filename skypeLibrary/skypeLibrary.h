#ifndef skypeLibrary
#define skypeLibrary
#include "Arduino.h"

class Status {
  public:	
    Status(long lastUpdate, boolean  state);	
    boolean  getState();
    long getLastUpdate();
	void setState(boolean  state);
	void setLastUpdate(long lastUpdate);
    
  private:
    boolean  _state;
    long _lastUpdate;
};
#endif
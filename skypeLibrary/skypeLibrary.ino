#ifndef skypeLibrary
#define skypeLibrary
#include "Arduino.h"

class Client {
  public:
    Client(String  ip, long saveTime);
    String getIp();
    long getTime();
    
  private:
    String _ip;
    long _saveTime;
};

Client::Client(String  ip, long saveTime) {
  _ip = ip;
  _saveTime = saveTime;
};

String Client::getIp() {
  return _ip;
};

long Client::getTime() {
  return _saveTime;
};
#endif

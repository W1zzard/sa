/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>
#include <skypeLibrary.h>

const char* ssid = "noveo";
const char* password = "9D86009C99";

const long timeoutMs = 1000 * 60;
const String uniqDeviceId = "jh5hi4uh5kjhd3o2389";
const String parcelStartString = "parcelStart";
const String onStatusString = "on";
const String offStatusString = "off";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(8035);

//current status of lamp
Status currentStatus(millis(), false);

//prototype function
void checkCurrentStatus ();
void parseRequest (WiFiClient client);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {

  checkCurrentStatus ();
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  parseRequest (client);
  client.stop();
  Serial.println("Client disonnected");
}

//if status not renewed all skype sessions is ended so switch off lamp
void checkCurrentStatus () {
  if (currentStatus.getState()) {
    if (millis() - currentStatus.getLastUpdate() > timeoutMs) {
      digitalWrite(2, false);
      currentStatus.setState(false);
    }
  }
}

void parseRequest (WiFiClient client) {
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }  

  int recievedStatus = -1;
  
  while (client.available()) {    
    String data = client.readStringUntil('\n');   
    if (parcelStartString.equals(data)) {
      //payload starts
      //check identifier
      if (client.available() && uniqDeviceId.equals(client.readStringUntil('\n')) ){
        Serial.println("Unique id positive");
        if (client.available()) {
          String newStatus = client.readStringUntil('\n');         
          if (onStatusString.equals(newStatus)) {
            recievedStatus = 1;
          } else if (offStatusString.equals(newStatus)) {
            recievedStatus = 0;
          }
          break;
        }
      }
    }    
  }

    client.flush();
  
  // Match the status if not 0 or 1 wrong request disconnect and return
  
  if (recievedStatus != 1 && recievedStatus != 0)
  {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  //renew status and switch lamp on
  if (recievedStatus == 1) {
    digitalWrite(2, true);
    currentStatus.setLastUpdate(millis());
    currentStatus.setState(true);
  }

  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (recievedStatus)?"On":"Off";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}


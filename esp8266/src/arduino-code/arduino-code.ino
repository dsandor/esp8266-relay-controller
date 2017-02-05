#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

const char* ssid = "";
const char* password = "";

WiFiServer server(80);

void setup() {
  pinMode(5, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(16, HIGH);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(25);
  }

  if (!MDNS.begin("fanswitch")) {
    while(1) { 
      delay(100);
    }
  }
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  while(!client.available()){
    delay(1);
  }
  
  String req = client.readStringUntil('\r');
  client.flush();
  
  // TODO: Do a better job of this.
  int switch_no = -1;
  if (req.indexOf("/switch/0") != -1)
    switch_no = 5;
  else if (req.indexOf("/switch/1") != -1)
    switch_no = 14;
  else if (req.indexOf("/switch/2") != -1)
    switch_no = 16;    
  else {
    String s = "HTTP/1.1 400 Bad request: invalid gpio index.\n";

    client.print(s);
    delay(1);
    return;
  }

  if (switch_no >=0) {
    digitalWrite(switch_no, LOW);
    delay(300);
    digitalWrite(switch_no, HIGH);
  }

  client.flush();

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>OK</html>\n";

  client.print(s);
  delay(1);
}


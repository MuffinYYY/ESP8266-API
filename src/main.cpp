#include <Arduino.h>
#include <config.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266WebServer.h>   // Include the WebServer library

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

const int output4 = 4;

void handleON();              // function prototypes for HTTP handlers
void handleOFF();
void handleNotFound();
void handleCurrent();

void setup(void){
  Serial.begin(115200);
  delay(10);

  pinMode(output4, OUTPUT);  
  digitalWrite(output4, LOW);

  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");

  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  server.on("/led/true", handleON);               // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/led/false", handleOFF);
  server.on("/led/current", handleCurrent);
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

//Handle LED on HTTP request 
void handleON() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/json", "{\"LED_state\": \"true\"}");
  digitalWrite(output4, HIGH);
}

//Handle LED off HTTP request 
void handleOFF() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/json", "{\"LED_state\": \"false\"}");
  digitalWrite(output4, LOW);
}

//Handle current LED state HTTP request
void handleCurrent() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  if(digitalRead(output4) == HIGH){
    server.send(200, "text/json", "{\"LED_state\": \"true\"}");
  }
  if(digitalRead(output4) == LOW){
    server.send(200, "text/json", "{\"LED_state\": \"false\"}");
  }
}

//Handle HTTP request to link that doesn't exist
void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}
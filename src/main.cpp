#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Streaming.h>
#include "config.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

ESP8266WebServer server(80);

void handleRoot() {
  sensors.requestTemperatures(); // Envoi de la demande de lecture
  String result = "{ \n";
  result += "\"sensors\" : [\n";

  for (int i=0; i<sensors.getDeviceCount(); i++) {
    if (i != 0) {
      result += ",\n";
    }
    result += "{ \"temperature\" : ";
    result += sensors.getTempCByIndex(i);
    result += "}";
  }
  result += "\n]\n}";

  server.send(200, "text/plain", result);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

/**
 * Configuration de l'interface Wifi
 */
void setupWifi() {
  Serial << "Connecting to " << ssid << endl;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial << ".";
  }
  Serial << endl << "WiFi connected; IP address: " << WiFi.localIP() << endl;
}

/** Fonction setup() **/
void setup() {
  Serial.begin(9600);

  // Initialize web server
  setupWifi();
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial << "HTTP server started" << endl;

  // Initialize sensors
  sensors.begin();
}

/** Fonction loop() **/
void loop() {
  server.handleClient();
}

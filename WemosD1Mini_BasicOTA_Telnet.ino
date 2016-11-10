#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

/*-------- shortcuts code ----------*/
boolean doSerial=true;      
#define sb    Serial.begin(9600)        // shortcuts for serial output
#define sp    Serial.print
#define spf   Serial.printf
#define spln  Serial.println

/*-------- Telnet code ----------*/
// start telnet server (do NOT put in setup())
const uint16_t aport = 23; // standard port
WiFiServer TelnetServer(aport);
WiFiClient TelnetClient;

void tp(String output) {
  if (!TelnetClient)  // otherwise it works only once
        TelnetClient = TelnetServer.available();
  if (TelnetClient.connected()) {
    TelnetClient.println(output);
  }  
}

// Hotspot SSID and password
const char* ssid = "My Azuz";
const char* password = "azzahra4579";

void setup() {
  sb;

  TelnetServer.begin();
  TelnetServer.setNoDelay(true);
 
  spln();
  spln();
  spln("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  spln("Ready");
  sp("IP address: "); spln(WiFi.localIP());

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  
  tp("LED OFF");
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);                       // wait for a second
  tp("LED ON");
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(5000);                       // wait for a second
}

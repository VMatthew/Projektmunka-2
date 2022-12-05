#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "SerialComs.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

SoftwareSerial softSerial(D5, D6);

SerialComs coms(100,100);

// Wifi név és jelszó
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// weblap domain
const char* serverName = "http://projektmunka2sze.000webhostapp.com/php/post-to-db.php";

// PHP kóddal kompatibilis api key
String apiKeyValue = "d7a03fee5546592a37e22ff8f45bbbe45da4632dfed9a774e085d0e8b5d3fa73";

String alldata="";

float ph; // ph len 5 float?
char tds[10]; // tds len 7 text?
char tdsEC[10]; // tdsEC len 7 text?


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP8266CSV_fromUno");

  SafeString::setOutput(Serial);

  softSerial.begin(115200); 
  coms.setAsController(); 

  if (!coms.connect(softSerial)) {
    while (1) {
      Serial.println(F("Out of memory"));
    }
  }
  WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Program Started!!");
  delay(1000);
  Serial.println(F("ESP8266 Setup finished."));

}
String str="";
void loop() {
  coms.sendAndReceive(); // must do this every loop

  if (!coms.textReceived.isEmpty()) { // got some data
    Serial.println("Recieved data on nodemcu: ")
    Serial.println(coms.getTextReceived());
    str= coms.getTextReceived();
    alldata="api_key=" + apiKeyValue + str;
    Serial.println("Data to send: ");
    Serial.println(alldata);
    
    //WIFI kapcsolat ellenőrzése
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
        
      // Domain név URL-el vagy IP-vel
      http.begin(client, serverName);
        
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        
      // HTTP POST request adat előkészítése
      String httpRequestData = alldata;
      Serial.print("httpRequestdoc: ");      
      Serial.println(httpRequestData);
      
      // HTTP POST küldése
      int httpResponseCode = http.POST(httpRequestData);
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // tárhely felszabadítása
      http.end();
      }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
  delay(5000);
}

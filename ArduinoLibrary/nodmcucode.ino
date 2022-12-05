#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial nodemcu(D6, D5);


// Wifi név és jelszó
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// weblap domain
const char* serverName = "http://projektmunka2sze.000webhostapp.com/php/post-to-db.php";

// PHP kóddal kompatibilis api key
String apiKeyValue = "d7a03fee5546592a37e22ff8f45bbbe45da4632dfed9a774e085d0e8b5d3fa73";

String alldoc;

void setup() {
	Serial.begin(9600);
  nodemcu.begin(9600);
  while(!Serial) continue;
  
  WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.piintln("Program Started!!")
  delay(1000);

}

void loop() {
	StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, nodemcu);
  
  // Test parsing
  while (error) {
    Serial.println("Invalid JSON Object");
    delay(500);
    DeserializationError error = deserializeJson(doc, nodemcu);
    Serial.println(String(error));    
  }

    
  Serial.println("JSON Object Recieved");
  Serial.print("Recieved humidity:  ");
  float humidity = doc["humidity"];
  Serial.println(humidity);
  Serial.print("Recieved temperature:  ");
  float temperature = doc["temperature"];
  Serial.println(temperature);
Serial.print("Recieved moisture:  ");
  int moisture = doc["moisture"];
  Serial.println(moisture);  
  Serial.print("Recieved light:  ");
  int light = doc["light"];
  Serial.println(light);
  Serial.print("Recieved color:  ");
  int color = doc["color"];
  Serial.println(color);
  Serial.print("Recieved Notification:  ");
  String notification = doc["tankdoc"];
  Serial.println(notification);
  
  
  
  Serial.println("-----------------------------------------");

  alldoc="api_key=" + apiKeyValue + "&humidity=" + String(humidity)+
      "&temperature=" + String(temperature)+ "&moisture=" + String(moisture)+
      "&light=" + String(light)+ "&color=" + String(color)+ "&tankdoc=" + notification+"";
  
  Serial.println("-----------------------------------------");  

  
	//WIFI kapcsolat ellenőrzése
	if(WiFi.status()== WL_CONNECTED){
		WiFiClient client;
		HTTPClient http;
		  
		// Domain név URL-el vagy IP-vel
		http.begin(client, serverName);
		  
		// Specify content-type header
		http.addHeader("Content-Type", "application/x-www-form-urlencoded");
		  
		// HTTP POST request adat előkészítése
    String httpRequestdoc = alldoc;
		Serial.print("httpRequestdoc: ");      
		Serial.println(httpRequestdoc);
		
		// HTTP POST küldése
		int httpResponseCode = http.POST(httpRequestdoc);
		
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
	delay(2000);  
}

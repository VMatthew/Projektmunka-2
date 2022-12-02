#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int sensorval;
int moisture;
int luminosity;

const int szaraz = 810;
const int nedves = 345;
const int sotet = 300;
const int fenyes = 700;


const char* ssid     = "*******";
const char* password = "*****";


const char* serverName = "https://projektmunka2sze.000webhostapp.com/";

String apiKeyValue = "grandeurlb5y82ax0ykm0jjohxmv7vo5";

String deviceId = "devicelb5ysw860yvi0jjoalihb7of";

void setup() {
  Serial.begin(9600);
  nodemcu.begin(9600);
  while (!Serial) continue;
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();
  delay(1000);
  
  Serial.println("Program started");
}

void loop() {

  StaticJsonDocument<256> doc;
  //DynamicJsonDocument doc(2048); - Heap
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(false);

  Serial.print("hum: ");
  Serial.println(humidity);
  Serial.print("temp: ");
  Serial.println(temperature);
  sensorval = analogRead(A0);
  moisture = map(sensorval, nedves, szaraz, 100, 0);
  sensorval = analogRead(A2);
  luminosity = map(sensorval, fenyes, sotet, 100, 0);
  Serial.print("moisture: ");
  Serial.print(moisture);
  Serial.println("%");
  Serial.print("lum: ");
  Serial.print(luminosity);
  Serial.println("%");
  
  doc["humidity"] = humidity;
  doc["temperature"] = temperature;
  doc["moisture"] = moisture;
  doc["luminosity"] = luminosity;
  
  serializeJson(doc, Serial); //print-eli a json objektumot
  
  float current = 0;
  if(millis() - current > 30000) 
  {
    current = millis();
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& data = jsonBuffer.parseObject(doc);
    

    if (data == JsonObject::invalid()) {
      //Serial.println("Invalid Json Object");
      jsonBuffer.clear();
      return;
    }

    Serial.println("JSON Object Recieved");
    Serial.print("Recieved distance:  ");
    int distance = data["distance"];
    Serial.println(distance);
    Serial.print("Recieved moisture:  ");
    int moisture = data["moisture"];
    Serial.println(moisture);
    Serial.print("Recieved humidity:  ");
    float humidity = data["humidity"];
    Serial.println(humidity);
    Serial.print("Recieved temperature:  ");
    float temperature = data["temperature"];
    Serial.println(temperature);
    Serial.println("-----------------------------------------");


    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      http.begin(client, serverName);
      
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
      String httpRequestData = "api_key=" + apiKeyValue + "&distance=" + String(distance)
                            + "&moisture=" + String(moisture) + "&humidity=" + String(humidity) + "&temperature=" + String(temperature) + "";
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);

      int httpResponseCode = http.POST(httpRequestData);
        
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }

  }
  delay(10000);
}

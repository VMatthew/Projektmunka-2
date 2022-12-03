#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <String.h>
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

// Replace with your network credentials
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://projektmunka2sze.000webhostapp.com/php/post-to-db.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "d7a03fee5546592a37e22ff8f45bbbe45da4632dfed9a774e085d0e8b5d3fa73";

String sensorName = "Sensors_4";
String sensorLocation = "Home";

const int szaraz = 810;
const int nedves = 345;
const int sotet = 300;
const int fenyes = 700;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  //Check WiFi connection status
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(false);

  sensorval = analogRead(A0);
  moisture = map(sensorval, nedves, szaraz, 100, 0);
  sensorval = analogRead(A2);
  luminosity = map(sensorval, fenyes, sotet, 100, 0);

  String Shumidity = String(humidity);
  String Stemperature = String(temperature)+" C°";
  String Smoisture = String(moisture)+"%";
  String Sluminosity = String(luminosity)+"%";

  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + "&humidity=" + Shumidity)
                          + "&temperature=" + Stemperature) + "&moisture=" + Smoisture + "" + "&luminosity=" + Sluminosity;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(30000);  
}

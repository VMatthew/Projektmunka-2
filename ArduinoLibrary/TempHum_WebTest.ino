#include "recipes/WiFi.h"     // Include recipe file to connect to wifi
#include <Grandeur.h>
#include <dht11.h>    //adding dht11 lib
#define DHT11PIN D4
dht11 DHT11;

// Grandeur credentials.
const char * apiKey = "grandeurlb5y82ax0ykm0jjohxmv7vo5";
const char* token = "b432e14b88d96526ce4bd7885563ed4f3a2921442ff5722fb8e5afb0a696aa14";
const char* deviceId = "devicelb62ya1v115d0jjocnyd4fqg";
const char* secretkey = "3a2f3eacb30ac6f7b427802ab8ae413f3bd07aac6fd8f8000fe8647e210e5161";

const char* ssid = "YourWiFiSsid"; //WIFI NEVE!!!
const char* passphrase = "YourWiFiPassphrase";  // WIFI JELSZAVA !!!

Grandeur::Project project;
Grandeur::Project::Device device;

void getHum (const char* code,int hum) {   // You can write any type int/double/bool/const char* in place of int and it'll cast humidity to that type.
  // This method prints "humidity" value after it is updated on Grandeur.
  Serial.print("Hum %: ");//Serial.print prints on serial monitor
  Serial.println (hum); 
}

void getTemp (const char* code,int temp) {   // You can write any type int/double/bool/const char* in place of int and it'll cast temperature to that type.
  // This method prints "temperature" value after it is updated on Grandeur.
  Serial.print("Temp (C): ");
  Serial.println (temp); 
}

void setup() {
    Serial.begin(9600); // Start serial on boot
    
   // connectWiFi(WIFI_SSID, WIFI_PASSPHRASE);                                                 
    connectWiFi(ssid, passphrase); // And connect to WiFi with WIFI_SSID and WIFI_PASSPHRASE (Paste your own WiFi SSID and Password here)

     // Initializing.
    project = grandeur.init(apiKey, secretkey); //Vagy lehet itt token kell idk, guide régi volt
    device = project.device(deviceID);
}

// Loop function repeats the code in it indefinitely.
void loop() {
    if (project.isConnected()) 
    {
  int readData = DHT11.read(DHT11PIN);   //read data from sensor 
  
  float t = DHT11.temperature ;           //stored value of temperature in variable "t
  device.data().set("tem",t,getTemp);       // This requests to set the "temperature" variable on every loop and calls getTemp() function when the
  // variable is actually updated on Grandeur.
  delay(1000);                            

  float h = DHT11.humidity ;            //stored value of humidity in variable "h"
  device.data().set("hum",h,getHum);      // This requests to set the "humidity" variable on every loop and calls getHum() function when the
  // variable is actually updated on Grandeur.
  delay(1000);

    }
      project.loop();                 // Synchronizing the ESP with the Cloud.
}

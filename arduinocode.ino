#include <SoftwareSerial.h>
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

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(1000);
  
  Serial.println("Program started");
}

void loop() {
  StaticJsonDocument<256> data;
  //DynamicJsonDocument data(2048); - Heap
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
  
  data["humidity"] = humidity;
  data["temperature"] = temperature;
  data["moisture"] = moisture;
  data["luminosity"] = luminosity;
  
  serializeJson(doc, Serial); //prit-eli a json objektumot
  
  delay(10000);
}


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
const in sotet = 300;
const int fenyes = 700;

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(1000);
  Serial.println("Program started");
}

void loop() {
  
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
  
  delay(10000);
}


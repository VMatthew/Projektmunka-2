#include <SerialSoftware.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#define s0 8  
#define s1 9
#define s2 10
#define s3 11
#define out 12
#define DHTPIN 2 //páratartalom és hőmérséklet érzékelő DATA-->>Digital 2 pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int sensorval;
int moisture;
int luminosity;
bool kifogyott;
int waterlevel;
int minviz = 400; //vízszint ami alatt öntözünk
int Red=0, Blue=0, Green=0;  //RGB values 

bool netrequest = false;

//Mért értékek a map függvényekhez
const int szaraz = 810;
const int nedves = 345;
const int sotet = 300;
const int fenyes = 700;
const int relepin = A5; //Relé INPUT -->> Analog 5 pin


SoftwareSerial nodemcu(5, 6);
String notification ="";

// PHP kóddal kompatibilis api key
String apiKeyValue = "d7a03fee5546592a37e22ff8f45bbbe45da4632dfed9a774e085d0e8b5d3fa73";

void setup() {
  Serial.begin(9600); 
  dht.begin();
  nodemcu.begin(9600);
  pinMode(s0,OUTPUT);    //pin modes
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
  digitalWrite(s0,HIGH);
  digitalWrite(s1,HIGH);
  GetColors();
  int ORed = Red;
  int OGreen  = Green;
  int OBlue = Blue;
  pinMode (relepin, OUTPUT);
  //pinMode (trigpin, OUTPUT);
  //pinMode (echopin, INPUT);
  digitalWrite (relepin, HIGH);  
  

  delay(1000);
  serial.println("Program Started!")

}

void loop() {
  //Változók feltöltése
  StaticJsonDocument<1000> doc;
  
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(false);

  GetColors();
  float Reddiff=ORed-Red;
  float Greendiff=OGreen-Green;
  float Bluediff=OBlue-Blue;
  float x =  (abs(Reddiff) + abs(Greendiff) + abs(Bluediff))/765;
  String ColourDistortion = String(x*100)+"%";

  sensorval = analogRead(A0); //Talajnedvesség érzékelő DATA-->>Analog 0 pin
  moisture = map(sensorval, nedves, szaraz, 100, 0);
  sensorval = analogRead(A2); //Fény érzékelő DATA-->>Analog 2 pin
  luminosity = map(sensorval, fenyes, sotet, 100, 0);
  waterlevel = analogRead(A3); //vízszint érzékelő érzékelő DATA-->>Analog 3 pin

  if (waterlevel < minviz){
    kifogyott = true;
    notfication+= "Víz =0; ";
  }
  else {
    kifogyott = false;
  }

  //Notfication
  if(humidity<40 && notification.indexOf("Pára++ ;") == -1) { notification+="Pára++ ;"; }
  if(temperature<15 && notification.indexOf("Temp++ ;") == -1) { notification+="Temp++ ;"; }
  if(luminosity<20 && notification.indexOf("Fény++ ;") == -1) { notification+="Fény++ ;"; }


  //nagyon alap öntöző kód
  if (moisture <85 and kifogyott == false){
    if(notification.indexOf("Öntözés történt ;") == -1 ) { notification+="Öntözés történt ;"; }
    digitalWrite (relepin, LOW);
    delay (1000);
    digitalWrite (relepin, HIGH);
  }
  
  //netrequest alapján öntözés
  if (netrequest == true and kifogyott == false){
    if(notification.indexOf("Öntözés történt ;") == -1 ) {
      digitalWrite (relepin, LOW);
      delay (1000);
      digitalWrite (relepin, HIGH);
    }
  }

  //max 49 char lehet ha minden notification hozzáadodik, itt ellenőrizzük
  if(notification.length() >= 50 ) {
    Serial.println("Notification hibás!!");
    notification="";
  }

  //JSON feltöltése
  doc["humidity"] = humidity;
  doc["temperature"] = temperature;
  doc["mositure"]=40;
  doc["light"]=50;
  doc["color"]=30;
  doc["tankData"]=notification;

  //nodemcu-ra küldés
  serializeJson(doc, nodemcu);

  delay(2000);
}

void GetColors()  //Színérzékelő fügvény
{    
  digitalWrite(s2, LOW);                                          
  digitalWrite(s3, LOW);                                           
  Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);       
  delay(20);  
  digitalWrite(s3, HIGH);                                       
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
  digitalWrite(s2, HIGH);  
  Green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
}


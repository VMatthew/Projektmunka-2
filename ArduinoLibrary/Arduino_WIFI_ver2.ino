#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

//PIN-ek:
  //páratartalom és hőmérséklet érzékelő DATA-->>Digital 2 pin
  //Talajnedvesség érzékelő DATA-->>Analog 0 pin
  //Fény érzékelő DATA-->>Analog 2 pin
  //vízszint érzékelő érzékelő DATA-->>Analog 3 pin
  //Relé INPUT -->> Analog 5 pin

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
String notfication ="";

/*int trigpin = 8;
int echopin = 9;
int duration;
int distance;*/

//Mért értékek a map függvényekhez
const int szaraz = 810;
const int nedves = 345;
const int sotet = 300;
const int fenyes = 700;
const int relepin = A5; //Relé INPUT -->> Analog 5 pin

// Wifi név és jelszó
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// weblap domain
const char* serverName = "https://projektmunka2sze.000webhostapp.com/";

// PHP kóddal kompatibilis api key
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorName = "Sensors";
String sensorLocation = "Home";

void setup() {
  Serial.begin(115200);
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

}
/*
int tavolsag(){
  int xd;
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration = pulseIn (echopin, HIGH);
  xd = duration * 0.034 / 2;
  return xd;
}
*/
void loop() {
  //Változók feltöltése
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

  //Stringbe konvertálás
  String Shumidity = String(humidity);
  String Stemperature = String(temperature)+" C°";
  String Smoisture = String(moisture)+"%";
  String Sluminosity = String(luminosity)+"%";

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

  //Bár a delay 5 secre van rakva ez a rész csak 30 secenként fusson le

   //WIFI kapcsolat ellenőrzése
   if(WiFi.status()== WL_CONNECTED){
     WiFiClient client;
     HTTPClient http;
      
     // Domain név URL-el vagy IP-vel
     http.begin(client, serverName);
    
     // Specify content-type header
     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
     // HTTP POST request adat előkészítése
     String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                            + "&location=" + sensorLocation + "&humidity=" + Shumidity
                            + "&temperature=" + Stemperature) + "&moisture=" + Smoisture 
                            + "&luminosity=" + Sluminosity + "&colour="+ ColourDistortion + "&notification=" + notification;
     Serial.print("httpRequestData: ");      
     Serial.println(httpRequestData);
     notification="";

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
  delay(30000);  
}

void GetColors()  
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

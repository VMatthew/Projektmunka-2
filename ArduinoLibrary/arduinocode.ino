#include "SerialComs.h"
#include "SoftwareSerial.h"
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
int light;
bool kifogyott;
int waterlevel;
int minviz = 400; //vízszint ami alatt öntözünk
int Red=0, Blue=0, Green=0, ORed=0, OBlue=0, OGreen=0;  //RGB values 
bool O=false;
bool netrequest = false;
int wateringmode = 1;

//Mért értékek a map függvényekhez
const int szaraz = 810;
const int nedves = 345;
const int sotet = 300;
const int fenyes = 700;
const int relepin = A5; //Relé INPUT -->> Analog 5 pin

// UNO
const int RX_pin = 5;
const int TX_pin = 6;

// else you need to define your own RX_pin, TX_pin
SoftwareSerial softSerial(RX_pin, TX_pin); // RX, TX
// sendLineLength default 60, receiveLineLength default 60
SerialComs coms(100,100);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("UnoSoftSerialCSV");
  SafeString::setOutput(Serial); // enable error msgs and debug
  softSerial.begin(115200);   // Initialize the "link" serial port
  if (!coms.connect(softSerial)) {
    while (1) {
      Serial.println(F("Out of memory"));
      delay(3000);
    }
  }
  pinMode(s0,OUTPUT);    //pin modes
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
  digitalWrite(s0,HIGH);
  digitalWrite(s1,HIGH);
  
  pinMode (relepin, OUTPUT);
  //pinMode (trigpin, OUTPUT);
  //pinMode (echopin, INPUT);
  digitalWrite (relepin, HIGH); 
  
  GetColors();
  ORed = Red;
  OGreen  = Green;
  OBlue = Blue;

  dht.begin(115200);
  delay(1000);
  Serial.println("Program Started!");
}

int count = 0;
void loop() {
	coms.sendAndReceive(); // must do this every loop
	
	float humidity = dht.readHumidity();
	float temperature = dht.readTemperature(false);
	GetColors();
	float Reddiff=ORed-Red;
	float Greendiff=OGreen-Green;
	float Bluediff=OBlue-Blue;
	float color =  (abs(Reddiff) + abs(Greendiff) + abs(Bluediff))/765;
	
	sensorval = analogRead(A0); //Talajnedvesség érzékelő DATA-->>Analog 0 pin
	moisture = map(sensorval, nedves, szaraz, 100, 0);
	sensorval = analogRead(A2); //Fény érzékelő DATA-->>Analog 2 pin
	light = map(sensorval, fenyes, sotet, 100, 0);
	waterlevel = analogRead(A3); //vízszint érzékelő érzékelő DATA-->>Analog 3 pin
  
	if (waterlevel < minviz){
		kifogyott = true;
	}
	else {
		kifogyott = false;
	}	

  if(netrequest) wateringmode=2;
  switch (wateringmode) {
  case 1:
    //nagyon alap öntöző kód
	  if (moisture <85 and kifogyott == false){
      O=true;
      digitalWrite (relepin, LOW);
      delay (1000);
      digitalWrite (relepin, HIGH);
	  }
    break;
  case 2:
    //netrequest alapján öntözés
	  if (!kifogyott){
      digitalWrite (relepin, LOW);
      delay (1000);
      digitalWrite (relepin, HIGH);
	  }
    break;
  case 3:
    //nagyon alap öntöző kód
	  if (moisture <50 and kifogyott == false){
      O=true;
      digitalWrite (relepin, LOW);
      delay (1000);
      digitalWrite (relepin, HIGH);
	  }
    break;
  case 4:
    //páratartalom alacsony
	  if (moisture <55 and kifogyott == false and humidity<40){
      O=true;
      digitalWrite (relepin, LOW);
      delay (1500);
      digitalWrite (relepin, HIGH);
	  }
    break;
  default:
    if (moisture <85 and kifogyott == false){
      O=true;
      digitalWrite (relepin, LOW);
      delay (1000);
      digitalWrite (relepin, HIGH);
	  }
    break;
}
	//nagyon alap öntöző kód
	if (moisture <85 and kifogyott == false){
		O=true;
		digitalWrite (relepin, LOW);
		delay (1000);
		digitalWrite (relepin, HIGH);
	}
	
	//netrequest alapján öntözés
	if (netrequest == true and kifogyott == false){
		digitalWrite (relepin, LOW);
		delay (1000);
		digitalWrite (relepin, HIGH);
	}
	if (coms.textToSend.isEmpty()) {


    coms.textToSend.print("&h=");
    coms.textToSend.print(humidity);
    coms.textToSend.print("&t=");
    coms.textToSend.print(temperature);
	  coms.textToSend.print("&m=");
    coms.textToSend.print(moisture);
	  coms.textToSend.print("&l=");
    coms.textToSend.print(light);
	  coms.textToSend.print("&c=");
    coms.textToSend.print(color);
	  coms.textToSend.print("&n=");
	
    //Notfication
    if(O) { coms.textToSend.print("Watering done ;"); }
    if(humidity<40 ) { coms.textToSend.print("HUM++ ;"); }
    if(temperature<15 ) { coms.textToSend.print("Temp++ ;"); }
    if(light<20 ) { coms.textToSend.print("Light++ ;"); }
    if(waterlevel<minviz) { coms.textToSend.print("WATER!"); }

	coms.textToSend.print("");
	
  Serial.println(coms.textToSend);
	O=false;
  }
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

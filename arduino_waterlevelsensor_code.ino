#include <SoftwareSerial.h>

int sensorval;
bool kifogyott;
int waterlevel;
int moisture; //main codeban van moisture érékének mérése

const int min_wlevel = 500; //még le kell mérni mennyi értéket ad vissza vízszint sensor ami a minimum vizszint ahoz hogy működjön a szívattyú
const int max_wlevel = 1000; //random szám. majd írjuk át mikor lemértük menyit mutat mikor a tároló teli van

void setup() {
  Serial.begin(9600);

  nodemcu.begin(9600);

  Serial.println("Program started");
}

void loop() {
  sensorval = analogRead(A4);
  if(sensorval<min_wlevel) {
    kifogyott = true;
  }else{
    kifogyott = false;
  }
  waterlevel=map(sensorval,max_wlevel,min_wlevel,100,0);
  
  Serial.print("water_level: ");
  Serial.println(waterlevel);
  
  if (moisture <85 and kifogyott == false){
    Serial.println("öntözzünk növényeket!!!"); //ide jön majd öntözés kódja
  }
}

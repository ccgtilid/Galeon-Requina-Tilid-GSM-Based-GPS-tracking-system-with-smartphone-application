#include <SoftwareSerial.h>
SoftwareSerial sim808Module(11, 10);//rx tx
String receive;
char d;
String data[3];
double dataD[3];
char phone_no[] = "09567627659";
int cmtiScan = 0;
int cmtiScanAnti = 0;
int cmtiLetter = 0;
int ctr = 0;
int i = 0;
int ender = 0;
int decX = 0, decY = 0;
String latitude, longitude;
void setup() {
  sim808Module.begin(9600);
  Serial.begin(9600);
  Serial.println("Begin");
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  delay(100);
  sim808Module.println("AT+CGPSINF=0");
  cmtiScanAnti = 1;
}

void loop() {
  digitalWrite(7, HIGH);
  if (Serial.available()>0){
    sim808Module.write(Serial.read());
  }
  if (sim808Module.available()>0){
    d = (sim808Module.read());
    Serial.print(d);
    CMTIScanner();
  }
  if((cmtiScan == 1)){
    if(cmtiScan == 1){
      Serial.print("Rec");
      sim808Module.println("AT+CGPSINF=0");
      delay(200);
      sim808Module.println(char(10));
      delay(500);
      cmtiScanAnti = 1;
      while(sim808Module.available()>0){
        char c = sim808Module.read();
        Serial.print(c);
        cmtiScanAnti = 1;
        if(c == ','){
          if(ender == 0){
             ctr = 1;
          }
        }
        if(ctr == 1){
          if(c != ','){
            data[i] += c;
          }
          else{
            i++;
          }
          if(i == 3){
            ctr = 0;
            ender = 1;
            i=0;
          }
        }
      }
      //-----------------
      dataD[1] = data[1].toDouble();
      while(dataD[1] >= 100){
        dataD[1] = dataD[1] - 100;
        decX++;
        cmtiScanAnti = 1;
      }
      dataD[1] = (dataD[1])/60.000000;
      dataD[1] = dataD[1]+decX;
      delay(2000);
      cmtiScanAnti = 1;
      Serial.println("\nLatitude: ");
      cmtiScanAnti = 1;
      Serial.print(dataD[1], 15);
      decX=0;
      //-------------------------
      dataD[2] = data[2].toDouble();
      while(dataD[2] >= 100){
        dataD[2] = dataD[2] - 100;
        decY++;
      }
      dataD[2] = (dataD[2])/60.000000;
      dataD[2] = dataD[2]+decY;
      cmtiScanAnti = 1;
      Serial.println("\nLongitude: ");
      cmtiScanAnti = 1;
      Serial.print(dataD[2], 15);
      decY=0;
      delay(1000);
      ender=0;
      i=0;
      ctr=0;
      data[1]=" ";
      data[2]=" ";
      sendCoords();
      cmtiScan = 0;
    }
    else{
      Serial.print(receive);
      delay(500);
      receive = "";
      cmtiScan = 0;
      cmtiLetter = 0;
    }
  }
}
void CMTIScanner(){
  if(d == ':'){
    if(cmtiScanAnti == 0){
      cmtiScan = 1;
      cmtiScanAnti = 1;
    }
    else{
      cmtiScanAnti = 0;
    }
  }
}
void sendCoords(){
  sim808Module.print("AT+CMGS=\"");
    sim808Module.print(phone_no);
    sim808Module.println("\"");
    sim808Module.print(dataD[1], 15);
    sim808Module.print(",");
    sim808Module.print(dataD[2], 15);
    delay(200);
    sim808Module.println(char(26));
    delay(500);
}

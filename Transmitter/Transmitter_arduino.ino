#include <require_cpp11.h>
#include <MFRC522.h>
#include <deprecated.h>
#include <MFRC522Extended.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <Arduino.h>
#include <TM1637Display.h>

#define CLK 3     //Display
#define DIO 4

#define CE  9     //RF24
#define CSN 10

#define RST  7     //RFID
#define SS 2

#define BUTTON 8
#define K_LED 5
#define Y_LED 6

MFRC522 rfid(SS, RST);
RF24 transmitter(CE, CSN);
TM1637Display display(CLK, DIO);

const uint64_t address = 0x123456789ABCDEFA;

String CihazID="A01";
String tempo=" ";
int butonDurumu = 0;
long sayac=-1L;
char butonmesaj[32];
String temp=" ";
int kod=9999;
int kod2=8888;
bool sorgu = true;

//=========================================================
void setup() {
  
  pinMode(BUTTON,INPUT);      
  pinMode(K_LED,OUTPUT);
  pinMode(Y_LED,OUTPUT);
  
  SPI.begin();
  Serial.begin(9600);
  transmitter.begin();
  transmitter.openWritingPipe(address);
  transmitter.setPALevel(RF24_PA_HIGH);
  transmitter.setDataRate(RF24_2MBPS);
  transmitter.stopListening();
  rfid.PCD_Init();
  
  display.setBrightness(0x0f);
  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display.setSegments(9999);
  display.showNumberDec(9999);

}

//=========================================================

void loop() {
  String mesaj3;
  String content=" ";

  if(tempo!=" "){
    butonDurumu = digitalRead(BUTTON);
      if (butonDurumu == HIGH) {
        delay(10);  
        sayac ++;
        digitalWrite(K_LED,HIGH);      
        mesaj3=CihazID+tempo+" "+sayac;
        mesaj3.toCharArray(butonmesaj,32);
        transmitter.write(&butonmesaj, sizeof(butonmesaj));
                Serial.println(mesaj3);

        display.setBrightness(0x0f);
        uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };     
        display.setSegments(sayac);
        display.showNumberDec(sayac);
        while(butonDurumu == HIGH){
          digitalWrite(K_LED,HIGH);
          butonDurumu = digitalRead(BUTTON); 
        }
        digitalWrite(K_LED,LOW);
        delay(500);    
      }
  }

  if ( ! rfid.PICC_IsNewCardPresent()){  
    return;
  }
  if ( ! rfid.PICC_ReadCardSerial()){  
    return;
  }
  
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(rfid.uid.uidByte[i], HEX));
  }
    delay(300);
    char mesaj[32] = "";
    String mesaj2="";
    content.toUpperCase();
    rfid.PICC_HaltA();
      if(content!=temp){          //YENİ KART OKUNDUĞU ZAMAN GİRER
        digitalWrite(Y_LED,HIGH);
        delay(400);
        digitalWrite(Y_LED,LOW);
        delay(400);
        sayac=0;
        display.showNumberDec(0);
        if(temp!=" "){
          mesaj2=CihazID+temp+" "+kod2;
          Serial.println(mesaj2);
          mesaj2.toCharArray(mesaj,32);
          transmitter.write(&mesaj, sizeof(mesaj));
        }       
        delay(400);
        mesaj2=CihazID+content+" "+kod;
        Serial.println(mesaj2);
        temp=content;
        tempo=content;
        sorgu=false;
      }
      else{
        display.setBrightness(0x0f);
        uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
        display.setSegments(8888);
        display.showNumberDec(8888);
        sayac=0;   
        tempo=" ";
        digitalWrite(Y_LED,HIGH);
        delay(400);
        digitalWrite(Y_LED,LOW);
        delay(400);
        temp=" ";
        mesaj2=CihazID+content+" "+kod2;
        Serial.println(mesaj2);
        sorgu=true;

      }
    mesaj2.toCharArray(mesaj,32);
    transmitter.write(&mesaj, sizeof(mesaj));  
    delay (1000); 
}

#include <nRF24L01.h>                         //ALICI
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

RF24 radio(7, 8);                                 // CE, CSN
IPAddress my_ip(192, 168, 1, 197);                //ethernet shild ip
IPAddress ip_client(192,168,1,107);            //pc ip
const uint64_t address = 0x123456789ABCDEFA;
byte my_mac[] = {0x90,0xA2,0XDA,0XF4,0X61,0XC6};
unsigned int localPort = 8080;

EthernetUDP Udp;
String dizi;
String inputString;

//=========================================================
void setup() {
  
  Serial.begin(9600);
  Serial.println(Ethernet.localIP());
  radio.begin();
  Ethernet.begin(my_mac, my_ip);
  Udp.begin(localPort);
  radio.openReadingPipe(0, address);  
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  radio.setDataRate(RF24_2MBPS);
  udp_setup();

}

//=========================================================
void udp_setup() {
  
  Ethernet.begin(my_mac, my_ip);
  Udp.begin(localPort);
}


//=========================================================
void udp_send(String u){
  
   Udp.beginPacket(ip_client, localPort);
   bool rst= Udp.print(u);
   if(rst==true){
    Udp.endPacket();}   
    else{
    }
    delay(300);
}

//=========================================================
void loop() {   
  
    if (radio.available()) {


      char text[32] = "";
      radio.read(&text, sizeof(text)); 
      Serial.println(text);
      inputString=String(text);
     // dizi=inputString.substring(9, 12);
     // Serial.println(dizi);
    } 
    udp_send(inputString);   
    inputString = "";
}

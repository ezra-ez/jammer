// Library
#include <WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#include "RF24.h"
#include "esp_bt.h"

// SSID dan Password WiFi
const char * ssid = "ssid";
const char * pwd = "password";

// IP Address dan Port Server
const char * udpAddress = "192.168.0.127"; //ip address laptop
const int udpPort = 9999;

volatile int status_led = 0;



// Objek Kelas WiFiUDP
WiFiUDP udp;

SPIClass *hp = nullptr;

RF24 radio(16, 15, 16000000);

byte i = 2;

void setup(){
  esp_bt_controller_deinit();
  // Serial Monitor
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Mulai Menghubungkan ke WiFi
  WiFi.begin(ssid, pwd);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }


  // Jika terhubung, maka akan mencetak IP Address ESP32
  Serial.println("");
  Serial.print("Ip: ");
  Serial.print(WiFi.localIP());

  initHP();
  Serial.println("");
}

void loop(){

  // variabel penampung untuk komunikasi
  
  uint8_t buffer_terima[50] = "0";

  // sweep
  for(int k = 0; k < 4800; k++){
    one();
  }
  Serial.println("-");
  
  switch(status_led){
    case 0:
    memcpy(buffer_terima, "0", 50);
    break;
    case 1:
    memcpy(buffer_terima, "1", 50);
    break;
    default:
    break;
  }

  // sweep
  for(int k = 0; k < 4800; k++){
    one();
  }
  Serial.println("");
  
  udp.beginPacket(udpAddress,udpPort);
  udp.write(buffer_terima, 1);
  udp.endPacket();

  
  // sweep
  for(int k = 0; k < 4800; k++){
    one();
  }
  Serial.println("-");

  memset(buffer_terima, 0, 50);
  
  udp.parsePacket();
  if(udp.read(buffer_terima, 50) > 0){
    
    // mencetak data yang diterima
    Serial.print("Data diterima: ");
    Serial.println((char *) buffer_terima);

    int terima_data;

    terima_data = (int)((char *) buffer_terima[0]) - 48;
    Serial.println(terima_data);
    
    if(terima_data == 0){
      digitalWrite(LED_BUILTIN, LOW);
      status_led = 0;
      Serial.println("ey");
      radio.stopConstCarrier();
    }
    else if(terima_data == 1){
      digitalWrite(LED_BUILTIN, HIGH);
      status_led = 1;
      Serial.println("oi");
      radio.startConstCarrier(RF24_PA_MAX, i);
    }
    else{
      Serial.println("none");
    }
  }
  
  // sweep
  for(int k = 0; k < 4800; k++){
    one();
  }
  Serial.println("-");
  
  Serial.println("test");

}

void initHP() {
  hp = new SPIClass(HSPI);
  hp->begin();
  if (radio.begin(hp)) {
    delay(200);
    Serial.println("Jammer is Started !!!");
    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPayloadSize(3);   ////SET VALUE ON RF24.CPP
    radio.setAddressWidth(3);  ////SET VALUE ON RF24.CPP
    radio.setPALevel(RF24_PA_MAX, true);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.printPrettyDetails();
  } else {
    Serial.println("Jammer couldn't be started !!!");
  }
}

void one() {

  ////SWEEP CHANNEL
  for (int i = 2; i <= 94; i = i+2) {
    radio.setChannel(i);
    if(i == 72) {
      i = i-3;
    }
  }

}


// NRF24 Connection
// SCK=14, MISO=12, MOSI=13, CS=15, CE=16


#include "RF24.h"
#include "esp_bt.h"

SPIClass *hp = nullptr;

RF24 radio(16, 15, 16000000);

byte i = 2;


void setup(void) {
  esp_bt_controller_deinit();
  Serial.begin(115200);
  initHP();
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
    radio.setPayloadSize(5);   ////SET VALUE ON RF24.CPP
    radio.setAddressWidth(3);  ////SET VALUE ON RF24.CPP
    radio.setPALevel(RF24_PA_MAX, true);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.printPrettyDetails();
    radio.startConstCarrier(RF24_PA_MAX, i);
  } else {
    Serial.println("Jammer couldn't be started !!!");
  }
}

void one() {


  ////SWEEP CHANNEL
  for (int i = 2; i <= 94; i = i+5) {
    radio.setChannel(i);
    if(i == 72) {
      i = i-3;
    }
  }
}

void loop(void) {
  one();
}

// source: https://github.com/WOD-MN/bt_jam/blob/main/BLE_JAMMER_HSPI_PIN.ino
// NRF24 Connection
// SCK=14, MISO=12, MOSI=13, CS=15, CE=16


#include "RF24.h"
#include <ezButton.h>
#include "esp_bt.h"
#include "esp_wifi.h"

SPIClass *hp = nullptr;

RF24 radio(16, 15, 16000000);

byte i = 45;

ezButton toggleSwitch(33);


void setup(void) {
  esp_bt_controller_deinit();
  esp_wifi_stop();
  esp_wifi_deinit();
  Serial.begin(115200);
  toggleSwitch.setDebounceTime(50);
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

  toggleSwitch.loop();  // MUST call the loop() function first

  /* if (toggleSwitch.isPressed())
    Serial.println("one");
  if (toggleSwitch.isReleased())
    Serial.println("two");*/

  int state = toggleSwitch.getState();

  if (state == LOW)
    one();

}

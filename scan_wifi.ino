/*
*  ESP32 WiFi Scanner Example. Examples > WiFi > WiFiScan
*  Full Tutorial @ https://deepbluembedded.com/esp32-wifi-library-examples-tutorial-arduino/
*/
 
#include <WiFi.h>
#include <WiFiUdp.h>

// SSID dan Password WiFi
const char * ssid = "isi ssid"; //ssid wifi
const char * pwd = "isi_password"; //password wifi

// IP Address dan Port Server
const char * udpAddress = "192.168.0.0"; //ganti dengan ip address PC - didapat dari run program python
const int udpPort = 9999;

// Objek Kelas WiFiUDP
WiFiUDP udp;


void setup()
{
    Serial.begin(115200);
 
    WiFi.mode(WIFI_STA);

    
    WiFi.begin(ssid, pwd);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }
    
    delay(100);
 
    Serial.println("Setup done");
}
 
void loop()
{
    Serial.println("Scan start");
 
    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");

    String str_data = "";
    
    if (n == 0) {
        Serial.println("no networks found");
    } 
    else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH ");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            str_data = str_data+String(WiFi.SSID(i).c_str())+" | "+String(WiFi.RSSI(i))+" | "+String(WiFi.channel(i))+"\n";
            delay(10);
        }
        Serial.println(str_data);
    }
    Serial.println("");
 
    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

    int size_data;
    // variabel penampung untuk komunikasi
    uint8_t buffer_terima[200] = "";

    memcpy(buffer_terima, str_data.c_str(), 200);

    Serial.println("sending data.....");
  
    // Mulai menghubungkan dan mengirim data ke Server
    udp.beginPacket(udpAddress,udpPort);
    udp.write(buffer_terima, sizeof(buffer_terima));
    udp.endPacket();

    Serial.println("data sent");
 
    // Wait a bit before scanning again.
    delay(1000);
}

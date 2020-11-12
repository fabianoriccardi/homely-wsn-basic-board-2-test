/*
 * Send and receive random data over UDP from another device
 * on the local network (possibly another ESP32-S2, so you
 * can reuse this sketch).
 * 
 * This sketch was used to test power consumption of MCU.
 */
#include <WiFi.h>
#include <WiFiUdp.h>

const int ledPin = 4;

// WiFi network name and password
const char * networkName = "";
const char * networkPswd = "";

// Broadcast address
const char * udpAddress = "192.168.0.255";
const int udpPort = 3333;

boolean connected = false;

WiFiUDP udp;

void setup(){
  Serial.begin(115200);
  while(!Serial);
  Serial.println();
  Serial.println("WiFi UDP");

  // Blink at boot
  pinMode(ledPin, OUTPUT);
  for(int i=0; i<10; i++){
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
    delay(50);  
  }
  
  connectToWiFi(networkName, networkPswd);
}

void loop(){
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char buff[packetSize+1] = {0};
    udp.read(buff, packetSize);
    Serial.println(buff);
  }
  
  if(connected){
    udp.beginPacket(udpAddress, udpPort);
    udp.printf("Millis since boot: %lu", millis());
    udp.endPacket();
  }  
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}

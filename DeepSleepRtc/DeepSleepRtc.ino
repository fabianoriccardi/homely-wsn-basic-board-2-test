/**
 * Test the deepsleep and the external RTC. This test should be run
 * for a while (an hour) and check it time remains in sync (+/-1s).
 * It requires recompiling Arduino core for ESP32-S2 
 * using the following parameters:
 * - CONFIG_ESP32S2_RTC_CLK_SRC = ESP32S2_RTC_CLK_SRC_EXT_CRYS
 * - CONFIG_ESP32S2_TIME_SYSCALL = ESP32S2_TIME_SYSCALL_USE_RTC
 */
#include <WiFi.h>
#include <time.h>

const char* ssid     = "";
const char* password = "";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 0;

// in seconds
const int sleepTime = 15;

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup(){
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Test deepsleep and RTC");

  // Print the wakeup reason for ESP32
  print_wakeup_reason();
  
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  if(bootCount == 1){
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println(" CONNECTED");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        Serial.printf("Waiting for system time to be set... (%d/%d)\n", retry, retry_count);
        delay(1000);
    }
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }

  printLocalTime();

  Serial.println(String("Going to sleep for ") + sleepTime + " seconds");
  Serial.flush(); 
  esp_deep_sleep(1000000LL * sleepTime);
}

void loop(){}

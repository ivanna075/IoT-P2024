#include "Ubidots.h"
#include "DHT.h"

const char* UBIDOTS_TOKEN = "BBUS-VuJyKs1WKrWxIwmdY0FbSiE6cSc5Ys";
const char* UBIDOTS_DEVICELABEL = "mqtt-publish-esp32";
const char* WIFI_SSID = "IoT";
const char* WIFI_PASS = "1t3s0IoT23";

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
char str_temp[10];
char str_hum[10];

#define PIN_DHT 15
DHT dht(PIN_DHT, DHT11);

void setup() {
  Serial.begin(115200);
  ubidots.setDebug(true);

  Serial.println("Connecting to Wi-Fi...");
  while (!ubidots.wifiConnect(WIFI_SSID, WIFI_PASS)) {
    Serial.println("Failed to connect to Wi-Fi. Retrying...");
    delay(1000);
  }
  Serial.println("Connected to Wi-Fi.");

  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);

    ubidots.add("temperature", temperature);
    ubidots.add("humidity", humidity);

    if (ubidots.send(UBIDOTS_DEVICELABEL)) {
      Serial.println("Data sent successfully to Ubidots.");
    } else {
      Serial.println("Failed to send data to Ubidots.");
    }
  } else {
    Serial.println("Failed to read data from sensor.");
  }

  delay(5000); // Wait for a few seconds before next reading
}

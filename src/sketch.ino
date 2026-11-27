
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "DHTesp.h"

// SENSOR PINS
#define DHT_PIN 15
#define SOIL_PIN 34
#define PUMP_PIN 26

// WIFI SETTINGS
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// THINGSPEAK SETTINGS
const char* writeAPIKey = "0ON85TZQLUQWYIWM";

const char* server = "https://api.thingspeak.com/update";

// SENSOR OBJECT
DHTesp dhtSensor;

// UPDATE INTERVAL: 20 SECONDS
unsigned long previousMillis = 0;
const unsigned long interval = 20000;

void connectWiFi() {

  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password, 6);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void sendToThingSpeak(float temperature,
                      float humidity,
                      int soilMoisture,
                      int irrigationStatus) {

  WiFiClientSecure client;

  // Prototype only: skips TLS certificate verification.
  // Do not use this setting in a production deployment.
  client.setInsecure();

  HTTPClient https;

  String url = String(server) +
               "?api_key=" + String(writeAPIKey) +
               "&field1=" + String(temperature, 2) +
               "&field2=" + String(humidity, 2) +
               "&field3=" + String(soilMoisture) +
               "&field4=" + String(irrigationStatus);

  Serial.println();
  Serial.println("Sending data to ThingSpeak...");

  if (https.begin(client, url)) {

    int httpCode = https.GET();

    if (httpCode > 0) {

      String response = https.getString();

      Serial.print("HTTP Response: ");
      Serial.println(httpCode);

      Serial.print("ThingSpeak Entry ID: ");
      Serial.println(response);

      if (response.toInt() > 0) {
        Serial.println("CLOUD UPLOAD SUCCESSFUL!");
      }
      else {
        Serial.println("UPLOAD REJECTED OR RATE LIMITED");
      }

    }
    else {
      Serial.print("HTTP Error: ");
      Serial.println(https.errorToString(httpCode));
    }

    https.end();

  }
  else {
    Serial.println("Unable to connect to ThingSpeak.");
  }
}

void setup() {

  Serial.begin(115200);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  Serial.println("SMART AGRICULTURE SYSTEM");
  Serial.println("========================");

  connectWiFi();

  Serial.println("System Ready!");
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    // READ SENSORS
    TempAndHumidity data = dhtSensor.getTempAndHumidity();

    int soilValue = analogRead(SOIL_PIN);

    int soilMoisture = map(
      soilValue, 0, 4095, 100, 0
    );

    // CHECK SENSOR VALUES
    if (isnan(data.temperature) || isnan(data.humidity)) {
      Serial.println("DHT22 reading failed!");
      return;
    }

    // IRRIGATION DECISION
    int irrigationStatus = 0;

    if (soilMoisture < 30) {
    irrigationStatus = 1;
    digitalWrite(PUMP_PIN, HIGH);
    }
    else {
    irrigationStatus = 0;
    digitalWrite(PUMP_PIN, LOW);
    }

    // DISPLAY READINGS
    Serial.println();
    Serial.println("----- SENSOR READINGS -----");

    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(data.humidity);
    Serial.println(" %");

    Serial.print("Soil Moisture: ");
    Serial.print(soilMoisture);
    Serial.println(" %");

    if (irrigationStatus == 1) {
      Serial.println("STATUS: SOIL DRY");
      Serial.println("IRRIGATION: REQUIRED");
    }
    else {
      Serial.println("STATUS: SOIL NORMAL");
      Serial.println("IRRIGATION: NOT REQUIRED");
    }

    // SEND DATA TO CLOUD
    sendToThingSpeak(
      data.temperature,
      data.humidity,
      soilMoisture,
      irrigationStatus
    );

    Serial.println("---------------------------");
  }
}

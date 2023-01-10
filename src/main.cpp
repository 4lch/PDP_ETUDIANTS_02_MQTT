#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "WiFiClientSecure.h"
#include <PubSubClient.h>

const char* wifi_ssid = "Bidul";
const char* wifi_password = "12345678";
const char* mqtt_server = "27cc61dbaffc4da08cd0081cabd8cf01.s2.eu.hivemq.cloud";
int mqtt_port = 8883;
const char* mqtt_user = "ocres4ever";
const char* mqtt_pass = "ocresse123";
const char* client_id = "TD06_GP01";

// Define the pins that we will use
#define SENSOR 33
#define LED 26
#define DHTTYPE DHT11

DHT_Unified dht(SENSOR, DHTTYPE);

WiFiClientSecure client;
PubSubClient mqtt_client(client); 

void connect_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(wifi_ssid, wifi_password);
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 2OOms for re-trying
    delay(200);
  }
  Serial.println("\nConnected.");
}



void setup() {
  // Begin serial communication
  Serial.begin(9600);
  delay(100);

  // Connect to WiFi
  connect_wifi();
  
  // Configure MQTT server
  mqtt_client.setServer(mqtt_server, mqtt_port);

  // Start listening to the DHT11
  dht.begin();

  sensors_event_t event;

  // Get temperature event and print its value
  float temp_measure = -999.0;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    temp_measure = event.temperature;
  }

  // Get humidity event and print its value.
  float relative_humidity_measure = -999.0;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    relative_humidity_measure = event.relative_humidity;
  }

  // Send data to the broker with MQTT
  if (mqtt_client.connect(client_id,mqtt_user,mqtt_pass)) {
    char tempString[8];
    char humString[8];
    dtostrf(temp_measure, 1,2,tempString);
    dtostrf(relative_humidity_measure, 1,2, humString);
    mqtt_client.publish("TD06_GP01/temp",tempString);
    mqtt_client.publish("TD06_GP01/relhum", humString);
    
  } else {
    Serial.print("error connection");
}

  Serial.println("Going to sleep for 5 seconds...");
  delay(100);
  ESP.deepSleep(5e6);
}

void loop() {
  // Not needed anymore, the function is kept so Platformio does not complain.
}
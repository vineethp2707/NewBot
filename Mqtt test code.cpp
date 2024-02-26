#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "satvik's wifi";
const char* password = "satvik1756";
const char* mqtt_server = "192.168.77.76";

const char* humidity_topic = "sensor/DHT11/humidity_topic";
const char* temperature_celsius_topic = "sensor/DHT11/temperature_in_celsius";
const char* temperature_fahrenheit_topic = "sensor/DHT11/temperature_in_fahrenheit";




WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, 1883);
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }

  dht.begin();
}

void loop() {
  delay(2000);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(humidity) || isnan(temperature) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("temperature in fahrenheit");
  Serial.print(f);
  Serial.print("%  Temperature in celsius ");
  Serial.print(temperature);
  Serial.println("°C");
  

  client.publish(humidity_topic, String(humidity).c_str());
  client.publish(temperature_celsius_topic, String(temperature).c_str());
  client.publish(temperature_fahrenheit_topic, String(f).c_str());
}


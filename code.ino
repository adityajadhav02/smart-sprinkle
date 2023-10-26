#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT11
#define MOISTUREPIN A0
#define RELAYPIN D1

const char* ssid = "Note8";
const char* password = "aditya123";
const char* server = "YOUR_SERVER_IP";
const int port = 3000;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(MOISTUREPIN, INPUT);
  pinMode(RELAYPIN, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void sendSensorData(float temperature, float humidity, int moisture, int relayState) {
  WiFiClient client;
  if (client.connect(server, port)) {
    String url = "/update?temp=" + String(temperature) + "&hum=" + String(humidity) + "&moist=" + String(moisture) + "&relay=" + String(relayState);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(10);
    client.stop();
  }
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int moisture = analogRead(MOISTUREPIN);
  
  int relayState = LOW;
  if (temperature > 28 && moisture < 500) {
    relayState = HIGH;
    digitalWrite(RELAYPIN, HIGH);
  } else {
    relayState = LOW;
    digitalWrite(RELAYPIN, LOW);
  }
  
  sendSensorData(temperature, humidity, moisture, relayState);
  
  delay(1000); // Send data every second
}

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
// #include<HTTPClient.h>

#define DHTPIN D3
#define DHTTYPE DHT11
#define MOISTUREPIN A0
#define RELAYPIN D1

const char* ssid = "Note8";
const char* password = "aditya123";
const char* server = "192.168.119.108";
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
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Connected!");
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
  else{
    String url = "/update?temp=" + String(temperature) + "&hum=" + String(humidity) + "&moist=" + String(moisture) + "&relay=" + String(relayState);
    
    Serial.print("[sendSensorData] Connection failed. Error code: ");
  Serial.println(client.status());
  Serial.print("Failed to connect to ");
Serial.print(server);
Serial.print(" on port ");
Serial.println(port);
Serial.println(url);

  }
}


void checkOverrideStatus() {
  WiFiClient client;

  if (client.connect("192.168.119.108", 3000)) {
    client.print("GET /getOverrideStatus HTTP/1.1\r\n");
    client.print("Host: 192.168.1.100\r\n");
    client.print("Connection: close\r\n\r\n");

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }

    String payload = client.readStringUntil('\n');
    if (payload == "ON") {
      // Handle the case when override is ON
      // For example, you can turn off the relay
      digitalWrite(RELAYPIN, LOW);
    } else if (payload == "OFF") {
      // Handle the case when override is OFF
      // For example, you can use the existing logic to control the relay
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      int moisture = analogRead(MOISTUREPIN);
      int relayState = (temperature > 28 && moisture < 500) ? HIGH : LOW;
      digitalWrite(RELAYPIN, relayState);
    }

    client.stop();
  }
}

void loop() {
  // checkOverrideStatus();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int moisture = 1024-analogRead(MOISTUREPIN);
  
  int relayState = LOW;
  if (temperature > 28 && moisture < 500) {
    relayState = HIGH;
    digitalWrite(RELAYPIN, HIGH);
  } else {
    relayState = LOW;
    digitalWrite(RELAYPIN, LOW);
  }
  Serial.println("Temp: " + String(temperature) + "Humidity: " + String(humidity) + "Moisture: "+ String(moisture));
  sendSensorData(temperature, humidity, moisture, relayState);
  
}
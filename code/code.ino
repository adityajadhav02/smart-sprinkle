#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
// #include<HTTPClient.h>

#define DHTPIN D2
#define DHTTYPE DHT11
#define MOISTUREPIN A0
#define RELAYPIN D1

const char* ssid = "Note8";
const char* password = "aditya123";
const char* server = " 192.168.88.108";
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

// void checkOverrideStatus() {
//   HTTPClient http;

//   String url = "http://192.168.1.100:3000/getOverrideStatus"; // Replace with your laptop's local IP address

//   http.begin(url);
//   int httpCode = http.GET();

//   if (httpCode > 0) {
//     if (httpCode == HTTP_CODE_OK) {
//       String payload = http.getString();
//       if (payload == "ON") {
//         // Handle the case when override is ON
//         // For example, you can turn off the relay
//         digitalWrite(RELAYPIN, LOW);
//       } else if (payload == "OFF") {
//         // Handle the case when override is OFF
//         // For example, you can use the existing logic to control the relay
//         float temperature = dht.readTemperature();
//         float humidity = dht.readHumidity();
//         int moisture = analogRead(MOISTUREPIN);
//         int relayState = (temperature > 28 && moisture < 500) ? HIGH : LOW;
//         digitalWrite(RELAYPIN, relayState);
//       }
//     }
//   }

//   http.end();
// }

void checkOverrideStatus() {
  WiFiClient client;

  if (client.connect("192.168.1.100", 3000)) {
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
  checkOverrideStatus();
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
  
}

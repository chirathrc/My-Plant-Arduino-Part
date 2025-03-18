#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Define the GPIO pin where the DHT11 data pin is connected
#define DHTPIN 4       // Change this to the GPIO pin you're using
#define RELAY1_PIN 23  // GPIO pin connected to IN1 (K1)
int ledPin = 2;        // LED pin
#define SOIL_SENSOR_PIN 34

// Set the type of DHT sensor
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);


const char* ssid = "";
const char* password = "";

// Adjust these based on your testing with dry and wet soil
int dryValue = 3000;  // Analog value when soil is completely dry
int wetValue = 1000;  // Analog value when soil is completely wet


// Placeholder for the network server, similar to WiFiServer on port 80
NetworkServer server(80);

void get_network_info() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("[*] Network information for ");
    Serial.println(ssid);

    Serial.println("[+] BSSID : " + WiFi.BSSIDstr());
    Serial.print("[+] Gateway IP : ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("[+] Subnet Mask : ");
    Serial.println(WiFi.subnetMask());
    Serial.println((String) "[+] RSSI : " + WiFi.RSSI() + " dB");
    Serial.print("[+] ESP32 IP : ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(ledPin, OUTPUT);  // Set the LED pin as output
  pinMode(SOIL_SENSOR_PIN, INPUT);

  pinMode(RELAY1_PIN, OUTPUT);    // Set K1 pin as output
  digitalWrite(RELAY1_PIN, LOW);  // Ensure relay is OFF initially

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to Wi-Fi network");
  server.begin();  // Start the server (assuming server.begin() is supported in NetworkServer)
  Serial.println("Server started");
  get_network_info();
}

void loop() {
  // Placeholder for a network client connection, similar to WiFiClient
  NetworkClient client = server.accept();  // Assuming `accept` method exists in NetworkServer

  if (client) {
    Serial.println("Client connected");

    String requestText = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        requestText += c;

        if (c == '\n') {
          Serial.println("Request: " + requestText);

          if (requestText.startsWith("GET /?status=8")) {
            digitalWrite(ledPin, HIGH);  // Turn LED on

          } else if (requestText.startsWith("GET /temp")) {

            float humidity = dht.readHumidity();
            float temperature = dht.readTemperature();

            Serial.print("Humidity: ");
            Serial.print(humidity);
            Serial.print(" %\t");
            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.println(" °C");


            if (isnan(humidity) || isnan(temperature)) {
              Serial.println("Failed to read from DHT sensor!");
              return;
            }

            String jsonResponse = "{\"temp\": " + String(temperature, 2) + ",\"humidity\":" + String(humidity, 2) + "}";

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Connection:Close");
            client.println();
            // client.println(temperature);  // Example temperature value
            client.println(jsonResponse);


          } else if (requestText.startsWith("GET /water")) {

            int soilMoistureValue = analogRead(SOIL_SENSOR_PIN);

            // Map the soil moisture value to a percentage
            int soilMoisturePercent = map(soilMoistureValue, dryValue, wetValue, 0, 100);

            // Constrain the value to ensure it's between 0 and 100
            soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

            // Determine if the soil is dry, moist, or wet
            String moistureStatus;
            if (soilMoisturePercent < 30) {
              moistureStatus = "Dry";
            } else if (soilMoisturePercent > 70) {
              moistureStatus = "Wet";
            } else {
              moistureStatus = "Moist";
            }

            String jsonResponse = "{\"water\": " + String(soilMoisturePercent) + ",\"humidity\":\"" + moistureStatus + "\"}";

            // Print the result
            // Serial.print("Soil Moisture (%): ");
            // Serial.print(soilMoisturePercent);
            // Serial.print("% - ");
            // Serial.println(moistureStatus);


            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Connection:Close");
            client.println();
            // client.println(temperature);  // Example temperature value
            client.println(jsonResponse);


          } else if (requestText.startsWith("GET /?status=1")) {


            Serial.println("Water Start");
            digitalWrite(RELAY1_PIN, HIGH);  // Turn ON K1 (relay activated)




          } else {
            // digitalWrite(ledPin, LOW);  // Turn LED off
            digitalWrite(RELAY1_PIN, LOW);

            Serial.println("Water Stop");
          }

          break;
        }
      }
    }

    client.stop();  // Close the connection
    // Serial.println("Client disconnected");
  }
}

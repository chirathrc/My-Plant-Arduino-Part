# 🌿 Arduino-Based Smart Plant Monitoring & Irrigation System

This project is a smart IoT-based plant monitoring and automatic watering system built using **Arduino (ESP32)**, **DHT11 sensor**, **Soil Moisture Sensor**, and **Relay Module** to control a water pump. The system can also communicate over WiFi to provide real-time soil moisture and humidity data and allow remote watering.

## 🚀 Features

- 📡 **WiFi-enabled**: Connects to your WiFi network and runs a local web server.
- 🌡 **DHT11 Sensor**: Reads temperature and humidity.
- 🌱 **Soil Moisture Monitoring**: Measures soil moisture levels and classifies them as Dry, Moist, or Wet.
- 💧 **Automated Irrigation**: Activates a relay to control a water pump based on user command or moisture levels.
- 🔗 **REST-like Endpoints**:
  - `GET /temp`: Get real-time temperature & humidity.
  - `GET /water`: Get real-time soil moisture percentage & status.
  - `GET /?status=1`: Activate water pump.
  - `GET /?status=8`: Turn on onboard LED.
  - Any other route stops the water pump.

## 🛠 Components Used

- ESP32 Module
- DHT11 Temperature & Humidity Sensor
- Soil Moisture Sensor (Analog)
- Relay Module (1-Channel)
- Water Pump
- LED Indicator
- Jumper Wires
- Breadboard

## 📊 Soil Moisture Calibration

| Condition     | Sensor Value (Analog) |
| ------------- | --------------------- |
| Dry Soil      | ~3000                 |
| Wet Soil      | ~1000                 |

These values are mapped to a percentage range (0% - 100%) and classified as Dry, Moist, or Wet.

## 🌐 Network Info Logging

Upon successful WiFi connection, the system prints:
- BSSID
- Gateway IP
- Subnet Mask
- RSSI
- ESP32 Local IP

## ⚙️ How it Works

1. ESP32 connects to the specified WiFi network.
2. A simple HTTP server runs on port `80`.
3. Clients (such as your mobile app or browser) can make HTTP GET requests to control the system and fetch sensor data.
4. Based on user requests or soil moisture readings, the water pump relay is turned ON/OFF.

## 💡 Example Usage

- To get temperature and humidity:  
  `GET http://<ESP32_IP>/temp`

- To get soil moisture status:  
  `GET http://<ESP32_IP>/water`

- To turn on water pump:  
  `GET http://<ESP32_IP>/?status=1`

- To stop water pump:  
  `GET http://<ESP32_IP>/` (any other route will stop the relay)

## 📟 Serial Monitor Logs

- "Connecting to WiFi..."
- "Connected to Wi-Fi network"
- "Client connected"
- "Request: ..."
- Sensor readings (temperature, humidity, soil moisture %)
- Pump control actions ("Water Start", "Water Stop")

---

## 📝 Notes

- Customize `ssid` and `password` with your WiFi credentials.
- Relay logic is active HIGH (`HIGH` to turn ON the water pump).
- Ensure power requirements for the relay and pump are safely met.

---

## ⚠️ Dependencies

- **ESP32 WiFi Library** (built-in)
- **Adafruit Unified Sensor Library**
- **DHT Sensor Library**

---

## 🖼 Suggested Improvements

- Add a scheduling feature for automatic watering based on time and moisture.
- Add remote control via a mobile or web app.
- Use secure HTTP or MQTT for safer communication.

---

🌿 Happy Gardening! 🚀

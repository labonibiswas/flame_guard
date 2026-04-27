# 🔥 Flame Guard 🛡️

An IoT-based real-time monitoring and alert system built with ESP32. This project actively monitors environmental conditions and triggers automated SMS alerts via Twilio when critical thresholds are reached, ensuring rapid response to potential fire hazards.

## ✨ Features

* **🌡️ Real-Time Monitoring:** Continuously tracks temperature and humidity using a DHT sensor.
* **📱 Instant Alerts:** Integrates with the Twilio API to send immediate SMS notifications during emergencies.
* **📟 Local Display:** Outputs real-time sensor readings to an I2C LCD screen for on-site visibility.
* **🚨 Audio-Visual Warnings:** Utilizes LEDs and a buzzer for immediate local hazard indication.
* **🌐 WiFi Enabled:** Connects directly to local networks for seamless cloud communication.

## 🛠️ Hardware Requirements

* ESP32 Development Board
* DHT Sensor (DHT11 or DHT22)
* LiquidCrystal I2C Display
* LEDs (Red and Green)
* Active Buzzer
* Jumper Wires & Breadboard
* Relay modul
* water pump

## 💻 Software & Libraries

* [PlatformIO](https://platformio.org/) / VS Code
* `DHT sensor library` by Adafruit
* `LiquidCrystal I2C`
* Twilio Account (for API credentials)

## 🚀 Installation & Setup

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/labonibiswas/flame_guard.git](https://github.com/labonibiswas/flame_guard.git)
   cd flame_guard

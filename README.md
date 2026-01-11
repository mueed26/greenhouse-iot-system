# Greenhouse Regulating and Monitoring System

![License](https://img.shields.io/badge/license-MIT-blue.svg)


An automated greenhouse climate control system using IoT technology for smart urban agriculture. This project contributes to **UN Sustainable Development Goal 11: Sustainable Cities and Communities**.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [System Architecture](#system-architecture)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)
- [Team](#team)

## 🌿 Overview

The Greenhouse Regulating and Monitoring System is an IoT-based solution that automates climate control in small-scale greenhouses. It monitors temperature, humidity, CO2/gas levels, and soil moisture in real-time, automatically regulating ventilation to maintain optimal growing conditions.

### Key Objectives

- Automate greenhouse climate control for urban agriculture
- Enable real-time remote monitoring via cloud dashboard
- Optimize resource usage (water, energy) through precision sensing
- Protect crops from harmful conditions
- Reduce manual labor and human error
- Provide email alerts for critical environmental conditions

## ✨ Features

- **Real-time Monitoring**: Continuous tracking of temperature, humidity, CO2, and soil moisture
- **Automated Ventilation**: Servo-controlled vent opens/closes based on threshold values
- **Cloud Integration**: Data uploaded to V-One IoT platform via MQTT over TLS
- **Email Alerts**: Notifications when environmental thresholds are exceeded
- **Multi-Device Architecture**: Separate device IDs for organized data visualization
- **Edge Computing**: Local control decisions for immediate response
- **Dashboard Visualization**: Real-time and historical data charts

## 🔧 Hardware Requirements

| Component | Model | Quantity | Purpose |
|-----------|-------|----------|---------|
| Microcontroller | ESP32 S3 Maker Feather | 1 | Gateway & Processing |
| Temperature/Humidity Sensor | DHT11 | 1 | Climate Monitoring |
| Gas Sensor | MQ2 | 1 | Air Quality Detection |
| Soil Sensor | Capacitive Moisture Sensor | 1 | Irrigation Monitoring |
| Actuator | Servo Motor (180°) | 1 | Ventilation Control |
| Power Supply | USB 5V | 1 | System Power |
| Breadboard | Standard | 1 | Prototyping |
| Jumper Wires | Male-to-Male | Multiple | Connections |
| Enclosure | plastic container | 1 | Greenhouse Simulation |

### Pin Connections

- **DHT11**: GPIO 41 (Data), 3.3V (VCC), GND
- **MQ2**: GPIO 4 (Analog), 5V (VCC), GND
- **Soil Moisture**: GPIO 5 (Analog), 3.3V (VCC), GND
- **Servo Motor**: GPIO 40 (PWM), 5V (VCC), GND

## 💻 Software Requirements

### Development Environment

- **Arduino IDE**: Version 2.0 or newer
- **ESP32 Board Support**: esp32 by Espressif Systems
- **Operating System**: Windows, macOS, or Linux

### Required Libraries

Install these libraries via Arduino IDE Library Manager:
```
- DHT sensor library by Adafruit (v1.4.4 or newer)
- PubSubClient by Nick O'Leary (v2.8.0 or newer)
- Arduino_JSON (v0.2.0 or newer)
- ESP32Servo (v0.13.0 or newer)
- WiFiClientSecure (included with ESP32 board support)
```

### Cloud Platform

- **V-One IoT Platform**: Account required at [v-one.my](https://v-one.my)
- **WiFi Network**: 2.4GHz network with internet access

## 🏗️ System Architecture

The system follows a 4-layer IoT architecture:
```
┌─────────────────────────────────────────────┐
│   Layer 4: Application Layer                │
│   - V-One Dashboard                         │
│   - Email Alert System                      │
└─────────────────────────────────────────────┘
                    ↕
┌─────────────────────────────────────────────┐
│   Layer 3: Processing Layer                 │
│   - V-One IoT Platform                      │
│   - MQTT Broker (Port 8883)                 │
│   - Data Storage & Analytics                │
└─────────────────────────────────────────────┘
                    ↕
┌─────────────────────────────────────────────┐
│   Layer 2: Network Layer                    │
│   - ESP32 S3 (Gateway)                      │
│   - WiFi Communication                      │
│   - MQTT over TLS                           │
└─────────────────────────────────────────────┘
                    ↕
┌─────────────────────────────────────────────┐
│   Layer 1: Perception Layer                 │
│   - DHT11 (Temp/Humidity)                   │
│   - MQ2 (Gas Sensor)                        │
│   - Soil Moisture Sensor                    │
│   - Servo Motor (Actuator)                  │
└─────────────────────────────────────────────┘
```

## 🚀 Installation

### 1. Hardware Setup

1. **Assemble Components on Breadboard**
   - Connect all sensors and servo motor according to pin connections above
   - Ensure common ground across all components
   - Use external 5V power for servo if available

2. **Create Greenhouse Enclosure**
   - Use cardboard/plastic box (approx. 30x30x30cm)
   - Cut ventilation hole in lid (5-8cm diameter)
   - Mount servo motor to control flap over ventilation hole
   - Place sensors inside enclosure

### 2. Software Setup

1. **Install Arduino IDE**
```
   Download from: https://www.arduino.cc/en/software
```

2. **Add ESP32 Board Support**
   - Open Arduino IDE
   - Go to File → Preferences
   - Add to "Additional Board Manager URLs":
```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
   - Go to Tools → Board → Board Manager
   - Search "ESP32" and install "esp32 by Espressif Systems"

3. **Install Required Libraries**
   - Go to Sketch → Include Library → Manage Libraries
   - Install: DHT sensor library, PubSubClient, Arduino_JSON, ESP32Servo

4. **Clone This Repository**
```bash
   git clone https://github.com/YOUR_USERNAME/greenhouse-iot-system.git
   cd greenhouse-iot-system
```

   OR download ZIP:
   - Click green "Code" button on GitHub
   - Select "Download ZIP"
   - Extract to your Arduino projects folder

## ⚙️ Configuration

### 1. V-One Platform Setup

1. **Create Account**
   - Register at [v-one.my](https://v-one.my)
   - Verify your email

2. **Create Gateway**
   - Navigate to Gateways → Add Gateway
   - Name: "Greenhouse_Gateway"
   - Note the **Gateway ID** and **Access Token**

3. **Create 4 Devices**
   - Device 1: "DHT11_Climate" (Humidity, Temperature, System_Status)
   - Device 2: "MQ2_Gas" (CO2_Level)
   - Device 3: "Soil_Moisture" (Soil_Moisture)
   - Device 4: "Servo_Vent" (Vent_Status)
   - Note each **Device ID**

4. **Configure Alert Rules**
   - Temperature > 30°C → Send Email
   - Humidity > 80% → Send Email
   - CO2 > 1500 → Send Email 

### 2. Code Configuration

1. **Edit `vonesetting.h`**

   Open `src/vonesetting.h` and update:
```cpp
   #define WIFI_SSID "YourWiFiName"           // Your WiFi network
   #define WIFI_PASSWORD "YourPassword"       // Your WiFi password
   #define MQTT_USERNAME "YourAccessToken"    // V-One Gateway Access Token
   #define GATEWAYID "YourGatewayID"          // V-One Gateway ID
```

2. **Edit Device IDs in Main Sketch**

   Open `src/greenhouse_system.ino` and update:
```cpp
   const char* dht_DeviceId      = "your-dht-device-id";
   const char* mq2_DeviceId      = "your-mq2-device-id";
   const char* moisture_DeviceId = "your-moisture-device-id";
   const char* servo_DeviceId    = "your-servo-device-id";
```

3. **Adjust Thresholds (Optional)**

   Modify thresholds based on your plants:
```cpp
   const int TEMP_THRESHOLD = 30;          // Temperature in °C
   const int GAS_THRESHOLD = 1500;         // Gas level (analog value)
   const int HUM_HIGH_THRESHOLD = 80;      // Humidity in %
```

### 3. Upload Firmware

1. Connect ESP32 to computer via USB
2. Select: Tools → Board → ESP32S3 Dev Module
3. Select: Tools → Port → (your ESP32 port)
4. Click Upload button (→)
5. Wait for "Done uploading" message

## 📊 Usage

### Starting the System

1. **Power On**
   - Connect ESP32 via USB to power supply
   - System automatically connects to WiFi and V-One platform

2. **Monitor via Serial**
   - Open Serial Monitor (Tools → Serial Monitor)
   - Set baud rate to 115200
   - Observe sensor readings every 2 seconds

3. **Access Dashboard**
   - Log into V-One platform
   - Navigate to your dashboard
   - View real-time sensor data and charts

### System Operation

**Normal Conditions:**
- Temperature < 30°C
- Humidity < 80%
- Gas < 1500
- Ventilation: CLOSED (90°)
- Status: "Normal"

**Regulation Mode:**
- Any threshold exceeded
- Ventilation: OPEN (180°)
- Status: "Regulating: [condition]"
- Email alert sent

**Danger Mode:**
- Gas > 1500
- Ventilation: OPEN (180°)
- Status: "DANGER: High CO2/Gas"
- High-priority email alert

### Monitoring Options

1. **Serial Monitor**: Real-time debugging and status
2. **V-One Dashboard**: Remote web/mobile access
3. **Email Alerts**: Proactive notifications
4. **Historical Data**: Trend analysis via V-One charts

## 📁 Project Structure
```
greenhouse-iot-system/
├── cpc357 project Greenhouse/
│   ├── Greenhouse.ino      # Main Arduino sketch
│   ├── VOneMqttClient.cpp         # V-One MQTT client implementation
│   ├── VOneMqttClient.h           # V-One client header
│   └── vonesetting.h              # WiFi and V-One credentials
├── README.md                      # This file
├── LICENSE                        # MIT License
└── .gitignore                     # Git ignore file
```

## 🤝 Contributing

This is an academic project for CPC357: IoT Architecture & Smart Applications. While direct contributions are not accepted during the assessment period, we welcome:

- Bug reports
- Feature suggestions
- Questions and discussions (open an Issue)

After course completion, pull requests will be welcome for:
- Additional sensor support
- Machine learning integration
- Mobile app development
- Documentation improvements

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👥 Team

**Group 48 - CPC357 IoT Project**

- **Azam Tamheed** (160610) - Hardware Integration, Sensor Calibration
- **Mueed Hyder Mir** (160796) - Software Development, Cloud Integration

**Institution**: Universiti Sains Malaysia (USM)  
**Course**: CPC357: IoT Architecture & Smart Applications  
**Semester**: 2025/2026  


## 📺 Demo Video

[Link to YouTube Demo Video](YOUR_YOUTUBE_LINK_HERE)

The video demonstrates:
- System overview and architecture
- Live hardware demonstration
- Real-time data visualization on V-One dashboard
- Alert system functionality

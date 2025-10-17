# ESP32-CAM Robot Greeter Hardware

## Overview
This directory contains the Arduino sketch for the ESP32-CAM module that acts as a physical robot greeter. The hardware detects visitors using a PIR motion sensor, captures images with the onboard camera, and communicates with the central server via MQTT.

## Hardware Requirements

### Core Components
- **ESP32-CAM Module** (AI-Thinker or similar)
- **PIR Motion Sensor** (HC-SR501 or similar)
- **Speaker/Buzzer** (8Ω 0.5W recommended)
- **MicroSD Card** (optional, for local image storage)
- **USB-to-Serial Programmer** (FTDI FT232RL or CP2102)

### Power Supply
- **5V DC Power Supply** (minimum 2A recommended)
- **Power Jack** or USB-C connector
- **Voltage Regulator** (if using external power)

## Pin Configuration

| Component | ESP32-CAM Pin | Description |
|-----------|---------------|-------------|
| PIR Sensor | GPIO 13 | Motion detection input |
| Speaker | GPIO 2 | Audio output for greetings |
| Status LED | GPIO 33 | Built-in LED for status indication |
| Flash LED | GPIO 4 | Camera flash (built-in) |
| Reset Button | GPIO 0 | Programming mode / reset |

## Wiring Diagram

```
ESP32-CAM Module
┌─────────────────┐
│     ESP32-CAM   │
│                 │
│ GPIO 13 ────────┼─── PIR Sensor (OUT)
│ GPIO 2  ────────┼─── Speaker (+)
│ GPIO 33 ────────┼─── Status LED (built-in)
│ GPIO 4  ────────┼─── Flash LED (built-in)
│                 │
│ 5V      ────────┼─── Power Supply (+)
│ GND     ────────┼─── Power Supply (-) & Sensor GND
└─────────────────┘

PIR Sensor (HC-SR501)
┌─────────────┐
│    PIR      │
│             │
│ VCC ────────┼─── 5V
│ OUT ────────┼─── GPIO 13
│ GND ────────┼─── GND
└─────────────┘
```

## Setup Instructions

### 1. Arduino IDE Configuration
```bash
# Install ESP32 board package
1. Open Arduino IDE
2. Go to File → Preferences
3. Add ESP32 board URL: https://dl.espressif.com/dl/package_esp32_index.json
4. Go to Tools → Board → Boards Manager
5. Search "ESP32" and install "ESP32 by Espressif Systems"
```

### 2. Required Libraries
Install these libraries via Arduino IDE Library Manager:
- **WiFi** (built-in with ESP32)
- **PubSubClient** by Nick O'Leary
- **ArduinoJson** by Benoit Blanchon
- **ESP32 Camera** (built-in with ESP32)

### 3. Configuration
Edit the following constants in `esp32-cam-greeter.ino`:
```cpp
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* MQTT_SERVER = "192.168.1.100";  // Your MQTT broker IP
const char* SERVER_URL = "http://192.168.1.100:3000";  // Your server IP
const char* ROBOT_ID = "robot_001";  // Unique identifier
```

### 4. Programming the ESP32-CAM
1. **Connect programmer**: Wire FTDI to ESP32-CAM programming pins
2. **Enter programming mode**: Hold GPIO 0 button while powering on
3. **Select board**: "AI Thinker ESP32-CAM" in Arduino IDE
4. **Upload sketch**: Compile and upload the code
5. **Reset**: Disconnect GPIO 0 and reset to run the program

## MQTT Communication

### Published Topics
- `robot/{ROBOT_ID}/status` - Periodic status reports (every 30 seconds)
- `robot/{ROBOT_ID}/visitor_detected` - Motion detection events

### Subscribed Topics  
- `robot/{ROBOT_ID}/greeting` - Greeting commands from server
- `robot/{ROBOT_ID}/config` - Configuration updates

### Message Formats

#### Status Report
```json
{
  "robotId": "robot_001",
  "status": "online",
  "uptime": 123456,
  "freeHeap": 234567,
  "wifiSignal": -45,
  "lastMotion": 12345
}
```

#### Visitor Detection
```json
{
  "timestamp": 123456,
  "robotId": "robot_001", 
  "sensorTriggered": true
}
```

#### Greeting Command
```json
{
  "type": "returning_visitor",
  "name": "John Doe",
  "visitCount": 5
}
```

## Troubleshooting

### Camera Issues
- **Black images**: Check camera module connection
- **Blurry images**: Adjust focus ring on camera lens
- **No image capture**: Verify power supply (minimum 2A)

### Connectivity Issues
- **WiFi connection fails**: Check SSID/password configuration
- **MQTT not connecting**: Verify broker IP and port 1883
- **Server upload fails**: Check server URL and network connectivity

### Motion Detection
- **No motion detected**: Adjust PIR sensor sensitivity potentiometer
- **False triggers**: Increase MOTION_COOLDOWN value
- **Constant triggers**: Check PIR sensor wiring and power supply

## File Structure
```
hardware/
├── esp32-cam-greeter/
│   ├── esp32-cam-greeter.ino    # Main Arduino sketch
│   └── README.md                # This documentation
└── schemas/
    ├── mqtt-messages.json       # MQTT message schemas
    └── hardware-api.json        # Hardware API specification
```

## Development Notes
- Uses JPEG compression for efficient image transmission
- Implements motion detection cooldown to prevent spam
- Includes error handling for network connectivity issues
- Supports over-the-air configuration updates via MQTT
- Flash LED automatically activates for better image quality

## Next Steps
1. Test hardware assembly and connections
2. Configure WiFi and MQTT broker settings  
3. Deploy and test with central server
4. Add multiple robots with unique IDs
5. Implement advanced features (sound recognition, gesture detection)
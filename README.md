# Robot Greeter System

A comprehensive IoT-based visitor greeting system with real-time face recognition, MQTT communication, and web-based management interface.

## 🤖 Overview

The Robot Greeter System combines hardware sensors, computer vision, and web technologies to create an intelligent visitor greeting experience. The system detects visitors using PIR motion sensors, captures images with ESP32-CAM modules, performs face recognition against a remote database, and delivers personalized greetings through speakers and web interfaces.

## 🏗️ Architecture

```
┌─────────────────┐    MQTT     ┌─────────────────┐    HTTP/WS   ┌─────────────────┐
│   ESP32-CAM     │◄───────────►│   Node.js       │◄─────────────►│   Web Interface │
│   Hardware      │             │   Server        │               │   Management    │
│                 │             │                 │               │                 │
│ • Motion Sensor │             │ • MQTT Broker   │               │ • Visitor Mgmt  │
│ • Camera Module │             │ • Face API      │               │ • Real-time     │
│ • Speaker       │             │ • SQLite DB     │               │ • Analytics     │
│ • Status LEDs   │             │ • WebSocket     │               │ • Config        │
└─────────────────┘             └─────────────────┘               └─────────────────┘
```

## 🚀 Features

### Hardware (ESP32-CAM)
- **Motion Detection**: PIR sensor triggers visitor detection
- **Image Capture**: High-quality JPEG images for face recognition
- **Audio Feedback**: Speaker plays greeting tones
- **Status Indication**: LED feedback for system status
- **MQTT Communication**: Real-time communication with central server

### Server (Node.js)
- **Face Recognition**: Integration with remote face databases
- **Real-time Updates**: WebSocket for live visitor notifications
- **Data Persistence**: SQLite database for visitor records
- **MQTT Broker**: Mosquitto integration for hardware communication
- **RESTful API**: Complete API for visitor and robot management

### Web Interface
- **Visitor Management**: Upload images, identify visitors, view history
- **Real-time Dashboard**: Live visitor detection and robot status
- **Fleet Management**: Configure and monitor multiple robots
- **Analytics**: Visit patterns and robot performance metrics

## 📁 Project Structure

```
Robot Greeter/
├── server.js                    # Main Node.js server
├── package.json                 # Dependencies and scripts
├── mosquitto.conf              # MQTT broker configuration
├── public/                     # Web interface files
│   ├── index.html              # Main greeting interface
│   └── visitor-management.html # Admin interface
├── hardware/                   # ESP32-CAM Arduino code
│   ├── esp32-cam-greeter/      # Main Arduino sketch
│   └── schemas/                # MQTT message schemas
├── robots/                     # Individual robot configurations
│   ├── robot_001/              # Lobby greeter config
│   └── robot_002/              # Conference room config
├── uploads/                    # Uploaded visitor images
└── copilot-house-style-*       # Development tools
```

## 🛠️ Installation

### Prerequisites
- **Node.js** (v18+ recommended)
- **Arduino IDE** (for ESP32-CAM programming)
- **Mosquitto MQTT Broker**
- **ESP32-CAM Hardware** (AI-Thinker model recommended)

### Software Setup

1. **Clone Repository**
   ```bash
   git clone https://github.com/YOUR_USERNAME/robot-greeter.git
   cd robot-greeter
   ```

2. **Install Dependencies**
   ```bash
   npm install
   ```

3. **Install MQTT Broker**
   ```bash
   # Windows (via winget)
   winget install EclipseFoundation.Mosquitto
   
   # macOS (via Homebrew)
   brew install mosquitto
   
   # Linux (Ubuntu/Debian)
   sudo apt-get install mosquitto mosquitto-clients
   ```

4. **Start Services**
   ```bash
   # Start MQTT broker
   mosquitto -c mosquitto.conf
   
   # Start Node.js server (in separate terminal)
   npm start
   ```

### Hardware Setup

1. **Prepare ESP32-CAM**
   - Install ESP32 board package in Arduino IDE
   - Install required libraries: PubSubClient, ArduinoJson

2. **Configure Network**
   - Edit `hardware/esp32-cam-greeter/esp32-cam-greeter.ino`
   - Update WiFi credentials and server IP addresses

3. **Wire Components**
   ```
   ESP32-CAM Connections:
   - GPIO 13 → PIR Sensor OUT
   - GPIO 2  → Speaker positive
   - GPIO 33 → Status LED (built-in)
   - 5V/GND → Power supply
   ```

4. **Upload Firmware**
   - Connect ESP32-CAM to programmer
   - Select "AI Thinker ESP32-CAM" board
   - Upload sketch to hardware

## 🌐 Usage

### Web Interfaces
- **Main Interface**: `http://localhost:3000`
- **Visitor Management**: `http://localhost:3000/visitor-management.html`
- **House Style Snippets**: `copilot-house-style-quick-access.html`

### API Endpoints
- `POST /api/greet` - Create new greeting
- `POST /api/visitors/upload` - Upload visitor image
- `GET /api/visitors` - Get visitor list
- `GET /api/stats` - System statistics

### MQTT Topics
- `robot/{id}/status` - Robot status reports
- `robot/{id}/visitor_detected` - Motion detection events
- `robot/{id}/greeting` - Greeting commands to robots
- `robot/{id}/config` - Configuration updates

## 🔧 Configuration

### Robot Configuration
Edit `robots/robot_XXX/config.json`:
```json
{
  "robotId": "robot_001",
  "name": "Lobby Greeter",
  "location": { "building": "Main Office", "room": "Lobby" },
  "configuration": {
    "motionCooldown": 5000,
    "imageQuality": 10,
    "statusInterval": 30000
  }
}
```

### Server Configuration
Key settings in `server.js`:
- MQTT broker connection
- Database configuration
- File upload limits
- Face recognition API integration

## 📊 Monitoring

### System Health
- Real-time robot status via MQTT
- Web dashboard for fleet monitoring
- Automatic error reporting and alerts
- Performance metrics and analytics

### Maintenance
- Automated health checks every 5 minutes
- Monthly maintenance reminders
- Firmware update notifications
- Sensor calibration tracking

## 🔒 Security

### Data Protection
- TLS encryption for HTTPS endpoints
- MQTT client certificates for production
- Input sanitization and validation
- Secure image storage and transmission

### Privacy Considerations
- Local face processing when possible
- Configurable data retention policies
- Anonymous visitor tracking options
- GDPR compliance features

## 🤝 Development

### Copilot Chat House Style
This project includes custom Copilot Chat snippets for consistent development:
- `cop-general` - General requests with structured responses
- `cop-code` - Code examples with minimal, runnable output
- `cop-review` - Code review with concrete issues and fixes
- `cop-decision` - Decision requests with pros/cons analysis

### Contributing
1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

## 🐛 Troubleshooting

### Common Issues
- **MQTT Connection**: Verify broker IP and port 1883
- **Camera Errors**: Check ESP32-CAM power supply (minimum 2A)
- **Upload Failures**: Ensure server URL is accessible from hardware
- **Face Recognition**: Verify remote database API configuration

### Support
- Check `hardware/README.md` for hardware-specific issues
- Review `robots/README.md` for deployment troubleshooting
- Monitor MQTT topics for real-time debugging
- Check server logs for API errors

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **ESP32-CAM Community** for hardware examples and libraries
- **Mosquitto Project** for reliable MQTT broker
- **Node.js Ecosystem** for excellent IoT integration tools
- **VS Code Copilot** for development assistance and house style integration

## 📈 Roadmap

### Planned Features
- [ ] Advanced facial recognition with cloud APIs
- [ ] Voice recognition and natural language greetings
- [ ] Mobile app for remote monitoring
- [ ] Integration with access control systems
- [ ] AI-powered visitor analytics and insights
- [ ] Multi-language greeting support

### Hardware Expansion
- [ ] Gesture recognition sensors
- [ ] Temperature and occupancy monitoring
- [ ] LCD displays for visual greetings
- [ ] 3D printed enclosures and mounting systems

---

**Built with ❤️ by the Robot Greeter Team**
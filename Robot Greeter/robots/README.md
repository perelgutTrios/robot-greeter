# Robot Greeter Fleet Management

## Overview
This directory contains configuration and deployment information for individual robot units in the Robot Greeter fleet.

## Directory Structure
```
robots/
├── robot_001/
│   ├── config.json          # Robot configuration
│   ├── logs/               # Local log files (if any)
│   └── calibration/        # Sensor calibration data
├── robot_002/
│   ├── config.json
│   ├── logs/
│   └── calibration/
└── README.md               # This file
```

## Robot Deployment Guide

### 1. Physical Installation
1. **Mount hardware** securely at eye level (1.5-1.8m height)
2. **Position camera** to capture face-level images
3. **Adjust PIR sensor** for optimal motion detection range
4. **Connect power supply** with adequate current capacity
5. **Test connectivity** to WiFi network

### 2. Configuration
1. **Copy base configuration** from template
2. **Update robot-specific settings** (ID, location, network)
3. **Upload configuration** to ESP32-CAM via Arduino IDE
4. **Test MQTT connectivity** and server communication
5. **Calibrate sensors** for environment

### 3. Registration
1. **Add robot to central database** via admin interface
2. **Verify MQTT topic subscriptions** are working
3. **Test image upload** and face recognition pipeline
4. **Configure greeting preferences** for location
5. **Set maintenance schedule** and monitoring alerts

## Configuration Parameters

### Required Settings
- `robotId`: Unique identifier (format: robot_XXX)
- `name`: Human-readable robot name
- `location`: Physical deployment location details
- `hardware`: Hardware specification and components
- `network`: WiFi and MQTT connection settings

### Optional Settings
- `configuration`: Runtime behavior parameters
- `deployment`: Installation and maintenance tracking
- `purpose`: Description of robot's role and objectives
- `active`: Enable/disable robot operation

## Network Requirements

### WiFi Configuration
- **SSID**: Office network name
- **Security**: WPA2-PSK or WPA3 recommended
- **Bandwidth**: Minimum 2 Mbps upload for image transmission
- **Range**: Signal strength > -70 dBm at robot location

### MQTT Broker Access
- **Host**: Central server IP address
- **Port**: 1883 (standard) or 8883 (TLS)
- **Authentication**: Client certificates for production
- **Topics**: Robot-specific topic namespace

## Maintenance Schedule

### Daily Checks (Automated)
- Network connectivity status
- Image capture functionality
- Motion sensor responsiveness
- Memory and storage usage

### Weekly Checks (Manual)
- Clean camera lens
- Verify mounting security
- Check power connections
- Review error logs

### Monthly Maintenance
- Sensor calibration verification
- Firmware update check
- Physical inspection
- Performance analytics review

## Fleet Status Dashboard

Access the fleet management interface at:
- **Web Interface**: `http://localhost:3000/fleet-management.html`
- **API Endpoint**: `http://localhost:3000/api/robots`
- **MQTT Monitor**: Subscribe to `robot/+/status` for real-time updates

## Troubleshooting

### Common Issues
1. **Robot not appearing online**
   - Check WiFi connection and credentials
   - Verify MQTT broker connectivity
   - Review robot logs for error messages

2. **Images not uploading**
   - Test HTTP endpoint connectivity
   - Check camera module initialization
   - Verify sufficient memory available

3. **False motion detection**
   - Adjust PIR sensor sensitivity
   - Increase motion cooldown period
   - Check for environmental interference

### Support Contacts
- **Technical Support**: admin@robotgreeter.local
- **Hardware Issues**: hardware@robotgreeter.local
- **Network Problems**: network@robotgreeter.local

## Deployment Checklist

### Pre-Installation
- [ ] Hardware components verified
- [ ] Network access confirmed
- [ ] Mounting location selected
- [ ] Power supply installed
- [ ] Configuration prepared

### Installation
- [ ] Hardware mounted securely
- [ ] Camera positioned correctly
- [ ] Sensors calibrated
- [ ] Power connected and tested
- [ ] Network connectivity verified

### Post-Installation
- [ ] Robot registered in system
- [ ] MQTT communication tested
- [ ] Image upload verified
- [ ] Greeting functionality tested
- [ ] Monitoring alerts configured

### Go-Live
- [ ] Documentation updated
- [ ] Staff training completed
- [ ] Support contacts established
- [ ] Maintenance schedule activated
- [ ] Performance baseline recorded
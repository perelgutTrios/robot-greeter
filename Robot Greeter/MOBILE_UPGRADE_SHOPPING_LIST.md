# Mobile Robot Upgrade Shopping List - Toronto Sources

## 🚀 **Phase 2: Mobile Platform Upgrade**

*This list assumes you've completed the stationary Robot Greeter from SHOPPING_LIST_TORONTO.md*

### 🎯 **Mobile Platform Core Components**

| Item | Quantity | Est. Price (CAD) | Best Local Source | Online Alternative | Notes |
|------|----------|------------------|-------------------|-------------------|-------|
| **4WD Robot Chassis Kit** | 1 | $60-90 | Canada Robotix, Robotshop.ca | Amazon.ca | Aluminum frame preferred |
| **DC Gear Motors (12V, 100RPM)** | 4 | $15-25 each | Active Electronics, Sayal | Amazon.ca | Usually included in chassis |
| **Motor Driver (L298N Dual)** | 2 | $12-18 each | Sayal Electronics, Canada Robotix | Amazon.ca | Controls 4 motors |
| **Arduino Uno R3** | 1 | $20-30 | Sayal Electronics, Canada Robotix | Amazon.ca | Dedicated motor controller |
| **Ultrasonic Sensor (HC-SR04)** | 2-3 | $8-12 each | Sayal Electronics, Canada Robotix | Amazon.ca | Front + side obstacle detection |

### 🔋 **Mobile Power System**

| Item | Quantity | Est. Price (CAD) | Best Local Source | Online Alternative | Notes |
|------|----------|------------------|-------------------|-------------------|-------|
| **18650 Li-ion Batteries** | 4-6 | $8-12 each | Vape shops, Battery Plus | Amazon.ca | Get protected cells |
| **18650 Battery Holder (2S2P)** | 1 | $10-15 | Active Electronics, Sayal | Amazon.ca | 7.4V configuration |
| **Li-ion Charging Module (TP4056)** | 2 | $8-12 each | Sayal Electronics | AliExpress | With protection circuit |
| **Buck Converter (12V to 5V, 3A)** | 1 | $10-15 | Active Electronics, Sayal | Amazon.ca | For ESP32-CAM power |
| **Power Distribution Board** | 1 | $12-18 | Canada Robotix, Sayal | Amazon.ca | Clean power routing |
| **Voltage Monitor/Display** | 1 | $8-12 | Sayal Electronics | Amazon.ca | Battery level indication |

### 🎮 **Movement & Control**

| Item | Quantity | Est. Price (CAD) | Best Local Source | Online Alternative | Notes |
|------|----------|------------------|-------------------|-------------------|-------|
| **Servo Motor (SG90)** | 2 | $8-12 each | Canada Robotix, Sayal | Amazon.ca | Camera pan/tilt mechanism |
| **Pan/Tilt Bracket** | 1 | $15-25 | Canada Robotix, 3D printing | Amazon.ca | For ESP32-CAM mounting |
| **Encoder Wheels** | 4 | $5-8 each | Canada Robotix | Amazon.ca | Position tracking (optional) |
| **Caster Wheels** | 2 | $8-12 each | Canadian Tire, Home Depot | Amazon.ca | Support wheels if needed |

### 🧠 **Enhanced Sensing**

| Item | Quantity | Est. Price (CAD) | Best Local Source | Online Alternative | Notes |
|------|----------|------------------|-------------------|-------------------|-------|
| **MPU6050 Gyroscope** | 1 | $8-15 | Sayal Electronics, Canada Robotix | Amazon.ca | Balance and orientation |
| **IR Distance Sensors** | 2-4 | $12-18 each | Active Electronics, Canada Robotix | Amazon.ca | Cliff detection |
| **Bumper Switches** | 4 | $3-5 each | Sayal Electronics | Amazon.ca | Physical collision detection |
| **RGB LED Strip (WS2812)** | 1m | $15-25 | Sayal Electronics, Canada Robotix | Amazon.ca | Status indication |

### 🔧 **Assembly & Integration**

| Item | Quantity | Est. Price (CAD) | Best Local Source | Online Alternative | Notes |
|------|----------|------------------|-------------------|-------------------|-------|
| **Acrylic Sheets (3mm)** | 2-3 | $10-15 each | Tap Plastics, Home Depot | Amazon.ca | Custom mounting plates |
| **M3 Standoffs/Screws Kit** | 1 | $15-25 | Sayal Electronics, Active | Amazon.ca | Extended hardware kit |
| **Cable Management Clips** | 1 pack | $8-12 | Canada Computers, Staples | Amazon.ca | Clean wire routing |
| **Velcro Strips** | 1 pack | $5-8 | Any dollar store | Amazon.ca | Battery/component mounting |
| **Protective Bumpers** | 1 set | $10-15 | Canadian Tire, Princess Auto | Amazon.ca | Foam padding for chassis |

## 🏪 **Additional Toronto Sources for Mobile Components**

### **🔋 Battery Specialists**

#### **Battery Plus** ⭐⭐⭐⭐⭐
- **Multiple GTA locations**
- **Specialty**: High-quality Li-ion batteries
- **Best For**: Protected 18650 cells, charging accessories
- **Rating**: 4.6/5 stars

#### **Vape Shops** ⭐⭐⭐⭐
- **Abundant in Toronto**
- **Best For**: Quality 18650 batteries (they know good cells)
- **Advantages**: Often cheaper than electronics stores
- **Check**: Battery ratings and protection circuits

### **🏗️ Manufacturing & Fabrication**

#### **Tap Plastics** ⭐⭐⭐⭐⭐
- **Address**: 1181 Dufferin St, Toronto
- **Specialty**: Acrylic cutting, custom plastic work
- **Best For**: Custom mounting plates, chassis modifications
- **Rating**: 4.7/5 stars

#### **3D Printing Services**
- **TorontoMake**: Custom brackets and mounts
- **UofT Libraries**: Student access to 3D printers
- **Hackerspaces**: Site3 coLaboratory, Toronto Tool Library

### **🤖 Robotics Specialists**

#### **RobotShop.ca** ⭐⭐⭐⭐⭐
- **Location**: Online + Montreal (ships to Toronto)
- **Specialty**: Complete robotics kits and components
- **Best For**: Chassis kits, sensors, actuators
- **Rating**: 4.8/5 stars
- **Advantage**: Robotics-focused, excellent technical support

## 💰 **Mobile Upgrade Budget Breakdown**

### **Basic Mobile Platform**: $200-300 CAD
- Simple 4WD chassis + motor control
- Basic battery system
- Obstacle avoidance

### **Advanced Mobile Platform**: $350-500 CAD
- Enhanced chassis with encoders
- Professional battery management
- Camera pan/tilt + multiple sensors

### **Professional Mobile Platform**: $500-750 CAD
- Precision navigation components
- Extended battery life
- Advanced sensor fusion

## 🛠️ **Integration Strategy**

### **Phase 2A: Power Independence** ($100-150 CAD)
1. Add battery pack to existing stationary robot
2. Test wireless operation and battery life
3. Develop power management software

### **Phase 2B: Basic Mobility** ($150-250 CAD)
1. Install chassis and motor control
2. Implement basic movement commands via MQTT
3. Add obstacle avoidance

### **Phase 2C: Advanced Navigation** ($100-200 CAD)
1. Add encoders and IMU for precise movement
2. Implement SLAM (Simultaneous Localization and Mapping)
3. Camera tracking and following behaviors

## 🔌 **Power System Architecture**

```
Battery Pack (7.4V, 5000mAh)
├── Charging Port (USB-C/DC Jack)
├── Main Switch + Fuse
├── Buck Converter → 5V/3A → ESP32-CAM + Arduino
├── Motor Driver → 7.4V Direct → Wheel Motors
└── Voltage Monitor → Battery Status Display
```

## 📡 **Communication Architecture**

```
Central Server (Node.js + MQTT)
    ↕ WiFi/MQTT
ESP32-CAM (Camera + Motion Detection)
    ↕ Serial/I2C
Arduino Uno (Motor Control + Sensors)
    ↕ PWM/Digital
Motors + Sensors + Servos
```

## 🎯 **Mobile-Specific Software Features**

### **New MQTT Topics**
- `robot/{ROBOT_ID}/move` - Movement commands (forward, turn, etc.)
- `robot/{ROBOT_ID}/navigate` - High-level navigation goals
- `robot/{ROBOT_ID}/obstacles` - Sensor data reporting
- `robot/{ROBOT_ID}/battery` - Power status updates

### **Enhanced Behaviors**
- **Visitor Following**: Track detected person with camera pan/tilt
- **Patrol Mode**: Autonomous area coverage
- **Return to Base**: Low battery automatic charging
- **Multi-Robot Coordination**: Fleet management via MQTT

## 🛒 **Shopping Strategy for Mobile Upgrade**

### **Step 1: Start with Power** (Visit Battery Plus)
- Quality 18650 batteries and charging system
- Test mobile power before adding motors

### **Step 2: Basic Chassis** (Visit Canada Robotix)
- 4WD chassis kit with motors
- Motor drivers and Arduino controller

### **Step 3: Sensors** (Visit Sayal Electronics)
- Ultrasonic sensors and collision detection
- IMU and positioning components

### **Step 4: Custom Parts** (Visit Tap Plastics)
- Custom mounting plates
- Protective housing modifications

## 📋 **Mobile Upgrade Checklist**

- [ ] Current stationary robot fully functional
- [ ] MQTT communication tested and stable
- [ ] Power requirements calculated (expect 5-10W total)
- [ ] Chassis dimensions fit through doorways
- [ ] Weight distribution planned (camera + battery placement)
- [ ] Software architecture designed for dual-processor
- [ ] Safety features planned (emergency stop, collision avoidance)

## 🚨 **Safety Considerations for Mobile Robot**

### **Physical Safety**
- Emergency stop button accessible
- Soft bumpers on all edges
- Maximum speed limits in software
- Timeout for all movement commands

### **Electrical Safety**
- Fused power distribution
- Over-discharge protection for batteries
- Isolated motor power from logic power

## 📞 **Mobile-Specific Toronto Contacts**

- **RobotShop.ca**: 1-866-ROBOTSHOP (technical support)
- **Battery Plus**: Multiple locations, call for 18650 availability
- **Tap Plastics**: (416) 787-7273 (custom acrylic work)
- **Site3 coLaboratory**: (416) 516-0662 (3D printing + workspace)

**Total Mobile Upgrade Cost: $200-750 CAD depending on complexity level**

*This shopping list builds on your existing stationary robot investment and provides a clear path to full mobility!*
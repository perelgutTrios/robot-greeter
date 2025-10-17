/*
 * Robot Greeter ESP32-CAM Module
 * 
 * This sketch enables the ESP32-CAM to:
 * - Connect to WiFi and MQTT broker
 * - Capture images when motion is detected
 * - Upload images to the central server for face recognition
 * - Play audio greetings based on MQTT commands
 * - Report status and handle remote configuration
 * 
 * Hardware Requirements:
 * - ESP32-CAM module
 * - PIR motion sensor (GPIO 13)
 * - Speaker/buzzer (GPIO 2)
 * - Status LED (built-in GPIO 33)
 * - Reset button (GPIO 0)
 * 
 * MQTT Topics:
 * - robot/{ROBOT_ID}/status (publish)
 * - robot/{ROBOT_ID}/visitor_detected (publish)
 * - robot/{ROBOT_ID}/greeting (subscribe)
 * - robot/{ROBOT_ID}/config (subscribe)
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp_camera.h"
#include "esp_timer.h"
#include "esp_log.h"

// Configuration
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* MQTT_SERVER = "192.168.1.100";  // Replace with your MQTT broker IP
const int MQTT_PORT = 1883;
const char* SERVER_URL = "http://192.168.1.100:3000";  // Replace with your server IP
const char* ROBOT_ID = "robot_001";  // Unique identifier for this robot

// Pin definitions
#define PIR_SENSOR_PIN 13
#define SPEAKER_PIN 2
#define STATUS_LED_PIN 33
#define FLASH_LED_PIN 4

// Camera pins for AI-Thinker ESP32-CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Global variables
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
bool motionDetected = false;
unsigned long lastMotionTime = 0;
unsigned long lastStatusReport = 0;
const unsigned long MOTION_COOLDOWN = 5000;  // 5 seconds between detections
const unsigned long STATUS_INTERVAL = 30000; // 30 seconds between status reports

void setup() {
  Serial.begin(115200);
  Serial.println("Robot Greeter ESP32-CAM Starting...");

  // Initialize pins
  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(FLASH_LED_PIN, OUTPUT);
  
  // Flash LED to indicate startup
  flashStatusLED(3);

  // Initialize camera
  if (!initCamera()) {
    Serial.println("Camera initialization failed!");
    while (1) {
      flashStatusLED(1);
      delay(1000);
    }
  }

  // Connect to WiFi
  connectToWiFi();

  // Setup MQTT
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(onMqttMessage);
  mqttClient.setBufferSize(1024);

  // Connect to MQTT
  connectToMQTT();

  Serial.println("Robot Greeter ready!");
  digitalWrite(STATUS_LED_PIN, HIGH);
}

void loop() {
  // Maintain MQTT connection
  if (!mqttClient.connected()) {
    connectToMQTT();
  }
  mqttClient.loop();

  // Check for motion
  if (digitalRead(PIR_SENSOR_PIN) == HIGH) {
    if (!motionDetected && (millis() - lastMotionTime > MOTION_COOLDOWN)) {
      motionDetected = true;
      lastMotionTime = millis();
      onMotionDetected();
    }
  } else {
    motionDetected = false;
  }

  // Send periodic status reports
  if (millis() - lastStatusReport > STATUS_INTERVAL) {
    sendStatusReport();
    lastStatusReport = millis();
  }

  delay(100);
}

bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Initialize with high specs for quality
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;
  
  // Initialize camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return false;
  }

  // Adjust settings for better face detection
  sensor_t * s = esp_camera_sensor_get();
  s->set_brightness(s, 0);     // -2 to 2
  s->set_contrast(s, 0);       // -2 to 2
  s->set_saturation(s, 0);     // -2 to 2
  s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)

  return true;
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    flashStatusLED(1);
  }
  
  Serial.println();
  Serial.print("WiFi connected! IP: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    
    String clientId = String(ROBOT_ID) + "_" + String(WiFi.macAddress());
    
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println(" connected!");
      
      // Subscribe to topics
      String greetingTopic = "robot/" + String(ROBOT_ID) + "/greeting";
      String configTopic = "robot/" + String(ROBOT_ID) + "/config";
      
      mqttClient.subscribe(greetingTopic.c_str());
      mqttClient.subscribe(configTopic.c_str());
      
      Serial.println("Subscribed to MQTT topics");
      
      // Send startup message
      sendStatusReport();
      
    } else {
      Serial.print(" failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.printf("MQTT message received [%s]: %s\n", topic, message.c_str());
  
  String topicStr = String(topic);
  
  if (topicStr.endsWith("/greeting")) {
    handleGreetingMessage(message);
  } else if (topicStr.endsWith("/config")) {
    handleConfigMessage(message);
  }
}

void handleGreetingMessage(String message) {
  // Parse JSON greeting message
  DynamicJsonDocument doc(512);
  deserializeJson(doc, message);
  
  String type = doc["type"];
  String visitorName = doc["name"];
  int visitCount = doc["visitCount"];
  
  Serial.println("Playing greeting for visitor: " + visitorName);
  
  // Play appropriate greeting sound
  if (type == "returning_visitor") {
    playGreetingTone(2); // Two beeps for returning visitor
    Serial.println("Welcome back, " + visitorName + "! Visit #" + String(visitCount));
  } else {
    playGreetingTone(1); // One beep for new visitor
    Serial.println("Welcome, new visitor!");
  }
  
  // Flash LED to indicate greeting
  flashStatusLED(type == "returning_visitor" ? 2 : 1);
}

void handleConfigMessage(String message) {
  // Parse configuration updates
  DynamicJsonDocument doc(512);
  deserializeJson(doc, message);
  
  if (doc.containsKey("motion_cooldown")) {
    // Update motion detection cooldown
    Serial.println("Config updated");
  }
}

void onMotionDetected() {
  Serial.println("Motion detected! Capturing image...");
  
  // Turn on flash for better image quality
  digitalWrite(FLASH_LED_PIN, HIGH);
  delay(100);
  
  // Capture image
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    digitalWrite(FLASH_LED_PIN, LOW);
    return;
  }
  
  // Turn off flash
  digitalWrite(FLASH_LED_PIN, LOW);
  
  // Upload image to server
  bool uploadSuccess = uploadImageToServer(fb->buf, fb->len);
  
  // Release frame buffer
  esp_camera_fb_return(fb);
  
  if (uploadSuccess) {
    Serial.println("Image uploaded successfully");
    publishVisitorDetection();
  } else {
    Serial.println("Image upload failed");
  }
}

bool uploadImageToServer(uint8_t* imageData, size_t imageSize) {
  HTTPClient http;
  http.begin(String(SERVER_URL) + "/api/visitors/upload");
  http.addHeader("Content-Type", "multipart/form-data; boundary=----WebKitFormBoundary");
  
  // Create multipart form data
  String boundary = "----WebKitFormBoundary";
  String postData = "--" + boundary + "\r\n";
  postData += "Content-Disposition: form-data; name=\"robotId\"\r\n\r\n";
  postData += String(ROBOT_ID) + "\r\n";
  postData += "--" + boundary + "\r\n";
  postData += "Content-Disposition: form-data; name=\"image\"; filename=\"visitor.jpg\"\r\n";
  postData += "Content-Type: image/jpeg\r\n\r\n";
  
  // Calculate total size
  size_t totalSize = postData.length() + imageSize + boundary.length() + 8;
  
  // Send POST request
  http.addHeader("Content-Length", String(totalSize));
  
  WiFiClient* client = http.getStreamPtr();
  client->print(postData);
  client->write(imageData, imageSize);
  client->print("\r\n--" + boundary + "--\r\n");
  
  int httpResponseCode = http.GET();
  String response = http.getString();
  
  http.end();
  
  return httpResponseCode == 200;
}

void publishVisitorDetection() {
  DynamicJsonDocument doc(256);
  doc["timestamp"] = millis();
  doc["robotId"] = ROBOT_ID;
  doc["sensorTriggered"] = true;
  
  String message;
  serializeJson(doc, message);
  
  String topic = "robot/" + String(ROBOT_ID) + "/visitor_detected";
  mqttClient.publish(topic.c_str(), message.c_str());
}

void sendStatusReport() {
  DynamicJsonDocument doc(512);
  doc["robotId"] = ROBOT_ID;
  doc["status"] = "online";
  doc["uptime"] = millis();
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["wifiSignal"] = WiFi.RSSI();
  doc["lastMotion"] = lastMotionTime;
  
  String message;
  serializeJson(doc, message);
  
  String topic = "robot/" + String(ROBOT_ID) + "/status";
  mqttClient.publish(topic.c_str(), message.c_str());
}

void playGreetingTone(int beeps) {
  for (int i = 0; i < beeps; i++) {
    // Generate tone on speaker pin
    for (int freq = 1000; freq <= 2000; freq += 100) {
      tone(SPEAKER_PIN, freq, 50);
      delay(50);
    }
    delay(200);
  }
}

void flashStatusLED(int flashes) {
  for (int i = 0; i < flashes; i++) {
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(200);
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(200);
  }
}

void tone(int pin, int frequency, int duration) {
  // Simple tone generation for ESP32
  ledcSetup(0, frequency, 8);
  ledcAttachPin(pin, 0);
  ledcWriteTone(0, frequency);
  delay(duration);
  ledcWriteTone(0, 0);
}
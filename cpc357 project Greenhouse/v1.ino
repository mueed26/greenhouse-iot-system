
/*
  Project: Greenhouse Regulation (Multi-Device Version)
  Hardware: ESP32 S3 Maker Feather
  Devices: 4 Separate V-ONE Device IDs
*/

#include "VOneMqttClient.h"
#include "DHT.h"
#include <ESP32Servo.h>
#include <WiFi.h>

// ==========================================
// 1. CONFIGURATION
// ==========================================

// --- DEVICE IDs (Separated) ---
const char* dht_DeviceId      = "5b078c0e-55a8-4839-a289-9977bc3a6591";
const char* mq2_DeviceId      = "3306bb05-30c8-41f3-9006-64f9f5a68b4d";
const char* moisture_DeviceId = "47f86b07-738f-4fe1-becb-0422c72c6d55";
const char* servo_DeviceId    = "7acbc685-79c5-45cc-8997-0e37fb0234e8";

// --- PIN DEFINITIONS ---
const int dhtPin = 41;       
const int mq2Pin = 4;        
const int moisturePin = 5;   
const int servoPin = 40;     //using 40

// --- SERVO ANGLES ---
const int VENT_CLOSED = 90;
const int VENT_OPEN = 180;

// --- THRESHOLDS ---
const int TEMP_THRESHOLD = 30;     
const int GAS_THRESHOLD = 1500;    
const int HUM_HIGH_THRESHOLD = 80; 

// --- OBJECT INITIALIZATION ---
#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);
VOneMqttClient voneClient;
Servo ventServo;

// Variables
unsigned long lastMsgTime = 0;
const unsigned long msgInterval = 2000; 

// ==========================================
// 2. SETUP
// ==========================================
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID); 

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void setup() {
  Serial.begin(115200);
  
  setup_wifi();       
  voneClient.setup();
  dht.begin();        
  
  pinMode(mq2Pin, INPUT);
  pinMode(moisturePin, INPUT);

  ventServo.setPeriodHertz(50);    
  ventServo.attach(servoPin, 500, 2400); 
  ventServo.write(VENT_CLOSED);    

  Serial.println("Greenhouse System Initialized .");
}

// ==========================================
// 3. MAIN LOOP
// ==========================================
void loop() {
  // 1. Cloud Connection (Check any one device to ensure connection)
  if (!voneClient.connected()) {
    voneClient.reconnect();
    voneClient.publishDeviceStatusEvent(dht_DeviceId, true); 
  }
  voneClient.loop(); 

  // 2. Main Logic Timer
  unsigned long cur = millis();
  if (cur - lastMsgTime > msgInterval) {
    lastMsgTime = cur;

    // --- A. READ SENSORS ---
    float h = dht.readHumidity();
    int t = dht.readTemperature();
    int gasValue = analogRead(mq2Pin);
    int moistureRaw = analogRead(moisturePin);

    // Convert Moisture
    int moisturePercent = map(moistureRaw, 4095, 0, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100); 

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return; 
    }

    // --- B. REGULATION LOGIC ---
    String statusMsg = "Stable";
    bool needVentilation = false;

    if (gasValue > GAS_THRESHOLD) {
      statusMsg = "DANGER: High CO2/Gas";
      needVentilation = true;
    }
    else if (t > TEMP_THRESHOLD) {
      statusMsg = "Regulating: High Temp";
      needVentilation = true;
    }
    else if (h > HUM_HIGH_THRESHOLD) {
      statusMsg = "Regulating: High Humidity";
      needVentilation = true; 
    }
    else {
      statusMsg = "Normal";
      needVentilation = false;
    }

    // --- C. CONTROL SERVO ---
    if (needVentilation) {
      ventServo.write(VENT_OPEN);   
    } else {
      ventServo.write(VENT_CLOSED); 
    }

    // --- D. UPLOAD TO 4 SEPARATE DEVICES ---
    // Note: This will print 4 separate debug logs in the terminal. This is normal.
    
    // 1. Upload DHT Data
    JSONVar dhtPayload;
    dhtPayload["Humidity"] = h;
    dhtPayload["Temperature"] = t;
    dhtPayload["System_Status"] = statusMsg; // <--- The Alert Message is via dht
    voneClient.publishTelemetryData(dht_DeviceId, dhtPayload);

    // 2. Upload MQ2 Data
    JSONVar mq2Payload;
    mq2Payload["CO2_Level"] = gasValue; 
    voneClient.publishTelemetryData(mq2_DeviceId, mq2Payload);

    // 3. Upload Moisture Data
    JSONVar moisturePayload;
    moisturePayload["Soil_Moisture"] = moisturePercent;
    voneClient.publishTelemetryData(moisture_DeviceId, moisturePayload);

    // 4. Upload Servo/System Status
    JSONVar servoPayload;
    servoPayload["Vent_Status"] = needVentilation ? "OPEN" : "CLOSED";
    // servoPayload["System_Status"] = statusMsg;
    voneClient.publishTelemetryData(servo_DeviceId, servoPayload);

    // --- E. CONSOLIDATED TERMINAL OUTPUT (Look here for values) ---
    Serial.println("\n------------------------------------------------");
    Serial.println("--- GREENHOUSE LIVE STATUS ---");
    Serial.print("Status : "); Serial.println(statusMsg);
    Serial.print("Vent   : "); Serial.println(needVentilation ? "OPEN (180)" : "CLOSED (90)");
    Serial.print("Temp   : "); Serial.print(t); Serial.println(" °C");
    Serial.print("Hum    : "); Serial.print(h); Serial.println(" %");
    Serial.print("Gas    : "); Serial.print(gasValue); Serial.println(" PPM");
    Serial.print("Moist  : "); Serial.print(moisturePercent); Serial.println(" %");
    Serial.println("------------------------------------------------\n");
  }
}
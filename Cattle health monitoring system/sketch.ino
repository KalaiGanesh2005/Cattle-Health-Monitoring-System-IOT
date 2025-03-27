#define BLYNK_AUTH_TOKEN "GGiaobnmFhfXKzeP-vBorMYmMrXcwLGT"
#define BLYNK_TEMPLATE_ID "TMPL3okV-M_VX"
#define BLYNK_TEMPLATE_NAME "cattle monitor"

#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h> // Include Blynk library for ESP32

#define DHTPIN 12
#define HR_PIN 32 // Heart Rate Potentiometer
#define SPO2_PIN 33 // SpO2 Potentiometer
#define PIR_PIN 25

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char auth[] = "GGiaobnmFhfXKzeP-vBorMYmMrXcwLGT";
const char *ssid = "Wokwi-GUEST";
const char *password = "";

BlynkTimer timer;

const int RS = 4, E = 15, DB4 = 5, DB5 = 18, DB6 = 19, DB7 = 21;
LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);

// Threshold values for abnormal states
const float TEMPERATURE_THRESHOLD = 40.0; // Celsius
const float HUMIDITY_THRESHOLD = 80.0;    // Percentage
const int HEART_RATE_THRESHOLD = 120;     // bpm
const int SPO2_THRESHOLD = 90;            // Percentage


void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int pirStatus = digitalRead(PIR_PIN);
  int heartRate = map(analogRead(HR_PIN), 0, 4095, 50, 150); // Assuming the heart rate range is from 50 to 150 bpm
  int spo2 = map(analogRead(SPO2_PIN), 0, 4095, 70, 100);

  // Send sensor data to Blynk app
  Blynk.virtualWrite(V5, temperature);
  Blynk.virtualWrite(V6, humidity);
  Blynk.virtualWrite(V7, pirStatus);
  Blynk.virtualWrite(V8, heartRate);
  Blynk.virtualWrite(V9, spo2);

  // Check for abnormal states and trigger alert
  if (temperature > TEMPERATURE_THRESHOLD || humidity > HUMIDITY_THRESHOLD || 
      heartRate > HEART_RATE_THRESHOLD || spo2 < SPO2_THRESHOLD) {
    triggerAlert(temperature, humidity, heartRate, spo2, pirStatus);
  } else {
    displayNormalCondition(temperature, humidity, heartRate, spo2, pirStatus);
  }
}

void triggerAlert(float temp, float hum, int hr, int spo2, int pir) {
  // Print alert and sensor data to Serial Monitor
  Serial.println("Cattle in abnormal state! Alert!");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println("%");
  Serial.print("Heart Rate: ");
  Serial.println(hr);
  Serial.print("SpO2: ");
  Serial.println(spo2);
  Serial.print("PIR Status: ");
  Serial.println(pir);

  // Display alert on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ALERT!");
  lcd.setCursor(0, 1);
  lcd.print("Check the cattle!");
}

void displayNormalCondition(float temp, float hum, int hr, int spo2, int pir) {
  // Print normal condition sensor data to Serial Monitor
  Serial.println("Cattle in normal state.");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println("%");
  Serial.print("Heart Rate: ");
  Serial.println(hr);
  Serial.print("SpO2: ");
  Serial.println(spo2);
  Serial.print("PIR Status: ");
  Serial.println(pir);

  // Display normal condition on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C H:");
  lcd.print(hum);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("HR:");
  lcd.print(hr);
  lcd.print("SpO2:");
  lcd.print(spo2);
  lcd.print("%");
  lcd.print("P:");
  lcd.print(pir);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.begin(16, 2);

  pinMode(HR_PIN, INPUT);
  pinMode(SPO2_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);

  connectWiFi(); // Connect to WiFi and start Blynk
}

void loop() {
  Blynk.run();
  timer.run();
}

void connectWiFi() {
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
  Blynk.config(BLYNK_AUTH_TOKEN);
  timer.setInterval(2000L, sendSensorData);
}
// Read sensor data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int pirStatus = digitalRead(PIR_PIN);
  int heartRate = map(analogRead(HR_PIN), 0, 4095, 50, 150); // Assuming the heart rate range is from 50 to 150 bpm
  int spo2 = map(analogRead(SPO2_PIN), 0, 4095, 70, 100);

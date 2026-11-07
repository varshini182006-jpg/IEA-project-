/*
  Smart Industrial Cable Health Monitoring System
  Controller: ESP32
  IoT platform: Blynk
  Sensors: ZMPT101B voltage sensor, Hall-effect sensor,
           vibration sensor, DHT11 temperature/humidity sensor
  Output: Buzzer + Blynk dashboard

  SECURITY:
  Enter your own Blynk token and Wi-Fi credentials locally.
  Do not commit real credentials to a public repository.
*/

#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

#define ZMPT_PIN 34
#define HALL_PIN 35
#define VIB_PIN 27
#define DHT_PIN 4
#define BUZZER 23

#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);
BlynkTimer timer;

// Thresholds (calibrate these values for your actual sensors and setup)
int HALL_MIDDLE = 2000;
int HALL_FAULT = 3000;

float TEMP_MIDDLE = 30;
float TEMP_FAULT = 35;

float VOLT_MIN = 200;
float VOLT_MAX = 260;

void readSensors()
{
  int hallValue = analogRead(HALL_PIN);
  int vibration = digitalRead(VIB_PIN);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Basic ADC-based estimate only; calibrate against a true-RMS meter.
  int zmptValue = analogRead(ZMPT_PIN);
  float voltage = zmptValue * (3.3 / 4095.0) * 100;

  bool fault = false;
  bool middle = false;

  // Hall-effect sensor
  if (hallValue >= HALL_FAULT)
    fault = true;
  else if (hallValue >= HALL_MIDDLE)
    middle = true;

  // Vibration sensor
  if (vibration == HIGH)
    fault = true;

  // Temperature sensor
  if (!isnan(temperature)) {
    if (temperature >= TEMP_FAULT)
      fault = true;
    else if (temperature >= TEMP_MIDDLE)
      middle = true;
  }

  // Voltage sensor
  if (voltage < 150 || voltage > 280)
    fault = true;
  else if (voltage < VOLT_MIN || voltage > VOLT_MAX)
    middle = true;

  String status;
  if (fault) {
    status = "FAULT";
    digitalWrite(BUZZER, HIGH);
  } else if (middle) {
    status = "MIDDLE LEVEL";
    digitalWrite(BUZZER, LOW);
  } else {
    status = "NORMAL";
    digitalWrite(BUZZER, LOW);
  }

  Serial.println("---------------------------");
  Serial.print("Voltage     : "); Serial.print(voltage); Serial.println(" V");
  Serial.print("Hall Effect : "); Serial.println(hallValue);
  Serial.print("Vibration   : "); Serial.println(vibration);

  Serial.print("Temperature : ");
  if (isnan(temperature)) Serial.println("Sensor error");
  else { Serial.print(temperature); Serial.println(" C"); }

  Serial.print("Humidity    : ");
  if (isnan(humidity)) Serial.println("Sensor error");
  else { Serial.print(humidity); Serial.println(" %"); }

  Serial.print("STATUS      : "); Serial.println(status);

  // Blynk virtual pin mapping:
  // V0 = voltage, V1 = Hall value, V2 = temperature,
  // V3 = humidity, V4 = vibration, V5 = status
  Blynk.virtualWrite(V0, voltage);
  Blynk.virtualWrite(V1, hallValue);
  if (!isnan(temperature)) Blynk.virtualWrite(V2, temperature);
  if (!isnan(humidity)) Blynk.virtualWrite(V3, humidity);
  Blynk.virtualWrite(V4, vibration);
  Blynk.virtualWrite(V5, status);
}

void setup()
{
  Serial.begin(115200);

  pinMode(VIB_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  dht.begin();
  analogReadResolution(12);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, readSensors);
}

void loop()
{
  Blynk.run();
  timer.run();
}

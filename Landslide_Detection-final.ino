#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLEf1p5GXP"
#define BLYNK_DEVICE_NAME "Mountain A IOT"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>

#include "DHT.h"
#define DHTTYPE DHT11
#define dhtPin 13
DHT dht(dhtPin, DHTTYPE);

char auth[] = "UMrR6JMt4T5Rh8i9ooNxgsq0JI6vFWbE";
char ssid[] = "BSNL FIBER";
char pass[] = "Shubham@9527389878";

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float t;
float h;
int moistureValue;
int warning;

int raindropPin = 14;
int moisturePin = 15;
float latitude = 19.9579;
float longitude = 73.7787;

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.println("Wrote to IMU");
  Serial.println("Connecting to ");
  pinMode(2, OUTPUT);
  pinMode(raindropPin, INPUT);
  dht.begin();
  pinMode(dhtPin, INPUT);
  delay(2000);
}

void loop()
{
  Blynk.run();
 mpu_read();
  if (AcX < -11000)
  {
    Serial.println("Landslide");
    Blynk.virtualWrite(V8, 1);
  }
  else
  {
    Serial.println("Normal");
    Blynk.virtualWrite(V8, 0);
  }

  //------Raindrop sensor -----
  int raindropValue = digitalRead(raindropPin);
  Serial.print("Raindrop:");
  Serial.println(raindropValue);
  Blynk.virtualWrite(V3, raindropValue);

  //------ DHT11 sensor -----
  t = dht.readTemperature();
  h = dht.readHumidity();
  Serial.print("humidity = ");
  Serial.println(h);
  Serial.print("temperature = ");
  Serial.println(t);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);

  //------ Soil moisture -----
  moistureValue = analogRead(moisturePin);
  Serial.print("Soil moisture value:");
  Serial.println(moistureValue);
  Blynk.virtualWrite(V0, moistureValue);

  /// -----Location ----
  Blynk.virtualWrite(V9, latitude);
  Blynk.virtualWrite(V10, longitude);
}

void mpu_read()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  //  Blynk.virtualWrite(V0, AcX);
  //  Blynk.virtualWrite(V1, AcY);
  //  Blynk.virtualWrite(V2, AcZ);

  float Axnorm = float(AcX / 16384);
  float Aynorm = float(AcY / 16384);
  float Aznorm = float(AcZ / 16384);

  Serial.print("Accelerometer Values: \n");
  Serial.print("AcX: "); Serial.print(AcX); Serial.print("\nAcY: "); Serial.print(AcY); Serial.print("\nAcZ: "); Serial.print(AcZ);

  //  Serial.print("Accelerometer normalized Values: \n");
  //  Serial.print("AcX: "); Serial.print(Axnorm); Serial.print("\nAcY: "); Serial.print(Aynorm); Serial.print("\nAcZ: "); Serial.print(Aznorm);
  delay(50);
  Serial.println();
  Serial.println();

}

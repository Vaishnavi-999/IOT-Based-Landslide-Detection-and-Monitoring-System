#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLEf1p5GXP"
#define BLYNK_DEVICE_NAME "Mountain A IOT"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>

char auth[] = "UMrR6JMt4T5Rh8i9ooNxgsq0JI6vFWbE";
char ssid[] = "BSNL FIBER";
char pass[] = "Shubham@3527389878";

int buzzPin = 26;
int warning;

BLYNK_WRITE(V8)
{
  Blynk.syncVirtual(V8);
  warning = param.asInt(); // Read data of button from pin V2
  Serial.print("Landslide warning is: ");
  Serial.println(warning);
}

BLYNK_WRITE(V7)
{
  Blynk.syncVirtual(V7);
  int buzzValue = param.asInt(); // Read data of button from pin V2
  if (buzzValue==HIGH)
  {
    digitalWrite(buzzPin, HIGH);
    delay(2000);
    digitalWrite(buzzPin, LOW);
  }
  else
  {
    digitalWrite(buzzPin, LOW);
  }
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(buzzPin, OUTPUT);
}

void loop()
{
  Blynk.run();
  Serial.print("Landslide warning is: ");
  Serial.println(warning);
  if (warning == HIGH)
  {
    digitalWrite(buzzPin, HIGH);
    delay(2000);
    digitalWrite(buzzPin, LOW);
  }
  else
  {
    digitalWrite(buzzPin, LOW);
  }
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V8);
  Blynk.syncVirtual(V7);
}

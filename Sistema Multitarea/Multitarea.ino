#include "AsyncTaskLib.h"
#include "DHTStable.h"
#include <LiquidCrystal.h>

#define DHT11_PIN 5
#define PHOTOCELL_PIN A0

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

DHTStable DHT;

int sensorValue = 0;

void fotoSensor(){
  lcd.setCursor(0,1);
  lcd.print("Luz:");
  lcd.setCursor(4,1);
  lcd.print("                ");
  lcd.setCursor(4,1);
  sensorValue = analogRead(PHOTOCELL_PIN);
  lcd.print(sensorValue);
}

void dhtFuncion(){
  lcd.setCursor(0,0);
  DHT.read11(DHT11_PIN);
  lcd.print("Tem:");
  lcd.println(DHT.getTemperature());
  delay(1500);
  lcd.setCursor(0,0);
  lcd.print("Hum:");
  lcd.print(DHT.getHumidity());
}

AsyncTask asyncTask1(2000, true, dhtFuncion);
AsyncTask asyncTask2(1000, true, fotoSensor);

void setup()
{
  Serial.begin(115200);
  lcd.begin(16, 2);
  asyncTask1.Start();
  lcd.setCursor(0,1);
  asyncTask2.Start();
}
void loop()
{
  asyncTask1.Update();
  asyncTask2.Update();
}
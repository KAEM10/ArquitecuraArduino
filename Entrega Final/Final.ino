#include <LiquidCrystal.h>
#include <Keypad.h>
#include "AsyncTaskLib.h"
#include "DHTStable.h"

/* Display */
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

/* Keypad setup */
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2}; // R1 = 5, R2 = 4, R3 = 3, R4 = 2
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'.', '0', '=', '/'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

DHTStable DHT;

#define DHT11_PIN 6
#define PHOTOCELL_PIN A8

#define  LED_YELLOW A5
#define  LED_BLUE A6
#define  LED_RED A7

char password[] = "3333"; // Contraseña de cuatro dígitos
char clave[4];
int idx = 0;
int intentos = 0;
int sensorValue = 0;
bool flag = false;

// Función para mostrar la lectura del sensor de luz en el LCD
void fotoSensor() {
  lcd.setCursor(0, 1);
  lcd.print("Luz:");
  lcd.setCursor(4, 1);
  lcd.print("                ");
  lcd.setCursor(4, 1);
  sensorValue = analogRead(PHOTOCELL_PIN);
  lcd.print(sensorValue);
}

// Función para mostrar la lectura de temperatura y humedad en el LCD
void dhtFuncion() {
  lcd.setCursor(0, 0);
  DHT.read11(DHT11_PIN);
  lcd.print("Tem:");
  lcd.println(DHT.getTemperature());
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.print("Hum:");
  lcd.print(DHT.getHumidity());
}

// Función para gestionar la seguridad y el ingreso de la clave
void seguridad() {
  char key = keypad.getKey();

  if (key) {
    clave[idx++] = key;
    lcd.print("*");

    if (idx == 4) {
      if (clave[0] == password[0] && clave[1] == password[1] &&
          clave[2] == password[2] && clave[3] == password[3]) {

        delay(300);
        lcd.clear();
        lcd.setCursor(0, 0);
        digitalWrite(LED_BLUE, HIGH);
        lcd.print("CLAVE CORRECTA");
        delay(500);
        lcd.clear();
        flag = true;
      }
      else {
        delay(300);
        lcd.clear();
        lcd.setCursor(0, 0);
        digitalWrite(LED_YELLOW, HIGH);
        lcd.print("CLAVE INCORRECTA");
        intentos++;
        delay(500);
        lcd.clear();
        digitalWrite(LED_YELLOW, LOW);
        if (intentos < 3) {
          lcd.setCursor(0, 0);
          lcd.print("INGRESE CLAVE:");
          lcd.setCursor(0, 1);
        }
      }
      idx = 0;
    }
    if (intentos >= 3) { //Maximo de intentos superados
      delay(300);
      lcd.clear();
      lcd.setCursor(0, 0);
      digitalWrite(LED_RED, HIGH);
      lcd.print("SISTEMA");
      lcd.setCursor(0, 1);
      lcd.print("BLOQUEADO");
    }
  }
}

AsyncTask asyncTask1(1000, true, dhtFuncion); // Tarea asincronica para el dht
AsyncTask asyncTask2(1000, true, fotoSensor); // Tarea asincronica para el fotosensor
AsyncTask asyncTask3(0, true, seguridad); // Tarea asincronica para el sistema de seguridad

// Configuración inicial
void setup() {
  lcd.begin(16, 2);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, LOW);

  asyncTask1.Start(); // Iniciar tarea asíncrona 1
  asyncTask2.Start(); // Iniciar tarea asíncrona 2

  lcd.setCursor(0, 0);
  lcd.print("  BIENVENIDO!");
  lcd.setCursor(0, 1);
  lcd.print("    (^-^)");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INGRESE CLAVE: ");
  lcd.setCursor(0, 1);
  asyncTask3.Start(); // Iniciar tarea asíncrona 3
}

void loop() {
  asyncTask3.Update(); // Actualizar tarea asíncrona 3

  if (flag == true) {
    asyncTask1.Update(); // Actualizar tarea asíncrona 1
    asyncTask2.Update(); // Actualizar tarea asíncrona 2
  }
}

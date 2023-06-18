#include <LiquidCrystal.h>
#include <Keypad.h>

/* Display */
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

/* Keypad setup */
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2}; //R1 = 5, R2 = 4, R3 = 3. R4 = 2
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'.', '0', '=', '/'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);


#define  LED_YELLOW A5
#define  LED_GREEN A4
#define  LED_RED A6


char password[] = "3333";
char clave[4];
int idx = 0;
int intentos = 0;

void setup() {

  lcd.begin(16, 2);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  lcd.setCursor(0, 0);
  lcd.print("INGRESE CLAVE: ");
  lcd.setCursor(0, 1);
}

void loop() {

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
        digitalWrite(LED_GREEN, HIGH);
        lcd.print("CLAVE CORRECTA");
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
    if (intentos >= 3) {
      delay(300);
      lcd.clear();
      lcd.setCursor(0, 0);
      digitalWrite(LED_RED, HIGH);
      lcd.print("SISTEMA BLOQUEADO");
    }
  }
}
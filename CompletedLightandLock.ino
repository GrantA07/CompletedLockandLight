#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// ========================= LCD SETUP =========================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ========================= SERVO SETUP =========================
Servo lockServo;
const int servoPin = 11;

// ========================= BUTTON SETUP =========================
const int orderButton = 10;

// ========================= NEOPIXEL SETUP =========================
#define PIXEL_PIN 12
#define NUM_PIXELS 10   // change this to match how many LEDs you want to use

Adafruit_NeoPixel strip(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ========================= KEYPAD SETUP =========================
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ========================= VARIABLES =========================
String orderCode = "";
String enteredCode = "";
bool codeActive = false;

// ========================= NEOPIXEL FUNCTIONS =========================
void neoPixelOn(int r, int g, int b) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void neoPixelOff() {
  strip.clear();
  strip.show();
}

// ========================= SETUP FUNCTION =========================
void setup() {
  pinMode(orderButton, INPUT_PULLUP);

  lockServo.attach(servoPin);
  lockServo.write(0);   // locked position

  randomSeed(analogRead(A0));

  lcd.init();
  lcd.backlight();
  lcd.clear();

  strip.begin();
  strip.show();   // initialize NeoPixel strip
  neoPixelOff();  // keep off at startup

  showReadyScreen();
}

// ========================= MAIN LOOP =========================
void loop() {
  checkOrderButton();
  checkKeypad();
}

// ========================= READY SCREEN =========================
void showReadyScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Locker Ready");
  lcd.setCursor(0, 1);
  lcd.print("Press Button");
}

// ========================= ENTER CODE SCREEN =========================
void showEnterCodeScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Code:");
  lcd.setCursor(0, 1);
  lcd.print("> ");
}

// ========================= BUTTON CHECK =========================
void checkOrderButton() {
  if (digitalRead(orderButton) == LOW) {
    generateOrder();
    delay(400);
  }
}

// ========================= GENERATE ORDER =========================
void generateOrder() {
  int code = random(1000, 10000);
  orderCode = String(code);

  enteredCode = "";
  codeActive = true;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Order Ready");

  lcd.setCursor(0, 1);
  lcd.print("Code: ");
  lcd.print(orderCode);

  delay(3000);

  showEnterCodeScreen();
}

// ========================= KEYPAD CHECK =========================
void checkKeypad() {
  char key = keypad.getKey();

  if (!key) return;

  if (!codeActive) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Active Order");

    lcd.setCursor(0, 1);
    lcd.print("Press Button");

    delay(1500);
    showReadyScreen();
    return;
  }

  if (key >= '0' && key <= '9') {
    if (enteredCode.length() < 4) {
      enteredCode += key;

      lcd.setCursor(0, 1);
      lcd.print(">     ");
      lcd.setCursor(2, 1);

      for (int i = 0; i < enteredCode.length(); i++) {
        lcd.print("*");
      }
    }
  }
  else if (key == '*') {
    enteredCode = "";
    showEnterCodeScreen();
  }
  else if (key == '#') {
    checkCode();
  }
}

// ========================= CHECK CODE =========================
void checkCode() {
  if (enteredCode == orderCode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Granted");

    lcd.setCursor(0, 1);
    lcd.print("Unlocking...");

    // Turn NeoPixel ON when servo unlocks
    neoPixelOn(255, 255, 255);   // white light
    lockServo.write(90);

    delay(4000);

    // Turn NeoPixel OFF when servo locks again
    lockServo.write(0);
    neoPixelOff();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Locker Locked");

    lcd.setCursor(0, 1);
    lcd.print("Order Done");

    delay(2000);

    orderCode = "";
    enteredCode = "";
    codeActive = false;

    showReadyScreen();
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wrong Code");

    lcd.setCursor(0, 1);
    lcd.print("Try Again");

    delay(1500);

    enteredCode = "";
    showEnterCodeScreen();
  }
}
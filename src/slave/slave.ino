#include <SPI.h>
#include <LiquidCrystal.h>

#define MSG_SIZE 16
#define DEL_PIN 9
#define SEND_PIN 8
#define JS_X_PIN 0    // analog pin connected to X output
#define JS_Y_PIN 1    // analog pin connected to Y output
#define JS_MAX 900    // when to register a movement
#define JS_MIN 100

char selectedChar = 'A';
char msg[MSG_SIZE] = "               ";
int msgIndex = 0;

bool viewReceivedMsg = false;
char receivedMsg[MSG_SIZE];
int receivedMsgIndex = 0;

bool startSending = false;
int sendIndex = 0;

// LCD
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);

  pinMode(DEL_PIN, INPUT);
  pinMode(SEND_PIN, INPUT);

  lcd.begin(16, 2);
  lcd.cursor();

  pinMode(MISO, OUTPUT);                      // Send data to master IN
  SPCR |= _BV(SPE);                           // SPI Control Register (SPCR) is used to set SPI in Slave Mode
  SPI.attachInterrupt();                      // Enable interrupts for SPI communication

  Serial.println("Initialized.");
}

// This interrupt routine is called whenever data is recieved from the master
ISR(SPI_STC_vect) {
  if (SPDR != 1) {
    Serial.println("Received: " + SPDR);
    receivedMsg[receivedMsgIndex++] = SPDR;
    if (SPDR == '\0') {
      Serial.println("Stopped receiving");
      viewReceivedMsg = true;
    }
  }
}

void loop() {
  if (viewReceivedMsg) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Received Message:");
    lcd.setCursor(0, 1);
    lcd.print(receivedMsg);

    while (!sendBtnPressed());

    lcd.clear();
    lcd.print(msg);

    viewReceivedMsg = false;
    receivedMsgIndex = 0;
  } else {

    if (joystickUp() && selectedChar != 'Z') {
      msg[msgIndex] = ++selectedChar;
      lcd.print(selectedChar);
    }

    if (joystickDown() && selectedChar != 'A') {
      msg[msgIndex] = --selectedChar;
      lcd.print(selectedChar);
    }

    if (joystickLeft() && msgIndex != 0) {
      msgIndex--;
      if (msg[msgIndex] == ' ') {
        selectedChar = 'A';
      } else {
        selectedChar = msg[msgIndex];
      }
    }

    if (joystickRight() && msgIndex != 15) {
      msgIndex++;
      if (msg[msgIndex] == ' ') {
        selectedChar = 'A';
      } else {
        selectedChar = msg[msgIndex];
      }
    }

    if (delBtnPressed()) {
      msg[msgIndex] = ' ';
      selectedChar = 'A';
      lcd.print(' ');
    }

    if (sendBtnPressed()) {
      Serial.println("Started sending!");
      startSending = true;
      sendIndex = 0;
    }

    if (startSending && sendIndex < 16) {
      SPDR = msg[sendIndex++];
      Serial.println("Sending: " + SPDR);
    } else {
      startSending = false;
      Serial.println("Stopped sending");
    }
  }

  lcd.setCursor(msgIndex, 0);
}

bool joystickLeft() {
  static int prevState = 500;
  if (prevState < 550 && prevState > 450) {
    if (analogRead(JS_Y_PIN) < JS_MIN) {
      prevState = 1000;
      return true;
    }
  } else {
    prevState = analogRead(JS_Y_PIN);
  }

  return false;
}

bool joystickRight() {
  static int prevState = 500;
  if (prevState < 550 && prevState > 450) {
    if (analogRead(JS_Y_PIN) > JS_MAX) {
      prevState = 1000;
      return true;
    }
  } else {
    prevState = analogRead(JS_Y_PIN);
  }

  return false;
}

bool joystickUp() {
  static int prevState = 500;
  if (prevState < 550 && prevState > 450) {
    if (analogRead(JS_X_PIN) > JS_MAX) {
      prevState = 1000;
      return true;
    }
  } else {
    prevState = analogRead(JS_X_PIN);
  }

  return false;
}

bool joystickDown() {
  static int prevState = 500;
  if (prevState < 550 && prevState > 450) {
    if (analogRead(JS_X_PIN) < JS_MIN) {
      prevState = 1000;
      return true;
    }
  } else {
    prevState = analogRead(JS_X_PIN);
  }

  return false;
}

bool delBtnPressed() {
  static int prevState = 0;
  if (prevState == 0) {
    if (digitalRead(DEL_PIN)) {
      prevState = 1;
      return true;
    }
  } else {
    prevState = digitalRead(DEL_PIN);
  }
  return false;
}

bool sendBtnPressed() {
  static int prevState = 0;
  if (prevState == 0) {
    if (digitalRead(SEND_PIN)) {
      prevState = 1;
      return true;
    }
  } else {
    prevState = digitalRead(SEND_PIN);
  }

  return false;
}

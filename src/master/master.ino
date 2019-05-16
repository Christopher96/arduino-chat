#include <SPI.h>
#include <SH1106.h>

#define SELECT_LEFT_BTN_PIN 6
#define SELECT_RIGHT_BTN_PIN 5
#define ADD_CHAR_BTN_PIN 4
#define DELETE_CHAR_BTN_PIN 3
#define SEND_CHARS_BTN_PIN 2
#define MESSAGE_SIZE 16
#define BAUD_RATE 9600

SH1106 display;
char message[MESSAGE_SIZE];
char messageIndex = 0;

char selectedKey = 'A';

void setup() {
  Serial.begin(BAUD_RATE);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  digitalWrite(SS, HIGH);

  pinMode(SELECT_LEFT_BTN_PIN, INPUT);
  pinMode(SELECT_RIGHT_BTN_PIN, INPUT);
  pinMode(ADD_CHAR_BTN_PIN, INPUT);
  pinMode(DELETE_CHAR_BTN_PIN, INPUT);
  pinMode(SEND_CHARS_BTN_PIN, INPUT);

  display.begin();
}

void loop() {
  display.clear();

  updateKeyboard();

  display.drawString(message, 2, 64 - display.getStringWidth(message, 2) / 2, 4);

  if (selectLeftBtnPressed()) {
    if (selectedKey == 'A') {
      selectedKey = ']';
    } else {
      selectedKey--;
    }
  }
  else if (selectRightBtnPressed()) {
    if (selectedKey == ']') {
      selectedKey = 'A';
    } else {
      selectedKey++;
    }
  }
  else if (addCharBtnPressed()) {
    switch (selectedKey) {
      case '[':
        addCharToMessage(' ');
        break;
      case '\\':
        addCharToMessage('!');
        break;
      case ']':
        addCharToMessage('?');
        break;
      default:
        addCharToMessage(selectedKey);
        break;
    }
  }
  else if (deleteCharBtnPressed()) {
    deleteCharFromMessage();
  }
  else if (sendCharsBtnPressed()) {
    digitalWrite(SS, LOW);
  
    for(int i = 0; i < MESSAGE_SIZE; i++){
      SPI.transfer(message[i]);
      delayMicroseconds(20);
    }
    
    digitalWrite(SS, HIGH);

    clearMessage();
  }

  display.update();
}

void updateKeyboard() {
  display.drawHorizontalLine(0, 20, 127);

  if (selectedKey >= 'A' && selectedKey < 'L') {
    display.drawString("ABCDEFGHIJK", 6, selectedKey - 0x41, 5, 28);
  }
  else {
    display.drawString("ABCDEFGHIJK", 6, 5, 28);
  }

  if (selectedKey >= 'L' && selectedKey < 'W') {
    display.drawString("LMNOPQRSTUV", 6, selectedKey - 0x4C, 5, 40);
  }
  else {
    display.drawString("LMNOPQRSTUV", 6, 5, 40);
  }

  if (selectedKey >= 'W' && selectedKey <= ']') {
    int outlineIndex = selectedKey - 0x57;

    if (selectedKey == '[')
      outlineIndex = 4;
    if (selectedKey == '\\')
      outlineIndex = 5;
    if (selectedKey == ']')
      outlineIndex = 6;

    display.drawString("WXYZ !?", 6, outlineIndex, 5, 52);
  } else {
    display.drawString("WXYZ !?", 6, 5, 52);
  }
}

bool selectLeftBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(SELECT_LEFT_BTN_PIN)) {
      previousState = 1;
      return true;
    }
  } else {
    previousState = digitalRead(SELECT_LEFT_BTN_PIN);
  }

  return false;
}

bool selectRightBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(SELECT_RIGHT_BTN_PIN)) {
      previousState = 1;
      return true;
    }
  } else {
    previousState = digitalRead(SELECT_RIGHT_BTN_PIN);
  }

  return false;
}

bool addCharBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(ADD_CHAR_BTN_PIN)) {
      previousState = 1;
      return true;
    }
  } else {
    previousState = digitalRead(ADD_CHAR_BTN_PIN);
  }

  return false;
}

bool deleteCharBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(DELETE_CHAR_BTN_PIN)) {
      previousState = 1;
      return true;
    }
  } else {
    previousState = digitalRead(DELETE_CHAR_BTN_PIN);
  }

  return false;
}

bool sendCharsBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(SEND_CHARS_BTN_PIN)) {
      previousState = 1;
      return true;
    }
  } else {
    previousState = digitalRead(SEND_CHARS_BTN_PIN);
  }

  return false;
}

void addCharToMessage(char c) {
  if (messageIndex == 15)
    return;
  message[messageIndex++] = c;
  message[messageIndex] = '\0';
}

void deleteCharFromMessage() {
  if (messageIndex == 0)
    return;
  message[--messageIndex] = '\0';
}

void clearMessage() {
  for (int i = 0; i < MESSAGE_SIZE; i++) {
    message[i] = 0;
  }
  messageIndex = 0;
}

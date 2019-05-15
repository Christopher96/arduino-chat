#include <SPI.h>
#include <SH1106.h>

#define SELECT_LEFT_BTN_PIN 6
#define SELECT_RIGHT_BTN_PIN 5
#define ADD_CHAR_BTN_PIN 4
#define DELETE_CHAR_BTN_PIN 3
#define SEND_CHARS_BTN_PIN 2
#define MSG_SIZE 16

SH1106 display;
char selectedKey = 'A';
char msg[MSG_SIZE] = "\0";
int msgCursor = 0;
char *receivedMsg;
boolean viewingReceivedMsg = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8); // 16/8 = 2 MHz.
  digitalWrite(SS, HIGH);
  setupBtns();
  display.begin();
}

void loop() {
  display.clear();
  if (viewingReceivedMsg) {
    display.drawString("Received message: ", 2, 0, 0);
    display.drawString(receivedMsg, 2, 64, - display.getStringWidth(msg, 2) / 2, 28);
    if (selectLeftBtnPressed() || selectRightBtnPressed() || addCharBtnPressed() || deleteCharBtnPressed() || sendCharsBtnPressed()) {
      viewingReceivedMsg = false;
    }
  } else {
    updateKeyboard();
    display.drawString(msg, 2, 64 - display.getStringWidth(msg, 2) / 2, 4);

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
          addCharToMsg(' ');
          break;
        case '\\':
          addCharToMsg('!');
          break;
        case ']':
          addCharToMsg('?');
          break;
        default:
          addCharToMsg(selectedKey);
          break;
      }
    }
    else if (deleteCharBtnPressed()) {
      deleteCharFromMsg();
    }

    // Nu kan vi kommunicera.
    digitalWrite(SS, LOW);

    if (sendCharsBtnPressed()) {
      SPI.transfer(msg, msgCursor);
      msgCursor = 0;
    }

    char received = SPI.transfer(1);
      
    if (received != 1) {
      Serial.println("Received: " + received);
      char receiveBuffer[MSG_SIZE];
      int i = 0;
      receiveBuffer[i++] = received;

      while ((received = SPI.transfer(1)) != '\0') {
        Serial.println("Received: " + received);
        receiveBuffer[i++] = received;
      }

      receiveBuffer[i] = '\0';

      receivedMsg = receiveBuffer;
      viewingReceivedMsg = true;
    }
  }

  display.update();
}

void setupBtns() {
  pinMode(SELECT_LEFT_BTN_PIN, INPUT);
  pinMode(SELECT_RIGHT_BTN_PIN, INPUT);
  pinMode(ADD_CHAR_BTN_PIN, INPUT);
  pinMode(DELETE_CHAR_BTN_PIN, INPUT);
  pinMode(SEND_CHARS_BTN_PIN, INPUT);
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

int selectLeftBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(SELECT_LEFT_BTN_PIN)) {
      previousState = 1;
      return 1;
    }
  } else {
    previousState = digitalRead(SELECT_LEFT_BTN_PIN);
  }

  return 0;
}

int selectRightBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(SELECT_RIGHT_BTN_PIN)) {
      previousState = 1;
      return 1;
    }
  } else {
    previousState = digitalRead(SELECT_RIGHT_BTN_PIN);
  }

  return 0;
}

int addCharBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(ADD_CHAR_BTN_PIN)) {
      previousState = 1;
      return 1;
    }
  } else {
    previousState = digitalRead(ADD_CHAR_BTN_PIN);
  }

  return 0;
}

int deleteCharBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(DELETE_CHAR_BTN_PIN)) {
      previousState = 1;
      return 1;
    }
  } else {
    previousState = digitalRead(DELETE_CHAR_BTN_PIN);
  }

  return 0;
}

int sendCharsBtnPressed() {
  static int previousState = 0;

  if (!previousState) {
    if (digitalRead(SEND_CHARS_BTN_PIN)) {
      previousState = 1;
      return 1;
    }
  } else {
    previousState = digitalRead(SEND_CHARS_BTN_PIN);
  }

  return 0;
}

void addCharToMsg(char c) {
  if (msgCursor == 15)
    return;
  msg[msgCursor++] = c;
  msg[msgCursor] = '\0';
}

void deleteCharFromMsg() {
  if (msgCursor == 0)
    return;
  msg[--msgCursor] = '\0';
}

#include <SPI.h>
#include <LiquidCrystal.h>

#define DEL_PIN 8
#define SEND_PIN 9
#define JS_X_PIN 0    // analog pin connected to X output
#define JS_Y_PIN 1    // analog pin connected to Y output
#define JS_MAX 900    // when to register a movement
#define JS_MIN 100

#include <SoftwareSerial.h>

// Initializing communication ports
SoftwareSerial mySerial(10, 11); // TX/RX pins

char selectedChar = 'A';
char msg[] = "                ";
int msgIndex = 0;

bool sending = false;

// LCD
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);   

    pinMode(DEL_PIN, INPUT);
    pinMode(SEND_PIN, INPUT);

    lcd.begin(16, 2);
    lcd.cursor();
    lcd.clear();
}

String getMessage(){
    String msg = "";
    char a;

    while(mySerial.available()) {
        a = mySerial.read();
        msg+=String(a);
    }
    return msg;
}

void loop() {
    String receivedMsg = getMessage();
    receivedMsg.trim();

    if(receivedMsg!=""){
        lcd.noCursor();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Received Message");
        lcd.setCursor(0, 1);
        lcd.print(receivedMsg);

        while (!sendBtnPressed());

        lcd.cursor();
        lcd.clear();
        lcd.print(msg);
    } else {

        if (joystickDown() && selectedChar != 'Z') {
            msg[msgIndex] = ++selectedChar;
            lcd.print(selectedChar);
        }

        if (joystickUp() && selectedChar != 'A') {
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
            mySerial.println(msg);
            clearMessage();
            sending = true;
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

void clearMessage() {
    msgIndex = 0;
    lcd.setCursor(0,0);
    lcd.clear();
    for(int i = 0; i < 16; i++) {
        msg[i] = ' ';
    }
}

#include <SPI.h>
#include <LiquidCrystal.h>
#include <Joystick.h>
#include "constants.h"

// State
typedef enum { RECEIVING, TYPING, SENDING } State;
State state;

// Timely loop
int lastTime = 0;
int timeInterval = 200;

// Chat
int messageIndex = 0;
int maxLength = 16;
char currentMessage[16];
char selectedChar = 'A';

// LCD
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Joystick js = Joystick();

// Slave
volatile boolean hasRecieved;
volatile char[16] recievedData;
volatile byte sendData;

void setup() {
    Serial.begin(BAUD_RATE);

    pinMode(DEL_PIN, INPUT);
    pinMode(SEND_PIN, INPUT);

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.cursor();

    pinMode(MISO, OUTPUT);                      // Send data to master IN

    SPCR |= _BV(SPE);                           // SPI Control Register (SPCR) is used to set SPI in Slave Mode
    SPI.attachInterrupt();                      // Enable interrupts for SPI communication

    Serial.println("Initialized.");
}

ISR(SPI_STC_vect) {                             // This interrupt routine is called whenever data is recieved from the master
    if(recievedData != 1) {
        recievedData = SPDR;                        // Data from master will be stored in the SPI Data Register (SPDR)
        state = RECIEVING;
        Serial.println("Data recevied");

    }
}

bool isIdle = false;

void loop() {
    switch(state) {
        case RECIEVING:
            sendData = 0;

            if(digitalRead(SEND_PIN) == HIGH) {
                state = TYPING;
            } else {
                lcd.setCursor(0, 1);
                lcd.print("New message: ");
                lcd.print(recievedData);

            }
            break;
        case TYPING:
            if(digitalRead(SEND_PIN) == HIGH) {
                state = SENDING;
            } else {
                typing();
            }
            break;
        case SENDING:
            sendData = currentMessage;
            break;
    }
}

void recieving() {
    
}


void typing() {
    if(isIdle) {
        checkJoystick();

        if(digitalRead(DEL_PIN) == HIGH) {
            deleteChar();
            delay(100);
        } else if(digitalRead(SEND_PIN) == HIGH) {
            sendMessage();
            delay(100);
        }
    } else if (js.isIdle()){
        isIdle = true;
    }

    if(selectedChar > 'Z') {
        selectedChar = 'A';
    }

    if(selectedChar < 'A') {
        selectedChar = 'Z';
    }

    currentMessage[messageIndex] = selectedChar;
    lcd.setCursor(messageIndex, 0);

}

void checkJoystick() {
    if(js.isUp()) {
        nextChar();
    } else if(js.isDown()) {
        prevChar();
    } else if(js.isRight()) {
        goForward();
    } else if(js.isLeft()) {
        goBackward();
    }
}

void sendMessage() {

}


void goBackward() {
    isIdle = false;
    messageIndex--;
    if(messageIndex <= 0) {
        messageIndex = 0;
    }
}

void goForward() {
    isIdle = false;
    messageIndex++;
    if(messageIndex >= maxLength) {
        messageIndex = maxLength;
    }
}

void deleteChar() {
    isIdle = false;
    lcd.print(' ');
    lcd.setCursor(messageIndex, 0);
    currentMessage[messageIndex] = 0;
    selectedChar = 'A';
}

void nextChar() {
    isIdle = false;
    selectedChar++; 
    printChar();
}

void prevChar() {
    isIdle = false;
    selectedChar--; 
    printChar();
}

void printChar() {
    lcd.print(selectedChar);
    lcd.setCursor(messageIndex, 0);
}

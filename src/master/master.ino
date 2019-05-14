#include <SPI.h>
#include <SH1106.h>
#include "constants.h"

SH1106 display;

void setup(){
  display.begin();
  display.clear();
  renderKeyboard();
  display.update();
  
  // Set the baud rate for communication.
  Serial.begin(BAUD_RATE);

  // Define LED as ouput and BTN as input.
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);

  SPI.begin();

  // We set the SPI clock frequency to 16 / 8 = 2 MHz.
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  // Set slave select (SS) to 1 which means no communication can occur between Master and Slave.
  digitalWrite(SS, HIGH);

  Serial.println("Setup done!");
}

void  loop(){
  // Set slave select to 0 (SS), we can now communicate with the slave.
  digitalWrite(SS, LOW);
  
  // We get the state of the button either HIGH (pressed) or LOW (not pressed).
  byte btnState = digitalRead(BTN_PIN);

  // We send the state of our button and receive from slave.
  byte received = SPI.transfer(btnState);

  digitalWrite(LED_PIN, received);
  Serial.println(received == 1 ? "Master LED on" : "Master LED off");

  
  
  delay(10);
}

int selectedKey = 0;

void renderKeyboard(){
  char c = 'A';
  int x = 0;
  int y = 31;
  
  while(c != 0x5B){
      display.drawChar(c, x, y);
      x += 7;
      c++;
  }
}

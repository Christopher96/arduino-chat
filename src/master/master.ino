#include <SPI.h>
#include <SH1106.h>

#define SELECT_LEFT_BTN_PIN 6
#define SELECT_RIGHT_BTN_PIN 5
#define ADD_CHAR_BTN_PIN 4
#define DELETE_CHAR_BTN_PIN 3
#define SEND_CHARS_BTN_PIN 2

SH1106 display;
char selectedKey = 'A';

void setup(){  
  Serial.begin(9600);
  
  // SPI.begin();

  // We set the SPI clock frequency to 16 / 8 = 2 MHz.
  // SPI.setClockDivider(SPI_CLOCK_DIV8);

  // Set slave select (SS) to 1 which means no communication can occur between Master and Slave.
  // digitalWrite(SS, HIGH);

  setupBtns();
   
  display.begin();
}

void  loop(){
  // Set slave select to 0 (SS), we can now communicate with the slave.
  // digitalWrite(SS, LOW);
  
  // We get the state of the button either HIGH (pressed) or LOW (not pressed).
  // byte btnState = digitalRead(BTN_PIN);

  // We send the state of our button and receive from slave.
  //byte received = SPI.transfer(btnState);

  //digitalWrite(LED_PIN, received);
  //Serial.println(received == 1 ? "Master LED on" : "Master LED off");


  if(digitalRead(SELECT_LEFT_BTN_PIN)){
    //Serial.println("LEFT SELECT!");
  }
  if(digitalRead(SELECT_RIGHT_BTN_PIN) == HIGH){
    // Serial.println("RIGHT SELECT!");
  }
  
  if(digitalRead(ADD_CHAR_BTN_PIN) == HIGH){
    Serial.println("ADD CHAR!");
  }
  
  if(digitalRead(DELETE_CHAR_BTN_PIN) == HIGH){
      Serial.println("DELETE CHAR");  
  }
  
  if(digitalRead(SEND_CHARS_BTN_PIN) == HIGH){
    Serial.println("SEND CHARS"); 
  }
  
  
  if(selectedKey == '^'){
    selectedKey = 'A'; 
  }
  
  display.clear();
  updateKeyboard();
  display.update();
    
  selectedKey++;

  delay(50);
}

void setupBtns(){
  // pinMode(SELECT_LEFT_BTN_PIN, INPUT);
  // pinMode(SELECT_RIGHT_BTN_PIN, INPUT);
  pinMode(ADD_CHAR_BTN_PIN, INPUT);
  pinMode(DELETE_CHAR_BTN_PIN, INPUT);
  pinMode(SEND_CHARS_BTN_PIN, INPUT);
}

void updateKeyboard(){
  display.drawHorizontalLine(0, 20, 127);

  if(selectedKey >= 'A' && selectedKey < 'L'){
      display.drawString("ABCDEFGHIJK", 6, selectedKey - 0x41, 5, 28); 
  }
  else{
    display.drawString("ABCDEFGHIJK", 6, 5, 28); 
  }

  if(selectedKey >= 'L' && selectedKey < 'W'){
    display.drawString("LMNOPQRSTUV", 6, selectedKey - 0x4C, 5, 40);
  }
  else{
    display.drawString("LMNOPQRSTUV", 6, 5, 40);
  }

  if(selectedKey >= 'W' && selectedKey <= ']'){
      int outlineIndex = selectedKey - 0x57;

      if(selectedKey == '[')
        outlineIndex = 4;
      if(selectedKey == '\\')
        outlineIndex = 5;
      if(selectedKey == ']')
        outlineIndex = 6;
       
      display.drawString("WXYZ !?", 6, outlineIndex, 5, 52); 
  }else{
      display.drawString("WXYZ !?", 6, 5, 52); 
  }
}

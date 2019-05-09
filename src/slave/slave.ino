#include <SPI.h>
#include <API.h>

// Slave variables
volatile boolean hasRecieved;
volatile byte recievedData;
volatile byte sendData;

void setup(){
    doSomething();
    Serial.begin(BAUD_RATE);                    // We begin by starting serial communication at the specified baud rate

    pinMode(BTN_PIN, INPUT);                    // Set the Button to input
    pinMode(LED_PIN, OUTPUT);                   // Set the LED to output 

    // Configuring the serial ports
    pinMode(MISO, OUTPUT);                      // Send data to master IN

    SPCR |= _BV(SPE);                           // SPI Control Register (SPCR) is used to set SPI in Slave Mode
    SPI.attachInterrupt();                      // Enable interrupts for SPI communication

    Serial.println("Initialized.");
}


ISR(SPI_STC_vect) {                             // This interrupt routine is called whenever data is recieved from the master
    recievedData = SPDR;                        // Data from master will be stored in the SPI Data Register (SPDR)
    hasRecieved = true;
    Serial.println("Data recevied");
}


void loop() {
    if(hasRecieved) {                           // Have we recieved data yet?
        if(recievedData == 1) {                 // Is the data equal to 1?
            digitalWrite(LED_PIN, HIGH);        // Yes, turn on the LED
            Serial.println("LED on");
        } else {
            digitalWrite(LED_PIN, LOW);         // No, turn off the LED
            Serial.println("LED off");
        }
        hasRecieved = false;
    } else {
        sendData = 0;
        if(digitalRead(BTN_PIN) == HIGH) {      // Is the button pressed?
            sendData = 1;
            Serial.println("Button pressed");
        }
        SPDR = sendData;                        // Send the data
    }
    
}

// Test some date and time functions on the RTC module
#include <Wire.h>
#include "RTClib.h"

// The TinyRTC uses the DS1307 RTC chip. If your RTC uses a different chip,
// look at the RTCLib for instructions on how to create the rtc variable.
RTC_DS1307 rtc;

// This code is executed once, every time the Arduino is rebooted or powers up.
void setup() {
  // Set the serial port baud rate so we can send some data back to the IDE
  Serial.begin(57600);

  // Check if we have a working RTC
  if(!rtc.begin()) {
    Serial.println("The RTC is not responding!");
    while(1); // Loop forever so nothing else will happen
  }

  // Let's set the RTC date and time to the compilation time of this sketch
  // When a sketch is compiled, the constants __DATE__ and __TIME__ are set to
  // the compilation time.
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

// This function prints the date and time stored in dt to the Serial console
void printDateToSerial(DateTime dt) {
  // First, print the date
  Serial.print(dt.year(), DEC);
  Serial.print('/');
  Serial.print(dt.month(), DEC);
  Serial.print('/');
  Serial.print(dt.day(), DEC);
  Serial.print(" ");

  // And now print the time...
  Serial.print(dt.hour(), DEC);
  Serial.print(':');
  Serial.print(dt.minute(), DEC);
  Serial.print(':');
  Serial.print(dt.second(), DEC);
  Serial.println();
}

void loop() {
    DateTime now = rtc.now();

    Serial.print("It is now ");
    printDateToSerial(now);
    
    // Wait a second before we print again!
    delay(1000);
}


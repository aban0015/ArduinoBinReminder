// Test some date and time functions on the RTC module
#include <Wire.h>
#include "RTClib.h"

// The TinyRTC uses the DS1307 RTC chip
RTC_DS1307 rtc;

// If you are using the Adafruit PFC8523 RTC module, comment out the above line
// and uncomment the line below
// RTC_PCF8523 rtc;

// Set the bin collection day (For me, that is Wednesday, day 3)
const int BIN_DAY = 3;

// This code is executed only once
void setup() {
  // Set the serial port baud rate so we can send some data back to the IDE
  Serial.begin(57600);

  // Check if we have a working RTC
  if(!rtc.begin()) {
    Serial.println("The RTC is not responding!");
    while(1); // Loop forever so nothing else will happen
  }

  // When a sketch is compiled, the constants __DATE__ and __TIME__ are set to
  // the compilation time
  // Let's set the RTC date and time to the compilation time of this sketch
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  DateTime now = rtc.now();

  if((BIN_DAY - now.dayOfTheWeek()) % 7 == 1) {
    Serial.println("Put your bins out tonight!");
  } else if (BIN_DAY == now.dayOfTheWeek()) {
    Serial.println("Oops! You missed bin night!");
  } else {
    Serial.println("Don't worry - it isn't bin night tonight.");
  }
}

void loop() {} // We don’t need to do anything in the loop!


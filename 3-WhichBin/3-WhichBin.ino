// Test some date and time functions on the RTC module
#include <Wire.h>
#include "RTClib.h"

// The TinyRTC uses the DS1307 RTC chip
RTC_DS1307 rtc;

// If you are using the Adafruit PFC8523 RTC module, comment out the above line
// and uncomment the line below
// RTC_PCF8523 rtc;

// Set our known recycling day
const DateTime KNOWN_RECYCLE_DAY = DateTime(2018, 6, 27, 0, 0, 0);

// Set the bin collection day
const int BIN_DAY = KNOWN_RECYCLE_DAY.dayOfTheWeek();

// This code is executed only once
void setup() {
  // Set the serial port baud rate so we can send some data back to the IDE
  Serial.begin(57600);

  // Check if we have a working RTC
  if(!rtc.begin()) {
    Serial.println("The RTC is not responding!");
    // Loop forever so nothing else will happen
    while(1);
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

  // We are subtracting one day (TimeSpan(1, 0, 0, 0)) from now, so we know which bin
  // goes out on the morning of bin day. (Trust me, this works!)
  int weeks = week_diff(now - TimeSpan(1, 0, 0, 0), KNOWN_RECYCLE_DAY);
  if((weeks % 2) == 0) {
    Serial.println("It is Green Waste Week!");
  } else {
    Serial.println("It is Recycling Week!");
  }
}

// Calculate the number of weeks between two dates
int week_diff(DateTime d1, DateTime d2) {
  TimeSpan ts = d1 - d2;
  int days = ts.days();
  
  return (days/7);
}

// We don't need to do anything in our loop
void loop() {}


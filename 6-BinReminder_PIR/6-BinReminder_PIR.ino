// Test some date and time functions on the RTC module
#include <Wire.h>
#include "RTClib.h"

// The TinyRTC uses the DS1307 RTC chip
RTC_DS1307 rtc;

// Set some constants for the LED pins
#define RECYCLE_LED 2
#define GREENWASTE_LED 3

// Set a constant for the button pin
#define BUTTON_PIN 11

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

  // Set the bin LED pins for output, so we can turn them on or off
  pinMode(RECYCLE_LED, OUTPUT);
  pinMode(GREENWASTE_LED, OUTPUT);

  // Set the button pin for input, so we can check when it has been pressed
  pinMode(BUTTON_PIN, INPUT);
}

// Check if today is bin day, and if so, which bins are due to be put out?
void checkBinDay() {
  // Get today's date and the day of the week
  DateTime now = rtc.now();
  int dayOfWeek = now.dayOfTheWeek();

  // Which LED will we light?
  int binLED = RECYCLE_LED;

  // Count how many weeks since our known bin day
  int weeks = week_diff(now - TimeSpan(1, 0, 0, 0), KNOWN_RECYCLE_DAY);

  if((weeks % 2) == 0) {
    Serial.println("It is Green Waste Week!");
    binLED = GREENWASTE_LED;
  } else {
    Serial.println("It is Recycling Week!");
    binLED = RECYCLE_LED;
  }

  // Set a variable to determine how many times to flash the LED
  // A flashRate of 0 will keep the LED on, 5 will flash 5 times, etc.
  int flashRate = 0;
  
  if((BIN_DAY - now.dayOfTheWeek()) % 7 == 1) {
    // Tonight is bin night, so flash the LED 5 times
    flashRate = 5;
    Serial.println("Put your bins out tonight!");
  } else if (BIN_DAY == now.dayOfTheWeek()) {
    // Today is bin _day_, so flash the LED 10 times
    flashRate = 10;
    Serial.println("Oops! You missed bin night!");
  } else {
    // It is not bin night, so let’s do nothing
    return;
  }

  // Flash the appropriate LED for 2 seconds
  flashLED(binLED, flashRate, 2000);
}

// Flash an LED flashRate times over flashMillis milliseconds
void flashLED(int ledPin, int flashRate, int flashMillis) {
  digitalWrite(ledPin, HIGH);

  if(flashRate > 0) {
    // Flash the LED a number of times
    for(int i=0; i < flashRate; i++) {
      delay((flashMillis / 2) / flashRate);
      digitalWrite(ledPin, LOW);
      delay((flashMillis / 2) / flashRate);
      digitalWrite(ledPin, HIGH);
    }
  } else {
    // Just leave the LED on for the entire duration
    delay(flashMillis);
  }

  // Make sure the LED is off when we are done
  digitalWrite(ledPin, LOW);
}

// Calculate the number of weeks between two dates
int week_diff(DateTime d1, DateTime d2) {
  TimeSpan ts = d1 - d2;
  int days = ts.days();
  
  return (days/7);
}

void loop() {
  // Check for button press
  if(digitalRead(BUTTON_PIN) == HIGH) {
    Serial.println("Button pressed!");
    checkBinDay();
  }

  delay(100);
}


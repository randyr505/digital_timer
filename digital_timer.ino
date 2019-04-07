// Based off work from
// hirez2006 @youtube and Jacek Radoszewski @youtube
//
// Modified by:
// Randy Rasmussen (Github @randyr505)
// This is based off of my version of the digital_clock script
// https://github.com/randyr505/digital_clock
//
#include "Timer.h"
#include <Wire.h>
#include <FastLED.h>

// Configure based on your setup
// Your total number of leds
#define NUM_LEDS 28 // 2x7 segment, 14 x 2 digits
#define LED_TYPE WS2811 // Led type, i.e. WS2812B. See more: FastLED @github
#define COLOR_ORDER BRG // Define color order for your strip
int BRIGHTNESS = 100; // Define your max brightness
#define LED_PIN 6 // Data pin for led comunication
long ledColor = CRGB::Red; // Default clock color, can be any valid color
int BAUD_RATE = 9600;
int total_seconds = 60;
bool TestAllOn = false; // Set to true to test all leds, i.e. sets digit=8
int timer_sleep = 5000; // Sleep this time between timers (1000 = 1 second)

// Use for 2 x 7 segments
// 7 segments of 2 leds = 14 total leds per digit for array below
// 1,2, 3,4, 5,6, 7,8, 9,10 11,12,13,14
byte digits[10][14] = {
  {0,0, 1,1, 1,1, 1,1, 1,1, 1,1,  1,1}, // Digit 0
  {0,0, 1,1, 0,0, 0,0, 0,0, 0,0,  1,1}, // Digit 1
  {1,1, 0,0, 1,1, 1,1, 0,0, 1,1,  1,1}, // Digit 2
  {1,1, 1,1, 1,1, 0,0, 0,0, 1,1,  1,1}, // Digit 3
  {1,1, 1,1, 0,0, 0,0, 1,1, 0,0,  1,1}, // Digit 4
  {1,1, 1,1, 1,1, 0,0, 1,1, 1,1,  0,0}, // Digit 5
  {1,1, 1,1, 1,1, 1,1, 1,1, 1,1,  0,0}, // Digit 6
  {0,0, 1,1, 0,0, 0,0, 0,0, 1,1,  1,1}, // Digit 7
  {1,1, 1,1, 1,1, 1,1, 1,1, 1,1,  1,1}, // Digit 8
  {1,1, 1,1, 1,1, 0,0, 1,1, 1,1,  1,1}  // Digit 9
}; // 2D Array for numbers on 7 segment

// Shouldn't need to change anything below this line
int LAST_SECTION = NUM_LEDS;
int LEDS_PER_DIGIT = NUM_LEDS / 2;
int DIGIT_LOOP = LEDS_PER_DIGIT - 1;
int FIRST_DIGIT2 = NUM_LEDS - LEDS_PER_DIGIT;
int FIRST_DIGIT1 = 0;
CRGB leds[NUM_LEDS]; // Define LEDs strip

void setup() {
  Serial.begin(BAUD_RATE);
  Wire.begin();
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

void GetDigits(int seconds) {
  int digit_location =  0;
  Serial.println("");
  Serial.print("Seconds : ");
  Serial.println(seconds);

  for (int i = 1; i <= 2; i++) {
    int digit = seconds % 10; // get last digit in time
    if (TestAllOn) {
      Serial.println("TestAllOn = true");
      digit = 8;
    }

    if (i == 1) {
      digit_location = FIRST_DIGIT2;
      Serial.print("Digit 2 is : ");
      Serial.print(digit);
      Serial.print(", the array is : ");
      for (int k = 0; k <= DIGIT_LOOP; k++) {
        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1) {
          leds[digit_location] = ledColor;
        }
        else {
          leds[digit_location] = 0x000000;
        };
        digit_location ++;
      };
      Serial.println();
    }
    else if (i == 2) {
      digit_location = FIRST_DIGIT1;
      Serial.print("Digit 1 is : ");
      Serial.print(digit);
      Serial.print(", the array is : ");
      for (int k = 0; k <= DIGIT_LOOP; k++) {
        Serial.print(digits[digit][k]);
        if (digits[digit][k] == 1) {
          leds[digit_location] = ledColor;
        }
        else {
          leds[digit_location] = 0x000000;
        };
        digit_location ++;
      };
    };
    seconds /= 10;
  };
};


// Main loop
void loop() {
  int seconds = total_seconds;
  while (seconds >= 0) {
    GetDigits(seconds); // Get leds array with required configuration
    FastLED.show(); // Display leds array
    delay(1000);
    seconds--;
  }
  delay(timer_sleep);
}

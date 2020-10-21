#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define PULSE_PIN 27

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float throttle;
float maxThrottle = 2000;
float minThrottle = 1000;
float centerThrottle = 1500;

unsigned int throttleBarWidth = 0;

void setup(){
	delay(1000);
	Serial.begin(115200);
    pinMode(PULSE_PIN, INPUT);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(2000); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();

    display.setTextWrap(false);
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(WHITE);

    display.setCursor(0, 0);
    display.print(F("Started "));

    display.display();
    //display.startscrollright(0x00, 0x0F);
}

float deltaThrottle = 0;

void loop() {
    display.clearDisplay();
    display.setCursor(0, 0);
    throttle = pulseIn(PULSE_PIN, HIGH);
    //Serial.print(throttle);

    if (throttle > maxThrottle) {
        maxThrottle = throttle;
        centerThrottle = (maxThrottle + minThrottle) / 2;
    } else if (throttle < minThrottle) {
        minThrottle = throttle;
        centerThrottle = (maxThrottle + minThrottle) / 2;
    }

    if (throttle >= centerThrottle) {
        throttleBarWidth = ((throttle - centerThrottle)/(maxThrottle - centerThrottle)) * SCREEN_WIDTH;
    } else {
        throttleBarWidth = ((centerThrottle - throttle)/(centerThrottle - minThrottle)) * SCREEN_WIDTH;
    }

    display.fillRect(0, 0, throttleBarWidth, SCREEN_HEIGHT, WHITE);
    display.display();
}
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <LoRa.h>

#define FREQ 915E6
String textToSend, textToDisplay;

// TFT Shield Reference Material:
// Adafruit ILI9341 graphics library code examples (Examples->Adafruit ILI9341->graphicstest)
// http://www.adafruit.com/products/1651

// Default pins for the TFT shield are TFT_DC 9 and TFT_CS 10
// The LoRa shield uses the same pins for SPI so I had to re-route these using a breadboard
#define TFT_DC 4
#define TFT_CS 5

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void setup() {
  Serial.begin(9600);

  while (!Serial)
    ;

  Serial.println("LoRa sender/receiver");
  if (!LoRa.begin(FREQ)) {
    Serial.println("LoRa initialization failed");
    while (true) delay(1000);
  }

  tft.begin();
}

void loop() {

  if (Serial.available()) {

    while (Serial.available()) {
      textToSend = Serial.readString();
    }
    Serial.print("Sending packet");
    LoRa.beginPacket();
    LoRa.print(textToSend);
    LoRa.endPacket();
  }

  if (!LoRa.parsePacket()) return;

  Serial.print("Received packet");
  while (LoRa.available()) textToDisplay = LoRa.readString();
  displayText(textToDisplay);
}

void displayText(String text) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(text);
}
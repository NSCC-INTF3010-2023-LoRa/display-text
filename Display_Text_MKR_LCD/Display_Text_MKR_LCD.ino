#include <LoRa.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define FREQ 915E6
String textToSend, textToDisplay;

void setup() {
  Serial.begin(9600);

  // initialize the lcd
  lcd.init();
  lcd.backlight();

  while (!Serial)
    ;

  Serial.println("LoRa sender/receiver");
  if (!LoRa.begin(FREQ)) {
    Serial.println("LoRa initialization failed");
    while (true) delay(1000);
  }
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
  if (LoRa.available()) {
    lcd.clear();
    while (LoRa.available()) textToDisplay = LoRa.readString();
    lcd.print(textToDisplay);
  }
}
#include "IRLremote.h"
#include <Adafruit_NeoPixel.h>

#define IR_PIN              2
#define LED_STRIP_PIN       6
#define LED_STRIP_PIXELS    100.0

CNec IRLremote;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_STRIP_PIXELS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial.println("running");

  IRLremote.begin(IR_PIN);
  pixels.begin();

  randomSeed(analogRead(0));
}

void loop() {
  if (!IRLremote.receiving()) {
    updateLEDs();
  }

  if (IRLremote.available()) {
    auto data = IRLremote.read();
    processRemote(data.command);
  }

  delay(10);
}

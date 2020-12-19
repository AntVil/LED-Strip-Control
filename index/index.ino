#include "IRLremote.h"
#include <Adafruit_NeoPixel.h>

#define POWER 69
#define VOL_PLUS 70
#define STOP 71
#define LEFT 68
#define PAUSE 64
#define RIGHT 67
#define DOWN 7
#define VOL_MINUS 21
#define UP 9
#define ZERO 22
#define EQ 25
#define REPT 13
#define ONE 12
#define TWO 24
#define THREE 94
#define FOUR 8
#define FIVE 28
#define SIX 90
#define SEVEN 66
#define EIGHT 82
#define NINE 74


#define pinIR 2

CNec IRLremote;
int lastPressed;


#define PIN            6
#define NUMPIXELS      100

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  while (!Serial);
  Serial.begin(9600);
  Serial.println(F("Startup"));

  if (!IRLremote.begin(pinIR)) {
    Serial.println(F("You did not choose a valid pin."));
  }

  pixels.begin();
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


bool on = true;
float hue = 0;
float light = 0.5;
float lightStep = 0.0625;
float saturation = 0;
float saturationStep = 0.0625;


void processRemote(int num) {
  if (num == POWER) {
    on = !on;
  } else if (num == VOL_PLUS) {
    light = min(1, light + lightStep);
  } else if (num == STOP) {

  } else if (num == LEFT) {
    hue--;
    if (hue < 0) {
      hue += 360;
    }
  } else if (num == PAUSE) {

  } else if (num == RIGHT) {
    hue++;
    if (hue > 360) {
      hue -= 360;
    }
  } else if (num == DOWN) {
    saturation = max(0, saturation - saturationStep);
  } else if (num == VOL_MINUS) {
    light = max(0, light - lightStep);
  } else if (num == UP) {
    saturation = min(1, saturation + saturationStep);
  } else if (num == ZERO) {

  } else if (num == EQ) {

  } else if (num == REPT) {

  } else if (num == ONE) {

  } else if (num == TWO) {

  } else if (num == THREE) {

  } else if (num == FOUR) {

  } else if (num == FIVE) {

  } else if (num == SIX) {

  } else if (num == SEVEN) {

  } else if (num == EIGHT) {

  } else if (num == NINE) {

  } else if (num == 0) {

  }

  lastPressed = num;
}


void updateLEDs() {
  hue += 1;
  if (hue > 360) {
    hue -= 360;
  }

  float r_ = 255 * (cos(hue / 360 * 2 * PI) + 1) / 2 * light;
  float g_ = 255 * (cos((hue + 120) / 360 * 2 * PI) + 1) / 2 * light;
  float b_ = 255 * (cos((hue + 240) / 360 * 2 * PI) + 1) / 2 * light;

  int r = r_ * (saturation) + (g_ + b_) / 2 * (1 - saturation);
  int g = g_ * (saturation) + (r_ + b_) / 2 * (1 - saturation);
  int b = b_ * (saturation) + (r_ + g_) / 2 * (1 - saturation);


  pixels.setPixelColor(1, pixels.Color(r, g, b));

  pixels.show();
}

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


bool on = true;
bool runAnimation = false;
float hue = 0;
float hueStep = 1;
float light = 0.5;
float lightStep = 0.0625;
float saturation = 0;
float saturationStep = 0.0625;

int frame = 0;

int state = 0;

int pulsateColorState = 0;
int rainbowColorState = 1;
int flashColorState = 2;
int blinkColorState = 3;
int movingLeftPointState = 4;
int movingRightPointState = 5;
int movingZickZackPointState = 6;
int movingLeftStripeState = 7;
int movingRightStripeState = 8;
int movingZickZackStripeState = 9;
int movingInStripeState = 10;
int movingOutStripeState = 11;
int movingRainbowState = 12;

int states = 13;


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
    runAnimation = !runAnimation;
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
    hue = 0;
    saturation = 0;
    light = 1;
  } else if (num == EQ) {
    state = (state + 1) % states;
  } else if (num == REPT) {

  } else if (num == ONE) {//rot
    hue = 0;
    saturation = 1;
    light = 0.5;
  } else if (num == TWO) {//orange
    hue = 30;
    saturation = 1;
    light = 0.5;
  } else if (num == THREE) {//gelb
    hue = 60;
    saturation = 1;
    light = 0.5;
  } else if (num == FOUR) {//grün
    hue = 120;
    saturation = 1;
    light = 0.5;
  } else if (num == FIVE) {//cyan
    hue = 180;
    saturation = 1;
    light = 0.5;
  } else if (num == SIX) {//blau
    hue = 220;
    saturation = 1;
    light = 0.5;
  } else if (num == SEVEN) {//lila
    hue = 280;
    saturation = 1;
    light = 0.5;
  } else if (num == EIGHT) {//pink
    hue = 315;
    saturation = 1;
    light = 0.5;
  } else if (num == NINE) {//grau
    hue = 0;
    saturation = 0;
    light = 0.25;
  } else if (num == 0) {
    
  }

  
  
  lastPressed = num;
}



void color(int * col, float h, float s, float l) {
  float r = max(0, (cos(h / 360 * 2 * PI) + 0.5) / 1.5);
  float g = max(0, (cos((h + 120) / 360 * 2 * PI) + 0.5) / 1.5);
  float b = max(0, (cos((h + 240) / 360 * 2 * PI) + 0.5) / 1.5);

  if (l < 0.5) {
    r = r * 2 * l;
    g = g * 2 * l;
    b = b * 2 * l;
  } else {
    r = min(1, (r + light - 0.5) * 2 * l);
    g = min(1, (g + light - 0.5) * 2 * l);
    b = min(1, (b + light - 0.5) * 2 * l);
  }

  float average = (r + g + b) / 3;
  float s_ = sqrt(s);
  r = r * s_ + average * (1 - s_);
  g = g * s_ + average * (1 - s_);
  b = b * s_ + average * (1 - s_);

  col[0] = 255 * r;
  col[1] = 255 * g;
  col[2] = 255 * b;
}


void updateLEDs() {


  if (on) {
    if (runAnimation) {
      frame++;
      if(state == pulsateColorState){
        pulsateColor();
      }else if(state == rainbowColorState){
        rainbowColor();
      }else if(state == flashColorState){
        flashColor();
      }else if(state == blinkColorState){
        blinkColor();
      }else if(state == movingLeftPointState){
        movingLeftPoint();
      }else if(state == movingRightPointState){
        movingRightPoint();
      }else if(state == movingZickZackPointState){
        movingZickZackPoint();
      }else if(state == movingLeftStripeState){
        movingLeftStripe();
      }else if(state == movingRightStripeState){
        movingRightStripe();
      }else if(state == movingZickZackStripeState){
        movingZickZackStripe();
      }else if(state == movingInStripeState){
        movingInStripe();
      }else if(state == movingOutStripeState){
        movingOutStripe();
      }else if(state == movingRainbowState){
        movingRainbow();
      }
    } else {
      flatColor();
    }
  } else {
    pixels.clear();
  }
  
  pixels.show();
}


void flatColor() {
  int col[3];
  color(col, hue, saturation, light);

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }
}

void pulsateColor() {
  saturation = (sin(frame / 60.0 * PI) + 1) / 4 + 0.5;

  Serial.println(saturation);
  
  int col[3];
  color(col, hue, saturation, light);

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }
}

void rainbowColor() {
  hue += hueStep;
  while (hue >= 360) {
    hue -= 360;
  }

  int col[3];
  color(col, hue, saturation, light);

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }
}

void flashColor() {
  if (frame % 10 < 5) {
    int col[3];
    color(col, hue, saturation, light);

    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }
  } else {
    pixels.clear();
  }
}

void blinkColor() {
  int col[3];
  color(col, hue, saturation, light);

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }

  for(int i=0;i<NUMPIXELS/3;i++){
    pixels.setPixelColor(random(0, NUMPIXELS), pixels.Color(0, 0, 0));
  }

  delay(100);
}

void movingLeftPoint(){
  pixels.clear();
  
  int col[3];
  color(col, hue, saturation, light);

  pixels.setPixelColor(frame % NUMPIXELS, pixels.Color(col[0], col[1], col[2]));
  delay(100);
}

void movingRightPoint(){
  pixels.clear();
  
  int col[3];
  color(col, hue, saturation, light);

  pixels.setPixelColor(NUMPIXELS - frame % NUMPIXELS, pixels.Color(col[0], col[1], col[2]));
  delay(100);
}

void movingZickZackPoint(){
  
}

void movingLeftStripe(){
  
}

void movingRightStripe(){
  
}

void movingZickZackStripe(){
  
}

void movingInStripe(){
  
}

void movingOutStripe(){
  
}

void movingRainbow(){
  
}

void movingSkipPoints(){
  
}

void pulsateColors(){ //two
  
}

void movingBackAndForth(){
  
}

void pulsatingToOffColor(){
  
}

void randomSmoothChangeColor(){
  
}

void randomBlinkColor(){
  
}

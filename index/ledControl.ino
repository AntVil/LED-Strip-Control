#define PULSATE_COLOR_STATE      0
#define RAINBOW_COLOR_STATE      1
#define FLASH_COLOR_STATE        2
#define BLINK_COLOR_STATE        3

#define PULSATE_RANDOM_STATE     4

#define PULSATE_LIGHT_STATE      5

#define LEFT_POINT_STATE         6
#define RIGHT_POINT_STATE        7
#define LEFT_RIGHT_POINT_STATE   8
#define ZICK_ZACK_POINT_STATE    9
#define JUMP_POINT_STATE        10

#define LEFT_STRIPE_STATE       11
#define RIGHT_STRIPE_STATE      12
#define LEFT_RIGHT_STRIPE_STATE 13
#define ZICK_ZACK_STRIPE_STATE  14
#define INWARD_STRIPE_STATE     15
#define OUTWARD_STRIPE_STATE    16

#define SLIDE_RAINBOW_STATE     17
#define FLASH_RAINBOW_STATE     18

#define STATES_AMOUNT           19
int state = 0;

bool on = true;
bool runAnimation = false;
bool saveElectricity = true;
float hue = 0;
float hueStep = 1;
float light = 0.5;
float lightStep = 0.0625;
float saturation = 0;
float saturationStep = 0.0625;

bool showNeeded = true;

int frame = 0;


void updateLEDs() {
  if (on) {
    if (runAnimation) {
      frame++;
      if (state == PULSATE_COLOR_STATE) {
        pulsateColor();
      } else if (state == RAINBOW_COLOR_STATE) {
        rainbowColor();
      } else if (state == FLASH_COLOR_STATE) {
        flashColor();
      } else if (state == BLINK_COLOR_STATE) {
        blinkColor();
      } else if (state == PULSATE_RANDOM_STATE) {
        pulsateRandom();
      } else if (state == PULSATE_LIGHT_STATE) {
        pulsateLight();
      } else if (state == LEFT_POINT_STATE) {
        leftPoint();
      } else if (state == RIGHT_POINT_STATE) {
        rightPoint();
      } else if (state == LEFT_RIGHT_POINT_STATE) {
        leftRightPoint();
      } else if (state == ZICK_ZACK_POINT_STATE) {
        zickZackPoint();
      } else if (state == JUMP_POINT_STATE) {
        jumpPoint();
      } else if (state == LEFT_STRIPE_STATE) {
        leftStripe();
      } else if (state == RIGHT_STRIPE_STATE) {
        rightStripe();
      } else if (state == LEFT_RIGHT_STRIPE_STATE) {
        leftRightStripe();
      } else if (state == ZICK_ZACK_STRIPE_STATE) {
        zickZackStripe();
      } else if (state == INWARD_STRIPE_STATE) {
        inwardStripe();
      } else if (state == OUTWARD_STRIPE_STATE) {
        outwardStripe();
      } else if (state == SLIDE_RAINBOW_STATE) {
        slideRainbow();
      } else if (state == FLASH_RAINBOW_STATE) {
        flashRainbow();
      }
    } else {
      flatColor();
    }
  } else {
    pixels.clear();
  }

  if (showNeeded) {
    pixels.show();
    showNeeded = false;
  }
}


//conversion from hsl to rgb (not accurate, but works)
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
  r = r * s + average * (1 - s);
  g = g * s + average * (1 - s);
  b = b * s + average * (1 - s);

  if (saveElectricity) {
    col[0] = 50 * r;
    col[1] = 50 * g;
    col[2] = 50 * b;
  } else {
    col[0] = 255 * r;
    col[1] = 255 * g;
    col[2] = 255 * b;
  }
}


void flatColor() {
  int col[3];
  color(col, hue, saturation, light);

  for (int i = 0; i < LED_STRIP_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }
}


void pulsateColor() {
  if (frame % 2 == 0) {
    saturation = (sin(frame / 60.0 * PI) + 1) / 4 + 0.5;

    int col[3];
    color(col, hue, saturation, light);

    for (int i = 0; i < LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

void rainbowColor() {
  if (frame % 2 == 0) {
    hue = (int)(hue + hueStep) % 360;

    int col[3];
    color(col, hue, saturation, light);

    for (int i = 0; i < LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

void flashColor() {
  int remainder = frame % 20;
  if (remainder == 0) {
    pixels.clear();
    showNeeded = true;
  } else if (remainder == 10) {
    int col[3];
    color(col, hue, saturation, light);

    for (int i = 0; i < LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

void blinkColor() {
  int remainder = frame % 20;
  if (remainder == 0) {
    int col[3];
    color(col, hue, saturation, light);

    for (int i = 0; i < LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    for (int i = 0; i < LED_STRIP_PIXELS / 2; i++) {
      pixels.setPixelColor(random(LED_STRIP_PIXELS), pixels.Color(0, 0, 0));
    }

    showNeeded = true;
  }
}

void pulsateRandom() {
  if (frame % 2 == 0) {
    if (frame % 60 == 0) {
      hue = random(360);
    }

    light = (sin(frame / 60.0 * PI) + 1) / 2;

    int col[3];
    color(col, hue, saturation, light);

    for (int i = 0; i < LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

void pulsateLight() {
  if (frame % 2 == 0) {
    light = (sin(frame / 60.0 * PI) + 1) / 2;

    int col[3];
    color(col, hue, saturation, light);

    for (int i = 0; i < LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

void leftPoint() {
  if (frame % 10 == 0) {
    pixels.clear();

    int col[3];
    color(col, hue, saturation, light);

    pixels.setPixelColor((frame / 10) % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));

    showNeeded = true;
  }
}

void rightPoint() {
  if (frame % 10 == 0) {
    pixels.clear();

    int col[3];
    color(col, hue, saturation, light);

    pixels.setPixelColor(LED_STRIP_PIXELS - (frame / 10) % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));

    showNeeded = true;
  }
}

void leftRightPoint() {
  if (frame % 10 == 0) {
    pixels.clear();

    int col[3];
    color(col, hue, saturation, light);

    if(frame / 10 % (int)(2 * LED_STRIP_PIXELS) > LED_STRIP_PIXELS){
      pixels.setPixelColor(LED_STRIP_PIXELS - (frame / 10) % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));
    }else{
      pixels.setPixelColor((frame / 10 - 1) % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

void zickZackPoint() {

}

void jumpPoint() {
  int remainder = frame % 20;
  if (remainder == 0) {
    pixels.clear();
    int col[3];
    color(col, hue, saturation, light);
    pixels.setPixelColor((frame / 20) % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));
    showNeeded = true;
  } else if (remainder == 10) {
    pixels.clear();
    int col[3];
    color(col, hue, saturation, light);
    pixels.setPixelColor(((frame + 30) / 20) % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));
    showNeeded = true;
  }
}

void leftStripe() {
  if (frame % 10 == 0) {
    pixels.clear();

    int col[3];
    color(col, hue, saturation, light);
    
    for(int i=0;i<(frame / 10) % (int)LED_STRIP_PIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

void rightStripe() {
  if (frame % 10 == 0) {
    pixels.clear();

    int col[3];
    color(col, hue, saturation, light);
    
    for(int i=0;i<LED_STRIP_PIXELS - (frame / 10) % (int)LED_STRIP_PIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

void leftRightStripe() {
  if (frame % 10 == 0) {
    pixels.clear();

    int col[3];
    color(col, hue, saturation, light);

    if(frame / 10 % (int)(2 * LED_STRIP_PIXELS) > LED_STRIP_PIXELS){
      for(int i=0;i<LED_STRIP_PIXELS - (frame / 10) % (int)LED_STRIP_PIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
      }
    }else{
      for(int i=0;i<(frame / 10 - 1) % (int)LED_STRIP_PIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
      }
    }

    showNeeded = true;
  }
}

void zickZackStripe() {

}

void inwardStripe() {

}

void outwardStripe() {

}

void slideRainbow() {
  for (int i = 0; i < LED_STRIP_PIXELS; i++) {
    int col[3];
    color(col, hue + ((i + frame) % (int)LED_STRIP_PIXELS) / LED_STRIP_PIXELS * 360, saturation, light);
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }

  showNeeded = true;
}

void flashRainbow() {
  int remainder = frame % 20;
  if (remainder == 0) {
    pixels.clear();
    showNeeded = true;
  } else if (remainder == 10) {
    hue = random(360);

    int col[3];
    color(col, hue, saturation, light);

    for (int i = 0; i < LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

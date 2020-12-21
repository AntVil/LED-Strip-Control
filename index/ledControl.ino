#define PULSATE_COLOR_STATE      0
#define RAINBOW_COLOR_STATE      1
#define BLINK_COLOR_STATE        2

#define PULSATE_LIGHT_STATE      3

#define LEFT_POINT_STATE         4
#define RIGHT_POINT_STATE        5
#define LEFT_RIGHT_POINT_STATE   6
#define JUMP_POINT_STATE         7

#define LEFT_STRIPE_STATE        8
#define RIGHT_STRIPE_STATE       9
#define LEFT_RIGHT_STRIPE_STATE 10
#define INWARD_STRIPE_STATE     11
#define OUTWARD_STRIPE_STATE    12

#define SLIDE_RAINBOW_STATE     13
#define SLIDE_LIGHT_STATE       14

#define STATES_AMOUNT           15
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
#define FRAME_STEP  10

void updateLEDs() {
  if (on) {
    if (runAnimation) {
      if (frame % FRAME_STEP == 0) {
        int f = frame / FRAME_STEP;
        if (state == PULSATE_COLOR_STATE) {
          pulsateColor(f);
        } else if (state == RAINBOW_COLOR_STATE) {
          rainbowColor(f);
        } else if (state == BLINK_COLOR_STATE) {
          blinkColor(f);
        } else if (state == PULSATE_LIGHT_STATE) {
          pulsateLight(f);
        } else if (state == LEFT_POINT_STATE) {
          leftPoint(f);
        } else if (state == RIGHT_POINT_STATE) {
          rightPoint(f);
        } else if (state == LEFT_RIGHT_POINT_STATE) {
          leftRightPoint(f);
        } else if (state == JUMP_POINT_STATE) {
          jumpPoint(f);
        } else if (state == LEFT_STRIPE_STATE) {
          leftStripe(f);
        } else if (state == RIGHT_STRIPE_STATE) {
          rightStripe(f);
        } else if (state == LEFT_RIGHT_STRIPE_STATE) {
          leftRightStripe(f);
        } else if (state == INWARD_STRIPE_STATE) {
          inwardStripe(f);
        } else if (state == OUTWARD_STRIPE_STATE) {
          outwardStripe(f);
        } else if (state == SLIDE_RAINBOW_STATE) {
          slideRainbow(f);
        } else if (state == SLIDE_LIGHT_STATE) {
          slideLight(f);
        }
      }
      frame++;
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


void pulsateColor(int frame) {
  saturation = (sin(frame / 6.0 * PI) + 1) / 4 + 0.5;

  int col[3];
  color(col, hue, saturation, light);

  for (int i = 0; i < LED_STRIP_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }

  showNeeded = true;
}

void rainbowColor(int frame) {
  hue = (int)(hue + frame) % 360;

  int col[3];
  color(col, hue, saturation, light);

  for (int i = 0; i < LED_STRIP_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }

  showNeeded = true;
}

void blinkColor(int frame) {
  if (frame % 10 == 0) {
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

void pulsateLight(int frame) {
  if (frame % 2 == 0) {
    light = (sin(frame / 6.0 * PI) + 1) / 8 + 0.25;

    int col[3];
    color(col, hue, saturation, light);

    for (int i = 0; i < LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }

    showNeeded = true;
  }
}

void leftPoint(int frame) {
  pixels.clear();

  int col[3];
  color(col, hue, saturation, light);

  pixels.setPixelColor(frame % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));

  showNeeded = true;
}

void rightPoint(int frame) {

  pixels.clear();

  int col[3];
  color(col, hue, saturation, light);

  pixels.setPixelColor(LED_STRIP_PIXELS - frame % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));

  showNeeded = true;
}

void leftRightPoint(int frame) {
  pixels.clear();

  int col[3];
  color(col, hue, saturation, light);

  if (frame % (int)(2 * LED_STRIP_PIXELS) > LED_STRIP_PIXELS) {
    pixels.setPixelColor(LED_STRIP_PIXELS - frame % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));
  } else {
    pixels.setPixelColor((frame - 1) % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));
  }

  showNeeded = true;
}

void jumpPoint(int frame) {
  pixels.clear();

  int col[3];
  color(col, hue, saturation, light);

  if (frame % 2 == 0) {
    pixels.setPixelColor((frame / 2) % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));
  } else {
    pixels.setPixelColor(((frame + 3) / 2) % (int)LED_STRIP_PIXELS, pixels.Color(col[0], col[1], col[2]));
  }

  showNeeded = true;
}

void leftStripe(int frame) {
  pixels.clear();

  int col[3];
  color(col, hue, saturation, light);

  for (int i = 0; i < frame % (int)LED_STRIP_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }

  showNeeded = true;
}

void rightStripe(int frame) {
  pixels.clear();

  int col[3];
  color(col, hue, saturation, light);

  for (int i = 0; i < LED_STRIP_PIXELS - frame % (int)LED_STRIP_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }

  showNeeded = true;
}

void leftRightStripe(int frame) {
  pixels.clear();

  int col[3];
  color(col, hue, saturation, light);

  if (frame % (int)(2 * LED_STRIP_PIXELS) > LED_STRIP_PIXELS) {
    for (int i = 0; i < LED_STRIP_PIXELS - frame % (int)LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }
  } else {
    for (int i = 0; i < frame % (int)LED_STRIP_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    }
  }

  showNeeded = true;
}

void inwardStripe(int frame) {
  pixels.clear();

  int col[3];
  color(col, hue, saturation, light);

  for(int i=0;i<(frame % (int)LED_STRIP_PIXELS) / 2;i++){
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
    pixels.setPixelColor(LED_STRIP_PIXELS - i - 1, pixels.Color(col[0], col[1], col[2]));
  }
  
  showNeeded = true;
}

void outwardStripe(int frame) {
  pixels.clear();

  int col[3];
  color(col, hue, saturation, light);

  for(int i=0;i<(frame % (int)LED_STRIP_PIXELS) / 2;i++){
    pixels.setPixelColor(LED_STRIP_PIXELS / 2 - i, pixels.Color(col[0], col[1], col[2]));
    pixels.setPixelColor(LED_STRIP_PIXELS / 2 + i, pixels.Color(col[0], col[1], col[2]));
  }
  
  showNeeded = true;
}

void slideRainbow(int frame) {
  for (int i = 0; i < LED_STRIP_PIXELS; i++) {
    int col[3];
    color(col, hue + ((int)(i + frame / 6.0) % (int)LED_STRIP_PIXELS) / LED_STRIP_PIXELS * 360, saturation, light);
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }

  showNeeded = true;
}

void slideLight(int frame) {
  for (int i = 0; i < LED_STRIP_PIXELS; i++) {
    int col[3];
    color(col, hue, saturation, (light + (sin(frame / 6.0) + 1) / 2) / 2);
    pixels.setPixelColor(i, pixels.Color(col[0], col[1], col[2]));
  }

  showNeeded = true;
}

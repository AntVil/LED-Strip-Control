//remote buttons
#define POWER       69
#define VOL_PLUS    70
#define STOP        71
#define LEFT        68
#define PAUSE       64
#define RIGHT       67
#define DOWN         7
#define VOL_MINUS   21
#define UP           9
#define ZERO        22
#define EQ          25
#define REPT        13
#define ONE         12
#define TWO         24
#define THREE       94
#define FOUR         8
#define FIVE        28
#define SIX         90
#define SEVEN       66
#define EIGHT       82
#define NINE        74

int lastPressed;

void processRemote(int num) {
  Serial.println(num);
  if (num == POWER) {
    on = !on;
  } else if (num == VOL_PLUS) {
    light = min(1, light + lightStep);
  } else if (num == STOP) {

  } else if (num == LEFT) {
    hue -= hueStep;
    if (hue < 0) {
      hue += 360;
    }
  } else if (num == PAUSE) {
    runAnimation = !runAnimation;
  } else if (num == RIGHT) {
    hue += hueStep;
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
    state = (state + 1) % STATES_AMOUNT;
  } else if (num == REPT) {
    saveElectricity = !saveElectricity;
  } else if (num == ONE) {
    hue = 0;
    saturation = 1;
    light = 0.5;
  } else if (num == TWO) {
    hue = 20;
    saturation = 1;
    light = 0.5;
  } else if (num == THREE) {
    hue = 40;
    saturation = 1;
    light = 0.5;
  } else if (num == FOUR) {
    hue = 120;
    saturation = 1;
    light = 0.5;
  } else if (num == FIVE) {
    hue = 180;
    saturation = 1;
    light = 0.5;
  } else if (num == SIX) {
    hue = 220;
    saturation = 1;
    light = 0.5;
  } else if (num == SEVEN) {
    hue = 280;
    saturation = 1;
    light = 0.5;
  } else if (num == EIGHT) {
    hue = 315;
    saturation = 1;
    light = 0.5;
  } else if (num == NINE) {
    hue = 0;
    saturation = 0;
    light = 0.25;
  } else if (num == 0) {
    if (num == VOL_PLUS) {
      light = min(1, light + lightStep);
    } else if (num == STOP) {

    } else if (num == LEFT) {
      hue -= hueStep;
      if (hue < 0) {
        hue += 360;
      }
    } else if (num == PAUSE) {
      runAnimation = !runAnimation;
    } else if (num == RIGHT) {
      hue += hueStep;
      if (hue > 360) {
        hue -= 360;
      }
    } else if (num == DOWN) {
      saturation = max(0, saturation - saturationStep);
    } else if (num == VOL_MINUS) {
      light = max(0, light - lightStep);
    } else if (num == UP) {
      saturation = min(1, saturation + saturationStep);
    }
  }

  lastPressed = num;
  showNeeded = true;
}

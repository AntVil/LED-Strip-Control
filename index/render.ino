#define HUE_LEVELS          36
#define SATURATION_LEVELS   10
#define LIGHT_LEVELS        10

int pixels[PIXELS][3];

int hueLevel = 0;
int saturationLevel = SATURATION_LEVELS;
int lightLevel = LIGHT_LEVELS / 2;

float hue = 0;
float saturation = 1;
float light = 0.5;

void setupRender(){
    for(int p=0;p<PIXELS;p++){
        pixels[p][0] = 0;
        pixels[p][1] = 0;
        pixels[p][2] = 0;
    }

    randomSeed(analogRead(0));
}

// interface for io
void increaseHue(){
    hueLevel++;
    if(hueLevel > HUE_LEVELS){
        hueLevel = 0;
    }
    hue = 360.0 * ((float)hueLevel / (float)HUE_LEVELS);
}

void decreaseHue(){
    hueLevel--;
    if(hueLevel < HUE_LEVELS){
        hueLevel = HUE_LEVELS-1;
    }
    hue = 360.0 * ((float)hueLevel / (float)HUE_LEVELS);
}

void increaseSaturation(){
    saturationLevel = min(SATURATION_LEVELS, saturationLevel+1);
    saturation = 1 - pow(0.5, SATURATION_LEVELS - saturationLevel);
    //Serial.print("saturation: ");
    //Serial.println(saturation);
}

void decreaseSaturation(){
    saturationLevel = max(0, saturationLevel-1);
    saturation = 1 - pow(0.5, SATURATION_LEVELS - saturationLevel);
    //Serial.print("saturation: ");
    //Serial.println(saturation);
}

void increaseLight(){
    lightLevel = min(LIGHT_LEVELS, lightLevel+1);
    light = pow(0.5, LIGHT_LEVELS - lightLevel);
    //Serial.print("light: ");
    //Serial.println(light);
}

void decreaseLight(){
    lightLevel = max(0, lightLevel-1);
    light = pow(0.5, LIGHT_LEVELS - lightLevel);
    //Serial.print("light: ");
    //Serial.println(light);
}

// render modes (to stop mode interrupted needs to be set true)
void renderNone(){
    for(int p=0;p<PIXELS;p++){
        pixels[p][0] = 0;
        pixels[p][1] = 0;
        pixels[p][2] = 0;
    }

    showArray(pixels);

    while(!interrupted){
        delay(100);
    }
}

void renderFlatColor(){
    for(int p=0;p<PIXELS;p++){
        hsl(pixels[p], hue, saturation, light);
    }

    showArray(pixels);

    while(!interrupted){
        delay(100);
    }
}

void renderFlatRainbow(){
    int frame = 0;
    while(!interrupted){
        for(int p=0;p<PIXELS;p++){
            hsl(pixels[p], frame, saturation, light);
        }
        frame = (frame + 1) % 360;
        showArray(pixels);
        delay(100);
    }
}

void renderLineRainbow(){
    int frame = 0;
    while(!interrupted){
        for(int p=0;p<PIXELS;p++){
            hsl(pixels[p], 360 * (float)(frame + p) / PIXELS, saturation, light);
        }
        frame = (frame + 1) % PIXELS;
        showArray(pixels);
        delay(100);
    }
}

void renderPointSparks(){
    while(!interrupted){
        for(int p=0;p<PIXELS;p++){
            pixels[p][0] *= 0.5;
            pixels[p][1] *= 0.5;
            pixels[p][2] *= 0.5;
        }
        hsl(pixels[random(PIXELS)], hue, saturation, light);
        showArray(pixels);
        delay(100);
    }
}

void renderFlatPulsate(){
    int frame = 0;
    while(!interrupted){
        for(int p=0;p<PIXELS;p++){
            hsl(pixels[p], hue, saturation, light * 0.25 * (sin(2 * PI * (float)frame / 100) + 1));
        }
        frame = (frame + 1) % 100;
        showArray(pixels);
        delay(100);
    }
}

void renderLinePulsate(){
    int frame = 0;
    while(!interrupted){
        for(int p=0;p<PIXELS;p++){
            hsl(pixels[p], hue, saturation, light * 0.25 * (sin(2 * PI * (float)(frame+p) / PIXELS) + 1));
        }
        frame = (frame + 1) % PIXELS;
        showArray(pixels);
        delay(100);
    }
}

void renderLineMove(){
    int frame = 0;
    while(!interrupted){
        for(int p=0;p<PIXELS;p++){
            if(sin(2 * PI * (float)(frame+p) / 100) < 0){
                hsl(pixels[p], 0, 0, 0);
            }else{
                hsl(pixels[p], hue, saturation, light);
            }
        }
        frame = (frame + 1) % PIXELS;
        showArray(pixels);
        delay(100);
    }
}

void renderPointsMove(){
    int frame = 0;
    while(!interrupted){
        for(int p=0;p<PIXELS;p++){
            hsl(pixels[p], 0, 1, 0.25 * (sin(5 * 2 * PI * (float)(frame+p) / 100) + 1));
        }
        frame = (frame + 1) % PIXELS;
        showArray(pixels);
        delay(100);
    }
}

void renderLineBounce(){
    
}

void renderPointsBounce(){
    
}

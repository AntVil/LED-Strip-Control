#define PIXELS 100

String state;

void setup() {
    setupStrip();
    setupRender();
    setupIO();
}


void loop() {
    if(state == "POWER"){
        renderFlatRainbow();
    }else{
        renderNone();
    }
    resetInterrupted();
}

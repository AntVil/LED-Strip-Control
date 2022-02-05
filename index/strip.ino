// These values depend on which pin your string is connected to and what board you are using 
// More info on how to find these at http://www.arduino.cc/en/Reference/PortManipulation

// These values are for the pin that connects to the Data Input pin on the LED strip. They correspond to...

// Arduino Yun:     Digital Pin 8
// DueMilinove/UNO: Digital Pin 12
// Arduino MeagL    PWM Pin 4

// You'll need to look up the port/bit combination for other boards. 

// Note that you could also include the DigitalWriteFast header file to not need to do this lookup.

#define PIXEL_PORT  PORTB  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRB   // Port of the pin the pixels are connected to
#define PIXEL_BIT   4      // Bit of the pin the pixels are connected to

// These are the timing constraints taken mostly from the WS2812 datasheets 
#define T1H  900    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns
#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns
#define RES 250000    // Width of the low gap between bits to cause a frame to latch


#define NS_PER_SEC (1000000000L)
#define CYCLES_PER_SEC (F_CPU)
#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )
#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )


inline void sendBit(bool bitVal){
    if(bitVal){   // 0 bit
        asm volatile(
            "sbi %[port], %[bit] \n\t"
            ".rept %[onCycles] \n\t"
            "nop \n\t"
            ".endr \n\t"
            "cbi %[port], %[bit] \n\t"
            ".rept %[offCycles] \n\t"
            "nop \n\t"
            ".endr \n\t"
            ::
            [port]    "I" (_SFR_IO_ADDR(PIXEL_PORT)),
            [bit]   "I" (PIXEL_BIT),
            [onCycles]  "I" (NS_TO_CYCLES(T1H) - 2),
            [offCycles]   "I" (NS_TO_CYCLES(T1L) - 2)
        );
    }else{        // 1 bit
        cli();
        asm volatile(
          "sbi %[port], %[bit] \n\t"
          ".rept %[onCycles] \n\t"
          "nop \n\t"
          ".endr \n\t"
          "cbi %[port], %[bit] \n\t"
          ".rept %[offCycles] \n\t"
          "nop \n\t"
          ".endr \n\t"
          ::
          [port]    "I" (_SFR_IO_ADDR(PIXEL_PORT)),
          [bit]   "I" (PIXEL_BIT),
          [onCycles]  "I" (NS_TO_CYCLES(T0H) - 2),
          [offCycles] "I" (NS_TO_CYCLES(T0L) - 2)
        );
        sei();
    }
}  

  
inline void sendByte(unsigned char byte){
    for(unsigned char bit=0;bit<8;bit++){
        sendBit(bitRead(byte, 7));
        byte <<= 1;
    }
}


void setupStrip(){
    bitSet(PIXEL_DDR, PIXEL_BIT);
}

inline void sendPixel(unsigned char r, unsigned char g , unsigned char b){  
    sendByte(g);
    sendByte(r);
    sendByte(b);
}

void show(){
    _delay_us((RES / 1000UL) + 1);
}


void showArray(int arr[][3]){
    for(int p=0;p<PIXELS;p++){
        sendPixel(arr[p][0] ,arr[p][1] , arr[p][2]);
    }
    show();
}


void hsl(int *pixel, float h, float s, float l){
    float r = max(0, (cos(h / 360 * 2 * PI) + 0.5) / 1.5);
    float g = max(0, (cos((h + 120) / 360 * 2 * PI) + 0.5) / 1.5);
    float b = max(0, (cos((h + 240) / 360 * 2 * PI) + 0.5) / 1.5);

    r = min(1, r * l);
    g = min(1, g * l);
    b = min(1, b * l);
  
    float average = (r + g + b) / 3;
    r = r * s + average * (1 - s);
    g = g * s + average * (1 - s);
    b = b * s + average * (1 - s);
  
    pixel[0] = (int)(255 * r);
    pixel[1] = (int)(255 * g);
    pixel[2] = (int)(255 * b);
}

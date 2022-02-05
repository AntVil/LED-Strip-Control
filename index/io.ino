#include <IRremote.hpp>

bool interrupted = false;

void setupIO(){
    //Serial.begin(9600);
    IrReceiver.begin(2, false);
    attachInterrupt(digitalPinToInterrupt(2), inter, CHANGE);
}

void resetInterrupted(){
    interrupted = false;
}

void inter(){
    if (IrReceiver.decode()) {
        switch(IrReceiver.decodedIRData.decodedRawData){
            case 0xBA45FF00: state = "POWER";         break;
            case 0xB847FF00: state = "FUNC/STOP";     break;
            case 0xB946FF00: increaseLight();         break;
            case 0xBB44FF00: decreaseHue();           break;
            case 0xBF40FF00: state = "PAUSE";         break;
            case 0xBC43FF00: increaseHue();           break;
            case 0xF807FF00: decreaseSaturation();    break;
            case 0xEA15FF00: decreaseLight();         break;
            case 0xF609FF00: increaseSaturation();    break;
            case 0xE619FF00: state = "EQ";            break;
            case 0xF20DFF00: state = "ST/REPT";       break;
            case 0xE916FF00: state = "0";             break;
            case 0xF30CFF00: state = "1";             break;
            case 0xE718FF00: state = "2";             break;
            case 0xA15EFF00: state = "3";             break;
            case 0xF708FF00: state = "4";             break;
            case 0xE31CFF00: state = "5";             break;
            case 0xA55AFF00: state = "6";             break;
            case 0xBD42FF00: state = "7";             break;
            case 0xAD52FF00: state = "8";             break;
            case 0xB54AFF00: state = "9";             break;
            //case 0x00000000: state = "REPEAT";               break;  
        }

        IrReceiver.resume();
        interrupted = true;
    }
}

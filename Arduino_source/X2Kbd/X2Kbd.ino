//
// X2 Kbd for PC
// by cloudree@naver.com
//

#include <Arduino.h>
#include <Keyboard.h>
#include "Common.h"
#include "Debug.h"
#include "X2Keyboard.h"
#include "Translate.h"
//#include "PCKeyboard.h"
#include "ps2dev.h"

void setup() 
{
    CX2Keyboard::GetInstance()->init();
    CTranslate::GetInstance()->init();
    //CPCKeyboard::GetInstance()->init();
    PS2dev::GetInstance()->keyboard_init();
}

void loop() 
{
    CX2Keyboard::GetInstance()->process();
    CTranslate::GetInstance()->process();
    //CPCKeyboard::GetInstance()->process();
    //Handle PS2 communication and react to keyboard led change
    //This should be done at least once each 10ms
    unsigned char leds;
    if(PS2dev::GetInstance()->keyboard_handle(&leds)) {
        Serial.print('LEDS');
        Serial.print(leds, HEX);
        digitalWrite(LED_BUILTIN, leds);
    }
}

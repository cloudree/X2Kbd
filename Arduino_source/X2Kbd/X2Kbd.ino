//
// X2 Kbd for PC
// by cloudree@naver.com
//

#include <Arduino.h>

#include "Common.h"
#include "Debug.h"
#include "X2Keyboard.h"
#include "Translate.h"
//#include "PCKeyboard.h"

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
}

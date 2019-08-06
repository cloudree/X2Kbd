//
// PC Keyboard
// by cloudree@naver.com
//

#include <Arduino.h>
#include <Keyboard.h>

#include "Common.h"
#include "PCKeyboard.h"

static CPCKeyboard    CPCKeyboard::m_instance;

void CPCKeyboard::init()
{
    Keyboard.begin();
}

void CPCKeyboard::process()
{
    
}

void CPCKeyboard::presskey( BYTE ch )
{
    Keyboard.press( ch );
}

void CPCKeyboard::releasekey( BYTE ch )
{
    Keyboard.release( ch );  
}

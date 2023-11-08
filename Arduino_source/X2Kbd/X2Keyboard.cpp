//
// X2 Keyboard for PC
// by cloudree@naver.com
//

#include <Arduino.h>

#include "Common.h"
#include "Debug.h"
#include "X2Keyboard.h"

static CX2Keyboard    CX2Keyboard::m_instance;

void CX2Keyboard::init() 
{
    pinMode( m_pinCAPS, OUTPUT );
    pinMode( m_pinCODE, OUTPUT );
    pinMode( m_pinCLK, OUTPUT );

    digitalWrite( m_pinCAPS, HIGH );
    digitalWrite( m_pinCODE, HIGH );
    digitalWrite( m_pinCLK, LOW );
}

void CX2Keyboard::process() 
{    
    for( int XS = 0; XS < sizeX; XS ++ )
    {
        digitalWrite( m_pinCLK, LOW );
        
        for( int i = 0; i < sizeS; i ++ )
        {
            pinMode( m_pinS[i], OUTPUT );
        }
         
        digitalWrite( m_pinS[0], ((XS & 1) > 0) ? HIGH : LOW );
        digitalWrite( m_pinS[1], ((XS & 2) > 0) ? HIGH : LOW );
        digitalWrite( m_pinS[2], ((XS & 4) > 0) ? HIGH : LOW );
        digitalWrite( m_pinS[3], ((XS & 8) > 0) ? HIGH : LOW );
        
        delay(1);
        digitalWrite( m_pinCLK, HIGH );   //trigger to get result
        
        m_readVal[ XS ] = 0;
        for( int i = 0; i < sizeY; i ++ )
        {
            pinMode( m_pinY[i], INPUT_PULLUP );
            m_readVal[ XS ] |= ( digitalRead( m_pinY[i] ) == HIGH ? 0 : 1 );
            if( i != 7 )
                m_readVal[ XS ] <<= 1;      // 0 MSB ... 7 LSB
        }
        DBG(">x2:");
        Dump( sizeX, m_readVal );
    }
}

void CX2Keyboard::SetLed_CAPS(bool isOn)
{
    digitalWrite( m_pinCAPS, isOn ? LOW : HIGH );
}
void CX2Keyboard::SetLed_CODE(bool isOn)
{
    digitalWrite( m_pinCODE, isOn ? LOW : HIGH );
}

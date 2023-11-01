//
// PC Keyboard
// by cloudree@naver.com
//

#include <Arduino.h>
#include <Keyboard.h>

#include "Common.h"
#include "Debug.h"
#include "X2Keyboard.h"
//#include "PCKeyboard.h"
#include "Translate.h"

static CTranslate    CTranslate::m_instance;

enum Translate_Code {
    TRANS_NORMAL  = 0,
    TRANS_SHIFT   = 1,
    TRANS_MAX,
};

#define KEY_PRTSCR    (0x46+136)
#define KEY_SCRLK     (0x47+136)
#define KEY_PAUSE     (0x48+136)

#define MOD_SHIFT_VAL   0x4000
#define MS              MOD_SHIFT_VAL + 

static int trans [TRANS_MAX] [sizeX] [sizeY] = {
  {   // normal
    { '0', '1', '2', '3', '4', '5', '6', '7' },
    { '8', '9', '-', '^', '\\', '@', '[', ';' },
    { ':', ']', ',', '.', '/',   0, 'a', 'b' },
    { 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' },
    { 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r' },
    { 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' },
    { 0,          KEY_LEFT_CTRL,    KEY_LEFT_ALT, KEY_CAPS_LOCK,  KEY_RIGHT_ALT,  KEY_F1,         KEY_F2,         KEY_F3 },
    { KEY_F4,     KEY_F5,           KEY_ESC,      KEY_TAB,        KEY_F11,        KEY_BACKSPACE,  KEY_F12,        KEY_RETURN },
    { ' ',        KEY_HOME,         KEY_END,      KEY_DELETE,     KEY_LEFT_ARROW, KEY_UP_ARROW,   KEY_DOWN_ARROW, KEY_RIGHT_ARROW },
    { KEY_SCRLK,  KEY_RIGHT_CTRL,   KEY_PRTSCR,   KEY_INSERT,     KEY_END,        KEY_DOWN_ARROW, KEY_PAGE_DOWN,  KEY_LEFT_ARROW },
    { 0,          KEY_RIGHT_ARROW,  KEY_HOME,     KEY_UP_ARROW,   KEY_PAGE_UP,    KEY_PAUSE,      KEY_RIGHT_ALT,  KEY_DELETE }
  },
  {   // shifted
    {   0, '!', '\"', '#', '$', '%', '&', '\'' },
    { '(', ')', '=', '~', '|', '`', '{', '+' },
    { '*', '}', '<', '>', '?', '_', 'A', 'B' },
    { 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' },
    { 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R' },
    { 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' },
    { 0,          KEY_LEFT_CTRL,    KEY_LEFT_ALT, KEY_CAPS_LOCK,  KEY_RIGHT_ALT,      KEY_F5,           KEY_F7,             KEY_F8 },
    { KEY_F9,     KEY_F10,          KEY_ESC,      KEY_TAB,        KEY_F11,            KEY_BACKSPACE,    KEY_F12,            KEY_RETURN },
    { ' ',        MS KEY_HOME,          MS KEY_END,   KEY_DELETE,       MS KEY_LEFT_ARROW,  MS KEY_UP_ARROW,    MS KEY_DOWN_ARROW,  MS KEY_RIGHT_ARROW },
    { KEY_SCRLK,  KEY_RIGHT_CTRL,       KEY_PRTSCR,   KEY_INSERT,       MS KEY_END,         MS KEY_DOWN_ARROW,  MS KEY_PAGE_DOWN,   MS KEY_LEFT_ARROW },
    { 0,          MS KEY_RIGHT_ARROW,   MS KEY_HOME,  MS KEY_UP_ARROW,  MS KEY_PAGE_UP,     KEY_PAUSE,          KEY_RIGHT_ALT,      KEY_DELETE }
  }
};

int shiftX = 6;
int shiftY_ = 0x80;

int capsX = 6;
int capsY = 4;

int hangulX = 6;
int hangulY = 3;

static unsigned char lastVal[ sizeX ]; 

void CTranslate::init()
{
    for( int i = 0; i < sizeX; i ++ )
        lastVal [i] = 0;
        
    m_isCAPS = false;
    m_isHANGUL = false;
    m_oldShifted = false;
    
    CX2Keyboard::GetInstance()->SetLed_CAPS( m_isCAPS );
    CX2Keyboard::GetInstance()->SetLed_CODE( m_isHANGUL );
}

void CTranslate::CheckToggleCAPS( int x, int y )
{
    if( x == capsX && y == capsY )
    {
        m_isCAPS = ! m_isCAPS;
        CX2Keyboard::GetInstance()->SetLed_CAPS( m_isCAPS );
    }
}

void CTranslate::CheckToggleHANGUL( int x, int y )
{
    if( x == hangulX && y == hangulY )
    {
        m_isHANGUL = ! m_isHANGUL;
        CX2Keyboard::GetInstance()->SetLed_CODE( m_isHANGUL );
    }
}

bool CTranslate::IsPressed( int x, int y_ )
{
    BYTE val = CX2Keyboard::GetInstance()->Get( x );
    return ( (val & y_) != 0 );
}

void CTranslate::process()
{
    int shifted = IsPressed( shiftX, shiftY_ ) ? TRANS_SHIFT : TRANS_NORMAL;
    if( shifted != m_oldShifted )
    {
        if( shifted )
        {
            DBGLN("On");
            //CPCKeyboard::GetInstance()->presskey( KEY_LEFT_SHIFT );
        }
        else 
        {
            DBGLN("Off");
            //CPCKeyboard::GetInstance()->releasekey( KEY_LEFT_SHIFT );
        }
        m_oldShifted = shifted;
    }
    
    for( int x = 0; x < sizeX; x ++ )
    {
        int val = CX2Keyboard::GetInstance()->Get( x );
        if( lastVal [x] == val )
            continue;
            
        int b = 0x80;
        for( int y = 0; y < sizeY; y ++ )
        {
            int last = ( lastVal [x] & b ) > 0 ? 1 : 0;
            int curr = ( val & b ) > 0 ? 1 : 0;
            b >>= 1;
            
            int key = trans [shifted] [x] [y];
            if( key == 0 )
                continue;

            if( shifted && (last!=curr) && ( key & MOD_SHIFT_VAL ) == 0 )
            {
                //DBGLN("On2 " + String(x) + "," + String(y) + "," + String(key,HEX) );
                //CPCKeyboard::GetInstance()->releasekey( KEY_LEFT_SHIFT );
            }
            
            if( last == 1 && curr == 0 )
            {
                //CPCKeyboard::GetInstance()->releasekey( key );
            }
            else if( last == 0 && curr == 1 )
            {
                CheckToggleCAPS( x, y );
                CheckToggleHANGUL( x, y );
                //CPCKeyboard::GetInstance()->presskey( key );
            }

            if( shifted && (last!=curr) && ( key & MOD_SHIFT_VAL ) == 0 )
            {
                //DBGLN("Off2 " + String(x) + "," + String(y) + "," + String(key,HEX) );
                //CPCKeyboard::GetInstance()->presskey( KEY_LEFT_SHIFT );
            }
        }
        
        lastVal [x] = val;
    }
}

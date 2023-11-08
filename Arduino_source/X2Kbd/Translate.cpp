//
// PC Keyboard
// by cloudree@naver.com
//

#include <Arduino.h>
//#include <Keyboard.h>

#include "Common.h"
#include "Debug.h"
#include "X2Keyboard.h"
//#include "PCKeyboard.h"
#include "ps2scancode.h"
#include "ps2dev.h"
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

// mapping table from msx keyboard matrix to ps/2 scan code.
// https://www.msx.org/wiki/Keyboard_Matrices#Korean_matrix_for_Daewoo_CPC-300.2C_CPC-300E.2C_CPC-400_and_CPC-400S
// NOTE: see with horizontal flip!
// ps/2 keyboard send same scan code whether caps lock is on or not.
// bypass press and release if possible!
// TODO: Emulate PS/2 only keys
// ex. MSX didn't have F5. How to press F5? SHIFT+F1? Then, how to press SHIFT+F5? So, we need "FN" key! When FN+F1..5 on MSX will be F5..10 on PC.
static int msx_keyboard_matrix[sizeX][sizeY] = {
  // 0               1!                       2"                3#              4$              5%              6&                      7'
  { PS2_0,           PS2_1,                   PS2_2,            PS2_3,          PS2_4,          PS2_5,          PS2_6,                  PS2_7           },
  // 8(              9)                       -=                ^~              \|              @'              [{                      ;+
  { PS2_8,           PS2_9,                   PS2_MINUS,        0,              PS2_BACKSLASH,  0,              PS2_LEFT_ANGLE_BRACKET, PS2_SEMICOLON   },
  // :*              ]}                       ,<                .<              /?              N/A             aA                      bB
  { 0,               PS2_RIGHT_ANGLE_BRACKET, PS2_COMMA,        PS2_PERIOD,     PS2_SLASH,      0,              PS2_A,                  PS2_B           },
  // cC              dD                       eE                fF              gG              hH              iI                      jJ
  { PS2_C,           PS2_D,                   PS2_E,            PS2_F,          PS2_G,          PS2_H,          PS2_I,                  PS2_J           },
  // kK              lL                       mM                nN              oO              pP              qQ                      rR
  { PS2_K,           PS2_L,                   PS2_M,            PS2_N,          PS2_O,          PS2_P,          PS2_Q,                  PS2_R           },
  // sS              tT                       uU                vV              wW              xX              yY                      zZ
  { PS2_S,           PS2_T,                   PS2_U,            PS2_V,          PS2_W,          PS2_X,          PS2_Y,                  PS2_Z           },
  // shift?          Ctrl                     Graph             Caps            Korean          F1              F2                      F3
  { 0,               PS2_LEFT_CTRL,           PS2_LEFT_ALT,     PS2_CAPS_LOCK,  PS2_RIGHT_ALT,  PS2_F1,         PS2_F2,                 PS2_F3          },
  // F4              F5                       Esc               Tab             Stop            Backspace       Select                  Return
  { PS2_F4,          PS2_F5,                  PS2_ESC,          PS2_TAB,        PS2_F11,        PS2_BACKSPACE,  PS2_F12,                PS2_ENTER       },
  // Space           Home                     Ins               Sup             Left            Up              Down                    Right
  { PS2_SPACE,       PS2_HOME,                PS2_INSERT,       PS2_DELETE,     PS2_LEFT_ARROW, PS2_UP_ARROW,   PS2_DOWN_ARROW,         PS2_RIGHT_ARROW },
  // *               +                        /                 0               1               2               3                       4
  { PS2_KP_ASTERISK, PS2_KP_PLUS,             PS2_KP_SLASH,     PS2_KP_0,       PS2_KP_1,       PS2_KP_2,       PS2_KP_3,               PS2_KP_4        },
  // 5               6                        7                 8               9               -               ,                       .
  { PS2_KP_5,        PS2_KP_6,                PS2_KP_7,         PS2_KP_8,       PS2_KP_9,       PS2_KP_MINUS,   PS2_COMMA,              PS2_PERIOD      }
};

#if 0
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
#endif

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
#if 0
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
#endif
    for( int x = 0; x < sizeX; x ++ )
    {
        int val = CX2Keyboard::GetInstance()->Get( x ); // 0..10
        if( lastVal [x] == val )
            continue;
            
        int b = 0x80;
        for( int y = 0; y < sizeY; y ++ ) // 0..7
        {
            int last = ( lastVal [x] & b ) > 0 ? 1 : 0;
            int curr = ( val & b ) > 0 ? 1 : 0;
            b >>= 1;
            
            int key = msx_keyboard_matrix[x][y];
            DBG("<x2: row=");DBG(x);DBG("bit=");DBG(x);DBG("->key=");DBGHEXLN(key);
#if 0
            int key = trans [shifted] [x] [y];
#endif
            if( key == 0 )
                continue;

            if( last == 1 && curr == 0 ) {
              if (key & PS2_PREFIX_SPECIAL) {
                DBG("*release special:");DBGHEXLN(key & 0xff);
                PS2dev::GetInstance()->keyboard_release_special(key & 0xff); // use lower byte
              } else {
                DBG("*release:");DBGHEXLN(key);
                PS2dev::GetInstance()->keyboard_release(key);
              }
            }
            else if( last == 0 && curr == 1 ) {
              if (key & PS2_PREFIX_SPECIAL) {
                DBG("*press special:");DBGHEXLN(key & 0xff);
                PS2dev::GetInstance()->keyboard_press_special(key & 0xff); // use lower byte
              } else {
                DBG("*press:");DBGHEXLN(key);
                PS2dev::GetInstance()->keyboard_press(key);
              }
            }

#if 0
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
#endif
        }
        
        lastVal [x] = val;
    }
}

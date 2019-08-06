# X2Kbd
Use the Daewoo MSX2 X-2 (CPC-400) Keyboard with Korean Modern PC (via arduino pro micro USB Port)

# copyright
GNU GPL

# Usage : See documents at here (Korean) : https://cafe.naver.com/msx/19349

1. prepare Daewoo MSX X-2 (CPC-400) Keyboard
  * disassemble it and remove cable (DIN 13 pin ~ Unknown 14 pin cable)

2. set up a arduino pro micro with source (from this github)

3. connect betwwen the arduino and CPC-300 Keyboard's 14 pin connector of inner PCB
  a. connection map
    * Arduino = 14pin connector of PCB in keyboard
    * D2 = 1 (CAPS)
    * D3 = 2 (CODE/HAN)
    * D4 = 3 (CLK)
    * D5 = 4 (D2)
    * D6 = 5 (D1)
    * D7 = 6 (D0)
    * D8 = 7 (D3)
    * D9 = 8 (D4)
    * D10 = 9 (D5)
    * D16 = 10 (D6)
    * D14 = 11 (D7)
    * VCC = 12 (VCC)
    * GND = 13 (GND)
    * GND = 14 (GND)

4. assemble keyboard and plug into PC USB

5. Special Key Mapping
    * all key is functional as "key cap described" 
      eg) Shift + 2 is " (double quotation) in CPC-300 keyboard, so this key is act as " (not pc keyaboard's @)
    * some special key (no existed in PC Keyboard) is mapped as :
      
    * CPC-300 Keyboard = PC
    * Graph = ALT
    * STOP = F11
    * SELECT = F12
    * INS = END
    * Shift + DEL = INS
    * numeric 0 = INS
    * numeric 1 = END
    * numeric 2 = cursor down
    * numeric 3 = PgDn
    * numeric 4 = cursor left
    * numeric 5 = (none)
    * numeric 6 = cursor right
    * numeric 7 = HOME
    * numeric 8 = cursor up
    * numeric 9 = PgDn
    * numeric / = PrtScr
    * numeric * = ScrLck
    * numeric - = PAUSE
    * numeric + = Korean-Chinese Translate (한자변환)
    * numeric . = DEL
    * numeric , = Korean-English Translate (한영전환)
      

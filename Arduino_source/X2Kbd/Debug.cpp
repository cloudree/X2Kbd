/*
 *  Debug.cpp in X2Kbd
 *  by cloudree@naver.com
 *  
 */

#include <Arduino.h>
#include <String.h>

#include "Common.h"
#include "Debug.h"

#ifdef _DEBUG

void DebugInit()
{
#ifdef _DEBUG
    Serial.begin( SERIAL_SPEED);
    // wait until teminal action
    //while ( !Serial.available() )
    //    continue;
    Serial.println( String(PRODUCT) + String(" ") + String(VERSION) );
#endif
}

void DBG(String msg)          { Serial.print(msg); }
void DBGHEX(long int data)    { Serial.print(data, HEX); Serial.print(" "); }
void DBG(int num)             { Serial.print(num); Serial.print(" "); }
void DBGLN(String msg)        { Serial.println(msg); }
void DBGHEXLN(long int data)  { Serial.print(data, HEX); Serial.println(" "); }
void DBGLN(int num)           { Serial.print(num); Serial.println(" "); }

void Dump(uint8_t len, uint8_t *buf)
{
  	for (int i = 0; i<len; i++) {
    		Serial.print(buf[i], HEX);
        if( i % 32 == 31 )
            Serial.println("");
        else
    		    Serial.print(",");
  	}
  	Serial.println("");
}
void WAIT(String msg)
{
    DBGLN(msg);
    while (Serial.read() <= 0) ;
}

#endif  // _DEBUG

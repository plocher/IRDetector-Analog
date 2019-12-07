/*
 Arduino based proximity sensor
 Based very loosely on work done by Dustin Andrews 2012


 Copyright (c) 2014 John Plocher, released under the terms of the MIT License (MIT)
 
 */
#include <elapsedMillis.h>

#define SDEBUG
// #define SLOWDEBUG
#ifdef SLOWDEBUG
elapsedMillis t1;
#endif

#include "Detector.h"

Detector circuit[4];

void setup() 
{       
#ifdef DEBUG
    // Configure the serial port
    Serial.begin(19200);
    Serial.println("Analog IR Detector");
#ifdef SLOWDEBUG
    t1 = 0; 
#endif
#endif

    analogReference(EXTERNAL);
    circuit[0].init(0,2,6,14);
    circuit[1].init(1,3,7,15);
    circuit[2].init(2,4,8,16);
    circuit[3].init(3,5,9,17);
    pinMode(12,      OUTPUT); digitalWrite(12, LOW);
}

void loop() 
{
#ifdef SLOWDEBUG
//    if (t1 > 100) { // delay between checks to keep serial debug stream reasonable
//        for (int x = 0; x < 4; x++) {
//            circuit[x].check();
//        }
//        t1 = 0;
//    }
#else // no delay needed
//    for (int x = 0; x < 4; x++) {
//        circuit[x].check();
//    }
#endif
digitalWrite(12, HIGH);
circuit[0].check();
circuit[1].check();
digitalWrite(12, LOW);
//delay(10);
}


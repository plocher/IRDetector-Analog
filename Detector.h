#ifndef _CIRCUIT_
#define _CIRCUIT_
#include <elapsedMillis.h>

#define DEBUG

#define NOISEFLOOR 10
#define HYSTERESIS 2000    // in mS units, 2000 = 2.0 seconds

#define OCCUPIED LOW
#define EMPTY    ~OCCUPIED

#include <Arduino.h>

class Detector {
public:
    Detector(void) {};
    void init(int number, int ir, int out, int sensor) {
        num = number;
        irPin = ir;
        outPin = out;
        sensorPin = sensor;
        detected = false;
        delaytime = 0;
        pinMode(irPin,      OUTPUT);
        pinMode(outPin,     OUTPUT);
        pinMode(sensorPin,  INPUT);
#ifdef DEBUG
        digitalWrite(outPin,EMPTY); delay(100); digitalWrite(outPin,OCCUPIED); delay(100); 
#endif
        digitalWrite(outPin,EMPTY); // Leave the detection pin alone
        IROFF();
    };
    
    void IRON(void)  { digitalWrite(irPin, HIGH); }
    void IROFF(void) { digitalWrite(irPin, LOW); }
#define DLY 10

    int check(void) {
        int B,D,F,H, Delta;
            // Software PLL -> ON B OFF D  F ON H OFF
            IRON();  delayMicroseconds(DLY);   // turn on IR transmitter
            B =  analogRead(sensorPin);   // read active light intensity
            IROFF(); delayMicroseconds(DLY);   // turn off IR transmitter
            D =  analogRead(sensorPin);   // read ambient light intensity
                     delayMicroseconds(DLY);   // wait just a bit more
            F =  analogRead(sensorPin);   // read ambient light intensity again
            IRON();  delayMicroseconds(DLY);   // turn on IR transmitter
            H =  analogRead(sensorPin);   // read active light intensity
            IROFF();
    
            Delta = (B+H) - (D+F);
            
            if ((Delta < abs(B-H)) ||
                (Delta < abs(D-F)) ||
                (Delta < (-1 * NOISEFLOOR))    ) {
                  // error?
            }
        // Delta >> 0 = DETECTED
        // Delta < 0 = EMPTY
        
        if (Delta > NOISEFLOOR) {    // if a positive difference, something has been detected...
            delaytime = 0;           // expiration timer is reset every time detection is seen
            if (detected == false) { // newly triggered
#ifdef DEBUG
                Serial.print("ON  ");
                Serial.print("Delta = ");
                dumpnum(Delta);
                Serial.print(" = (");
                dumpnum(B);Serial.print("+ ");dumpnum(H);Serial.print(")-(");
                dumpnum(D);Serial.print("+ ");dumpnum(F);Serial.print(") = (");
                dumpnum(B+H);Serial.print(")-(");
                dumpnum(D+F);Serial.print(") ");
                Serial.print(detected ? "DETECT " : "------ ");
                Serial.print(delaytime, DEC);
                Serial.print(", ");
                Serial.print(HYSTERESIS);
                Serial.println();

#endif
            }
            detected = true;
        }
        if (detected) {
            if (delaytime < HYSTERESIS) {
                digitalWrite(outPin, OCCUPIED); 
            } else {
                digitalWrite(outPin, EMPTY); 
                detected = false;
#ifdef DEBUG
                Serial.print("OFF ");
                Serial.print("Delta = ");
                dumpnum(Delta);
                Serial.print(" = (");
                dumpnum(B);Serial.print("+ ");dumpnum(H);Serial.print(")-(");
                dumpnum(D);Serial.print("+ ");dumpnum(F);Serial.print(") = (");
                dumpnum(B+H);Serial.print(")-(");
                dumpnum(D+F);Serial.print(") ");
                Serial.print(detected ? "DETECT " : "------ ");
                Serial.print(delaytime, DEC);
                Serial.print(", ");
                Serial.print(HYSTERESIS);
                Serial.println();
#endif
            }
        }
        return detected ? 1 : 0;
    };
    
private:  
    void dumpnum(int num) {
      if (num > 0) Serial.print("+");
      else if (num == 0) Serial.print(" ");
      else {
        Serial.print("-");
        num = -num;
      }
      for (int len = Serial.print(num, DEC); len < 4; len++) { Serial.print(" "); }
    }
    int num;
    elapsedMillis delaytime;
    int detected;
    int irPin;
    int outPin;
    int sensorPin;
};

#endif


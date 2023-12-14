#include "MotorWheel.h"
#include "Omni4WD.h"
#include "PID_Beta6.h"
#include "PinChangeInt.h"
#include "PinChangeIntConfig.h"

irqISR(irq1,isr1);
MotorWheel wheel1(3,2,14,15,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(11,12,4,5,&irq2);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,7,18,19,&irq4);

Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);


String pyInput;                                             // Input from python
char controllerInput = 0;                                   // Input from the coontroller.
unsigned int speed = 150;                                   // Speed of movement in milimeters per second
unsigned int speed_1 = 50;
unsigned int speed_2 = 50;
int s=0;

int sp_x = 75;

// Movement functions

void goAhead(unsigned int speedMMPS){
  
    analogWrite(3,sp_x); // Lower left -- Determiines ON or OFF
    digitalWrite(2,LOW); // HIGH=FORWARD -- LOW=BACKWARDS
     
    analogWrite(11,sp_x); // Upper Left -- Determines ON or OFF
    digitalWrite(12,LOW); //  HIGH=FORWARD -- LOW=BACKWARDS
    
    analogWrite(9,sp_x);   // UPPER right
    digitalWrite(8,HIGH); // LOW Forward -- HIGH BACKWARDS
    
    analogWrite(10,sp_x); // Lower Right Determines ON or OFF
    digitalWrite(7,HIGH); // LOW FORWARD - HIGH BACKWARDS
}

void backOff(unsigned int speedMMPS){
   
    analogWrite(3,speed_2); // Lower left -- Determiines ON or OFF
    digitalWrite(2,LOW); // HIGH=FORWARD -- LOW=BACKWARDS
     
    analogWrite(11,speed_2); // Upper Left -- Determines ON or OFF
    digitalWrite(12,LOW); //  HIGH=FORWARD -- LOW=BACKWARDS
    
    analogWrite(9,speed_2);   // UPPER right
    digitalWrite(8,HIGH); // LOW Forward -- HIGH BACKWARDS
    
    analogWrite(10,speed_2); // Lower Right Determines ON or OFF
    digitalWrite(7,HIGH); // LOW FORWARD - HIGH BACKWARDS 

}

void moveLeft(unsigned int speedMMPS){
    
    analogWrite(3,speed); // Lower left -- Determines ON or OFF
    digitalWrite(2,LOW); // HIGH=FORWARD -- LOW=BACKWARDS
     
    analogWrite(11,speed); // Upper Left -- Determines ON or OFF
    digitalWrite(12,HIGH); //  HIGH=FORWARD -- LOW=BACKWARDS
    
    analogWrite(9,speed);   // UPPER right
    digitalWrite(8,HIGH); // LOW Forward -- HIGH BACKWARDS
    
    analogWrite(10,speed); // Lower Right Determines ON or OFF
    digitalWrite(7,LOW); // LOW FORWARD - HIGH BACKWARDS
}

void moveRight(unsigned int speedMMPS){
    
    analogWrite(3,speed); // Lower left -- Determines ON or OFF
    digitalWrite(2,HIGH); // HIGH=FORWARD -- LOW=BACKWARDS
     
    analogWrite(11,speed); // Upper Left -- Determines ON or OFF
    digitalWrite(12,LOW); //  HIGH=FORWARD -- LOW=BACKWARDS
    
    analogWrite(9,speed);   // UPPER right
    digitalWrite(8,LOW); // LOW Forward -- HIGH BACKWARDS
    
    analogWrite(10,speed); // Lower Right Determines ON or OFF
    digitalWrite(7,HIGH); // LOW FORWARD - HIGH BACKWARDS
}



void rotateLeft(unsigned int speedMMPS){
  
    analogWrite(3,speed_2); // Lower left -- Determines ON or OFF
    digitalWrite(2,HIGH); // HIGH=FORWARD -- LOW=BACKWARDS
     
    analogWrite(11,speed_2); // Upper Left -- Determines ON or OFF
    digitalWrite(12,HIGH); //  HIGH=FORWARD -- LOW=BACKWARDS
    
    analogWrite(9,speed_2);   // UPPER right
    digitalWrite(8,HIGH); // LOW Forward -- HIGH BACKWARDS
    
    analogWrite(10,speed_2); // Lower Right Determines ON or OFF
    digitalWrite(7,HIGH); // LOW FORWARD - HIGH BACKWARDS
}

void rotateRight(unsigned int speedMMPS){
 
    analogWrite(3,speed_2); // Lower left -- Determines ON or OFF
    digitalWrite(2,LOW); // HIGH=FORWARD -- LOW=BACKWARDS
     
    analogWrite(11,speed_2); // Upper Left -- Determines ON or OFF
    digitalWrite(12,LOW); //  HIGH=FORWARD -- LOW=BACKWARDS
    
    analogWrite(9,speed_2);   // UPPER right
    digitalWrite(8,LOW); // LOW Forward -- HIGH BACKWARDS
    
    analogWrite(10,speed_2); // Lower Right Determines ON or OFF
    digitalWrite(7,LOW); // LOW FORWARD - HIGH BACKWARDS
}

void allStopModified(void){
    if(Omni.getCarStat()!=Omni4WD::STAT_STOP) Omni.setCarSlow2Stop(300);
      Omni.setCarStop();
    digitalWrite(3,LOW);
    digitalWrite(11,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
}


void setup() {
    Serial.begin(19200);

    TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
    TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz

    Omni.PIDEnable(6.0,1,0.1,1);
}

void loop() {  
   if (Serial.available() > 0) {
    // read the incoming byte:
    //controllerInput=Serial.read(); //Changes here:
    pyInput = Serial.readStringUntil('\n');
      if (pyInput == "x"){
        controllerInput = 'x';
//        Serial.write("Stopped");
      }
      if (pyInput == "w"){
        controllerInput = 'w';
//        Serial.write("Forward");
      }
      if (pyInput == "s"){
        controllerInput = 's';
      }
      if (pyInput == "a"){
        controllerInput = 'a';
      }
      if (pyInput == "d"){
        controllerInput = 'd';
      }
    }

    switch (controllerInput){
	case ('w'):
	    goAhead(speed); // move forward
	    break;
	case ('s'):
	    backOff(speed); // move backwards
	    break;
	case ('d'):
	    moveRight(speed); // move right
	    break;
	case ('a'):
	    moveLeft(speed); // move left
	    break;
	case ('e'):
	    rotateRight(speed); // rotate right
	    break;
	case ('q'):
	    rotateLeft(speed); // rotate left
	    break;
	case ('x'):  
	    allStopModified(); // stop all movement
	    break;
	}

Serial.print(wheel1.getSpeedRPM(),DEC);
Serial.print("\t"); 
Serial.print(wheel1.getSpeedMMPS(),DEC); 
Serial.print("\t");  
Serial.print(wheel2.getSpeedRPM(),DEC);
Serial.print("\t"); 
Serial.print(wheel2.getSpeedMMPS(),DEC); 
Serial.print("\t");  
Serial.print(wheel3.getSpeedRPM(),DEC);
Serial.print("\t");
Serial.print(wheel3.getSpeedMMPS(),DEC); 
Serial.print("\t");   
Serial.print(wheel4.getSpeedRPM(),DEC);
Serial.print("\t");  
Serial.print(wheel4.getSpeedMMPS(),DEC); 
Serial.println("");
}

/*
 * Code for custom wireless controler for a 1/3rd Scale BB9E, arduino reads value from the 
 * Wii Nunchuck via I2C and transmits using HC-05 Bluetooth as master via UART
 * Created by: Alwurts alejandro.wurtsss@udlap.mx
 * Date: 26/04/18
 * Libraries: Wiichuck library by jnw.walker@gmail.com  / https://github.com/coopermaa/Wiichuck
 * 
 */

#include <Wiichuck.h>     
#include <Wire.h>         // Allows communication with I2C devices

Wiichuck wii;

byte modeSelect = 1;       // Variable to store mode being used


void setup() {
  Serial.begin(9600);     // Starts serial for communication with HC-05
  wii.init();             // Start Command
  wii.calibrate();        // Calibration Command
  
}

void loop() {

  
  
  if (wii.poll()) {                 // Checks if wiichuck is transmitting

    if (wii.buttonZ()==1 and wii.buttonC()==1){    // If both wiichuchk button pressed calibrate accelerometer
       wii.calibrate();
       //Serial.println("calibrated");
    }
    if (wii.buttonZ()==1 and wii.buttonC()==0){    // Mode 1 (Joystick Drive only), mapped to Z button
       modeSelect= 0;
       wii.calibrate();
       //Serial.println("mode 1");
       //Serial.println(modeSelect);
    }
    if (wii.buttonC()==1 and wii.buttonZ()==0){    // Mode 2 (Full drive and head movement), mapped to C
       modeSelect= 1;
       //Serial.println("mode 2");
       //Serial.println(modeSelect);
    }
    
    
    //if(Serial.available()>0){       // Communication are send in format (Main Motor):(Pendulum):(Head X):(Head Y)/
                                
      if(modeSelect==1){            // This mode only runs BB9E with joystick from wiichuck 
        //Serial.println("mode 1");
        //Serial.println(modeSelect);
        Serial.print((wii.joyY())*2.5);   // Main Motor Value / Better to code value to -255 - 255 in here to save processing in robot
        Serial.print(":");
        Serial.print(((wii.joyX())+100)*.8);   // Pendulum Servo Value / Better to code value to 30 - 150 in here to save processing in robot
        Serial.print(":");
        Serial.print(90);           // Head x Servo Value (Static in this mode)
        Serial.print(":");
        Serial.print(90);           // Head y Servo Value (Static in this mode)
        Serial.println("/");
        
      } 
      if (modeSelect==0){    // This mode maps head movement to x,y acceleration from wiichuck
        //Serial.println("mode 2");
        //Serial.println(modeSelect);
        Serial.print((wii.joyY())*2.5);   // Main Motor Value
        Serial.print(":");
        Serial.print(((wii.joyX())+100)*.8);   // Pendulum Servo Value
        Serial.print(":");
        Serial.print(((wii.accelX())+100)*.8);           // Head x Servo Value mapped to x accelerometer
        Serial.print(":");          
        Serial.print(((wii.accelY())+100)*.8);           // Head y Servo Value mapped to y accelerometer
        Serial.println("/");
      }
    //}
  }
  //delay(500);
}

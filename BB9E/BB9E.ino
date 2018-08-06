/*
 * Code for custom 1/3rd Scale BB9E, arduino reads value from the HC-06 Bluetooth via UART 
 * Movement of sphere drive mechanism is a Non-Holonimic System
 * Motor main drive consists of Main motor for Forwards/Backwards and Pendulumn Servo for Left/Right
 * Head movement consists of two servo motor to move the head in the top half of the sphere
 * Created by: Alwurts alejandro.wurtsss@udlap.mx
 * Date: 22/03/18
 * Libraries: Servo Arduino Library
 */

// Libraries to be used
 
#include <Servo.h>

// Variables 

String dataReceived;        // String to store value received through UART from HC-06     

Servo pendulumServo, headServoX, headServoY;   // Creates Servo objects to wich we write values.

// Actuators position variables
int mainMotorValue = 0;     // Main Motor value initialized to static robot position
int pendulumValue = 81;     // Pendulumn Servo value initialized to static robot position
int HeadXValue = 90;        // Head X Servo value initialized to static robot position
int HeadYValue = 90;        // Head Y Servo value initialized to static robot position


// Pinout of devices connected to the arduino nano

int motorInput3 = 8;    // Input3 of 1293d connected to pin 8 of arduino as output 
int motorInput4 = 7;    // Input4 of 1293d connected to pin 7 of arduino as output
int enableInput = 5;    // Enable input of l293d connected to pin 5 of arduino as output, must be pwm capable

int pendulumPin = 11;   // Pin to set as output for Pendulumn Servo, must be pwm capable
int headXPin = 10;      // Pin to set as output for Head Servo X, must be pwm capable
int headYPin = 9;       // Pin to set as output for Head Servo Y, must be pwm capable

      
void setup()
{
  
    Serial.begin(9600);    // UART Communication with HC-06

    // Attach correct servo object to correct pwm output pin
    
    pendulumServo.attach(pendulumPin);
    headServoX.attach(headXPin);
    headServoY.attach(headYPin);

    // Set as output correct pins for the 1293d driver for the main motor
    
    pinMode (enableInput, OUTPUT); 
    pinMode (motorInput3, OUTPUT);
    pinMode (motorInput4, OUTPUT);
    
} 

void loop()
{
   if(Serial.available()>0)      
   {
      dataReceived = Serial.readStringUntil('/');   // Receive value through serial port until it finds "/" and assigns to string
      valuedecode(dataReceived);                    // Send value to the decoder function
      
      pendulumServo.write(pendulumValue);     // If value received is not 0 -180 you need to convert it
      headServoX.write(HeadXValue);           // If value received is not 0 -180 you need to convert it
      headServoY.write(HeadYValue);           // If value received is not 0 -180 you need to convert it

      /* We receive -255 - 255 from controller motor turns in different direction depending on
      *  Input 3 and Input 4
      *     1           0    =   Left
      *     0           1    =   Right
      *     1           1    =   Stop
      *     
      *  Then enable input controls speed with pwm output 0 - 255 
      */
      
      if (mainMotorValue > 0){      // If value received is less than 0 turn left
        
        digitalWrite (motorInput3, LOW);           
        digitalWrite (motorInput4, HIGH);
        analogWrite(enableInput,mainMotorValue);
        
        } else {                      // If value is greater than 0 turn right 
          
          digitalWrite (motorInput3, HIGH);
          digitalWrite (motorInput4, LOW);
          analogWrite(enableInput,abs(mainMotorValue));     // Speed value to be set must be converted to 0-255 with abs() function
          
        }
      }
   }

void valuedecode (String serialIn){      // Function to decode incoming value in format  (Main Motor):(Pendulum):(Head X):(Head Y)/

  // More values can be decoded by adding new delimitators
  int commaIndex = serialIn.indexOf(':');                           // First delimitator ":"
  int secondCommaIndex = serialIn.indexOf(':', commaIndex+1);       // Second delimitator ":"
  int thirdCommaIndex = serialIn.indexOf(':', secondCommaIndex+1);  // Third delimitator ":
  
  mainMotorValue = serialIn.substring(0, commaIndex).toInt(); 
  pendulumValue = serialIn.substring(commaIndex+1, secondCommaIndex).toInt();
  HeadXValue = serialIn.substring(secondCommaIndex+1,thirdCommaIndex).toInt();
  HeadYValue = serialIn.substring(thirdCommaIndex+1).toInt();
  }


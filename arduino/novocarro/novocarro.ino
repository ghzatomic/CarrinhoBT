
#include "Arduino.h"
//#include "DCMDriverL298.h"
#include "NewPing.h"
#include "Servo.h"
#include <SoftwareSerial.h>

// Pin Definitions
#define BTHC05_PIN_TXD  9
#define BTHC05_PIN_RXD  10
#define INTA1  A2 
#define ENB 13 
#define INTA2  A3 
#define ENA 12 
#define INTB1  A4 
#define INTB2  A5 
#define trig 6 // trig pin for HC-SR04
#define echo 7 // echo pin for HC-SR04

#define servo_pin 8 

Servo servo_1; // servo controller (multiple can exist)

int pos = 0;    // servo starting position
float duration,distance;


// Global variables and defines
const int servo9gRestPosition   = 20;  //Starting position
const int servo9gTargetPosition = 150; //Position when event is detected
SoftwareSerial bthc05(BTHC05_PIN_RXD,BTHC05_PIN_TXD); // RX, TX // Lembrar de inverter no arduino .. onde ta tx vira rx
//DCMDriverL298 dcMotorDriverL298(DCMOTORDRIVERL298_PIN_ENA,DCMOTORDRIVERL298_PIN_INT1,DCMOTORDRIVERL298_PIN_INT2,DCMOTORDRIVERL298_PIN_ENB,DCMOTORDRIVERL298_PIN_INT3,DCMOTORDRIVERL298_PIN_INT4);
//NewPing hcsr04(HCSR04_PIN_TRIG,HCSR04_PIN_ECHO);


// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(57600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
      
    bthc05.begin(9600);
    bthc05.setTimeout(1);
    //servo9g.attach(SERVO9G_PIN_SIG);
    //servo9g.write(servo9gRestPosition);
    //delay(100);
    //servo9g.detach();
    //menuOption = menu();
    pinMode(ENA, OUTPUT);     
    pinMode(ENB, OUTPUT);     
    pinMode(INTA1, OUTPUT);     
    pinMode(INTA2, OUTPUT);     
    pinMode(INTB1, OUTPUT);     
    pinMode(INTB2, OUTPUT);


    servo_1.attach(servo_pin); // start servo control
    servo_1.write(servo9gRestPosition);
    delay(1000);
    servo_1.detach();
    pinMode(trig,OUTPUT);
    pinMode(echo,INPUT);
}


String comando = "";

void comandoEncontrado(String comando){
  Serial.println(comando);
  if (comando[0] == '1'){
    setMotorA(255,1);
    setMotorB(255,1);
  }else if (comando[1] == '1'){
    setMotorA(255,0);
    setMotorB(255,0);
  }else if (comando[2] == '1'){
    setMotorA(255,1);
    setMotorB(255,0);
  }else if (comando[3] == '1'){
    setMotorA(255,0);
    setMotorB(255,1);
  }else{
    stopMotors();
  }
  if (comando[4] == '1'){
    scan();
  }
  
}

void loop() 
{
    int aval = bthc05.available();
    char character;
    if (aval) {
      character = bthc05.read();
      if (character == '|'){
        //Serial.println(comando);
        comandoEncontrado(comando);
        comando = "";        
      }else{
        comando+=character;
      }  
    
    }
//   if(menuOption == '2') {
//    // L298N Motor Driver with Dual Micro DC Motors (Geared) - Test Code
//    //Start both motors. note that rotation direction is determined by the motors connection to the driver.
//    //You can change the speed by setting a value between 0-255, and set the direction by changing between 1 and 0.
//    dcMotorDriverL298.setMotorA(200,1);
//    dcMotorDriverL298.setMotorB(200,0);
//    delay(2000);
//    //Stop both motors
//    dcMotorDriverL298.stopMotors();
//    delay(2000);
//
//    }
//    else if(menuOption == '3') {
//    // Ultrasonic Sensor - HC-SR04 - Test Code
//    // Read distance measurment from UltraSonic sensor           
//    int hcsr04Dist = hcsr04.ping_cm();
//    delay(10);
//    Serial.print(F("Distance: ")); Serial.print(hcsr04Dist); Serial.println(F("[cm]"));
//
//    }
//    else if(menuOption == '4') {
//    // 9G Micro Servo - Test Code
//    // The servo will rotate to target position and back to resting position with an interval of 500 milliseconds (0.5 seconds) 
//    servo9g.attach(SERVO9G_PIN_SIG);         // 1. attach the servo to correct pin to control it.
//    servo9g.write(servo9gTargetPosition);  // 2. turns servo to target position. Modify target position by modifying the 'ServoTargetPosition' definition above.
//    delay(500);                              // 3. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
//    servo9g.write(servo9gRestPosition);    // 4. turns servo back to rest position. Modify initial position by modifying the 'ServoRestPosition' definition above.
//    delay(500);                              // 5. waits 500 milliseconds (0.5 sec). change the value in the brackets (500) for a longer or shorter delay in milliseconds.
//    servo9g.detach();                    // 6. release the servo to conserve power. When detached the servo will NOT hold it's position under stress.
//    }
//    
//    if (millis() - time0 > timeout)
//    {
//        menuOption = menu();
//    }
    
}

void setMotor(int pinPWM, int pinDir1, int pinDir2, int speed, bool dir)
{
  analogWrite(pinPWM, speed);
  digitalWrite(pinDir1, !dir);
  digitalWrite(pinDir2, dir);
}

void setMotorA(int speed, bool dir){
  setMotor(ENA, INTA1, INTA2, speed, dir);
}

void setMotorB(int speed, bool dir){
  setMotor(ENB, INTB1, INTB2, speed, dir);
}

void stopMotorA(){
 off(ENA, INTA1, INTA2);
}

void stopMotorB(){
  off(ENB, INTB2, INTB2);
}

void stopMotors(){
  stopMotorA();
  stopMotorB();
}

void off(int pinPWM, int pinDir1, int pinDir2){
  analogWrite(pinPWM, 0);
  digitalWrite(pinDir1, LOW);
  digitalWrite(pinDir2, LOW);
}



// ---------------------------------------------------------------------------

float dist_calc(int pos){
  // trigger 40kHz pulse for ranging
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  // convert from duration for pulse to reach detector (microseconds) to range (in cm)
  duration = pulseIn(echo,HIGH); // duration for pulse to reach detector (in microseconds)
  distance = 100.0*(343.0*(duration/2.0))/1000000.0; // 100.0*(speed of sound*duration/2)/microsec conversion
 
  Serial.print(pos); // position of servo motor
  Serial.print(","); // comma separate variables
  Serial.println(distance); // print distance in cm
}

void scan(){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo_1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(60); // delay to allow the servo to reach the desired position
    dist_calc(pos);
  }
 
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo_1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(60);
    dist_calc(pos);
  }
}


#include "Arduino.h"
//#include "DCMDriverL298.h"
#include "NewPing.h"
#include "Servo.h"
#include <SoftwareSerial.h>
#include <stdlib.h>

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
#define laserPin 4
#define servo_pin 8 
const int servo9gRestPosition   = 90;  //Starting position

Servo servo_1; // servo controller (multiple can exist)
char buff[10];
int pos = servo9gRestPosition;    // servo starting position
float duration,distance;


// Global variables and defines
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
    pinMode(laserPin, OUTPUT);    
    pinMode(INTB1, OUTPUT);     
    pinMode(INTB2, OUTPUT);


    servo_1.attach(servo_pin); // start servo control
    servo_1.write(servo9gRestPosition);
    delay(1000);
    //servo_1.detach();
    pinMode(trig,OUTPUT);
    pinMode(echo,INPUT);
}


String comando = "";

int modo = 0;
int modo_laser = false;
int lado_radar = 0;
bool radar_em_operacao = false;

int last_radar_pos = servo9gRestPosition;

void comandoEncontrado(String comando){
  Serial.println(comando);
  if (comando[0] == '1'){
    if (modo == 0){
      setMotorA(255,1);
      setMotorB(255,1);
    }
  }else if (comando[1] == '1'){
    if (modo == 0){
      setMotorA(255,0);
      setMotorB(255,0);
    }
  }else if (comando[2] == '1'){
    if (modo == 0){
      setMotorA(255,1);
      setMotorB(255,0);
    }else{
      lado_radar = 1 ;
      radar_em_operacao = true;
    }
  }else if (comando[3] == '1'){
    if (modo == 0){
      setMotorA(255,0);
      setMotorB(255,1);
    }else{
      lado_radar = 0;
      radar_em_operacao = true;
    }
  }else{
    stopMotors();
    stopRadar();
  }
  if (comando[4] == '1'){
    modo=1;
  }else{
    modo=0;
  }
  Serial.println(modo);
  if (comando[5] == '1'){
    enable_laser();
  }
  
}

void enable_laser(){
  if (modo_laser){
    modo_laser = false;
  }else{
    modo_laser = true;
  }
}

void loop() 
{
    gira_radar();
    fire(modo_laser);
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

char floatToStr(float val){
  char* arr;
  String value = String(val);
  strcpy( arr, value.c_str() );
  return *arr;
}

void fire(bool modo_laser_local){
  if (modo_laser_local){
    Serial.println("ATIRANDO");
    digitalWrite(laserPin, HIGH);
//    float dist = dist_calc();
//    char ret = floatToStr(dist)+';';
//    bthc05.write(ret);
  }else{
    digitalWrite(laserPin, LOW);
  }
}

// ---------------------------------------------------------------------------

float dist_calc(){
  // trigger 40kHz pulse for ranging
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  // convert from duration for pulse to reach detector (microseconds) to range (in cm)
  duration = pulseIn(echo,HIGH); // duration for pulse to reach detector (in microseconds)
  distance = 100.0*(343.0*(duration/2.0))/1000000.0; // 100.0*(speed of sound*duration/2)/microsec conversion
 
  Serial.println(distance); // print distance in cm
  return distance;
}

void gira_radar(){
  if (radar_em_operacao){
    if (lado_radar == 0){
      if (last_radar_pos+1 > 0 && last_radar_pos+1 < 180){
        last_radar_pos = last_radar_pos+1 ;
        servo_1.write(last_radar_pos); 
      }
    }else{
      if (last_radar_pos-1 > 0 && last_radar_pos-1 < 180){
        last_radar_pos = last_radar_pos-1 ;
        servo_1.write(last_radar_pos); 
      }
    }
    delayMicroseconds(5000);
    //float dist = dist_calc();
    //bthc05.write(floatToStr(dist));
  }
}

void stopRadar(){
  radar_em_operacao=false;
}

void scan(){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo_1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(60); // delay to allow the servo to reach the desired position
    dist_calc();
  }
 
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo_1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(60);
    dist_calc();
  }
}

#include <SoftwareSerial.h>
#include <NewPing.h>
#include <Servo.h>
#define ECHOPIN 22   // Pin to receive echo pulse
#define TRIGPIN 23   // Pin to send trigger pulse
#define SERVO 24   // Servo
#define MAXANGLE 180
#define PINLDR 0
#define PINLEDSMOOTH 9
#define PINLED 26
#define sensorTrazeiro 1

#define  margem 10
#define  ENA 2
#define  INA1 3
#define  INA2 4
#define  INB1 5
#define  INB2 6
#define  ENB 7
#define  tempoPasso 1
#define  led 13
int brightness = 0;    // quanto brilho terá o led
int fadeAmount = 5;    // how many points to fade the LED by

//int matriz[MAXANGLE+1][1] ;

Servo myservo;// cria um objeto do tipo myservo para acessar os metodos
// podem ser criados um maximo de 8 objetos do tipo servo
int pos = 0; // variavel para armazenar a posi��o do servo
NewPing sonar(TRIGPIN, ECHOPIN, 400); // NewPing setup of pins and maximum distance.

boolean debug = false;

int distanciaMaxDeObjCM = 40;

boolean sonarMais = true;
boolean modoSonar = false;
boolean naoDeixaIrSul = false;



boolean enableNorte = false;
boolean enableSul = false;

boolean enableLeste = false;
boolean enableOeste = false;

int tempoPadrao = 10;

SoftwareSerial bt(10,11); // RX, TX // Lembrar de inverter no arduino .. onde ta tx vira rx

void setup() { 
  if (debug){
    Serial.begin(9600);
  }
  bt.begin(9600);
  myservo.attach(SERVO);// atribui o pino 9 do arduino para ser controlado pelo objeto
  pinMode(ENA, OUTPUT);     
  pinMode(ENB, OUTPUT);     
  pinMode(INA1, OUTPUT);     
  pinMode(INA2, OUTPUT);     
  pinMode(INB1, OUTPUT);     
  pinMode(INB2, OUTPUT);     
  pinMode(led, OUTPUT);
  pinMode(PINLDR, INPUT); 
  pinMode(sensorTrazeiro, INPUT); 
  pinMode(PINLEDSMOOTH, OUTPUT); 
  pinMode(PINLED, OUTPUT); 
  zeraMatriz();
}

void loop() {
  ajustaLuz();
  digitalWrite(led,HIGH);
  char character;
  String comando = "";
  String tempo = "";
  boolean achou = false;
  while(bt.available()) {
    character = bt.read();
    if (character == ';'){
      achou = true;
    }
    if (character != ';'){
      if (!achou){
        comando.concat(character);
      }else{
        if (character != '\n'){
          tempo.concat(character);
        }
      }
    }
  }
  if (debug){Serial.println("Comando : "+comando);}
  if (comando != "") {
    if (debug){
      Serial.println("Comando : "+comando);
      Serial.println("tempo : '"+tempo+"'");
    }
  }
  char tempo_as_char[tempo.length()+2];
  tempo.toCharArray(tempo_as_char, tempo.length()+1);

  if(tempo != ""){
    if (debug){Serial.println("tempo2 : "+tempo+" - ");}
  }

  if (comando == "1"){//Vai
    modoSonar = false;
    bt.println(comando);
    if (tempo == "0"){
      enableSul = false;
      enableNorte = true;
      if (debug){Serial.println("Ativou norte");}
    }else{
      allOff();
      norte(atoi(tempo_as_char));
      enableSul = false;
      enableNorte = false;
    }
  }else if (comando == "2") {//sul
    modoSonar = false;
    bt.println(comando);
    if (tempo == "0"){
      enableSul = true;
      enableNorte = false;
      if (debug){Serial.println("Ativou sul");}
    }else{
      allOff();
      sul(atoi(tempo_as_char));
      enableSul = false;
      enableNorte = false;
    }
  }else if (comando == "3") {//oeste
    modoSonar = false;
    bt.println(comando);
    allOff();
    if (tempo == "0"){
      enableLeste = false;
      enableOeste = true;
    }else{
      oeste(atoi(tempo_as_char));
    }
  }else if (comando == "4") {//leste
    modoSonar = false;
    bt.println(comando);
    allOff();
    if (tempo == "0"){
      enableLeste = true;
      enableOeste = false;
    }else{
      leste(atoi(tempo_as_char));
    }
  }else if (comando == "5") {//stop
    bt.println(comando);
    allOff();
    enableNorte = false;
    enableSul = false;
    enableLeste = false;
    enableOeste = false;
    modoSonar = false;
  }else if (comando == "6") {//Modo Sonar on
    bt.println(comando);
    allOff();
    enableNorte = false;
    enableSul = false;
    enableLeste = false;
    enableOeste = false;
    modoSonar = true;
  }else if (comando == "7") {//Modo Sonar off
    bt.println(comando);
    allOff();
    enableNorte = false;
    enableSul = false;
    enableLeste = false;
    enableOeste = false;
    modoSonar = false;
    zeraMatriz();
    myservo.write(90);
  }else{
    if(enableNorte){
      bt.println("1");
      enableSul = false;
      enableLeste = false;
      enableOeste = false;
      allOff();
      norte(tempoPadrao);
    }else if(enableSul){
      bt.println("2");
      enableNorte = false;
      enableLeste = false;
      enableOeste = false;
      allOff();
      sul(tempoPadrao);
    }else if(enableLeste){
      bt.println("4");
      enableOeste = false;
      enableNorte = false;
      enableSul = false;
      allOff();
      leste(tempoPadrao);
    }else if(enableOeste){
      bt.println("3");
      enableNorte = false;
      enableSul = false;
      enableLeste = false;
      allOff();
      oeste(tempoPadrao);
    }else if (modoSonar){
      int cm = movimentaSonar();
    }
  }
}

void sul(int passos){
  digitalWrite(ENA, HIGH);   
  digitalWrite(ENB, HIGH);   
  digitalWrite(INA1, HIGH);   
  digitalWrite(INB2, HIGH);   
  delay(tempoPasso*passos);               
  digitalWrite(INA1, LOW);   
  digitalWrite(INB2, LOW);   
  digitalWrite(ENA, LOW);   
  digitalWrite(ENB, LOW);   
}

void norte(int passos){
  digitalWrite(ENA, HIGH);   
  digitalWrite(ENB, HIGH);   
  digitalWrite(INA2, HIGH);   
  digitalWrite(INB1, HIGH);   
  delay(tempoPasso*passos);               
  digitalWrite(INA2, LOW);   
  digitalWrite(INB1, LOW);   
  digitalWrite(ENA, LOW);   
  digitalWrite(ENB, LOW);   
}

void leste(int passos){
  digitalWrite(ENA, HIGH);   
  digitalWrite(ENB, HIGH);   
  digitalWrite(INA1, HIGH);   
  digitalWrite(INB1, HIGH);   
  delay(tempoPasso*passos);               
  digitalWrite(INA1, LOW);   
  digitalWrite(INB1, LOW);   
  digitalWrite(ENA, LOW);   
  digitalWrite(ENB, LOW);   
}

void oeste(int passos){
  digitalWrite(ENA, HIGH);   
  digitalWrite(ENB, HIGH);   
  digitalWrite(INA2, HIGH);   
  digitalWrite(INB2, HIGH);   
  delay(tempoPasso*passos);               
  digitalWrite(INA2, LOW);   
  digitalWrite(INB2, LOW);   
  digitalWrite(ENA, LOW);   
  digitalWrite(ENB, LOW);   
}

void allOff(){
  digitalWrite(INA2, LOW);   
  digitalWrite(INB2, LOW);   
  digitalWrite(INA1, LOW);   
  digitalWrite(INB1, LOW);   
  digitalWrite(ENA, LOW);   
  digitalWrite(ENB, LOW);
}

int8_t satoi( char *str, int len ){  
  int8_t r = 0;  
  for(int i=0; i<len; i++){  
    //Check if this is a number  
    if ( str[i] < 0x3a && str[i] > 0x2f){  
      // is a ASCII number, return it  
      r = r * 10;  
      r += (str[i]-0x30);  
    }
    else{  
      i = len; //exit!  
      r = -1;  
      break;  
    }    
  }  
  return r;   
}  
int8_t satoi( char *str ){  
  int8_t r = 0;  
  int len = strlen(str);  
  for(int i=0; i<len; i++){  
    //Check if this is a number  
    if ( str[i] < 0x3a && str[i] > 0x2f){  
      // is a ASCII number, return it  
      r = r * 10;  
      r += (str[i]-0x30);  
    }
    else{  
      i = len; //exit!  
      r = -1;  
      break;  
    }    
  }  
  return r;   
}  

long pingSensor(){
  //digitalWrite(TRIGPIN, HIGH);
  //delayMicroseconds(delayPing);
  //digitalWrite(TRIGPIN, LOW);
  //return pulseIn(ECHOPIN, HIGH);
  return sonar.ping();
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void zeraMatriz(){
  /*for(int i=0; i<=MAXANGLE; i++){  
    matriz[pos][1]=0;
  }*/
}

long movimentaSonar(){
  ajustaLuz();
  int traz = analogRead(sensorTrazeiro);
  long cm;
  cm = microsecondsToCentimeters(pingSensor());
  if (cm != 0){
    if (distanciaMaxDeObjCM >= cm){
      allOff();
      //oeste(valorMov);
      ajustaLuz();
      viraEmGraus(pos);
      ajustaLuz();
      if (analogRead(sensorTrazeiro) >=990){
        sul(cm*5);
      }
    }
  }
  myservo.write(pos);
  if (pos==MAXANGLE){
    sonarMais = false;
  }else if (pos==1){
    sonarMais = true;
  }
  if (!sonarMais){
    pos-=1;
  }else{
    pos+=1;
  }
  return cm;
}

void viraEmGraus(int grau){
  allOff();
  if (grau > 90){
    // Direita
    leste(grau);
  }else{
    // Esquerda
    oeste(grau);
  }
  allOff();
}

void ajustaLuz(){
  int photocellReading0 = analogRead(PINLDR);
  int sub = (photocellReading0 * 6)+30; 
  if (sub<140){
    analogWrite(PINLEDSMOOTH,(250-sub));
    digitalWrite(PINLED,HIGH);
  }else{
    analogWrite(PINLEDSMOOTH,0);
    digitalWrite(PINLED,LOW);
  }
    
}

int normalRelativeAngleDegrees(int angle) {
  return (angle %= 360) >= 0 ? (angle < 180) ? angle : angle - 360 : (angle >= -180) ? angle : angle + 360;
}


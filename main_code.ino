#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "IRremote.h"
#include "CppList.h"
#define ILERI 2
#define GERI  3
#define SAG   4
#define SOL   5
#define AUTO  7

bool _initialized = false;
int front = 0;
int right = 0;
int left = 0;
int rcv_count;
IRrecv **all_rcv;

SoftwareSerial ArduinoMega(13,12);
int echo=38;
int trig=39;
int sol_motor_f=2;
int sol_motor_b=3;
int sag_motor_f=5;
int sag_motor_b=6;
int sol_motor_en=4;
int sag_motor_en=7;
long sure, uzaklik;
int rs=53,en=49,d4=45,d5=52,d6=48,d7=44; //VSS-->gnd,VDD-->+5V,V0-->potansiyometrenin orta pini,RW-->gnd,A-->220R-->+5V,K,gnd,potansiyometre V tarafı gnd,L tarafı +5V
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
int giris=A0;
int yuzde=0;
double okunan;
double voltaj;
int i, j;

void setup(){

  Serial.begin(9600);
  ArduinoMega.begin(4800);
  pinMode(echo,INPUT);
  pinMode(trig,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(giris,INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();

  if (_initialized) return; 
  rcv_count = 3;
  all_rcv = (IRrecv **)malloc(rcv_count*sizeof(int));
  all_rcv[0] = new IRrecv(8); //8.pin on
  all_rcv[1] = new IRrecv(9); //9.pin sol
  all_rcv[2] = new IRrecv(10); //10.pin sag
    
  for (int i=0; i<rcv_count; ++i){
    all_rcv[i]->enableIRIn();
  }
  
  _initialized = true;
  
}
void pil(){
     voltaj = 0;
    for(i = 0; i < 1000 ; i++){
      okunan  = analogRead(giris);
      voltaj  += (okunan * 5) / 1024;
     }
  
   yuzde  =((voltaj*100)/5)/1000;
   Serial.print("voltaj:");Serial.print(voltaj/1000);
   Serial.print(" yuzde:");Serial.println(yuzde);
   lcd.setCursor(0,0);
   lcd.print("Doluluk: %");
   lcd.print(yuzde);
   //delay(1000);
     if(yuzde < 20){
      Serial.println("Pil Kritik");
     }
     else{       
      Serial.print(" [");
      Serial.print("|");
         for(i = 0; i < yuzde/10; i++)
            Serial.print("|");
         for(j = 0; j<10-i ; j++)
            Serial.print(" ");
            Serial.println("]");
     }
     if(yuzde<=20){
       lcd.setCursor(1,1);
       lcd.print("[ ||         ]");
     }
     else if (yuzde<=40){
       lcd.setCursor(1,1);
       lcd.print("[ ||||       ]");
     }
     else if(yuzde<=60){
      lcd.setCursor(1,1);
      lcd.print("[ ||||||     ]");
     }
      else if(yuzde<=80){
       lcd.setCursor(1,1);
       lcd.print("[ ||||||||   ]");
      }
      else{
       lcd.setCursor(1,1);
       lcd.print("[ |||||||||| ]");
      }
  
  //delay(1000);
}
void mesafe(){
  digitalWrite(trig,LOW);
 delayMicroseconds(10);
 digitalWrite(trig,HIGH);
  delayMicroseconds(15);
 digitalWrite(trig,LOW);
 sure =pulseIn(echo,HIGH);
 uzaklik = sure /29.1/2;
 }
 void geri(){ 

  digitalWrite(sag_motor_f, HIGH); 
  digitalWrite(sag_motor_b, LOW); 
  analogWrite(sag_motor_en, 150); 

  digitalWrite(sol_motor_b, HIGH); 
  digitalWrite(sol_motor_f, LOW); 
  analogWrite(sol_motor_en, 150); 
  Serial.println("Geri donuyor");
  
}  
      void sol(){ 

  digitalWrite(sag_motor_f, LOW); 
  digitalWrite(sag_motor_b, HIGH); 
  analogWrite(sag_motor_en, 100); 

  digitalWrite(sol_motor_f, HIGH); 
  digitalWrite(sol_motor_b, LOW); 
  analogWrite(sol_motor_en, 150); 
  Serial.println("Sola donuyor");
  
}  
 void sag(){ 

  digitalWrite(sag_motor_f, HIGH); 
  digitalWrite(sag_motor_b, LOW); 
  analogWrite(sag_motor_en, 150); 

  digitalWrite(sol_motor_f, LOW); 
  digitalWrite(sol_motor_b, HIGH); 
  analogWrite(sol_motor_en, 150); 
  Serial.println("Saga donuyor");
  
  
}  
  void ileri(){ 

  digitalWrite(sag_motor_f, LOW); 
  digitalWrite(sag_motor_b, HIGH); 
  analogWrite(sag_motor_en, 150); 

  digitalWrite(sol_motor_b, LOW); 
  digitalWrite(sol_motor_f, HIGH); 
  analogWrite(sol_motor_en, 150); 
  Serial.println("Ileri donuyor");
  
}  

void istasyon(int dly, int pwr){ 
  front=0; right=0; left=0;
    decode_results results;
    front=all_rcv[0]->decode(&results);
    right=all_rcv[1]->decode(&results);
    left=all_rcv[2]->decode(&results);
    
    if (front==1 && right==0 && left ==0) {
      Serial.println("onden veri alindi");
      ileri();
      right=0;front=0;left=0;
      all_rcv[0]->resume();
      all_rcv[1]->resume();
      all_rcv[2]->resume();        
    }
    else if (front==0 && right==1 && left==0) {
      Serial.println("sagdan veri alindi");
      sag();
      right=0;front=0;left=0;
      all_rcv[0]->resume();
      all_rcv[1]->resume();  
      all_rcv[2]->resume(); 
    }
    else if (front==0 && right==0 && left ==1) {
      Serial.println("soldan veri alindi");
      sol();
      right=0;front=0;left=0;
      all_rcv[0]->resume();
      all_rcv[1]->resume();  
      all_rcv[2]->resume();   
    }
    else if (front==1 && right==1 && left ==0) {
      Serial.println("onden ve sagdan veri alindi");
      ileri();
      right=0;front=0;left=0;
      all_rcv[0]->resume();
      all_rcv[1]->resume();  
      all_rcv[2]->resume();   
    }
    else if (front==1 && right==0 && left ==1) {
      Serial.println("onden ve soldan veri alindi");
      ileri();
      right=0;front=0;left=0;
      all_rcv[0]->resume();
      all_rcv[1]->resume();  
      all_rcv[2]->resume();   
    }
    else if (front==1 && right==1 && left ==1) {
      Serial.println("hepsinden veri alindi");
      ileri();
      right=0;front=0;left=0;
      all_rcv[0]->resume();
      all_rcv[1]->resume();  
      all_rcv[2]->resume();   
    }
    else if (front==0 && right==1 && left ==1) {
      Serial.println("soldan ve sagdan veri alindi");
      sag();
      right=0;front=0;left=0;
      all_rcv[0]->resume();
      all_rcv[1]->resume();  
      all_rcv[2]->resume();   
    }
    else if (front==0 && right==0 && left ==0) {
      Serial.println("hicbir veri alinamadi");
      sol();
      right=0;front=0;left=0;
      all_rcv[0]->resume();
      all_rcv[1]->resume();  
      all_rcv[2]->resume();   
    }
    delay(dly);
}


int Auto ()
{
    mesafe();
    Serial.print("uzaklik: ");Serial.print(uzaklik);
    if(uzaklik>15){
       Serial.println(" devam et");
       ileri();
   }
   else {
      Serial.println(" geri");
      geri();
      Serial.println(" sag");
      sag();
  }
  pil();
  if(yuzde<20){
    istasyon(500,0);
    return 0;
  }
  
}
    
void loop(){

  int val;
  
    while(ArduinoMega.available()>0){
     val  = ArduinoMega.parseInt();
      if(ArduinoMega.read()== '\n'){
      Serial.println(val);
      }
    
      switch(val)
      {
        case AUTO:
          val = Auto();
          break;
  
       case ILERI:
         ileri();
         break;
  
       case GERI:
         geri();
         break;
         
       case SAG:
         sag();
         break;
         
       case SOL:
         sol();
         break;
         
       case DEFAULT:
       break;           
      }
  }
}

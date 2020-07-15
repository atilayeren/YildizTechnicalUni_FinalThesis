#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <MQ2.h>
#include "DHTesp.h"
#define FIREBASE_HOST "proje-35a52.firebaseio.com"
#define FIREBASE_AUTH "jdmUiz4zkXOe5BRkH62RQKScHcz1HfzgjyykNgTd"
#define WIFI_SSID "TurkTelekom_T1913"
#define WIFI_PASSWORD "ZxTSGGfH"
String ileri,geri,sag,sol,oto;
DHTesp dht;
int esik=200;
int pin=A0;
int lpg, co, smoke;
MQ2 mq2(pin);

SoftwareSerial NodeMCU(D2,D3);

void setup(){
  Serial.begin(9600);
  dht.setup(D0,DHTesp::DHT11);
  mq2.begin();
  NodeMCU.begin(4800);
  pinMode(D0,INPUT);
  pinMode(D3,OUTPUT);
 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting"); 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("ileri",0);
  Firebase.set("geri",0);
  Firebase.set("sag",0);
  Firebase.set("sol",0);
  Firebase.set("oto",0);
}

void loop(){
  float*values=mq2.read(true);
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();
  Serial.print("LPG seviyesi:");Serial.println(lpg);
   Serial.print("CO seviyesi:");Serial.println(co);
    Serial.print("Duman seviyesi:");Serial.println(smoke);
  if (smoke>esik ){
    
      Serial.print("Gaz algilandi ");
  }
  else if (smoke<esik){
   
    Serial.print("Gaz algilanmadi ");
  }
  double hum=dht.getHumidity();
double temp=dht.getTemperature();
  Serial.print("hum: ");Serial.println(hum);
  Serial.print("temp: ");Serial.println(temp);
  Firebase.setInt("hum",hum);
      if (Firebase.failed()) {
      Serial.print("setting /hum failed:");
      Serial.println(Firebase.error());    
  }
  Firebase.setInt("temp",temp);
      if (Firebase.failed()) {
      Serial.print("setting /temp failed:");
      Serial.println(Firebase.error());  
      }
       Firebase.setInt("smoke",smoke);
      if (Firebase.failed()) {
      Serial.print("setting /smoke failed:");
      Serial.println(Firebase.error());  
      }
       Firebase.setInt("co",co);
      if (Firebase.failed()) {
      Serial.print("setting /co failed:");
      Serial.println(Firebase.error());  
      }
       Firebase.setInt("lpg",lpg);
      if (Firebase.failed()) {
      Serial.print("setting /lpg failed:");
      Serial.println(Firebase.error());  
      }
  ileri=Firebase.getString("ileri");Serial.println(ileri);
  geri=Firebase.getString("geri");Serial.println(geri);
  sag=Firebase.getString("sag");Serial.println(sag);
  sol=Firebase.getString("sol");Serial.println(sol);
  oto=Firebase.getString("oto");Serial.println(oto);
  Serial.println("------------------------------------------");

if(Firebase.failed())
{
  setup();
}

  
 if(ileri.equals("1")){
    int i = 2;
    NodeMCU.print(i);
    NodeMCU.println("\n");
  delay(300); 
  }
  else if(geri.equals("1")){
     int i = 3;
     NodeMCU.print(i);
     NodeMCU.println("\n");
  delay(300);
  }
else if(sag.equals("1")){
     int i = 4;
     NodeMCU.print(i);
     NodeMCU.println("\n");
  delay(300);
  }
   else if(sol.equals("1")){
     int i = 5;
     NodeMCU.print(i);
     NodeMCU.println("\n");
  delay(300);
  }
   else if(oto.equals("1")){
     int i = 6;
     NodeMCU.print(i);
     NodeMCU.println("\n");
  delay(300);
  }
   else{
     int i = 7;
     NodeMCU.print(i);
     NodeMCU.println("\n");
  delay(300);
  } 
}

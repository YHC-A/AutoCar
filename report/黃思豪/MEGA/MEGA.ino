#include <Wire.h>

//#include <Wire.h>
#define TWI_FREQ 400000L

/*#include <NewPing.h>
#define SONAR_NUM 3     // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping
NewPing sonar0[SONAR_NUM] = {   // Sensor object array.
  NewPing(13, 7, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(13, 6, MAX_DISTANCE),
  NewPing(13, 5, MAX_DISTANCE), 
};
NewPing sonar1[SONAR_NUM] = { 
  NewPing(13, 31, MAX_DISTANCE),
  NewPing(13, 33, MAX_DISTANCE),
  NewPing(13, 35, MAX_DISTANCE),
};*/
const int buzzer = 7; 
char val = 3;
int rr=0;
int DIS[6];
int MIN =0;
int AR =45;
int AL =47;
int BR =49; 
int BL =51;
int can_i_in = 0;
int TTT=0;
int staring =0; //車行進時才啟動避障
byte onoroff_line = 0;
int led1=2;
int led2=3;
int led3=12;
int led4=13;
int PWM1=9;    //H橋1:正轉 H橋模組A機
//int pWM1=6;    //H橋1:逆轉
int PWM2=10;    //H橋2:正轉 H橋模組B機
//int pWM2=7;    //H橋2:逆轉

int Stop = 20;
int Stop1 = 5;
int Stopall = 21;
volatile int k = 0;

int PGo=0;
int P1=0;
int P2=0;
int R=0;
//======================================================
int stop_D = 30;
int ZigbeePWM1=30;
int ZigbeePWM2=32;
int RssiPWM1 = 0;
int RssiPWM2 = 0;
int Resetpin = 8;
int starpin = 11; 
int turntime = 0;
long int star = 0;
volatile int INT = 0;

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  Wire.begin();

  
  //ArduinoSerial.begin(1000);
  //Serial1.begin(9600);
  //Serial2.begin(9600);
  //Serial2.end();


  TCCR1B = TCCR1B & 0xF8 | 1; //PWM頻率31K
  TCCR2B = TCCR2B & 0xF8 | 1;
  pinMode(buzzer,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(PWM1,OUTPUT);
  //pinMode(pWM1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  //pinMode(pWM2,OUTPUT);
  pinMode(Resetpin,OUTPUT);
  pinMode(starpin,OUTPUT);  
  pinMode(AR,OUTPUT);
  pinMode(AL,OUTPUT);
  pinMode(BR,OUTPUT);
  pinMode(BL,OUTPUT);  
  pinMode(Stop,INPUT);
  pinMode(Stop1,INPUT);
  pinMode(Stopall,INPUT);
  pinMode(ZigbeePWM1,INPUT);
  pinMode(ZigbeePWM2,INPUT);
 //attachInterrupt(3,StopGo,RISING);  //中斷3
  //attachInterrupt(2,StopAllSystem,RISING);  //中斷2

  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);
  digitalWrite(PWM1,LOW);
  digitalWrite(Resetpin,LOW);
  digitalWrite(starpin,LOW);  
  digitalWrite(PWM2,LOW);    
  digitalWrite(AR,LOW);
  digitalWrite(AL,LOW);
  digitalWrite(BR,LOW); 
  digitalWrite(BL,LOW);
  digitalWrite(Stop,LOW);  
}

void loop() {
  
  //check();
  //Serial.println(P1);
  //Serial.println(P2);
   // Serial.println("1234");
uartrssi1();
uartrssi2();
if(RssiPWM1*RssiPWM2 != 0){
staring=1;
if(star==0){
  digitalWrite(starpin,HIGH);
  //digitalWrite(   ,HIGH);
  star++;
  delay(3000);
}
  check(); 
  while(MIN>130){
    tone(buzzer,500);
    delay(10);
    noTone(buzzer);
    check();      
  }
    if(DIS[0]<40 && DIS[2]>60 &&DIS[3]>60){
      Serial.println("DIS--0");
      Serial.println(DIS[0]);
      digitalWrite(AR,HIGH);
      digitalWrite(BR,HIGH);  
      analogWrite(PWM1,40);    //暫定  
      analogWrite(PWM2,10);    //暫定
      delay(1000);
      analogWrite(PWM1,40);    //暫定  
      analogWrite(PWM2,40);    //暫定
        loop();
    }
   
    if(DIS[5]<40&& DIS[2]>60 &&DIS[3]>60){
      Serial.println("DIS--5");
      Serial.println(DIS[5]);
      digitalWrite(AR,HIGH);
      digitalWrite(BR,HIGH);  
      analogWrite(PWM1,10);    //暫定  
      analogWrite(PWM2,40);    //暫定
      delay(500);
      analogWrite(PWM1,40);    //暫定  
      analogWrite(PWM2,40);    //暫定
    loop();
    }
    
  if(DIS[1]==MIN && DIS[1]<=130){
    digitalWrite(led1,HIGH);
    delay(10);
    digitalWrite(led1,LOW);
    Serial.println("DIS--1");
    Serial.println(DIS[1]);
    if(DIS[1]<stop_D){
      stopp();
    }
    else{
      turn_R();
  }
  }
  else if(DIS[2]==MIN && DIS[2]<=130){
    digitalWrite(led2,HIGH);
    delay(10);
    digitalWrite(led2,LOW);
    Serial.println("DIS--2");
    Serial.println(DIS[2]);
    if(DIS[2]<stop_D){
      stopp();
    }
    else if(DIS[3]-DIS[2]<=15 && DIS[2]>=stop_D){
      no_turn();
    }
    else if(DIS[3]-DIS[2]>15 && DIS[2]>=stop_D){
      turn_r();
    }
  }
  else if(DIS[3]==MIN && DIS[3]<=130){
    digitalWrite(led3,HIGH);
    delay(10);
    digitalWrite(led3,LOW);
    Serial.println("DIS--3");
    Serial.println(DIS[3]);
    if(DIS[3]<stop_D){
      stopp();
    }
    else if(DIS[2]-DIS[3]<=15 && DIS[3]>=stop_D){
      no_turn();
    }
    else if(DIS[2]-DIS[3]>15 && DIS[3]>=stop_D){
      turn_l();
    }
  }
  else if(DIS[4]==MIN && DIS[4]<=130){
    digitalWrite(led4,HIGH);
    delay(10);
    digitalWrite(led4,LOW);
    Serial.println("DIS--4");
    Serial.println(DIS[4]);
    if(DIS[4]<stop_D){
      stopp();
    }
    else{
      turn_L();
    }
  }
  //delay(5);          
}

else{
  //ZigbeeReset();
  car_stop();
  //onoroff_line = 0;
  set_zero();
  PWM_zero();
  
  delay(15);
  }
}

void car_stop(){                                //car_stop-------------------
  Serial.println("car_stop");
 /* if((P1 >= 60 && P2 >= 60)|| PGo >= 60){
  for(int x=60;x>0;x=x-3){
  analogWrite(PWM1,x);    //暫定  
  analogWrite(PWM2,x);    //暫定
  delay(120);    
}  
P1=0;
P2=0;
    }else if(P1 >= 60 && P2 <= 0){
  for(int x=60;x>0;x=x-3){
  analogWrite(PWM1,x);    //暫定  
  delay(120);    
} 
P1=0;
P2=0;     
    }else if(P2 >= 60 && P1 <= 0){      
  for(int x=60;x>0;x=x-3){
  analogWrite(PWM2,x);    //暫定  
  delay(120);    
}*/
P1=0;
P2=0;       
      
digitalWrite(starpin,LOW);
//digitalWrite(   ,LOW);
star=0;      
  }
  
void set_zero(){
  digitalWrite(AR,LOW);
  digitalWrite(AL,LOW);
  digitalWrite(BR,LOW);
  digitalWrite(BL,LOW);
 }
      
void PWM_zero(){
  analogWrite(PWM1,0);   //門全關    
  analogWrite(PWM2,0); 
}

void no_turn(){
Serial.println("no_turn");  
  P1=50;
  P2=50;
  digitalWrite(AR,HIGH);
  digitalWrite(BR,HIGH);  
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
  loop();
}

void stopp(){
Serial.println("stopp~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");   
  P1=0;
  P2=0;
  digitalWrite(AR,LOW);
  digitalWrite(BR,LOW);  
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
  //stopp();
  //loop();  
  RX();
}

void turn_L(){
Serial.println("turn_L");    
/*if(DIS[2]==MIN || DIS[3]==MIN){
  P1=50;
  P2=50;
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
  loop();    
}*/
//else{
  P1=60;
  P2=10;
  digitalWrite(AR,HIGH);
  digitalWrite(BR,HIGH);  
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
 // delay(5); 
  check();
  if(MIN <stop_D){
    stopp();
  }
  if(DIS[4]==MIN){
  turn_L(); 
  }
  else{
    P1=50;
    P2=50;
    analogWrite(PWM1,P1);    //暫定  
    analogWrite(PWM2,P2);    //暫定
    loop();
  }
//}
}

void turn_l(){
Serial.println("turn_l"); 
/*if(DIS[3]-DIS[2]<=15){
  P1=50;
  P2=50;
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
  loop();      
}*/
//else{
  P1=40;
  P2=10;
  digitalWrite(AR,HIGH);
  digitalWrite(BR,HIGH);  
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定 
  //delay(5); 
  check();
  if(MIN <stop_D){
  stopp();
  }
  if(DIS[2]==MIN && DIS[3]-DIS[2]>15){
  turn_l();  
  }
  else{
  P1=50;
  P2=50;
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
  loop();
  } 
//}
}

void turn_R(){
Serial.println("turn_R");   
/*if(DIS[2]==MIN || DIS[3]==MIN){
  P1=50;
  P2=50;
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
  loop();    
}*/
//else{
  P1=10;
  P2=60;
  digitalWrite(AR,HIGH);
  digitalWrite(BR,HIGH);  
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定 
  //delay(5);
  check();
  if(MIN <stop_D){
  stopp();
  }
  if(DIS[1]==MIN){
  turn_R(); 
  }
  else{
  P1=50;
  P2=50;
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
  loop();    
  }

//}
}

void turn_r(){
Serial.println("turn_r");    
/*if(DIS[2]-DIS[3]<=15){
  P1=50;
  P2=50;
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
  loop();      
}*/
//else{
  P1=10;
  P2=40;
  digitalWrite(AR,HIGH);
  digitalWrite(BR,HIGH);  
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定 
  //delay(5);
  check();
  if(MIN <stop_D){
  stopp();
  }
  if(DIS[3]==MIN && DIS[2]-DIS[3]>15){
  turn_r();
  }
  else{
  P1=50;
  P2=50;
  analogWrite(PWM1,P1);    //暫定  
  analogWrite(PWM2,P2);    //暫定
  loop();     
  }
//}
}

void check(){
 /*for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
  delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  //exDIS[i]=DIS[i];
  DIS0[i]=sonar0[i].ping_cm();
  if(DIS0[i]==0){
    DIS0[i]=200;
  }
 Serial.print(DIS0[i]);
 Serial.print("  ");
}
 for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
  delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  //exDIS[i]=DIS[i];
  DIS1[i]=sonar1[i].ping_cm();
  if(DIS1[i]==0){
    DIS1[i]=200;
  }
 Serial.print(DIS1[i]);
 Serial.print("  ");
}
  Serial.println();
  DIS[0]=DIS0[0]; 
  DIS[1]=DIS0[1]; 
  DIS[2]=DIS0[2]; 
  DIS[3]=DIS1[0]; 
  DIS[4]=DIS1[1];   
  DIS[5]=DIS1[2];  */
  Serial.println("check"); 
  Wire.requestFrom(8,6);
  while(Wire.available()){
    
    for(int I=0;I<6;I++){
      DIS[I]=Wire.read();
      Serial.print(DIS[I]);
      Serial.print("  ");
    }
     Serial.println();
  }
  MIN =DIS[1];
  for(int I=1;I<4;I++){
  MIN=min(MIN,DIS[I+1]);
  }
}

void RX(){
  //Serial.println("RX");
    Wire.end();
    Wire.endTransmission();
    Serial2.begin(10000);
  if (Serial2.available()>=14) {

      
    if(Serial2.read() == 0x7E) {
      // 連續讀取11個字元，不儲存。
      for (byte i = 1; i<12; i++) {
        Serial2.read();
      }
      // 讀取第13個字元，並顯示其16進位值。
          val = Serial2.read();
          Serial.println(val, HEX);
      // 讀取最後一個字元，不儲存。
      Serial2.read();
    }
  }
 
 
if(val == 0x10){
  Serial.println("ONONONONONONONONN");
  val=3;
  Serial2.end();
  delay(5);
  Wire.begin();
  loop();
}
else if(val == 0){
  Serial.println("OFFOFFOFFOFFOFF");
  val=3;
  Serial2.end();
  delay(5);
  Wire.begin();
  digitalWrite(AL,HIGH);
  digitalWrite(BL,HIGH);
  analogWrite(PWM1,40);
  analogWrite(PWM2,40);
  delay(3000);            //暫訂
  digitalWrite(AL,LOW);
  digitalWrite(BL,LOW);
  analogWrite(PWM1,0);
  analogWrite(PWM2,0);
  find_RL();
}
RX();
//Wire.beginTransmission(8);
}

void find_RL(){
  Serial.println("find_RL");
  check();
  if(DIS[0]>DIS[5]){
  digitalWrite(AR,HIGH);
  digitalWrite(BL,HIGH);
  analogWrite(PWM1,40);
  analogWrite(PWM2,40);
  delay(3000);
  digitalWrite(AR,LOW);
  digitalWrite(BL,LOW);
  analogWrite(PWM1,0);
  analogWrite(PWM2,0);
  loop();
  }
  else if(DIS[0]<DIS[5]){
  digitalWrite(AL,HIGH);
  digitalWrite(BR,HIGH);
  analogWrite(PWM1,40);
  analogWrite(PWM2,40);
  delay(3000);
  digitalWrite(AL,LOW);
  digitalWrite(BR,LOW);
  analogWrite(PWM1,0);
  analogWrite(PWM2,0);
  loop();    
  }
  else if(DIS[0]==DIS[5]){
    find_RL();
  }
}

 void uartrssi1(){
 /*g=0;
 g1=0;
 g2=0;
 j=0;
 
 Serial1.begin(9600);
 //Serial2.begin(9600);
 delay(100);//500*/
   //Serial.println("123456789");
 RssiPWM1=0;
 RssiPWM1= pulseIn(ZigbeePWM1,HIGH,200); //暫定200000

//Serial1.write(plus,sizeof(plus));
//delay(10); 
  Serial.print("RssiPWM1");  
  Serial.println(RssiPWM1);   
  //Serial.println("A1");
/*if(Serial1.available()){ 
for (byte i = 0; i<3; i++) {
     YY[i]=Serial1.read();             
     Serial.print(YY[i]); 
     //Serial.print(i);    
     delay(10);  
     }
     }
    //Serial.println();
    delay(100);
 
if(YY[0]==79&&YY[1]==75&&YY[2]==13){
Serial.println("B1");
 Serial1.write(ATDB,sizeof(ATDB));

while(Serial1.available()==0){
} 
    i=0;
    delay(100);//500
    for(byte i=0;i<=2;i++){
     YY[i]=0;
      Y1[i]=0; 
      Y2[i]=0;
      }

while(Serial1.available()>0){
 Y1[j]=Serial1.read();
//Serial.print(Y1[j]);
  //Serial.println("QQ"); 
  j++;
 }

 if(j == 2){
   Rssi1 = Y1[0]-48;

  }else if(j == 3){
   ASCIItoINT1();
    }

}*/
/*else{
     digitalWrite(Resetpin,HIGH);                     //進來後讓Zigbee重置一下，會讓DB命令的rssi歸零
     delay(200);//500
     digitalWrite(Resetpin,LOW);
     delay(100);
}
Serial.print("Rssi1");
Serial.println(Rssi1);
Serial1.end();*/
        }
 void uartrssi2(){
 /*g=0;
 g1=0;
 g2=0;
 j=0;

 Serial3.begin(9600);
 delay(100);//500*/
RssiPWM2=0; 
RssiPWM2= pulseIn(ZigbeePWM2,HIGH,200);  //暫定200000

//Serial3.write(plus,sizeof(plus));
//delay(10);
Serial.print("RssiPWM2");
Serial.println(RssiPWM2);   
/*if(Serial3.available()){  
for (byte i = 0; i<3; i++) {
     YY[i]=Serial3.read();             
     //Serial.print(YY[i]); 
     //Serial.print(i);    
     delay(10);  
     }
     }
    delay(100);

if(YY[0]==79&&YY[1]==75&&YY[2]==13){
Serial.println("B2");
 Serial3.write(ATDB,sizeof(ATDB));

while(Serial3.available()==0){
} 
   
    i=0;
    delay(100);//500
    for(byte i=0;i<=2;i++){
     YY[i]=0;
      Y1[i]=0; 
      Y2[i]=0; 
      }

while(Serial3.available()>0){
 Y2[j]=Serial3.read();
//Serial.print(Y2[j]);
  //Serial.println("QQ"); 
  j++;
 }

 if(j == 2){
   Rssi2 = Y2[0]-48;
  }else if(j == 3){
   ASCIItoINT2();
    }

}*/
/*else{
     digitalWrite(Resetpin,HIGH);                     //進來後讓Zigbee重置一下，會讓DB命令的rssi歸零
     delay(100);//500
     digitalWrite(Resetpin,LOW);
     delay(100);
}
Serial.print("Rssi2");
Serial.println(Rssi2);
Serial3.end();*/
        }        
/*void ASCIItoINT1(){
      if(Y1[0]>=48 && Y1[0]<=57){
      g1 = (Y1[0]-48)*16;
      }
    if(Y1[1]>=48 && Y1[1]<=57){
      g2 = Y1[1]-48;
      }else if(Y1[1]>=65 && Y1[1]<=70){
      g2 = Y1[1]-55;  
        }
      Rssi1 = g1+g2;  
  }      
  void ASCIItoINT2(){
      if(Y2[0]>=48 && Y2[0]<=57){
      g1 = (Y2[0]-48)*16;
      }
    if(Y2[1]>=48 && Y2[1]<=57){
      g2 = Y2[1]-48;
      }else if(Y2[1]>=65 && Y2[1]<=70){
      g2 = Y2[1]-55;  
        }
      Rssi2 = g1+g2;  
  }   */
  /*void ZigbeeReset(){
if(PGo>0 || P1 >0 || P2 >0){    
     digitalWrite(Resetpin,HIGH);                     //進來後讓Zigbee重置一下，會讓DB命令的rssi歸零
     delay(200);//500
     digitalWrite(Resetpin,LOW);
     delay(300);  
  }
    }*/

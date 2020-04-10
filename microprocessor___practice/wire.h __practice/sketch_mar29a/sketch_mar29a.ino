int af = 13;
int as = 12;
int bf = 9;
int bs = 8;
int pwma = 4;
int pwmb = 3;
const int trig = 7;
const int echo = 6;

const int inter_time = 1000;

int time = 0;
int data = 1;


void setup() {
  Serial.begin(9600);
  pinMode (trig, OUTPUT);
  pinMode (echo, INPUT);
  
  pinMode (af, OUTPUT);
  pinMode (as, OUTPUT);
  pinMode (bf, OUTPUT);
  pinMode (bf, OUTPUT);

  digitalWrite(af, HIGH);
  digitalWrite(as, LOW);
  digitalWrite(bf, HIGH);
  digitalWrite(bf, LOW);
  analogWrite(pwma, 70);
  analogWrite(pwmb, 70);
  
}

void loop() {
  double duration, distance;
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  duration = pulseIn (echo, HIGH);
  distance = (duration/2)/29;
  Serial.print("Data:");
  Serial.print (data);
  Serial.print(", d = ");
  Serial.print(distance);
  Serial.println(" cm");
  time = time + inter_time;
  data++;

  if ( distance <= 50){
    digitalWrite(af, LOW);
    digitalWrite(bf, LOW);  
  } else {
      digitalWrite(af, HIGH);
      digitalWrite(bf, HIGH);
    }

  
  delay(inter_time);
  
}

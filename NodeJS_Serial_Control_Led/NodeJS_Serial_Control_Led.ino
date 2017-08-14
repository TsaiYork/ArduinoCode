
const int G1=5;
const int G2=6;
const int G3=9;
const int G4=10;
const int G5=11;

String messenge = "";
int val=0;
int read_length=0;
char cmd;
char RX[4];
void Breath_Light();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //pinMode(R1,OUTPUT);pinMode(R2,OUTPUT);pinMode(R3,OUTPUT);pinMode(R4,OUTPUT);pinMode(R5,OUTPUT);
  pinMode(G1,OUTPUT);pinMode(G2,OUTPUT);pinMode(G3,OUTPUT);pinMode(G4,OUTPUT);pinMode(G5,OUTPUT);
//  digitalWrite(R1,LOW);digitalWrite(R2,LOW);digitalWrite(R3,LOW);digitalWrite(R4,LOW);digitalWrite(R5,LOW);
  digitalWrite(G1,LOW);digitalWrite(G2,LOW);digitalWrite(G3,LOW);digitalWrite(G4,LOW);digitalWrite(G5,LOW);
}

void loop() {
  if(Serial.available()){
    cmd = Serial.read();
    }
  switch(cmd){
    case 'B':
      Breath_Light();
      break;
    case 'C':
      digitalWrite(G1,LOW);digitalWrite(G2,LOW);digitalWrite(G3,LOW);digitalWrite(G4,LOW);digitalWrite(G5,LOW);
      break;
    case 'A':
      digitalWrite(G1,HIGH);digitalWrite(G2,HIGH);digitalWrite(G3,HIGH);digitalWrite(G4,HIGH);digitalWrite(G5,HIGH);
      break;
    }
    delay(10);
}

void Breath_Light(){
  for(int i=0;i <= 255 ;i=i+5){
    analogWrite(G1,i);analogWrite(G2,i);analogWrite(G3,i);analogWrite(G4,i);analogWrite(G5,i);
    delay(3);
  }
  delay(10);
  for(int i=255;i >= 0 ;i=i-5){
    analogWrite(G1,i);analogWrite(G2,i);analogWrite(G3,i);analogWrite(G4,i);analogWrite(G5,i);
    delay(3);
  }
}


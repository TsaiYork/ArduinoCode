
/////////////////////////////////////////<Announce Aria>/////////////////////////////////////////
#define debug 1
#define ServoDelay 250

bool UltrasonicWalk = 1;

const byte TRIG = 8;
const byte ECHO = 9;
const byte R_DIR = 10;
const byte L_DIR = 11;
const byte R_VAL = 5;
const byte L_VAL = 6; 

long F_distance = 0;
long R_distance = 0;
long L_distance = 0;

bool r_dir = 1;
bool l_dir = 1;
int r_val = 0;
int l_val = 0;

unsigned long last_detect_time;

long GetDis(int ServoDegree);
void MotorOutput(bool r_direction, int r_speed, bool l_direction, int l_speed);
void SetServoPositionByArduino(int pin,int ServoPosition,int ServoDegree,int DelayTime);

/////////////////////////////////////////<#Announce Aria#>/////////////////////////////////////////


/////////////////////////////////////////<Fuction Aria>/////////////////////////////////////////

//Measure distance from Ultrasonic 
long GetDis(int ServoDegree){
  long dis,dur;       //Anounce dis:distance dur:duration  
  if(ServoDegree != 90){
    SetServoPositionByArduino(3,1,ServoDegree,ServoDelay);
    }
  digitalWrite(TRIG , HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIG , LOW);
  dur = pulseIn (ECHO , HIGH);
  dis = (dur/2) * 0.034;
  if(debug){
    Serial.print("Degree: ");
    Serial.print(ServoDegree);
    Serial.print("  ,");
    Serial.print(dis);
    Serial.println("  cm");
    }
  if(ServoDegree != 90){
     SetServoPositionByArduino(3,1,90,ServoDelay);
   }
  return dis;  
  }

//Motor Output Setting
void MotorOutput(bool r_direction, int r_speed, bool l_direction, int l_speed){
  digitalWrite(R_DIR, r_direction);
  digitalWrite(L_DIR, l_direction);
  analogWrite(R_VAL, r_speed);
  analogWrite(L_VAL, l_speed);
  }

//Servo Output Setting  
void SetServoPositionByArduino(int pin,int ServoPosition,int ServoDegree,int DelayTime){
      byte bytPosition = ServoDegree;

      // Ensure the position values are within end stop range
      if (bytPosition < 24) 
      {
        bytPosition = 24;
      }
      if (bytPosition > 232) 
      {
        bytPosition = 232;
      }

      // Get the servo position if it currently disabled
      if (arrMeccanoidData[pin][ServoPosition] == 0xFA) {
        arrCurrentValue[pin][ServoPosition] = meccanoidGetServoMotorPosition(pin,ServoPosition);
      }

      // Store the start value, end value, start time and duration
      arrStartValue[pin][ServoPosition] = arrCurrentValue[pin][ServoPosition];
      arrEndValue[pin][ServoPosition] = bytPosition;
      arrStartTime[pin][ServoPosition] = millis();
      arrDuration[pin][ServoPosition] = DelayTime;

      bytCommandResponse = enuCommandResponseOK;
  }  

/////////////////////////////////////////<#Function Aria#>/////////////////////////////////////////


/////////////////////////////////////////<Setup Aria>/////////////////////////////////////////

//Initial setup
void setup(){
  if(debug){  
    Serial.begin(19200);
    }
  pinMode(TRIG , OUTPUT);
  pinMode(ECHO , INPUT);
  pinMode(R_DIR , OUTPUT);
  pinMode(L_DIR , OUTPUT);
  pinMode(R_VAL , OUTPUT);
  pinMode(L_VAL , OUTPUT);
  last_detect_time = millis();
  }

/////////////////////////////////////////<#Setup Aria#>/////////////////////////////////////////


/////////////////////////////////////////<Loop Aria>/////////////////////////////////////////

void loop(){
  if(UltrasonicWalk){
    F_distance = GetDis(90);           //////
    if(F_distance <= 160){
      MotorOutput(1,0,1,0);   //Stop!!!!
      delay(10);              
      MotorOutput(0,150,0,150);   //back
      delay(1000);
      MotorOutput(1,0,1,0);   //stop
      
      R_distance = GetDis(135);
      L_distance = GetDis(45);
      
      if(R_distance >= L_distance){
         MotorOutput(0,100,1,100);   //turn right
         delay(1000);
         }
      else if(L_distance > R_distance){
        MotorOutput(1,100,0,100);     //turn left      
        delay(1000);
        }
      else{
        MotorOutput(0,150,0,150);   //back
        delay(1000);
        MotorOutput(1,0,1,0);   //stop   
        }
      }
      
    else{
      if(millis()-last_detect_time >= 4000){
        R_distance = GetDis(135);
        L_distance = GetDis(45);
        
        if(F_distance >= R_distance || F_distance >= L_distance){
          //nothing to do, keep go straight
          }
          
        else{
          if(R_distance >= L_distance){
            MotorOutput(0,150,1,150);   //turn right
            delay(1000);
            }
          else{
            MotorOutput(1,150,0,150);   //turn left
            delay(1000); 
            }
          }
         last_detect_time = millis();
        }
      else
        MotorOutput(1,200,1,200);
        delay(10);
      }
  }
}  
/////////////////////////////////////////<#Loop Aria#>/////////////////////////////////////////

  

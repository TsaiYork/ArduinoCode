#include <Servo.h>
#define DEBUG 1
//#define SERVO_CENTER 75
#define INF_MAX 1000

Servo myservo;

const int dis_limit = 20;
const int side_limit = 25;

const int R_dir1 = 7;
const int R_dir2 = 8;
const int L_dir1 = 12;      
const int L_dir2 = 13;
const int Trig = 11;
const int Echo = 10;
const int R_pwm = 5;
const int L_pwm = 6;
const int pin_Servo = 9;  

int L_distance, FL_distance, F_distance, FR_distance, R_distance;

int SERVO_CENTER = 75;
bool UltrasonicWalk = 1;
int last_detect_time;
int Obstacle_count;
char Direction;
int GoStraightCount = 0;

int rdir,ldir,rpwm,lpwm;

int GetDis(int ServoDegree,int delaytime){
  long dis,dur;       //Anounce dis:distance dur:duration  
  myservo.write(ServoDegree);
  delay(delaytime);
  digitalWrite(Trig , LOW);
  delayMicroseconds(5);
  digitalWrite(Trig , HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig , LOW);
  dur = pulseIn (Echo , HIGH);
  dis = dur*0.01657;
//  if(DEBUG){
//    Serial.print("Degree: ");
//    Serial.print(ServoDegree);
//    Serial.print("  ,");
//    Serial.print(dis);
//    Serial.println("  cm");
//    }
   if(dis >= 200)
      dis = INF_MAX;
   if(dis < 0)
      dis = 0;
  return round(dis);//Measure distance from Ultrasonic 
}

void MotorOutput(bool r_direction, int r_speed, bool l_direction, int l_speed){
  if(r_direction){
    digitalWrite(R_dir2, HIGH);
    digitalWrite(R_dir1, LOW);
    }else{
    digitalWrite(R_dir2, LOW);
    digitalWrite(R_dir1, HIGH);
    }
  if(l_direction){
    digitalWrite(L_dir2, HIGH);
    digitalWrite(L_dir1, LOW);
    }else{
    digitalWrite(L_dir2, LOW);
    digitalWrite(L_dir1, HIGH);
    }
  analogWrite(R_pwm, r_speed);
  analogWrite(L_pwm, l_speed);
}

void Stop(){
  digitalWrite(R_dir1, LOW);
  digitalWrite(R_dir2, LOW);
  digitalWrite(L_dir1, LOW);
  digitalWrite(L_dir2, LOW);
  }

void setup()  
{
  if(DEBUG){  
    Serial.begin(19200);
  }

  myservo.attach(9); // 連接數位腳位9，伺服馬達的訊號線  
  pinMode(R_dir1, OUTPUT);
  pinMode(R_dir2, OUTPUT);
  pinMode(L_dir1, OUTPUT);
  pinMode(L_dir2, OUTPUT);
  pinMode(R_pwm, OUTPUT);
  pinMode(L_pwm, OUTPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  myservo.write(SERVO_CENTER);
  last_detect_time = millis();
  Stop();
      
}


void loop(){

  FR_distance = GetDis(SERVO_CENTER-15,1000);
  F_distance = GetDis(SERVO_CENTER,1000);
  FL_distance = GetDis(SERVO_CENTER+15,1000);
  F_distance = GetDis(SERVO_CENTER,1000);
  if(DEBUG){Serial.print("R:");Serial.print(FR_distance);Serial.print("  F:");Serial.print(F_distance);Serial.print("  L:");Serial.println(FL_distance);}

  if(FR_distance <= F_distance + 1 && FL_distance <= F_distance + 1){
    if(DEBUG){Serial.print("Straight");}
    if(F_distance >= 12 && F_distance < 18){
      if(DEBUG){Serial.println("Stop");}
      Stop();
      }
    else if(F_distance >=25){
      if(DEBUG){Serial.println("Out of Range");}
      rdir=1;ldir=1;rpwm=0;lpwm=0;
      }
    else if(F_distance <12){
      if(DEBUG){Serial.println("Back");}
      rdir=0;ldir=0;rpwm=255;lpwm=255;
      }
    else if(F_distance >=18 && F_distance <25){
      if(DEBUG){Serial.println("Foward");}
      rdir=1;ldir=1;rpwm=255;lpwm=255;
      }
    }
  else if(FR_distance <= F_distance + 1 && FL_distance >= FR_distance + 2){
      rdir=1;ldir=1;rpwm=200;lpwm=255;
      if(DEBUG){Serial.println("Right");}
    }
  else if(FL_distance <= F_distance + 1 && FR_distance >= FL_distance + 2){
      rdir=1;ldir=1;rpwm=255;lpwm=200;
      if(DEBUG){Serial.println("Left");}    
    }
    

}
/*
void loop(){
  MotorOutput(1,220,1,220);
  delay(3000);
  MotorOutput(1,0,1,0);
  delay(3000);
  }
*/

/*
void loop(){
  F_distance = GetDis(SERVO_CENTER,100); 
  if(F_distance < dis_limit){
    if(DEBUG){Serial.println("Wall!!!Wall!!!");Serial.println(Obstacle_count);}
    Stop();
    delay(10);
    Direction = DirDecide();
    switch(Direction){
      case 'r':
        MotorOutput(0,255,1,0);
        delay(500);
        MotorOutput(1,0,1,0);
        if(DEBUG){Serial.println("***Turn RRRR Watch");} 
        break;
      case 'l':
        MotorOutput(1,0,0,255);
        delay(500);
        MotorOutput(1,0,1,0);
        if(DEBUG){Serial.println("***Turn LLLL Watch");} 
        break;
    }
    Obstacle_count = Obstacle_count + 1;
    }
    
  F_distance = GetDis(SERVO_CENTER,100);
  if(F_distance > dis_limit){
    Obstacle_count = 0;
    MotorOutput(1,255,1,255);
    delay(100);
    GoStraightCount = GoStraightCount + 1;
  }
  else{GoStraightCount = 0;}
      
    
  if(GoStraightCount >= 15){
    if(DEBUG){Serial.println("Wathc~~");}
    //Stop();
    MotorOutput(1,0,1,0);
    watchsurrounding();
    if(L_distance < side_limit || FL_distance < F_distance || (FR_distance-F_distance) >= 5){
      if(DEBUG){Serial.println("Turn RIGHT Watch");}
      MotorOutput(0,210,1,255);
      delay(500);
      MotorOutput(1,0,1,0);
      //Stop();  
      }
    else if(R_distance < side_limit || FR_distance < F_distance || (FL_distance-F_distance) >= 5){
      if(DEBUG){Serial.println("Turn LEFT Watch");}
      MotorOutput(1,255,0,210);
      delay(500);
      MotorOutput(1,0,1,0);
      //Stop();
      }
    GoStraightCount = 0;
    }
    
  if(Obstacle_count >= 3){
  Direction = DirDecide();
  switch(Direction){
    case 'l':
      MotorOutput(1,0,0,255);
      delay(1000);
      Stop();
      if(DEBUG){Serial.println("Wall!!!Turn LEFT");}
      break;
    case 'r':
      MotorOutput(0,255,1,0);
      delay(1000);
      Stop();
      if(DEBUG){Serial.println("Wall!!!Turn RIGHT");}
      break;        
    case 'b':
      MotorOutput(0,255,0,255);
      delay(1000);
      MotorOutput(1,255,0,255);
      delay(1000);
      Stop();
      if(DEBUG){Serial.println("Wall!!! BACK");}
      break;
    }
  }  
}
*/


/////////////////////////////////
/*
void loop() {

//  F_distance = GetDis(SERVO_CENTER); 
//  if(F_distance <=4) {
//    UltrasonicWalk=1;
//    if(DEBUG)
//      Serial.println(" Start walking");
//  } else if(F_distance <=10) {
//    UltrasonicWalk=0;
//    if(DEBUG)
//      Serial.println(" Stop walking");
//  }


  if(UltrasonicWalk){
    F_distance = GetDis(SERVO_CENTER);           //////
    if(F_distance <= 10){
      MotorOutput(1,0,1,0);   //Stop!!!!              
      MotorOutput(0,80,0,80);   //back
      delay(1000);
      MotorOutput(1,0,1,0);   //stop
      if(DEBUG){
        delay(1000);
        Serial.println("WALL!!Whtch new way"); 
      }
      R_distance = GetDis(SERVO_CENTER-10);
      L_distance = GetDis(SERVO_CENTER+10);
      if(DEBUG){
        //Serial.print("R:");
        //Serial.print(R_distance);
        //Serial.print("L:");
        //Serial.println(L_distance);
        }
      
      if(R_distance >= L_distance && R_distance >=10){
         MotorOutput(0,90,1,90);   //turn right
         delay(300);
         MotorOutput(1,0,1,0);   //stop 
         if(DEBUG){
          delay(1000);
          Serial.println("WALL!!! turn Right"); 
         }
      }
      else if(L_distance > R_distance && L_distance >=10){
        MotorOutput(1,90,0,90);     //turn left      
        delay(300);
        MotorOutput(1,0,1,0);   //stop 
          if(DEBUG){
          delay(1000);
          Serial.println("WALL!!! turn Left"); 
         }
        }
      else{
        MotorOutput(0,80,0,80);   //back
        delay(1000);
        MotorOutput(1,0,1,0);   //stop   
          if(DEBUG){
            delay(1000);
            Serial.println("WALL!!!Again"); 
          }
        }
      }
      
    else{
      if(millis()-last_detect_time >= 10000){
        if(DEBUG){
          delay(1000);
          Serial.println("Whtch new way"); 
        }
        R_distance = GetDis(SERVO_CENTER-10);
        L_distance = GetDis(SERVO_CENTER+10);
        if(F_distance >= R_distance || F_distance >= L_distance){
          //nothing to do, keep go straight
          }
          
        else{
          if(R_distance >= L_distance){
            MotorOutput(1,90,0,90);   //turn right
            delay(300);
            MotorOutput(1,0,1,0);   //stop
            if(DEBUG){
              delay(1000);
              Serial.println("Go Right"); 
            }
          }
          else{
            MotorOutput(0,90,1,90);   //turn left
            delay(300); 
            MotorOutput(1,0,1,0);   //stop 
            if(DEBUG){
              delay(1000);
              Serial.println("Go Left"); 
            } 
          }
        }
        last_detect_time = millis();
      }
      else
        {
        MotorOutput(1,80,1,80);
        delay(10);
        if(DEBUG){
          delay(1000);
          Serial.println("Go!!"); 
        }
      }
      }
  }
}
*/

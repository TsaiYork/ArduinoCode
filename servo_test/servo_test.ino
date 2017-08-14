#include <Servo.h>

Servo myservo; // 建立Servo物件，控制伺服馬達
int angle;
void setup() 
{ 
  Serial.begin(115200);
  myservo.attach(9,800,2200); // 連接數位腳位9，伺服馬達的訊號線//SC0352Pulse Width range:800~2200
  myservo.write(90);
} 

void loop() 
{
  angle=90;
  myservo.write(angle);
  Serial.println(angle);
  delay(15000);
  Serial.println("ready");
  delay(5000);
  
  angle=0;
  myservo.write(angle);
  Serial.println(angle);
  delay(15000);
  Serial.println("ready");
  delay(5000);
  
  angle=90;
  myservo.write(angle);
  Serial.println(angle);
  delay(15000);
  Serial.println("ready");
  delay(5000);

  angle=180;
  myservo.write(angle);
  Serial.println(angle);
  delay(15000);
  Serial.println("ready");
  delay(5000);
}

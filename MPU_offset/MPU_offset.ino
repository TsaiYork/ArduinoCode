//  MPU<--->Arduino
//  SDA<--->AD4
//  SCL<--->AD5

#include <Wire.h>
#define SAMPLE 1000
//#define GYRO_X_OFFSET 0//423.4

int count = SAMPLE + 1;
double ACC_X=0;
double ACC_Y=0;
double ACC_Z=0;
double GYRO_X=0;
double GYRO_Y=0;
double GYRO_Z=0;

int16_t accX,accY,accZ;
int16_t gyroX,gyroY,gyroZ;

uint32_t timer;
uint8_t i2cData[14];

//I2C
const uint8_t IMUAddress = 0x68;
const uint8_t I2C_TIMEOUT = 1000;

uint8_t i2cWrite(uint8_t registerAddress, uint8_t data, bool sendStop){
  return i2cWrite(registerAddress, &data, 1, sendStop);
  }

uint8_t i2cWrite(uint8_t registerAddress, uint8_t *data, uint8_t dataLength, bool sendStop){
  Wire.beginTransmission(IMUAddress);
//  Wire.beginTransmission(0x68);
  Wire.write(registerAddress);
//  Wire.write(0x6B);
  Wire.write(data, dataLength);
  //Wire.write(0);

  uint8_t rcode = Wire.endTransmission(sendStop);
  if(rcode){
    Serial.print(F("QQ i2cWrite failed:"));
    Serial.println(rcode);
    }
  return rcode;
  }

uint8_t i2cRead(uint8_t registerAddress, uint8_t *data, uint8_t nbytes){
  uint32_t timeOutTimer;
  Wire.beginTransmission(IMUAddress);
  Wire.write(registerAddress);
  uint8_t rcode = Wire.endTransmission(false);
  
  if(rcode){
    Serial.print(F("i2cWrite failed:"));
    Serial.println(rcode);
    return rcode;
    }

  Wire.requestFrom(IMUAddress, nbytes, (uint8_t)true);
  for(uint8_t i = 0; i < nbytes; i++){
    if(Wire.available())
      data[i] = Wire.read();
    else{
      timeOutTimer = micros();
      while(((micros() - timeOutTimer) < I2C_TIMEOUT) && !Wire.available());  // out of time or have data and go
      if(Wire.available())
        data[i] = Wire.read();
      else{
        Serial.println(F("i2cRead timeout"));
        return 5;
        }
      }
    }
  return 0; //success  
  } 
       
void main_set(){
  i2cData[0] = 7;
  i2cData[1] = 0x00;
  i2cData[2] = 0x03;      //Set gyro Full Scale
  i2cData[3] = 0x03;      //Set Accelerometer

  while(i2cWrite(0x19, i2cData, 4, false));

  while(i2cWrite(0x6B, 0x01, true));

//  while(i2cWrite(0x1B, 0x00, true));
//  int16_t acc_config_set = ((acc_config_set & 0xE7 ) | (f << 3 ));


  while(i2cRead(0x75, i2cData, 1));
  if(i2cData[0] != 0x68){
    Serial.print(F("Error reading sensor"));
    while(1);
    }
  delay(100);
  timer = micros();
  }

void read_config(){
  while(i2cRead(0x1B, i2cData, 4));
  int16_t gyro_config = ((i2cData[0] << 8) | i2cData[1]);
  int16_t acc_config = ((i2cData[2] << 8) | i2cData[3]);
  Serial.print(gyro_config);
  Serial.print("\t");
  Serial.println(acc_config);
    
  }

void sample_angle(){
  while(i2cRead(0x3B, i2cData, 14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);

  gyroX = ((i2cData[8] << 8) | i2cData[9]);
  gyroY = ((i2cData[10] << 8) | i2cData[11]);
  gyroZ = ((i2cData[12] << 8) | i2cData[13]);  
  }

void setup() {
  Serial.begin(19200);
  Wire.begin();
  main_set();
 }

void loop() {
  //read_config();
  sample_angle();
  Serial.print(".");
  if(count != 0){
    ACC_X += accX;
    ACC_Y += accY;
    ACC_Z += accZ;
    GYRO_X += gyroX;
    GYRO_Y += gyroY;
    GYRO_Z += gyroZ;
    count = count - 1;
      
    }
  else {
    ACC_X = ACC_X / SAMPLE;
    ACC_Y = ACC_Y / SAMPLE;
    ACC_Z = ACC_Z / SAMPLE;
    GYRO_X = GYRO_X / SAMPLE;
    GYRO_Y = GYRO_Y / SAMPLE;
    GYRO_Z = GYRO_Z / SAMPLE;
    Serial.println();
    Serial.println("offset sampling over");
    Serial.print(ACC_X);Serial.print("\t");
    Serial.print(ACC_Y);Serial.print("\t");
    Serial.print(ACC_Z);Serial.print("\t");
    Serial.print(GYRO_X);Serial.print("\t");
    Serial.print(GYRO_Y);Serial.print("\t");
    Serial.println(GYRO_Z);;
    delay(10000);
    count = SAMPLE + 1;
    ACC_X=0;
    ACC_Y=0;
    ACC_Z=0;
    GYRO_X=0;
    GYRO_Y=0;
    GYRO_Z=0;
    }
}

#define DEBUG 0
#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 8
#define NUM_METEORS 3
#define MAX_VOL 160

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
//#define CLOCK_PIN 13
#define MAX_BRIGHTNESS 150
#define BRIGHT_INTERVAL MAX_BRIGHTNESS/NUM_METEORS

char RX[16];
char TX[16]={"/0"};
int read_length = 0;


// Define the array of leds
CRGB leds[NUM_LEDS];
int meteors[NUM_METEORS][3];

void MeteorLight();
float CalculateNum(char buf[],int len);
void VolumeLight(int vol);
void BlinkLight();
void Sleep();

int leds_start,leds_end,start,num;
int bright = 50;
int last_vol;
int now_led;
int target;
int flag;
int thunder_count = 0;
bool thunder_flag = 0;

float val = 0;

void setup() { 
  Serial.begin(115200);
//  FastLED.addLeds<WS2812, DATA_PIN, RGB>(meteors, NUM_METEORS);
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
//
  bright = 50;
  for(int i = 0; i < NUM_METEORS  ; i++ ){
    meteors[i][0] = 0;
    meteors[i][1] = 100;
    meteors[i][2] = bright;
    bright = bright + BRIGHT_INTERVAL;
    }
//
  for(int i = 0; i <= NUM_LEDS  ; i++ ){
    leds[i].setHSV(0,0,0);
    }
  FastLED.show();
    target = 0;
    now_led = 0;
  }

void loop() {
  int count = 0;
  int ptr = 0;
  
  int power = 0;
  read_length = 0;
  read_length = Serial.available();

  if(read_length != 0)
  {
    //Serial.println(RX);
    Serial.print("length:");
    Serial.println(read_length);
    while(true)
    {
      for(int i=ptr;i<ptr+read_length;i++)
      {
        RX[i] = Serial.read();
      }
      ptr += read_length;

      //NL&CR
      //RX = {.....\r\n}
      if(RX[ptr-1]==13)
      {
        RX[ptr] = "\0";
        Serial.println("break");
        break;
      }
    } 
    Serial.println(RX);
  }
  
  if(RX[0] == 65)
  {     //"A" = 65
    Serial.println("State A");
    BlinkLight();
  }
  else if(RX[0] == 66)
  {    //"B" = 66
    Serial.println("State B");
    ThunderLight();
  }
  else if(RX[0] == 67)
  {
        //"C" = 67
    Serial.println("State C");
    MeteorLight();
  }  
  else 
  {
    val = CalculateNum(RX,ptr);
    VolumeLight(val);
  }


//ThunderLight();
//BlinkLight();
 // delay(10);
}


  //Serial.println("a"); 
//  for(int i=0; i<=10 ; i++)
//    ThunderLight();
//  for(int i=0; i<=10 ; i++)
//    MeteorLight();

 // leds[0].setHSV(0,0,50);
//  FastLED.show();
//  delay(2000); 
//  leds[1].setHSV(0,0,50);
//  FastLED.show();
//  delay(2000);
//  leds[2].setHSV(0,0,50);
//  FastLED.show();
//  delay(2000);
//    leds[3].setHSV(0,0,50);
//  FastLED.show();
//  delay(2000)

float CalculateNum(char buf[],int ptr){
  float val=0;
  int power=0;
  for(int i=ptr-2 ; i>=0 ; i--){
    if(buf[i]<48 && buf[i] < 49){
      Serial.print("error");
      break;
      }
    else  {
      val = val + pow(10,power)*(RX[i]-48); 
      power++;
      }
    }
  return val;
  }
  
void VolumeLight(int vol)
{
    leds[0].setRGB(0,255,0);
    FastLED.show();
    for(int i=0;i<NUM_LEDS;i++)
    {
      if(vol >= (MAX_VOL/NUM_LEDS)*i && vol < (MAX_VOL/NUM_LEDS)*(i+1))
      {
        target = i;
      }
    }
    if(target!=now_led)
    {
      if(target > now_led)
      {
        now_led++;
        leds[now_led].setHSV(0,0,100);
        FastLED.show();
        delay(500);
        
      }
      else
      {
        leds[now_led].setHSV(0,0,0);
        FastLED.show();
        delay(500);
        if(now_led>0)
        {
          now_led--;
        }
      }
    }
}

void Sleep(){
  leds[0].setRGB(0,255,0);
  FastLED.show();
  delay(10);
  }

void ThunderLight(){ 
  //Turn off all light
  for(int j = 0; j <= NUM_LEDS  ; j++ ){
    leds[j].setHSV(0,0,0);
    }
    
  if(thunder_flag == 0){
    if(thunder_count < NUM_LEDS){
      
      leds[thunder_count].setHSV(0,100,100);
      if(thunder_count!=0)
        leds[thunder_count-1].setHSV(0,0,0);
      FastLED.show();
      delay(50);
      
      if(thunder_count == NUM_LEDS -1){
//        leds[thunder_count].setHSV(20,100,100);
//        FastLED.show();
 //       delay(10000);
        thunder_flag = 1;
        thunder_count = NUM_LEDS-2;
        }
      else
        thunder_count++;  
      }
  }
  if(thunder_flag == 1){
    if(thunder_count > 0){
      leds[thunder_count].setHSV(0,100,100);
      if(thunder_count!=NUM_LEDS-1)
        leds[thunder_count+1].setHSV(0,0,0);
        FastLED.show();
        delay(50);
        thunder_count--;
        if(thunder_count == 0){
          thunder_flag = 0;
          thunder_count = 0;
        }
      }
  }
  }


  
void MeteorLight(){
  for(int i=0 ; i < NUM_LEDS+NUM_METEORS ; i++){
    for(int j = 0; j <= NUM_LEDS  ; j++ ){
    leds[j].setHSV(0,0,0);
    }
    
    if(i < NUM_METEORS){
      leds_start = 0;
      start = NUM_METEORS - i;
      num = i;
      }
      
    else if(i > NUM_LEDS){
      leds_start = i-NUM_METEORS;
      start = 0;
      num = (NUM_LEDS+NUM_METEORS) - i;
    }
    
    else{
      leds_start = i-NUM_METEORS;
      start = 0;
      num = NUM_METEORS;
      }
      
//      Serial.print("i:");
//      Serial.print(i);
//      Serial.print("\tled:");
//      Serial.print(leds_start);
//      Serial.print("\tnum:");
//      Serial.print(num);
//      Serial.print("\t meteors:");
//      Serial.println(start);
      
    for(int j = 0 ; j < num ; j++){
      leds[leds_start].setHSV(meteors[start][0],meteors[start][1],meteors[start][2]); 
      leds_start ++;
      start ++ ;

      }
    FastLED.show();
    delay(50);
  }
}

void BlinkLight(){
  for(int i=0; i<=MAX_BRIGHTNESS;i=i+10){
    for(int j=0; j<NUM_LEDS ; j++){
      leds[j].setHSV(0,0,i);   
      }
    FastLED.show();
    delay(10);
    }
  for(int i=MAX_BRIGHTNESS ; i>=0 ; i=i-10){
    for(int j=0; j<NUM_LEDS ; j++){
      leds[j].setHSV(0,0,i);   
      }
    FastLED.show();
    delay(10);
    }
}






    

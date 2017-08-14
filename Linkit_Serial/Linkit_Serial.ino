#include <ArduinoJson.h>
#include <Wire.h> 
#include <SoftwareSerial.h>


    long pmcf10=0;
    long pmcf25=0;
    long pmcf100=0;
    long pmat10=0;
    long pmat25=0;
    long pmat100=0;

    String fpmcf010;
    String fpmcf025;
    String fpmcf100;

    String fpmat010;
    String fpmat025;
    String fpmat100;


// instead of using Serial1 for the UART,
// using software serial TX/RX to read PMS3003 sensor:

SoftwareSerial SSerial(9,10);


// initciate setup LCD address to 0x27 for a 16 chars and 2 line display:
#define ROW_NUM 2                             // 2��� 嚗�虾憿舐內2��梹��
#define COL_NUM 16                            // 16甈�嚗����堒虾憿舐內16�见�梹��
  


// initciate setup for DS1307 RTC:
const byte DS1307_I2C_ADDRESS = 0x68; // DS1307 (I2C) �𧑐��
const byte NubberOfFields = 7;        // DS1307 (I2C) 鞈��嗵����
int yy;                               // 撟�
byte mm, dd, ww, hh, mi, ss;          // ���/�𠯫/��/��/��/蝘�
String RTC_DATE, RTC_TIME;            // �𠯫���/�����



void setup() {
// put your setup code here, to run once:

  Serial.begin(9600);
  SSerial.begin(9600);

  
  Wire.begin();

  // Linkit Smart 7688 Duo use URAT1 which is Serial1 for data exchange between MCU & MPU 
  // to communicate with MPU, MCU side need to set baud rate to 57600 
  Serial1.begin(57600);          

  delay(3000);

}

void loop() {

    getTime(); // ��硋�埈����

    int count = 0;
    unsigned char c;
    unsigned char high;

    while (SSerial.available()) {

      c = SSerial.read();
      
      if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){
        Serial.println("check failed");
        break;
      }

      if(count > 15){
        /* Serial.println("complete"); */
        break;
      }
      else if(count == 4 || count == 6 || count == 8 || count == 10 || count == 12 || count == 14) high = c;
      else if(count == 5){
        pmcf10 = 256*high + c;
      }
      else if(count == 7){
        pmcf25 = 256*high + c;
      }
      else if(count == 9){
        pmcf100 = 256*high + c;
      }
      else if(count == 11){
        pmat10 = 256*high + c;
      }
      else if(count == 13){
        pmat25 = 256*high + c;
      }
      else if(count == 15){
        pmat100 = 256*high + c;
      }

      count++;

   }



    fpmcf010 = FormatDigits(pmcf10); 
    fpmcf025 = FormatDigits(pmcf25); 
    fpmcf100 = FormatDigits(pmcf100); 

    fpmat010 = FormatDigits(pmat10); 
    fpmat025 = FormatDigits(pmat25); 
    fpmat100 = FormatDigits(pmat100); 


    StaticJsonBuffer<200> jsonBuffer;   // if your payload size is too big, please extend buffer to 300 or whatever.

    JsonObject& Jasonroot = jsonBuffer.createObject();
    Jasonroot["sensor"] = "PMS3003";
    Jasonroot["date"] = RTC_DATE;
    Jasonroot["time"] = RTC_TIME;

    Jasonroot["pm010_TSI"] = fpmcf010;
    Jasonroot["pm010_ATM"] = fpmat010;
    Jasonroot["pm025_TSI"] = fpmcf025;
    Jasonroot["pm025_ATM"] = fpmat025;
    Jasonroot["pm100_TSI"] = fpmcf100;
    Jasonroot["pm100_ATM"] = fpmat100;
  

    Jasonroot.printTo(Serial1);
    Jasonroot.prettyPrintTo(Serial);
    /*Jasonroot.prettyPrintTo(Serial1);*/
    Serial1.print("\r\n");
    

//    lcd.setCursor(0, 0); 
//    lcd.println("PM = 1.0/2.5/10         ");
    
//    lcd.setCursor(0, 1);
//    lcd.println("VL = " + fpmat010 + "/" + fpmat025 + "/" + fpmat100+"     ");

   
    while(SSerial.available()) SSerial.read();
    
    delay(10000);

}




String FormatDigits(int digits) {
  // utility for digital clock display: prints preceding colon and leading 0
  String StrInput = String(digits, DEC);

  if (digits >= 10) {
    return StrInput;
  } else {
    return "0" + StrInput;
  }

}





/* BCD 頧� DEC */ byte bcdTodec(byte val){ return ((val / 16 * 10) + (val % 16)); }
/* DEC 頧� BCD */ byte decToBcd(byte val){ return ((val / 10 * 16) + (val % 10)); }

/* 閮剖�𡁏����    */

void setTime(byte y, byte m, byte d, byte w, byte h, byte mi, byte s){
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write(0);
    Wire.write(decToBcd(s));
    Wire.write(decToBcd(mi));
    Wire.write(decToBcd(h));
    Wire.write(decToBcd(w));
    Wire.write(decToBcd(d));
    Wire.write(decToBcd(m));
    Wire.write(decToBcd(y));
    Wire.endTransmission();
}

/* ��硋�埈���� */

void getTime(){
    Wire.beginTransmission(DS1307_I2C_ADDRESS);
    Wire.write(0);
    Wire.endTransmission();

    Wire.requestFrom(DS1307_I2C_ADDRESS, NubberOfFields);

    ss = bcdTodec(Wire.read() & 0x7f);
    mi = bcdTodec(Wire.read());
    hh = bcdTodec(Wire.read() & 0x7f);
    ww = bcdTodec(Wire.read());
    dd = bcdTodec(Wire.read());
    mm = bcdTodec(Wire.read());
    yy = bcdTodec(Wire.read()) + 2000;

    RTC_TIME = FormatDigits(hh) + ":" 
             + FormatDigits(mi) + ":" 
             + FormatDigits(ss); 
    RTC_DATE = FormatDigits(yy) + "-" 
             + FormatDigits(mm) + "-" 
             + FormatDigits(dd);
}

// 憿舐內�����
void digitalClockDisplay(){
    Serial.println(RTC_DATE + " " + RTC_TIME );
}



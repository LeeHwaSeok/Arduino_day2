/*  라이브 선언 및 Port(pin) 번호 설정   */

//DHT (온도/습도) 
#include "DHT.h"        // DHT 사용 
#define DHTPIN A1       // 온/습도 - A1 pin
#define DHTTYPE DHT22   // DHT22 버전을 사용하겠다. 
DHT dht(DHTPIN, DHTTYPE); // 핀 = A1 , 모델 = DHT22

//SOIL-LINK (토양 습도)
#define SOILHUMI A6     // 토양 습도 - A6 pin

//OLED 
#include <U8g2lib.h>    // OLED 사용
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//PUMP 
#define PUMP 16

//LED 
#define LAMP 17

//PWM [dc_ fan]
#include <SoftPWM.h>    // DC 모터 사용
SOFTPWM_DEFINE_CHANNEL(A3); // DC 모터 - A3 pin



/*  전역변수 선언   */
uint32_t delay1 = 2000;  //2초마다 실행 
uint32_t delay2 = 3000;  //3초마다 실행 
long time = millis();    //실행시간 시간

float Temp;     //온도 사용 위한 선언
float Humi;     //습도 사용 위한 선언
int soil = 0;   //토양 습도확인
int _fan = 0;   //fan 속도 확인 
String flag_l =""; // Lamp On/Off 
String flag_p =""; // Pump On/Off


//main
void setup() {
  SoftPWM.begin(490);
  Serial.begin(9600);

  //온도 습도 + OELD + 토지 + 펌프 + LAMP 사용
  dht.begin();
  u8g2.begin();
  pinMode(PUMP,OUTPUT);
  pinMode(SOILHUMI,INPUT);
  pinMode(LAMP,OUTPUT);

}

void loop() {
  digitalWrite(LAMP,LOW);
  flag_l = "X";
  
  if( time + 2000 < millis()){ // 2초마다 온/습도 체크
    Humi = dht.readHumidity();
    Temp = dht.readTemperature();
    digitalWrite(LAMP,HIGH);
    flag_l = "O";
    
    if (isnan(Humi) || isnan(Temp)){ //에러 처리
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
   }
  }  

  if(time + 3000 < millis()){ // 3초마다 토양 습도 체크
      soil = map(analogRead(SOILHUMI),0,1023,100,0);  
      time = millis() ;
  }
  
  if (30 < soil && soil < 60) {
    digitalWrite(PUMP,LOW);
    flag_p = "X";
  }
  else if(soil < 30) {
    digitalWrite(PUMP,HIGH);
    flag_p = "O";  
    }
  
  if (Temp < 29){
     SoftPWM.set(80);
     _fan = 80;
  }  else if (Temp > 20) {
     SoftPWM.set(0);
     _fan = 0;
  }  else {
     SoftPWM.set(65);
     _fan = 65;
  }

  OLEDdraw();
}



//OLED 함수선언
void OLEDdraw(){
  //매 출력 시작할 때 초기화
  u8g2.clearBuffer();
  
  //폰트 지정 및 동준우 출력
  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1, 15, "DonjunWoo");

  //온도
  u8g2.drawStr(15, 26, "Temp");
  u8g2.setCursor(85,26);
  u8g2.print(Temp);
  u8g2.drawStr(114,26,"\xb0");
  u8g2.drawStr(119,26,"C");

  //습도
  u8g2.drawStr(15,37,"Humidity");
  u8g2.setCursor(85,37);
  u8g2.print(Humi);
  u8g2.drawStr(116,37,"%");

  //토양 습도 체크
  u8g2.drawStr(15,48,"Soil Humi");
  u8g2.setCursor(85,48);
  u8g2.print(soil);
  u8g2.drawStr(116,48,"%");

  //Fan, Pump, Lamp 출력
  u8g2.drawStr(15,59,"F/P/L");
  u8g2.setCursor(85,59);
  u8g2.print(_fan);
  u8g2.drawStr(100,59,"/");

  u8g2.setCursor(105,59);
  u8g2.print(flag_p);
  u8g2.drawStr(115,59,"/");

  u8g2.setCursor(120,59);
  u8g2.print(flag_l);
  u8g2.drawStr(130,59,"/");
    
  //버퍼 전송
  u8g2.sendBuffer();
}

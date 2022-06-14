#define PUMP 16
#define SOILHUMI A6

int _cycle = 2000;  // 주기는 2초로
int soil = 0;       // 토양 습도 값 받아오기

void setup() {
  Serial.begin(9600);
  pinMode(PUMP,OUTPUT);
  pinMode(SOILHUMI,INPUT);
}

void loop() {
  soil = map(analogRead(SOILHUMI),0,1023,100,0); // 범위 선전 0~1023 -> 0~100

  if (soil < 30){
    digitalWrite(PUMP,HIGH);
  }else if(soil > 60){
    digitalWrite(PUMP,LOW);
  }

  Serial.println(PUMP);
  Serial.println(soil);
  delay(_cycle);
  
}

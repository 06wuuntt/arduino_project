#include <TM1637.h>
#include <Servo.h>
#define CLK 2
#define DIO 3
#define BUTTONPIN1 9
#define BUTTONPIN2 8
#define BUTTONPIN3 7
#define BUTTONPIN4 6
#define BUTTONPIN5 5
#define BUTTONPIN6 4

TM1637 tm1637(CLK, DIO);
Servo myservo;  //馬達命名
int point_flag = 1;
int state1, state2, state3, state4, state5, state6;
int flag1 = LOW, flag2 = LOW, flag3 = LOW, flag4 = LOW, flag5 = LOW, flag6 = LOW;
int thousand, hundred, ten, one, countdown;

void setup() {
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);
  pinMode(BUTTONPIN1, INPUT);
  pinMode(BUTTONPIN2, INPUT_PULLUP);
  pinMode(BUTTONPIN3, INPUT_PULLUP);
  pinMode(BUTTONPIN4, INPUT_PULLUP);
  pinMode(BUTTONPIN5, INPUT_PULLUP);
  pinMode(BUTTONPIN6, INPUT_PULLUP);
  int thousand = 0, hundred = 0, ten = 0, one = 0;
  Serial.begin(9600);
  myservo.attach(10);
}

void deBouncing() {  //按鈕反彈跳設計，並藉由前五顆按鈕設定顯示屏上的數字
  if (state1 == HIGH && flag1 == LOW) {
    thousand += 1;  //千位數+1
    flag1 = HIGH;
    Serial.println(digitalRead(BUTTONPIN1));
    if (thousand == 10) {
      thousand = 0;
    }
  } else if (state1 == LOW && flag1 == HIGH) {
    flag1 = LOW;
  }
  if (state2 == HIGH && flag2 == LOW) {
    hundred += 1;  //百位數+1
    flag2 = HIGH;
    if (hundred == 10) {
      hundred = 0;
    }
  } else if (state2 == LOW && flag2 == HIGH) {
    flag2 = LOW;
  }
  if (state3 == HIGH && flag3 == LOW) {
    ten += 1;  //十位數+1
    flag3 = HIGH;
    if (ten == 10) {
      ten = 0;
    }
  } else if (state3 == LOW && flag3 == HIGH) {
    flag3 = LOW;
  }
  if (state4 == HIGH && flag4 == LOW) {
    one += 1;  //個位數+1
    flag4 = HIGH;
    if (one == 10) {
      one = 0;
    }
  } else if (state4 == LOW && flag4 == HIGH) {
    flag4 = LOW;
  }
  if (state5 == HIGH && flag5 == LOW) {
    thousand = hundred = ten = one = 0;  //歸零
    flag5 = HIGH;
  } else if (state5 == LOW && flag5 == HIGH) {
    flag5 = LOW;
  }
}
void countdowning() {  //倒計時
  while (countdown != 0) {
    int thousand = countdown / 1000;
    int hundred = (countdown / 100) % 10;
    int ten = (countdown / 10) % 10;
    int one = countdown % 10;  //將秒數的位數分別宣告成個十百千
    tm1637.display(0, thousand);
    tm1637.display(1, hundred);
    tm1637.display(2, ten);
    tm1637.display(3, one);  //將數字呈現在顯示屏上
    countdown--;
    delay(1000);  //每過一秒，秒數減一
  }
  if (countdown == 0) {  //當countdown=0時，馬達轉成解鎖狀態
    unlock();
  }
}
void unlock() {
  myservo.write(0);
}

void loop() {
  state1 = digitalRead(BUTTONPIN1);
  state2 = digitalRead(BUTTONPIN2);
  state3 = digitalRead(BUTTONPIN3);
  state4 = digitalRead(BUTTONPIN4);
  state5 = digitalRead(BUTTONPIN5);
  state6 = digitalRead(BUTTONPIN6);

  tm1637.display(0, thousand);
  tm1637.display(1, hundred);
  tm1637.display(2, ten);
  tm1637.display(3, one);

  deBouncing();
  countdown = thousand * 1000 + hundred * 100 + ten * 10 + one;  //倒數計時秒數

  if (state6 == HIGH && flag6 == LOW) {  //按下此按鈕即可開始倒計時，且馬達上鎖，同樣利用反彈跳理論
    flag6 = HIGH;
    myservo.write(90);
    countdowning();

  } else if (state6 == LOW && flag6 == HIGH) {
    flag6 = LOW;
  }
}

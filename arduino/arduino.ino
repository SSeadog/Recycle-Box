#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 150
#define SERVOMAX 600
#define USMIN 600
#define USMAX 2400
#define SERVO_FREQ 50

int trigPin = 12;
int echoPin = 11;
bool st1, st2, st3;
char kind;

// 모터 돌리는 함수
// 당시에 사용했던 모터의 위치를 기준으로 몇번 모터를 몇만큼 돌리는지 작성한 함수
// 따라서 모터를 돌리기 위해서 어떻게 작성하는지만 참고
void mv(char n)
{
  if (n == '1')
  {
    move1();
  }
  else if (n == '2')
  {
    move2();
  }
  else if (n == '3')
  {
    move3();
  }
  else
  {
    move4();
  }
}

void move1()
{ //1번째 칸에 버리는 기준

  for (uint16_t pulselen = 275; pulselen < 480; pulselen += 2)
  {
    pwm.setPWM(0, 0, pulselen);
    delay(1);
  }

  delay(200);

  for (uint16_t pulselen = 480; pulselen > 275; pulselen--)
  {
    pwm.setPWM(0, 0, pulselen);
    delay(1);
  }
  pwm.setPWM(0, 0, 4096);
  delay(150);

  for (uint16_t pulselen = 220; pulselen < 490; pulselen++)
  {
    pwm.setPWM(2, 0, pulselen);
    delay(1);
  }

  delay(600);

  for (uint16_t pulselen = 490; pulselen > 220; pulselen--)
  {
    pwm.setPWM(2, 0, pulselen);
    delay(1);
  }
  pwm.setPWM(2, 0, 4096);
}

void move2()
{ //2번째 칸에 버리는 기준
  for (uint16_t pulselen = 275; pulselen < 480; pulselen++)
  {
    pwm.setPWM(0, 0, pulselen);
    delay(1);
  }

  delay(200);

  for (uint16_t pulselen = 480; pulselen > 275; pulselen--)
  {
    pwm.setPWM(0, 0, pulselen);
    delay(1);
  }
  pwm.setPWM(0, 0, 4096);
  delay(150);

  for (uint16_t pulselen = 220; pulselen > 60; pulselen--)
  {
    pwm.setPWM(2, 0, pulselen);
    delay(2);
  }

  delay(1000);

  for (uint16_t pulselen = 60; pulselen < 220; pulselen++)
  {
    pwm.setPWM(2, 0, pulselen);
    delay(1);
  }
  pwm.setPWM(2, 0, 4096);
}

void move3()
{ //3번째 칸에 버리는 기준
  for (uint16_t pulselen = 275; pulselen > 60; pulselen -= 2)
  {
    pwm.setPWM(0, 0, pulselen);
    delay(1);
  }

  delay(200);

  for (uint16_t pulselen = 60; pulselen < 275; pulselen++)
  {
    pwm.setPWM(0, 0, pulselen);
    delay(1);
  }
  pwm.setPWM(0, 0, 4096);
  delay(150);

  for (uint16_t pulselen = 430; pulselen < 540; pulselen++)
  {
    pwm.setPWM(1, 0, pulselen);
    delay(1);
  }

  delay(1000);

  for (uint16_t pulselen = 540; pulselen > 430; pulselen--)
  {
    pwm.setPWM(1, 0, pulselen);
    delay(1);
  }
  pwm.setPWM(1, 0, 4096);
}

void move4()
{ //4번째 칸에 버리는 기준
  for (uint16_t pulselen = 275; pulselen < 550; pulselen += 4)
  {
    pwm.setPWM(0, 0, pulselen);
  }

  delay(200);

  for (uint16_t pulselen = 550; pulselen > 80; pulselen -= 4)
  {
    pwm.setPWM(0, 0, pulselen);
    delay(1);
  }

  delay(700);

  for (uint16_t pulselen = 80; pulselen < 275; pulselen++)
  {
    pwm.setPWM(0, 0, pulselen);
    delay(1);
  }
  pwm.setPWM(0, 0, 4096);
  delay(150);

  for (uint16_t pulselen = 430; pulselen > 130; pulselen--)
  {
    pwm.setPWM(1, 0, pulselen);
    delay(1);
  }

  delay(1000);

  for (uint16_t pulselen = 130; pulselen < 430; pulselen++)
  {
    pwm.setPWM(1, 0, pulselen);
    delay(1);
  }
  pwm.setPWM(1, 0, 4096);
}

void setup()
{

  Serial.begin(9600);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  pwm.setPWM(0, 0, 4096);
  pwm.setPWM(1, 0, 4096);
  pwm.setPWM(2, 0, 4096);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  st1 = true;
  st2 = false;
  st3 = false;
}

// 절차적 특성을 이용해 무한반복되는 loop문 아래에 실행할 동작을 state(st)별로 관리함
// st1->st2->st3->st1->··· 이런식으로 무한 반복되도록 구성함
void loop()
{
  String result;
  // st1 => 분류할 물건이 들어오기를 기다리는 상태
  if (st1)
  {
    // 초음파 센서로부터 물체간의 거리를 인식하는 부분. 212~218번줄을 거쳐 distance에는 cm단위로 거리 값이 저장됨.
    long duration, distance;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = 17 * duration / 1000;

    Serial.println(distance);
    // 센서와 물건 간의 거리가 20cm이하가 되면 물건이 들어왔다고 판단
    if (distance <= 20)
    {
      pwm.setPWM(0, 0, 275);
      delay(500);
      pwm.setPWM(0, 0, 4096);
      Serial.println("1"); // Serial통신으로 1출력. 이 출력 값을 python프로그램에서 읽은 후 물건 분류 시작
      st2 = true;          // st2부분 실행을 위해 true로 변경
      st1 = false;         // st1부분은 실행되지 않도록 하기 위해 false로 변경
    }
  }
  // st2 =>  python프로그램의 분류 신호를 기다리는 상태
  if (st2)
  {
    if (Serial.available()) // Serial포트에 값이 들어올 때까지 기다림
    {
      kind = Serial.read(); // 물건의 종류
      Serial.read();        // python프로그램에서 Serial통신으로 값을 출력할 때 1을 출력시키면 줄넘김문자가 함께 넘어오므로 줄넘김 문자 제거

      st3 = true;  // st3부분 실행을 위해 true로 변경
      st2 = false; // st2부분은 실행되지 않도록 하기 위해 false로 변경
    }
  }
  // st3 => 분류 값을 기준으로 모터를 돌려 물건을 버리는 상태
  if (st3)
  {
    mv(kind);    // 모터 돌려서 물건 버림
    st3 = false; // st3부분은 실행되지 않도록 하기 위해 false로 변경
    st1 = true;  // st1부분 실행을 위해 true로 변경
  }
  delay(50);
}
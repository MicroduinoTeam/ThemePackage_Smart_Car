#include <Microduino_Motor.h>

ColorLED strip = ColorLED(LED_NUM, PIN_LED);

/***********定义彩灯颜色*/
const uint32_t colorArray[10] = {
  COLOR_RED,        //0xff0000, 红色，Red
  COLOR_ORANGE,     //0xea8b17, 橙色，Orange
  COLOR_YELLOW,     //0xffff00, 黄色，Yellow
  COLOR_GREEN,      //0x00ff00, 绿色，Green
  COLOR_CYAN,       //0x00ffff, 青色，Cyan
  COLOR_BLUE,       //0x0000ff, 蓝色，Blue
  COLOR_PURPLE,     //0xff00ff, 紫色，Purple
  COLOR_WARM,       //0x9b6400, 暖色，Warm
  COLOR_COLD,       //0x648264, 冷色，Cold
  COLOR_NONE       //0x000000, 彩灯关闭，colorLED OFF
};

#define CMD_STOP   -1
#define CMD_UP     1
#define CMD_DOWN   2
#define CMD_LEFT   3
#define CMD_RIGHT  4

#define RIGHT  1
#define LEFT   0

Motor MotorLeft(MOTOR0_PINA, MOTOR0_PINB);
Motor MotorRight(MOTOR1_PINA, MOTOR1_PINB);

bool leftFlag = false;
bool rightFlag = false;
bool back;
int16_t leftSpeed = 0;
int16_t rightSpeed = 0;
uint32_t micTime = 0;
uint32_t trackTime = 0, trackTime1 = 0;

bool fback = true;

void motorUpdate(int16_t speed1, int16_t speed2)
{
  MotorLeft.setSpeed(speed1);
  MotorRight.setSpeed(speed2);
}

void motorCoast()
{
  leftSpeed = 0;
  rightSpeed = 0;
}

void bleControl(int8_t bleCmd)
{
  switch (bleCmd)
  {
    case CMD_UP:
      strip.setAllLED(COLOR_COLD);
      strip.show();
      leftSpeed = MAX_THROTTLE;
      rightSpeed = MAX_THROTTLE;
      fback = true;
      break;
    case CMD_DOWN:
      strip.setAllLED(COLOR_RED);
      strip.show();
      leftSpeed = -(255 - MAX_THROTTLE);
      rightSpeed = -(255 - MAX_THROTTLE);
      fback = false;
      break;
    case CMD_LEFT:
      if (fback)
      {
        leftSpeed = MAX_THROTTLE - STEER;
        rightSpeed = MAX_THROTTLE + STEER;
        strip.setOneLED(RIGHT, COLOR_NONE);
        strip.show();
      }
      else
      {
        leftSpeed = -MAX_THROTTLE + STEER;
        rightSpeed = -MAX_THROTTLE + - STEER;
        strip.setOneLED(RIGHT, COLOR_NONE);
        strip.show();
      }
      //ledBlink(500, COLOR_RED, LEFT);
      break;
    case CMD_RIGHT:
      if (fback)
      {
        leftSpeed = MAX_THROTTLE + STEER;
        rightSpeed = MAX_THROTTLE - STEER;
        strip.setOneLED(LEFT, COLOR_NONE);
        strip.show();
      }
      else
      {
        leftSpeed = -MAX_THROTTLE - STEER;
        rightSpeed = -MAX_THROTTLE + STEER;
        strip.setOneLED(LEFT, COLOR_NONE);
        strip.show();
      }
      //ledBlink(500, COLOR_RED, RIGHT);
      break;
    case CMD_STOP:
      if (leftSpeed != 0 || rightSpeed != 0)
      {
        strip.setAllLED(COLOR_NONE);
        strip.show();
        leftSpeed = 0;
        rightSpeed = 0;
      }
      break;
    default:
      break;
  }
}

void trackControl(uint16_t trackVal1, uint16_t trackVal2)
{
  if (trackVal1 < TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD)
  {
    leftSpeed = MAX_THROTTLE - tackforward;
    rightSpeed = MAX_THROTTLE - tackforward;
    strip.setAllLED(COLOR_COLD);
    strip.show();
  }
  else if (trackVal1 < TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD)
  {
    leftSpeed = MAX_THROTTLE - tackforward + tacksteer;
    rightSpeed = MAX_THROTTLE - tackforward - tacksteer;
    strip.setOneLED(LEFT, COLOR_WARM);
    strip.setOneLED(RIGHT, COLOR_NONE);
    strip.show();
  }
  else if (trackVal1 > TRACK_THRESHOLD && trackVal2 < TRACK_THRESHOLD)
  {
    leftSpeed = MAX_THROTTLE - tackforward - tacksteer;
    rightSpeed = MAX_THROTTLE - tackforward + tacksteer;
    strip.setOneLED(RIGHT, COLOR_WARM);
    strip.setOneLED(LEFT, COLOR_NONE);
    strip.show();
  }
  else if (trackVal1 > TRACK_THRESHOLD && trackVal2 > TRACK_THRESHOLD)
  {
    strip.setAllLED(COLOR_NONE);
    strip.show();
    leftSpeed = 0;
    rightSpeed = 0;
  }
}

void micControl(long micVal) {
  if (micVal > 300 && micTime == 0)
    micTime = millis() + 6000;
  else if (millis() < micTime - 4000 && micTime > 0)
  {
    leftSpeed = MAX_THROTTLE;
    rightSpeed = MAX_THROTTLE;
    delay(2);
  }
  else if (micTime - 4000 < millis() && millis() < micTime - 2000 && micTime > 0)
  {
    leftSpeed =  MAX_THROTTLE;
    rightSpeed = -(255 - MAX_THROTTLE);
    delay(2);
  }
  else if (micTime - 2000 < millis() && millis() < micTime && micTime > 0)
  {
    leftSpeed = MAX_THROTTLE;
    rightSpeed = MAX_THROTTLE;
    delay(2);
  }
  else {
    leftSpeed = 0;
    rightSpeed = 0;
    micTime = 0;
  }
}

void soundmode(uint8_t _mode)
{
  if (_mode == 0)
  {
    tone(PIN_BUZZER, 523);
    strip.setAllLED(COLOR_RED);
    strip.show();
    delay(100);
    noTone(PIN_BUZZER);
    strip.setAllLED(COLOR_NONE);
    strip.show();
    delay(100);
    tone(PIN_BUZZER, 523);
    strip.setAllLED(COLOR_RED);
    strip.show();
    delay(100);
    noTone(PIN_BUZZER);
    strip.setAllLED(COLOR_NONE);
    strip.show();
    delay(100);
    tone(PIN_BUZZER, 523);
    strip.setAllLED(COLOR_RED);
    strip.show();
    delay(100);
    noTone(PIN_BUZZER);
    strip.setAllLED(COLOR_NONE);
    strip.show();
    delay(100);
  }
  if (_mode == 1)
  {
    tone(PIN_BUZZER, 523);
    strip.setAllLED(COLOR_RED);
    strip.show();
    delay(300);
    noTone(PIN_BUZZER);
    strip.setAllLED(COLOR_NONE);
    strip.show();
    delay(300);
  }
  else if (_mode == 2)
  {
    tone(PIN_BUZZER, 523);
    strip.setAllLED(COLOR_GREEN);
    strip.show();
    delay(300);
    noTone(PIN_BUZZER);
    strip.setAllLED(COLOR_NONE);
    strip.show();
    delay(300);
    tone(PIN_BUZZER, 590);
    strip.setAllLED(COLOR_GREEN);
    strip.show();
    delay(300);
    noTone(PIN_BUZZER);
    strip.setAllLED(COLOR_NONE);
    strip.show();
    delay(300);
  }
  else if (_mode == 3)
  {
    tone(PIN_BUZZER, 500);
    strip.setAllLED(COLOR_BLUE);
    strip.show();
    delay(300);
    noTone(PIN_BUZZER);
    strip.setAllLED(COLOR_NONE);
    strip.show();
    delay(300);
    tone(PIN_BUZZER, 590);
    strip.setAllLED(COLOR_BLUE);
    strip.show();
    delay(300);
    noTone(PIN_BUZZER);
    strip.setAllLED(COLOR_NONE);
    strip.show();
    delay(300);
    tone(PIN_BUZZER, 660);
    strip.setAllLED(COLOR_BLUE);
    strip.show();
    delay(300);
    noTone(PIN_BUZZER);
    strip.setAllLED(COLOR_NONE);
    strip.show();
    delay(300);
  }
}

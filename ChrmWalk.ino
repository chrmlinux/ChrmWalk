
#include <ESP32Servo.h>

#define SERVO_MAX   (4)

#define LIM_DEG     (45)
#define MAX_DEG     (180 - LIM_DEG)
#define MIN_DEG     (  0 + LIM_DEG)
#define WALK_DELAY  (30)
#define WALK_STEP   (-4)

#define MIN_uSEC ( 600)
#define MAX_uSEC (2500)

enum {RIGHT_TOES = 0, RIGHT_HIP, LEFT_TOES, LEFT_HIP};
enum {FWD = 0, BACK, LEFT, RIGHT};
typedef struct {
  int pin;    // pin no.
  int deg;    // degree
} SERVO_T;
Servo sv[SERVO_MAX];

SERVO_T _svc[SERVO_MAX] = {
  33, 90,
  23, 90,
  19, 90,
  22, 90,
};
static int _lr = 0;

void setDeg(int hipdeg, int toesdeg)
{
  _svc[RIGHT_HIP].deg = hipdeg;
  _svc[LEFT_HIP].deg = hipdeg;
  _svc[RIGHT_TOES].deg = (180 - toesdeg);
  _svc[LEFT_TOES].deg = (180 - toesdeg);
}

void execServo() {
  for (int i = 0; i < SERVO_MAX; i++) {
    sv[i].write(_svc[i].deg);
  }
  delay(WALK_DELAY);
}

void walk(int fb)
{
  static int hipdeg = 90;
  static int toesdeg = 90;
  static int stp = WALK_STEP;

  Serial.printf("%d %d\n", hipdeg, stp);
  setDeg(hipdeg, toesdeg);
  execServo();
  hipdeg += stp;
  toesdeg += stp;
  if ((hipdeg <= MIN_DEG) || (hipdeg >= MAX_DEG)) {
    stp = -stp;
    toesdeg = 90;
    _lr = (_lr + 1) % (SERVO_MAX / 2);
  }

}

void turn(int lr)
{
}

void setup() {
  Serial.begin( 115200 );
  for (int i = 0; i < SERVO_MAX; i++) {
    sv[i].setPeriodHertz(50);
    sv[i].attach(_svc[i].pin, MIN_uSEC, MAX_uSEC);
    sv[i].write (_svc[i].deg);
  }
}

void loop()
{
  walk(FWD);
  delay(1);
}

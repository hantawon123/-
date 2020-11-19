// Arduino pin assignment
#include <Servo.h>
#define PIN_IR A0
#define PIN_LED 9
#define PIN_SERVO 10
#define _SERVO_SPEED 270
#define INTERVAL 20

#define _DUTY_MIN 1350
#define _DUTY_MID 1450
#define _DUTY_MAX 1550
Servo myservo;

#define _DIST_ALPHA 0.5

//global variables
int a, b; // unit: mm
int duty_chg_per_interval;
float dist_ema, alpha;
int duty_target, duty_curr;
float pause_time;
int toggle_interval, toggle_interval_cnt;

void setup() {
  // initialize serial port
  Serial.begin(115200);
  myservo.attach(PIN_SERVO);
  duty_target = duty_curr = _DUTY_MID;
  myservo.writeMicroseconds(duty_curr);
  // convert angle speed into duty change per interval.
  duty_chg_per_interval = (_DUTY_MAX - _DUTY_MIN) * ((float)_SERVO_SPEED / 180) * ((float)INTERVAL / 1000);
  a = 95;
  b = 380;

  // initialize variables for servo update.
  pause_time = 1;
  toggle_interval = (180.0 / _SERVO_SPEED + pause_time) * 1000 / INTERVAL;
  toggle_interval_cnt = toggle_interval;
}
float ir_distance(void) { // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0 / (volt - 9.0)) - 4.0) * 10.0;
  return val;
}
void loop() {
  float raw_dist = ir_distance();
  float dist_cali = 95 + 380.0 / (b - a) * (raw_dist - a);
  alpha = 0.9;
  dist_ema = alpha * dist_cali + (1 - alpha) * dist_ema;

    
  if (dist_ema < 255)
    duty_target = _DUTY_MIN;
  else
    duty_target = _DUTY_MAX;

  
  if (duty_target > duty_curr) {
    duty_curr += duty_chg_per_interval;
    if (duty_curr > duty_target) duty_target = _DUTY_MAX;
  }
  else {
    duty_curr -= duty_chg_per_interval;
    if (duty_curr < duty_target) duty_target = _DUTY_MIN;
  }

  // update servo position
  myservo.writeMicroseconds(duty_curr);

  Serial.print("min:0,max:500,dist:");
  Serial.print(raw_dist);
  Serial.print(",dist_ema:");
  Serial.println(dist_ema);
  Serial.print(",dist_curr:");
  Serial.println(duty_curr);
  Serial.print(",duty_target:");
  Serial.println(duty_target);

  // toggle duty_target between _DUTY_MIN and _DUTY_MAX.
  /*if (toggle_interval_cnt >= toggle_interval) {
    toggle_interval_cnt = 0;
    if (duty_target == _DUTY_MIN) duty_target = _DUTY_MAX;
    else duty_target = _DUTY_MIN;
  }
  else {
    toggle_interval_cnt++;
  }*/

}

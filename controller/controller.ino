#include <TimerOne.h>

static const long TIMER_PERIOD = 100000;  // microseconds (10 Hz)

static const byte PIN_LEFT_X  = 3;
static const byte PIN_LEFT_Y  = 2;
static const byte PIN_RIGHT_X = 1;
static const byte PIN_RIGHT_Y = 0;

static const char ACTION_STOP          = 's';
static const char ACTION_MOVE_FORWARD  = 'f';
static const char ACTION_MOVE_BACKWARD = 'b';
static const char ACTION_TURN_LEFT     = 'l';
static const char ACTION_TURN_RIGHT    = 'r';

volatile boolean sampleSensors = false;

void setup(void) {
  Serial1.begin(38400);

  Timer1.initialize(TIMER_PERIOD);
  Timer1.attachInterrupt(timerHandler);
}

void timerHandler(void) {
  sampleSensors = true;
}

void loop(void) {
  if (sampleSensors) {
    sampleSensors = false;

    int leftX  = map(analogRead(PIN_LEFT_X),    0,  1017, 0, 10);
    int leftY  = map(analogRead(PIN_LEFT_Y),    0,   950, 0, 10);
    int rightX = map(analogRead(PIN_RIGHT_X),  895,  140, 0, 10);
    int rightY = map(analogRead(PIN_RIGHT_Y), 1023,   50, 0, 10);
/*
    Serial1.print(leftX);   Serial.print(", ");  Serial.print(leftY);
    Serial1.print(", ");
    Serial1.print(rightX);  Serial.print(", ");  Serial.println(rightY);
 */
    char action = ACTION_STOP;
    char value  = '0';

    if (leftY > 5  &&  rightY > 5) {
      action = ACTION_MOVE_FORWARD;
      value = leftY - 5 + '0';
    }

    if (leftY < 5  &&  rightY < 5) {
      action = ACTION_MOVE_BACKWARD;
      value = 5 - leftY + '0';
    }

    if (leftY > 5  &&  rightY < 5) {
      action = ACTION_TURN_RIGHT;
      value = (leftY - rightY) / 2 + '0';
    }

    if (leftY < 5  &&  rightY > 5) {
      action = ACTION_TURN_LEFT;
      value = (rightY - leftY) / 2 + '0';
    }

    Serial1.print(action);
    Serial1.println(value);
  }
}

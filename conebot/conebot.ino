#include <TimerOne.h>

static const long TIMER_PERIOD = 100000;  // microseconds (10 Hz)

const int channel_a_enable  = 6;
const int channel_a_input_1 = 4;
const int channel_a_input_2 = 7;

const int channel_b_enable  = 5;
const int channel_b_input_3 = 3;
const int channel_b_input_4 = 2;

volatile boolean sampleSensors = false;

void setup(void) {
  Serial.begin(38400);

  pinMode(channel_a_enable,  OUTPUT);
  pinMode(channel_a_input_1, OUTPUT);
  pinMode(channel_a_input_2, OUTPUT);
  
  pinMode(channel_b_enable,  OUTPUT);
  pinMode(channel_b_input_3, OUTPUT);
  pinMode(channel_b_input_4, OUTPUT);

  Timer1.initialize(TIMER_PERIOD);
  Timer1.attachInterrupt(timerHandler);
}

void timerHandler(void) {
  sampleSensors = true;
}

char buffer[2];
byte bufferIndex = 0;

void loop() {
  if (Serial.available()) {
    int ch = Serial.read();

    if (ch == '\r') {
      char action = buffer[0];
      byte value  = buffer[1] - '0';
      bufferIndex = 0;
/*
      Serial.print("a: ");    Serial.print(action);
      Serial.print(", v: ");  Serial.println(value);
 */
      byte dutyCycle = map(value, 1, 5, 100, 255);

      switch (action) {
        case 'e':
          break;

        case 'f':
          analogWrite (channel_a_enable,  dutyCycle);
          digitalWrite(channel_a_input_1, HIGH);
          digitalWrite(channel_a_input_2, LOW);
          analogWrite (channel_b_enable,  dutyCycle);
          digitalWrite(channel_b_input_3, LOW);
          digitalWrite(channel_b_input_4, HIGH);
          break;

        case 'b':
          analogWrite (channel_a_enable,  dutyCycle);
          digitalWrite(channel_a_input_1, LOW);
          digitalWrite(channel_a_input_2, HIGH);
          analogWrite (channel_b_enable,  dutyCycle);
          digitalWrite(channel_b_input_3, HIGH);
          digitalWrite(channel_b_input_4, LOW);
          break;

        case 'l':
          analogWrite (channel_a_enable,  dutyCycle);
          digitalWrite(channel_a_input_1, HIGH);
          digitalWrite(channel_a_input_2, LOW);
          analogWrite (channel_b_enable,  dutyCycle);
          digitalWrite(channel_b_input_3, HIGH);
          digitalWrite(channel_b_input_4, LOW);
          break;

        case 'r':
          analogWrite (channel_a_enable,  dutyCycle);
          digitalWrite(channel_a_input_1, LOW);
          digitalWrite(channel_a_input_2, HIGH);
          analogWrite (channel_b_enable,  dutyCycle);
          digitalWrite(channel_b_input_3, LOW);
          digitalWrite(channel_b_input_4, HIGH);
          break;

        case 's':
          motorsOff();
          break;
      }
    }
    else {
      if (bufferIndex < 2) {
        if (ch != '\n') buffer[bufferIndex ++] = ch;
      }
      else {
        buffer[0] = 'e';
        buffer[1] = '0';
      }
    }
  }
}

void motorsOff() {
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  digitalWrite(5, LOW);
}

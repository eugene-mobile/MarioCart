#include "ESC.h"

#define ESC_INIT_MIN_VALUE (1000)     // Set the Minimum ESC PWM in microseconds
#define ESC_INIT_MAX_VALUE (2000) 
#define SPEED_ZERO (1450)             // Set the speed where motor is stopped
#define SPEED_MAX_FORWARD (1800)                                  // Set the Maximum ESC PWM in microseconds
#define PEDAL_PIN (2)
#define FORWARD_PIN (4)
#define BACKWARDS_PIN (3)
#define STOPPED (0)
#define ACCELERATING (1)
#define DECELERATING (2)

ESC myESC (10, ESC_INIT_MIN_VALUE, ESC_INIT_MAX_VALUE, 500);          // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
int currentESCSpeed = SPEED_ZERO;

void calibrate();

void setup() {
  pinMode(PEDAL_PIN, INPUT_PULLUP);
  pinMode(FORWARD_PIN, INPUT_PULLUP);
  pinMode(BACKWARDS_PIN, INPUT_PULLUP);

  myESC.arm(); 

  delay(500);                                         // Wait for a while

  calibrate();
  myESC.stop();
}

void calibrate() {
  for (int oESC = ESC_INIT_MIN_VALUE; oESC <= SPEED_ZERO; oESC += 1) {
    myESC.speed(oESC);
    delay(10);
  }
  delay(100);
}

void loop() {
  bool directionForward = !digitalRead(FORWARD_PIN);
  bool directionBacwards = !digitalRead(BACKWARDS_PIN);
  bool pedalEngaged = !digitalRead(PEDAL_PIN);

  if (!pedalEngaged) {
    myESC.stop();
    currentESCSpeed = SPEED_ZERO;
    delay(100);
    return;
  }

  if (!directionForward & !directionBacwards) {
    myESC.stop();
    currentESCSpeed = SPEED_ZERO;
    delay(100);
    return;
  }

  if (directionForward) {
    currentESCSpeed = currentESCSpeed + 30;
    currentESCSpeed = min(currentESCSpeed, SPEED_MAX);
  }

  if (directionBacwards) {
    currentESCSpeed = currentESCSpeed - 30;
    currentESCSpeed = max(currentESCSpeed, SPEED_MIN);
  }

  myESC.speed(currentESCSpeed);
  delay(500);
}
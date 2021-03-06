#include "ESC.h"

#define ESC_INIT_MIN_VALUE (1000)     // Set the Minimum ESC PWM in microseconds
#define ESC_INIT_MAX_VALUE (2000) 
#define SPEED_ZERO (1450)             // Set the PWM to stop a motor
#define SLOW_FORWARD_SPEED (1550)      // Set the PWM for a slow motor forward speed
#define DEFAULT_MAX_FORWARD_SPEED (1650)      // Set the PWM for a default motor forward speed
#define NO_LIMIT_FORWARD_SPEED (2000)      // Set the PWM for plaid mode motor forward speed
#define SPEED_MAX_BACKWARD (1300)     // Set the PWM for a maximum motor reverse speed
#define PEDAL_PIN (2)
#define FORWARD_PIN (4)
#define BACKWARDS_PIN (3)
#define MODE_1_PIN (8)
#define MODE_2_PIN (9)

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
  pinMode(MODE_1_PIN, INPUT_PULLUP);
  pinMode(MODE_2_PIN, INPUT_PULLUP);

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
  bool directionBackwards = !digitalRead(BACKWARDS_PIN);
  bool pedalEngaged = !digitalRead(PEDAL_PIN);
  bool modeOne = !digitalRead(MODE_1_PIN);
  bool modeTwo = !digitalRead(MODE_2_PIN);
  int selectedMaxSpeed = DEFAULT_MAX_FORWARD_SPEED;
  
  if (!pedalEngaged) {
    myESC.stop();
    currentESCSpeed = SPEED_ZERO;
    delay(100);
    return;
  }

  if (!directionForward & !directionBackwards) {
    myESC.stop();
    currentESCSpeed = SPEED_ZERO;
    delay(100);
    return;
  }


  if (modeOne) {
    selectedMaxSpeed = SLOW_FORWARD_SPEED;
  } else if (modeTwo) {
    selectedMaxSpeed = NO_LIMIT_FORWARD_SPEED;
  }

  if (directionForward) {
    currentESCSpeed = currentESCSpeed + 25;
    currentESCSpeed = min(currentESCSpeed, selectedMaxSpeed);
  }

  if (directionBackwards) {
    currentESCSpeed = currentESCSpeed - 10;
    currentESCSpeed = max(currentESCSpeed, SPEED_MAX_BACKWARD);
  }

  myESC.speed(currentESCSpeed);
  delay(500);
}

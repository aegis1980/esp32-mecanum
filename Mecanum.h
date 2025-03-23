#ifndef MECANUM_H
#define MECANUM_H

#include <FastAccelStepper.h>


struct Wheel {
  const char* name;
  uint8_t stepPin;
  uint8_t dirPin;
  FastAccelStepper* stepper;
};


class Mecanum {

private:
  static constexpr uint8_t NUM_WHEELS = 4;
  static constexpr uint16_t STEPS_PER_REV = 200;
  static constexpr uint8_t MICROSTEPPING = 1;
  static constexpr uint32_t EFFECTIVE_STEPS_PER_REV = STEPS_PER_REV * MICROSTEPPING;
  static constexpr uint32_t ACCELERATION = 300;  // Steps/sec^2


  void _setWheelRPM(Wheel& wheel, float rpm);

  FastAccelStepperEngine _engine;

  Wheel _wheels[NUM_WHEELS];

  float _wheelbase;
  float _trackWidth;
  float _wheelRadius;

public:
    // Constructor: Initialize 4 stepper motors
    Mecanum(int frontLeftPins[2], int frontRightPins[2], int rearLeftPins[2], int rearRightPins[2]);

    // Movement functions

    unsigned int combined(float throttle, float strafe, float omega);
    unsigned int  stop();




};

#endif

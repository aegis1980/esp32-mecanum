
#include "Mecanum.h"

#include <FastAccelStepper.h>


    // Constructor: Initialize 4 stepper motors
Mecanum::Mecanum(int frontLeftPins[2], int frontRightPins[2], int rearLeftPins[2], int rearRightPins[2]) : _wheelbase(0.2f), _trackWidth(0.2f), _wheelRadius(0.05f)
{

    _engine = FastAccelStepperEngine();
    _engine.init();
    
    _wheels[0] = {"FL", frontLeftPins[1], frontLeftPins[0], nullptr};
    _wheels[1] = {"FR", frontRightPins[1], frontRightPins[0], nullptr};
    _wheels[2] = {"RL", rearLeftPins[1], rearLeftPins[0], nullptr};
    _wheels[3] = {"RR", rearRightPins[1], rearRightPins[0],  nullptr};

    for (uint8_t i = 0; i < NUM_WHEELS; ++i) {
      _wheels[i].stepper = _engine.stepperConnectToPin(_wheels[i].stepPin);
      if (_wheels[i].stepper) {
        _wheels[i].stepper->setDirectionPin(_wheels[i].dirPin);
        _wheels[i].stepper->setAcceleration(ACCELERATION);
        _wheels[i].stepper->setEnablePin(26);
        _wheels[i].stepper->setAutoEnable(true);
      } else {
        Serial.printf("Failed to init stepper %s\n", _wheels[i].name);
      }
    }

}

void Mecanum::_setWheelRPM(Wheel& wheel, float rpm) {
  if (!wheel.stepper) return;

  if (rpm == 0.0f) {
    wheel.stepper->forceStop();
    return;
  }

  // Convert RPM → steps/sec
  float steps_per_sec = (abs(rpm) * EFFECTIVE_STEPS_PER_REV) / 60.0f;

  wheel.stepper->setSpeedInHz((uint32_t)steps_per_sec);

  if (rpm > 0){
    wheel.stepper->runForward();
  }else{
    wheel.stepper->runBackward();
  }
}

unsigned int Mecanum::combined(float throttle, float strafe, float omega){
  // Calculate wheel speeds in m/s
  float v1 = (strafe - throttle - (_wheelbase + _trackWidth) * omega) / _wheelRadius;
  float v2 = (strafe + throttle + (_wheelbase + _trackWidth) * omega) / _wheelRadius;
  float v3 = (-strafe + throttle - (_wheelbase + _trackWidth) * omega) / _wheelRadius;
  float v4 = (-strafe - throttle + (_wheelbase + _trackWidth) * omega) / _wheelRadius;

  // Convert wheel speeds (m/s) to RPM
  float rpm1 = (v1 * 60) / (2 * PI * _wheelRadius);
  float rpm2 = (v2 * 60) / (2 * PI * _wheelRadius);
  float rpm3 = (v3 * 60) / (2 * PI * _wheelRadius);
  float rpm4 = (v4 * 60) / (2 * PI * _wheelRadius);
 


  _setWheelRPM(_wheels[0], rpm1);
  _setWheelRPM(_wheels[1], rpm2);
  _setWheelRPM(_wheels[2], rpm3);
  _setWheelRPM(_wheels[3], rpm4);
  
    Serial.printf(
        "lf: %.3f, rf: %.3f lr: %.3f rr: %.3f\n",
        rpm1,    
        rpm2,    
        rpm3,
        rpm4     
    );

   return 1;
}


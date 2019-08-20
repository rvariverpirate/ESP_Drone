/*
    CotorController.h - Library used for 
    control of 4 DC motors over UART to ATMEga328
    1 Byte of resoution.
    Created by Joseph Cannella, 8/20/2019
*/
#ifndef MotorController_h
#define MotorController_h

#include "Arduino.h"

class MotorController
{
  public:
    MotorController();
    String intToString(int pwm);
    void sendPWM(int pwmA, int pwmB, int pwmC, int pwmD);
    void debugPWM();
  private:
    int debug_count;
    int pwm_array[4] = {100, 0, 0, 0};
};
#endif

/*
  PID.h - Library for Basic PID loops
  Created by Tripp Cannella, 8/20/2019
*/

#ifndef PID_h
#define PID_H
#include <float.h>

double PWM_res = 255;

typedef struct PID_vals
{

  // Define The Gains
  double kP = 1;
  double kI = 0;
  double kD = 0;

  // Define the Setpoint, Input, and Output
  double desired = 0;// Desired Value
  double measured = 0;// Measured Value
  double U = 0;// Control Variable (Output)

};

// Map Control Vars to Each Motor
int * mapControlVarsToMotors(double U_R, double U_P, double U_Y, double U_A){
  // Store PWM for all 4 Motors in Array
  static int PWM_array[4];

  double pwm_RF = U_A - U_R - U_P;// Right Front Motor
  double pwm_RB = U_A - U_R + U_P;// Right Back Motor
  double pwm_LB = U_A + U_R + U_P;// Left Back Motor
  double pwm_LF = U_A + U_R - U_P;// Left Front Motor

  int pwm_RF_int = (int) map(pwm_RF, DBL_MIN, DBL_MAX, 0, PWM_res);
  int pwm_RB_int = (int) map(pwm_RB, DBL_MIN, DBL_MAX, 0, PWM_res);
  int pwm_LB_int = (int) map(pwm_LB, DBL_MIN, DBL_MAX, 0, PWM_res);
  int pwm_LF_int = (int) map(pwm_LF, DBL_MIN, DBL_MAX, 0, PWM_res);

  PWM_array[0] = pwm_RF_int;
  PWM_array[1] = pwm_RB_int;
  PWM_array[2] = pwm_LB_int;
  PWM_array[3] = pwm_LF_int;

  return PWM_array;
} 
#endif

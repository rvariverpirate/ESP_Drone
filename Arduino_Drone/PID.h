typedef struct PID_vals
{
  // Define PID Variables
  double P = 0;
  double I = 0;
  double D = 0;

  // Define The Gains
  double k_P = 1;
  double k_I = 1;
  double k_D = 1;

  // Store the Previous Error for Derivative
  double previousError = 0;
  
};

// Define Inner Control PID (pitch(m,d), roll(m,d), yaw(m,d), altitude(m,d)) -> (U_p, U_r, U_y, U_a)
double PID_inner(double measured, double target, struct PID_vals * PID_state){
  // Calculate the Error
  double error = target - measured;

  // Calculate the Integral
  PID_state->I += error;

  // Calculate the Derivative
  PID_state->D = error - PID_state->previousError;

  // Calculate the Control Variable
  double U = (PID_state->k_P*error) + (PID_state->I*PID_state->k_I) + (PID_state->D*PID_state->k_D);
  
}; 
 
// Control Variables for RPY & Alt
double U_R = 0;
double U_P = 0;
double U_Y = 0;
double U_A = 0;

// Map Control Vars to Each Motor
double * mapControlVarsToMotors(){
  // Store PWM for all 4 Motors in Array
  static double PWM_array[4];

  double pwm_RF = U_A - U_R - U_P;// Right Front Motor
  double pwm_RB = U_A - U_R + U_P;// Right Back Motor
  double pwm_LB = U_A + U_R + U_P;// Left Back Motor
  double pwm_LF = U_A + U_R - U_P;// Left Front Motor

  PWM_array[0] = pwm_RF;
  PWM_array[1] = pwm_RB;
  PWM_array[2] = pwm_LB;
  PWM_array[3] = pwm_LF;

  return PWM_array;
};

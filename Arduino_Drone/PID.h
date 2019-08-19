typedef struct PID_vals
{
  // Define PID Variables
  int P = 0;
  int I = 0;
  int D = 0;

  // Define The Gains
  int k_P = 1;
  int k_I = 0;
  int k_D = 0;

  // Store the Previous Error for Derivative
  int previousError = 0;
  
};

// Define Inner Control PID (pitch(m,d), roll(m,d), yaw(m,d), altitude(m,d)) -> (U_p, U_r, U_y, U_a)
int PID_inner(int measured, int target, struct PID_vals * PID_state){
  // Calculate the Error
  int error = target - measured;

  // Calculate the Integral
  PID_state->I += error;

  // Calculate the Derivative
  PID_state->D = error - PID_state->previousError;

  // Calculate the Control Variable
  int U = (PID_state->k_P*error) + (PID_state->I*PID_state->k_I) + (PID_state->D*PID_state->k_D);
  
}; 
 
// Control Variables for RPY & Alt
int U_R = 0;
int U_P = 0;
int U_Y = 0;
int U_A = 0;

// Map Control Vars to Each Motor
int * mapControlVarsToMotors(){
  // Store PWM for all 4 Motors in Array
  static int PWM_array[4];

  int pwm_RF = U_A - U_R - U_P;// Right Front Motor
  int pwm_RB = U_A - U_R + U_P;// Right Back Motor
  int pwm_LB = U_A + U_R + U_P;// Left Back Motor
  int pwm_LF = U_A + U_R - U_P;// Left Front Motor

  PWM_array[0] = pwm_RF;
  PWM_array[1] = pwm_RB;
  PWM_array[2] = pwm_LB;
  PWM_array[3] = pwm_LF;

  return PWM_array;
};

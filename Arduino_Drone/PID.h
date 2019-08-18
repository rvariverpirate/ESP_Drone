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

// Create Instances of PID States
struct PID_vals roll_PID_vals;
struct PID_vals pitch_PID_vals;
struct PID_vals yaw_PID_vals;
struct PID_vals altitude_vals;

// Define Inner Control PID (e_pitch, e_roll, e_yaw, e_altitude) -> (U_p, U_r, U_y, U_a)
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

// Define Outer Control PID (X,Y) -> (Roll, Pitch) 


// Create Motor Command

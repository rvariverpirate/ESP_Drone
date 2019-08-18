typedef struct PID_gains
{
  int P = 0;// Proportional Gain
  int I = 0;// Integral Gain
  int D = 0;// Derivative Gain
};

// Define Various Gains for Control of Drone
struct PID_gains roll_gains;
struct PID_gains pitch_gains;
struct PID_gains yaw_gains;
struct PID_gains thrust_gains;
struct PID_gains altitude_gains;

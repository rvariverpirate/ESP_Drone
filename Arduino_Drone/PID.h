typedef struct PID_vals
{
  int P = 0;
  int I = 0;
  int D = 0;  
};

struct PID_vals roll_PID_vals;
struct PID_vals pitch_PID_vals;
struct PID_vals yaw_PID_vals;
struct PID_vals thrust_PID_vals;
struct PID_vals altitude_vals;

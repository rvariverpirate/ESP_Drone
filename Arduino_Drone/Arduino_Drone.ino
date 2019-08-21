
#define BAUD_RATE 4800
#include "MPU9250_Read.h"
#include "MotorController.h"
// Create the Motor Controller Object
MotorController MC;
#include "ESP8266Node.h"
#include <PID_v1.h>
#include "PID.h"


// Intialize motor PWM to 0 duty cycle (Range 0-255)
int * allPWM;

/* Motor Mapping

   LF[3]-RF[0]
     |     |
   LB[2]-RB[1]

 */

// Create Instances of PID States
struct PID_vals roll;
struct PID_vals pitch;
struct PID_vals yaw;
struct PID_vals altitude;

// Create actual PID Loops
PID roll_PID(&roll.measured, &roll.U, &roll.desired, roll.kP, roll.kI, roll.kD, DIRECT);
PID pitch_PID(&pitch.measured, &pitch.U, &pitch.desired, pitch.kP, pitch.kI, pitch.kD, DIRECT);
PID yaw_PID(&yaw.measured, &yaw.U, &yaw.desired, yaw.kP, yaw.kI, yaw.kD, DIRECT);
PID altitude_PID(&altitude.measured, &altitude.U, &altitude.desired, altitude.kP, altitude.kI, altitude.kD, DIRECT);

// Debug Modes
boolean debug_IMU = false;
boolean debug_ROS = false;
boolean debug_PWM = !debug_ROS;

void setup() {
  Serial.begin(BAUD_RATE);// Can probably bump this up when we add Crystal Oscilator to ATMega
  setupIMU();// Setup i2c connection to IMU
  setupConnection();// Setup network connection and ROS for ESP8266

  // Turn on Control Loops
  roll_PID.SetMode(AUTOMATIC);
  pitch_PID.SetMode(AUTOMATIC);
  yaw_PID.SetMode(AUTOMATIC);
  altitude_PID.SetMode(AUTOMATIC);


  rpy_command_global.header.frame_id = "/world";
  rpy_command_global.header.stamp = nh.now();// timestamp
  rpy_command_global.roll = 0;
  rpy_command_global.pitch = 0;
  rpy_command_global.yaw = 0;
  
  str_msg.data = "This is the default message";
}

void loop() {
  if (nh.connected()) {
    if(debug_ROS){
      Serial.println("Connected to ROS");
    }
    
    //const char* IMU_data = getIMU_vals().c_str();
    // Add IMU data to Pose message
    double * IMU_data = getIMU_vals();
    if(debug_IMU){
      Serial.println(String(IMU_data[0], DEC));
      Serial.println(String(IMU_data[1], DEC));
    }

    // Publish Debug Message
    chatter.publish( &str_msg );

    // Create Roll Pitch Yaw Message
    rpy_msg.header.frame_id = "/world";
    rpy_msg.header.stamp = nh.now();// timestamp
    rpy_msg.roll = IMU_data[0];// TODO: I have no idea if these are
    rpy_msg.pitch = IMU_data[1];// in the right locations
    rpy_msg.yaw = IMU_data[2];// Too lazy to check right now

    // Publish Roll, Pitch, Yaw message
    rpy.publish(&rpy_msg);
    
    // Receive User Command (this is handled by the RPY Command Callback from ROS subscriber)
    // Stored ing global message "rpy_command_global"


    // Update PID values Vars for Roll, Pitch, Yaw and Altitude
    roll.desired = rpy_command_global.roll;
    roll.measured = IMU_data[0];
    pitch.desired = rpy_command_global.pitch;
    pitch.measured = IMU_data[1];
    yaw.desired = rpy_command_global.yaw;
    yaw.measured = IMU_data[2];
    altitude.desired = 1;//rpy_command_global.roll;
    altitude.measured = 1;//IMU_data[0];

    // Update Control Vars by calculating the PID for RPY and Alt
    roll_PID.Compute();
    pitch_PID.Compute();
    yaw_PID.Compute();
    altitude_PID.Compute(); 
    
    // Determine Motor PWM Values
    allPWM = mapControlVarsToMotors(roll.U, pitch.U, yaw.U, altitude.U);

    // Send the Motor Command to ATMega328 over UART
    MC.sendPWM(allPWM[0], allPWM[1], allPWM[2], 100);
    //MC.debugPWM();
    
  } else {
    if(debug_ROS) Serial.println("Trying to connect to ROS");
    else MC.sendPWM(0, 100, 0, 100);
  }
  // Publish ROS
  nh.spinOnce();
  // Loop at about 50Hz (Max)
  delay(10);
  
}

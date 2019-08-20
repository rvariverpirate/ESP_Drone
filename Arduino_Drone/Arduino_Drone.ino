
#include "MPU9250_Read.h"
#include "MotorController.h"
#include "ESP8266Node.h"
#include "PID.h"


// Intialize motor PWM to 0 duty cycle (Range 0-255)
double * allPWM;

/* Motor Mapping

   LF[3]-RF[0]
     |     |
   LB[2]-RB[1]

 */

// Create Instances of PID States
struct PID_vals roll_PID;
struct PID_vals pitch_PID;
struct PID_vals yaw_PID;
struct PID_vals altitude_PID;


// Debug Modes
boolean debug_IMU = false;
boolean debug_ROS = false;
boolean debug_PWM = !debug_ROS;

void setup() {
  Serial.begin(4800);// Can probably bump this up when we add Crystal Oscilator to ATMega
  setupIMU();// Setup i2c connection to IMU
  setupConnection();// Setup network connection and ROS for ESP8266
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
    
    

    // Get Control Vars for Roll, Pitch, Yaw and Altitude
    U_R = PID_inner(IMU_data[0], rpy_command_global.roll, &roll_PID);
    U_P = PID_inner(IMU_data[1], rpy_command_global.pitch, &pitch_PID);
    U_Y = PID_inner(IMU_data[2], rpy_command_global.yaw, &yaw_PID);
    U_A = PID_inner(1, 1, &altitude_PID);    
    
    // Determine Motor PWM Values
    allPWM = mapControlVarsToMotors();

    // Send the Motor Command to ATMega328 over UART
    sendPWM(allPWM[0], allPWM[1], allPWM[2], 100);
    //debugPWM();
    
  } else {
    if(debug_ROS) Serial.println("Trying to connect to ROS");
    else sendPWM(0, 100, 0, 100);
  }
  // Publish ROS
  nh.spinOnce();
  // Loop at about 50Hz (Max)
  delay(10);
  
}

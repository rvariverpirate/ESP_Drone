
#include "MPU9250_Read.h"
#include "ESP8266Node.h"
#include "MotorController.h"
#include "PID_gains.h"
#include "PID.h"


// Intialize motor PWM to 0 duty cycle (Range 0-255)
int pwm_A = 0;
int pwm_B = 0;
int pwm_C = 0;
int pwm_D = 0;

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
    };
    
    chatter.publish( &str_msg );
    
    rpy_msg.header.frame_id = "/world";
    rpy_msg.header.stamp = nh.now();// timestamp
    rpy_msg.roll = IMU_data[0];// TODO: I have no idea if these are
    rpy_msg.pitch = IMU_data[1];// in the right locations
    rpy_msg.yaw = IMU_data[2];// Too lazy to check right now

    // Publish Roll, Pitch, Yaw message
    rpy.publish(&rpy_msg);

    // TODO: Receive User Command
    

    // TODO: Populate PID for Roll, Pitch, Yaw


    // Command Motors
    //sendPWM(0, 100, 0, 100);
    debugPWM();
    
  } else {
    if(debug_ROS) Serial.println("Trying to connect to ROS");
    else sendPWM(0, 100, 0, 100);
  }
  // Publish ROS
  nh.spinOnce();
  // Loop at about 100Hz (Max)
  delay(10);
  
}

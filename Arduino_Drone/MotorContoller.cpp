/*
    CotorController.h - Library used for 
    control of 4 DC motors over UART to ATMEga328
    1 Byte of resoution.
    Created by Joseph Cannella, 8/20/2019
*/

#include "MotorController.h"
#include "Arduino.h"
// Contructor for Motor Controller Class
MotorController::MotorController()
{
  debug_count = 0;
}


// Convert our int for PWM to a string for UART
String MotorController::intToString(int pwm)
{
  char hundereds = (char) (pwm/100 + 48);// ASCII
  char tens = (char) ((pwm%100)/10 + 48);// ASCII
  char ones = (char) ((pwm%10) + 48);// ASCII
  String PWM = "";
  PWM += hundereds;
  PWM += tens;
  PWM += ones;
  return PWM;
}

// Create the PWM Command for all 4 Motors
void MotorController::sendPWM(int pwmA, int pwmB, int pwmC, int pwmD)
{
  String command = "s";
  command += intToString(pwmA);
  command += intToString(pwmB);
  command += intToString(pwmC);
  command += intToString(pwmD);
  command += "e";
  Serial.print(command);
}

// Helps check that everything is working
void MotorController::debugPWM(){
    sendPWM(pwm_array[debug_count % 4], pwm_array[(debug_count + 1) % 4],
            pwm_array[(debug_count + 2) % 4], pwm_array[(debug_count + 3) % 4]);
    debug_count ++;
    debug_count %= 4;
}

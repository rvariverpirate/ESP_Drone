int debug_count = 0;
int pwm_array[] = {100, 0, 0, 0};

String intToString(int pwm){
    char hundereds = (char) (pwm/100 + 48);// ASCII
    char tens = (char) ((pwm%100)/10 + 48);// ASCII
    char ones = (char) ((pwm%10) + 48);// ASCII
    String PWM = "";
    PWM += hundereds;
    PWM += tens;
    PWM += ones;
    return PWM;
  }

void sendPWM(int pwmA, int pwmB, int pwmC, int pwmD){
  String command = "s";
  command += intToString(pwmA);
  command += intToString(pwmB);
  command += intToString(pwmC);
  command += intToString(pwmD);
  command += "e";
  //Serial.print(command);
  }

void debugPWM(){
    sendPWM(pwm_array[debug_count % 4], pwm_array[(debug_count + 1) % 4],
            pwm_array[(debug_count + 2) % 4], pwm_array[(debug_count + 3) % 4]);
    debug_count ++;
    debug_count %= 4;
  }

//GND - GND
//VCC - VCC
//SDA - D1
//SCL - D2

#include <Wire.h>
 
#define    MPU9250_ADDRESS            0x68
#define    WHO_AM_I_MPU9250           0x75
#define    INT_PIN_CFG                0x37


#define    MAG_ADDRESS                0x0C
#define    MAG_ST1                    0x02// data ready status bit 0
#define    MAG_CNTL                   0x0A// Mag config address
 
#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18
 
#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

#define    ACCEL_GYRO_OUT_BASE_ADDR  0x3B
 
 
//Funcion auxiliar lectura
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.endTransmission();
 
   Wire.requestFrom(Address, Nbytes);
   uint8_t index = 0;
   while (Wire.available())
      Data[index++] = Wire.read();
}
 
// Funcion auxiliar de escritura
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
   Wire.beginTransmission(Address);
   Wire.write(Register);
   Wire.write(Data);
   Wire.endTransmission();
}

void  whoAmI(){
  uint8_t Data[1];
  I2Cread(MPU9250_ADDRESS, WHO_AM_I_MPU9250, 1, Data);
  Serial.print("My address is: 0x");
  Serial.println(Data[0], HEX);
}
 
void setupIMU()
{
   Wire.begin(D1, D2);
   delay(2000);
   Serial.println("Attempting to setup MPU");
   whoAmI();   
   // Configurar acelerometro
   I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);
   // Configurar giroscopio
   I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);
   // Configurar magnetometro
   I2CwriteByte(MPU9250_ADDRESS, INT_PIN_CFG, MAG_ST1);
   I2CwriteByte(MAG_ADDRESS, MAG_CNTL, 0x16);//0x01);
}

// Convert Acceleration to Roll Pitch Yaw
double* getRollPitchAccel(int16_t x, int16_t y, int16_t z, double *rp_tmp){
   double X = float(x);
   double Y = float(y);
   double Z = float(z);
   double roll_a = atan2(Y, Z);
   //Serial.println(roll_a);
   double pitch_a = atan2((-X), sqrt(Y*Y +Z*Z));
   //Serial.println(pitch_a);
   rp_tmp[0] = roll_a;
   rp_tmp[1] = pitch_a;
   return rp_tmp;
}

double * getIMU_vals(){
   // ---  Lectura acelerometro y giroscopio --- 
   uint8_t Buf[14];
   I2Cread(MPU9250_ADDRESS, ACCEL_GYRO_OUT_BASE_ADDR, 14, Buf);
   // Convertir registros acelerometro
   int16_t ax = -(Buf[0] << 8 | Buf[1]);
   int16_t ay = -(Buf[2] << 8 | Buf[3]);
   int16_t az = Buf[4] << 8 | Buf[5];
 
   // Convertir registros giroscopio
   int16_t gx = -(Buf[8] << 8 | Buf[9]);
   int16_t gy = -(Buf[10] << 8 | Buf[11]);
   int16_t gz = Buf[12] << 8 | Buf[13];
 
   // ---  Lectura del magnetometro --- 
   uint8_t ST1;
   do
   {
      I2Cread(MAG_ADDRESS, MAG_ST1, 1, &ST1);
   } while (!(ST1 & 0x01));
   uint8_t Mag[7];
   I2Cread(MAG_ADDRESS, 0x03, 7, Mag);
   
   // Convertir registros magnetometro
   int16_t mx = -(Mag[3] << 8 | Mag[2]);
   int16_t my = -(Mag[1] << 8 | Mag[0]);
   int16_t mz = -(Mag[5] << 8 | Mag[4]);
   


   // Get the Roll and Pitch
   double rp_tmp[5];
   double * rollPitch_accel = getRollPitchAccel(ax, ay, az, rp_tmp);
   double roll = rollPitch_accel[0];
   double pitch = rollPitch_accel[1];

   // Get the Yaw from Magnetometer
   double Yh = (my * cos(roll)) - (mz * sin(roll));
   double Xh = (mx * cos(pitch))+(my * sin(roll)*sin(pitch)) + (mz * cos(roll) * sin(pitch));
   double yaw = atan2(Yh, Xh);

 
   // --- Create Container for Data ---
   double static IMU_data[10];

   IMU_data[0] = roll;//rollPitch_accel[0];
   IMU_data[1] = pitch;//rollPitch_accel[1];
   IMU_data[2] = yaw;
  
   return IMU_data;
}

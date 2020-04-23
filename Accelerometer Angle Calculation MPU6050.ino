/*
THIS CODE IS PROVIDED AS IT IS WITHOUT ANY WARRANTY. THE CODE IS FREE TO USE FOR DOMESTIC AND COMMERCIAL LEVEL AND CAN BE DISTRIBURED. THIS CODE IS PROVIDED
UNDER MIT LICENSE, SO MAKE SURE TO READ THAT TOO.

THIS CODE HELPS TO CALCULATE THE GYROSCOPE (MOUNTED ON MPU6050) ANGLE VERY PRECISELY. BUT THIS ALGORITM CAN ALSO BE APPLIED TO ANY OTHER DYROSCOPE AFTER CHANGING THE
REGISTER VALUES AS PER REQUIREMENT. ANY SUGGESTIONS AND CORRECTIONS ARE HIGHLY APPRECIATED AND ARE HIGHLY WELCOME.

*/


#include<Wire.h>
#define addr 0x68
float accelX, accelY, accelZ;                                                // Defining variables
float X_Accel, Y_Accel, Z_Accel;
float angleX,angleY , gForce, pitch, roll, average =0;

                                                                             // Declaring Functions
void setupMPU();
void readAccel();

void setup(){  // this will run only once
Serial.begin(9600);
Wire.begin();
Serial.println("Starting MPU...");
setupMPU();
}

void loop(){
  readAccel();                                                              // Reading Acceleromter Output Registers
  gForce = sqrt((X_Accel*X_Accel)+(Y_Accel*Y_Accel)+(Z_Accel*Z_Accel));
  angleX = asin((float)X_Accel/gForce)*180/PI;
  angleY = asin((float)Y_Accel/gForce)*180/PI;

  Serial.print("Roll : ");                                                    // printing the output values on Arduino Serial Monitor
  Serial.print(angleX);                                                     
  Serial.print("   Pitch: ");
  Serial.println(angleY);
}


void setupMPU(){                                                           // Setting the registers of the MPU6050
  Wire.beginTransmission(addr);                                            // Start I2C communication with the MPU6050
  Wire.write(0x6B);                                                        // clearing "PWR_MGMT_1" register
  Wire.write(0x00);                                                        // Exiting the sleep mode of the Accelerometer
  Wire.endTransmission(addr);                                              // I2C communcation stopped
                                    
  // Accelerometer (+/-8g)
  Wire.beginTransmission(addr);                                            // Start I2C communication with the MPU6050
  Wire.write(0x1C);                                                        // Setting "ACCEL_CONFIG" register
  Wire.write(0x10);                                                        // Select  +/- 8g
  Wire.endTransmission(addr);                                              // I2C communcation stopped
  //Configure DLPF
  Wire.beginTransmission(addr);                                            // Start I2C communication with the MPU6050
  Wire.write(0x1A);                                                        // Setting "CONFIG" register to enable LOW PASS Filter
  Wire.write(0x06);                 
  Wire.endTransmission(addr);                                              // I2C communcation stopped 

}

void readAccel(){                                                          // Reading the registers of the Accelerometer
  Wire.beginTransmission(addr);                                            // Start I2C communication with the Gyro
  Wire.write(0x3B);                                                        // Writting the address of first output register for gAccelerometer
  Wire.endTransmission(addr);                                              // I2C communcation stopped 
  Wire.requestFrom(addr, 6);                                               // Requesting for 6 Bytes of data from the Accelerometer
  while(Wire.available()<6);                                               // Waiting for 6 Bytes of data from Accelerometer
  X_Accel = Wire.read()<<8 | Wire.read();                                  // Storing HIGH and LOW Bytes of X-axis
  Y_Accel = Wire.read()<<8 | Wire.read();                                  // Storing HIGH and LOW Bytes of Y-axis
  Z_Accel = Wire.read()<<8 | Wire.read();                                  // Storing HIGH and LOW Bytes of Z-axis

  X_Accel/=4096;                                                           // Dividing each axis by 4096 in order to get angle from the raw data output
  Y_Accel/=4096;
  Z_Accel/=4096;
}

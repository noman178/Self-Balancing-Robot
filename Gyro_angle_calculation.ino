/*
THIS CODE IS PROVIDED AS IT IS WITHOUT ANY WARRANTY. THE CODE IS FREE TO USE FOR DOMESTIC AND COMMERCIAL LEVEL AND CAN BE DISTRIBURED. THIS CODE IS PROVIDED
UNDER MIT LICENSE, SO MAKE SURE TO READ THAT TOO.

THIS CODE HELPS TO CALCULATE THE GYROSCOPE (MOUNTED ON MPU6050) ANGLE VERY PRECISELY. BUT THIS ALGORITM CAN ALSO BE APPLIED TO ANY OTHER DYROSCOPE AFTER CHANGING THE
REGISTER VALUES AS PER REQUIREMENT. ANY SUGGESTIONS AND CORRECTIONS ARE HIGHLY APPRECIATED AND ARE HIGHLY WELCOME.

*/

#include<Wire.h>
#define addr 0x68
long timer=0;
long X_Gyro, Y_Gyro, Z_Gyro;
float sumX, sumY, sumZ =0;
float pitch, roll, yaw =0;

// Declaring Functions
void setupMPU();
void readGyro();

void setup(){  // this will run only once
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Initializing Gyro...");
  setupMPU(); 
  delay(100);
  Serial.print("Calculating Offset of Gyro");
  for(int cal = 0; cal<2000; cal++){              
    if(cal%250==0)Serial.print(".");
    readGyro();
    sumX+=X_Gyro;                           //Integrating the Gyro X-axis Values to calculate offset
    sumY+=Y_Gyro;                           //Integrating the Gyro Y-axis Values to calculate offset
    sumZ+=Z_Gyro;                           //Integrating the Gyro Z-axis Values to calculate offset
    delay(3);
  }
  sumX/=2000;                              // calcuating average of 2000 values
  sumY/=2000;
  sumZ/=2000;
  Serial.println("");
  timer = micros();                        // Storing the time which has been elapdsed since the program has started 
}

void loop(){
  readGyro();                              // Reading Gyroscope Output Registers
  
  X_Gyro-=sumX;                            // Subtractng offset from current Gyro output resgisters every loop
  Y_Gyro-=sumY;
  Z_Gyro-=sumZ;

  roll   += X_Gyro * 0.005;                // As the controller has refresh rate of 200 Hz, so 1/200 Hz = 0.005
  pitch  += Y_Gyro * 0.005;                // So multiply ouput by 0.005
  yaw    += Z_Gyro * 0.005;

  Serial.print("Roll: ");                  // printing the output values on Arduino Serial Monitor
  Serial.print(roll);
  Serial.print("   Pitch : ");
  Serial.print(pitch);                      
  Serial.print("   Yaw: ");
  Serial.println(yaw);
  while(micros() - timer < 5000);          // waiting till the 10ms (5,000 uS) time is passed because the controller has refresh rate of 200Hz
  timer = micros();                        // Storing the time for next iteration
  
}


void setupMPU(){                           // Setting the registers of the Gyroscope
  Wire.beginTransmission(addr);            // Start I2C communication with the Gyro
  Wire.write(0x6B);                        // clearing "PWR_MGMT_1" register
  Wire.write(0x00);                        // Exiting the sleep mode of the Gyro..
  Wire.endTransmission(addr);              // I2C communcation stopped
                                           
  Wire.beginTransmission(addr);            // Setting the registers of the Gyroscope
  Wire.write(0x1B);                        // Writting on "GYRO_CONFIG" register
  Wire.write(0x08);                        // Gyro (500dps full scale)
  Wire.endTransmission(addr);              // I2C communcation stopped
}

void readGyro(){                           // Reading the registers of the Gyroscope
  Wire.beginTransmission(addr);            // Start I2C communication with the Gyro
  Wire.write(0x43);                        // Writting the address of first output register for gyro
  Wire.endTransmission(addr);              // I2C communcation stopped
  Wire.requestFrom(addr, 6);               // Requesting for 6 Bytes of data from the Gyro..
  while(Wire.available()<6);               // Waiting for 6 Bytes of data from Gyro
  X_Gyro = Wire.read()<<8 | Wire.read();   // Storing HIGH and LOW Bytes of X-axis
  Y_Gyro = Wire.read()<<8 | Wire.read();   // Storing HIGH and LOW Bytes of Y-axis
  Z_Gyro = Wire.read()<<8 | Wire.read();   // Storing HIGH and LOW Bytes of Z-axis
  
  X_Gyro/=65.5;                            // Dividing each axis by 65.5 in order to get angle from the raw data output
  Y_Gyro/=65.5;
  Z_Gyro/=65.5;
}

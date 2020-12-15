#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>

#define Gyro_X  124
#define Gyro_Y  24
#define Gyro_Z  19
#define Accel_X -14
#define Accel_Y -879
#define Accel_Z 1189
MPU6050 mpu;
static uint8_t mpuIntStatus;
static bool dmpReady = false;  // set true if DMP init was successful
static uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)

float  Roll_Now = 0, Roll = 0, Roll_Offset = 0;
float  Pitch_Now = 0, Pitch = 0, Pitch_Offset = 0;
float  Yaw_Now = 0, Yaw = 0, Yaw_Offset = 0;
uint16_t fifoCount;
uint8_t fifoBuffer[64]; // FIFO storage buffer                 // orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

void setup() {
  Serial.begin(9600);
  Serial.println("Device No.3");
  Wire.begin();
  
  Gryo_Start();
}

void loop() {
GyroGet();

Serial.print(Roll);
Serial.print(",");
Serial.print(Pitch);
Serial.print(",");
Serial.println(Yaw);
}

void GyroGet() {
  mpuIntStatus = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
  }
  else if (mpuIntStatus & 0x02) {
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    Roll_Now = degrees(ypr[2]) + 180;
    Pitch_Now = degrees(ypr[1]) + 180;
    Yaw_Now = degrees(ypr[0]) + 180;
    Roll = Roll_Now + Roll_Offset;
    Pitch = Pitch_Now + Pitch_Offset;
    Yaw = Yaw_Now + Yaw_Offset;
    if (Roll < 0) Roll += 360;
    if (Roll > 359) Roll -= 360;
    if (Pitch < 0) Pitch += 360;
    if (Pitch > 359) Pitch -= 360;
    if (Yaw < 0) Yaw += 360;
    if (Yaw > 359) Yaw -= 360;
  }
}

void Gryo_Start() {
  mpu.initialize();
  if (mpu.testConnection() != true) {
    Serial.println("MPU disconection");
    while (true) {}
  }
  if (mpu.dmpInitialize() != 0) {
    Serial.println("MPU break");
    while (true) {}
  }
  mpu.setXGyroOffset(Gyro_X);
  mpu.setYGyroOffset(Gyro_Y);
  mpu.setZGyroOffset(Gyro_Z);
  mpu.setXAccelOffset(Accel_X);
  mpu.setYAccelOffset(Accel_Y);
  mpu.setZAccelOffset(Accel_Z);
  mpu.setDMPEnabled(true);
  mpuIntStatus = mpu.getIntStatus();
  dmpReady = true;
  packetSize = mpu.dmpGetFIFOPacketSize();
}

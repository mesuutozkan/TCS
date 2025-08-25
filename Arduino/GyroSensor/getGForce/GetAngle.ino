#include <SPI.h>
#include <mcp2515.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);

MPU6050 mpu6050(Wire);

void setup()
 {

  while (!Serial);
  Serial.begin(9600);

  SPI.begin();

  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() 
{
  mpu6050.update();
  int g = mpu6050.getAngleY();
  g = abs(g);

  canMsg.can_id  = 0x56;
  canMsg.can_dlc = 8;
  canMsg.data[0] = 0x00;
  canMsg.data[1] = 0x00;
  canMsg.data[2] = 0x00;
  canMsg.data[3] = g;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;

 mcp2515.sendMessage(&canMsg);
  
}

#include "SPI.h"
#include "mcp2515.h"
int MOTOR = 6;
 
struct can_frame canMsg;
MCP2515 mcp2515(10);

void setup()
 {
  pinMode(MOTOR, OUTPUT);

  SPI.begin();
  Serial.begin(9600);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  digitalWrite(MOTOR, HIGH);

}
void loop()
{
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {
    if (canMsg.can_id  == 0x55)
    {
    int z = canMsg.data[2];
    if(z == 2)
    {
      digitalWrite(MOTOR, LOW);
    }
    else
    {
      digitalWrite(MOTOR, HIGH);
    }
    }
  }
}
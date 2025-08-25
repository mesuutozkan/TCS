#include <SPI.h>
#include <mcp2515.h>
#include "TimerOne.h"

struct can_frame canMsg;
MCP2515 mcp2515(10);

const byte s1 = 2;
const byte s2 = 3;
unsigned int counter1 = 0;
unsigned int counter2 = 0;
float diskslots = 20.00;
int r1;
int r2;
int dif;
int rdif = 0;
void ISR_count1()
{
  counter1++;
}
void ISR_count2()
{
  counter2++;
}
void ISR_timerone()
{
  Timer1.detachInterrupt();
   Serial.print("Motor Speed 1: "); 
  float rotation1 = (counter1 / diskslots) * 120.00;
  Serial.print(rotation1);  
  Serial.print(" RPM - "); 
  r1 = int(rotation1);
  counter1 = 0;
  Serial.print("Motor Speed 2: "); 
  float rotation2 = (counter2 / diskslots) * 120.00;
   Serial.print(rotation2);  
  Serial.println(" RPM"); 
  r2 = int(rotation2);
  counter2 = 0;
  Timer1.attachInterrupt( ISR_timerone );
}
void setup()
{
  
  while (!Serial);
  Serial.begin(9600);

  SPI.begin();

  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();

  Timer1.initialize(250000);
  attachInterrupt(digitalPinToInterrupt (s1), ISR_count1, RISING);
  attachInterrupt(digitalPinToInterrupt (s2), ISR_count2, RISING);
  Timer1.attachInterrupt( ISR_timerone );
}
void loop()
{
  diff = r1-r2;
  diff = abs(diff);
  if(diff >=7)
  {
    rdiff = 2;
  }
  else
  {
   rdiff = 0;
  } 

  canMsg.can_id  = 0x55;
  canMsg.can_dlc = 8;
  canMsg.data[0] = r1;
  canMsg.data[1] = r2;
  canMsg.data[2] = rdif;
  canMsg.data[3] = 0x00;
  canMsg.data[4] = 0x00;
  canMsg.data[5] = 0x00;
  canMsg.data[6] = 0x00;
  canMsg.data[7] = 0x00;

 mcp2515.sendMessage(&canMsg);

}

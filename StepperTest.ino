/* 
Suspension Control Script
*/
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>

int data2=0;

int x = 0;

int stepCount1 = 0;
int stepCount2 = 0;
int stepCount3 = 0;
int stepCount4 = 0;


int StoredFL = 0;
int StoredFR = 0;
int StoredRL = 0;
int StoredRR = 0;




// motor shield I2C address
Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x60); 
Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x61);

// stepper motor steps
Adafruit_StepperMotor *myMotorFL = AFMS1.getStepper(200, 1);
Adafruit_StepperMotor *myMotorFR = AFMS1.getStepper(200, 2);
Adafruit_StepperMotor *myMotorRL = AFMS2.getStepper(200, 1);
Adafruit_StepperMotor *myMotorRR = AFMS2.getStepper(200, 2);

void setup() {
  Serial.begin(115200);           // set up Serial library at 115200 bps
  
 

  // motor setup
  AFMS1.begin(); 
  AFMS2.begin();
  
  
  
  //motor speed
  myMotorFL->setSpeed(0.1); 
  myMotorFR->setSpeed(0.1);
  myMotorRL->setSpeed(0.1);
  myMotorRR->setSpeed(0.1);
  
  //Zero motors
 
  myMotorFL->step(400, FORWARD, DOUBLE);
  myMotorFR->step(400, FORWARD, DOUBLE);
  myMotorRL->step(400, FORWARD, DOUBLE);
  myMotorRR->step(400, FORWARD, DOUBLE);

  //Set previous settings
  if (EEPROM.read(0)==255)
  {
    EEPROM.write(0, 0);
    EEPROM.write(1, 0);
    EEPROM.write(2, 0);
    EEPROM.write(3, 0);
  }
  myMotorFL->step(EEPROM.read(0)*4, BACKWARD, DOUBLE);
  myMotorFR->step(EEPROM.read(1)*4, BACKWARD, DOUBLE);
  myMotorRL->step(EEPROM.read(2)*4, BACKWARD, DOUBLE);
  myMotorRR->step(EEPROM.read(3)*4, BACKWARD, DOUBLE);
  stepCount1 = EEPROM.read(0);
  stepCount2 = EEPROM.read(1);
  stepCount3 = EEPROM.read(2);
  stepCount4 = EEPROM.read(3); 

  
}


void DefaultDisplay() 
{
  StoredFL = EEPROM.read(0);
    StoredFR = EEPROM.read(1);
    StoredRL = EEPROM.read(2);
    StoredRR = EEPROM.read(3);

    if (StoredFL!=stepCount1 || StoredFR!=stepCount2 || StoredRL!=stepCount3 || StoredRR!=stepCount4)
    {
      EEPROM.update(0, stepCount1);
      EEPROM.update(1, stepCount2);
      EEPROM.update(2, stepCount3);
      EEPROM.update(3, stepCount4);
    } 
    delay(200);

    
}


//Increase motor count to make suspension softer
void MoveMotorIncrease(int StepC)
{
    StepC++;
    DefaultDisplay();
    delay(300); 
 }
 

//Decrease motor count to make suspenseion harder
void MoveMotorDecrease(int StepC)
{
  
    StepC--;
    DefaultDisplay();
    delay(300); 
 }
  
void UpdateSerial()
{
  String data3 = String(stepCount1)+String(stepCount2)+String(stepCount3)+String(stepCount4);
Serial.println(data3);
/*
  for(int i=0; i<data3.length(); i++)
  {
    Serial.write(data3[i]);
  }
  */
}

void Preset(int Setting)
{
  if (stepCount1<Setting)
  {
    while(stepCount1<Setting)
    {
      stepCount1++;
      myMotorFL->step(4, BACKWARD, DOUBLE);
    }
  }
  if (stepCount1>Setting)
  {
    while(stepCount1>Setting)
    {
      stepCount1--;
      myMotorFL->step(4, FORWARD, DOUBLE);
    }
  }

  if (stepCount2<Setting)
  {
    while(stepCount2<Setting)
    {
      stepCount2++;
      myMotorFR->step(4, BACKWARD, DOUBLE);
    }
  }
  if (stepCount2>Setting)
  {
    while(stepCount2>Setting)
    {
      stepCount2--;
      myMotorFR->step(4, FORWARD, DOUBLE);
    }
  }

  if (stepCount3<Setting)
  {
    while(stepCount3<Setting)
    {
      stepCount3++;
      myMotorRL->step(4, BACKWARD, DOUBLE);
    }
  }
  if (stepCount3>Setting)
  {
    while(stepCount3>Setting)
    {
      stepCount3--;
      myMotorRL->step(4, FORWARD, DOUBLE);
    }
  }

  if (stepCount4<Setting)
  {
    while(stepCount4<Setting)
    {
      stepCount4++;
      myMotorRR->step(4, BACKWARD, DOUBLE);
    }
  }
  if (stepCount4>Setting)
  {
    while(stepCount4>Setting)
    {
      stepCount4--;
      myMotorRR->step(4, FORWARD, DOUBLE);
    }
  }
}

void loop() 
{
  
  if(Serial.available()>0)
  {
  int data =Serial.parseInt();
  
  delay(500);
    if(data>0)
    {
      
      //delay(500);
      data2 = data;
      //delay(500);
      //Serial.println(data2);
    }
  }

  if ( data2==13)
  {
    Preset(5);
  }

  if ( data2==14)
  {
    Preset(20);
  }

  if ( data2==15)
  {
    Preset(60);
  }
  //Front Left Motor
  if ( data2==11 || data2==51 || data2==71)
  {
    if(stepCount1<100)
    {
    stepCount1++;
    myMotorFL->step(4, BACKWARD, DOUBLE);
    UpdateSerial();
    }
  }
  else if ( data2==12 || data2==52 || data2==72)
  {
    if(stepCount1>0)
    {
    stepCount1--;
    myMotorFL->step(4, FORWARD, DOUBLE);
    UpdateSerial();
    }
  }

  //Front Right Motor
  if ( data2==21 || data2==51 || data2==71)
  {
    if(stepCount2<100)
    {
    stepCount2++;
    myMotorFR->step(4, FORWARD, DOUBLE);
    UpdateSerial();
    }
  }
  else if ( data2==22 || data2==52 || data2==72)
  {
    if(stepCount2>0)
    {
    stepCount2--;
    myMotorFR->step(4, BACKWARD, DOUBLE);
    UpdateSerial();
    }
  }

  //Rear Left Motor
  if ( data2==31 || data2==61 || data2==71)
  {
    if(stepCount3<100)
    {
    stepCount3++;
    myMotorRL->step(4, BACKWARD, DOUBLE);
    UpdateSerial();
    }
  }
  else if ( data2==32 || data2==62 || data2==72)
  {
    if(stepCount3>0)
    {
    stepCount3--;
    myMotorRL->step(4, FORWARD, DOUBLE);
    UpdateSerial();
    }
  }  

  //Rear Right Motor
  if ( data2==41 || data2==61 || data2==71)
  {
    if(stepCount4<100)
    {
    stepCount4++;
    myMotorRR->step(4, FORWARD, DOUBLE);
    UpdateSerial();
    }
  }
  else if ( data2==42 || data2==62 || data2==72)
  {
    if(stepCount4>0)
    {
    stepCount4--;
    myMotorRR->step(4, BACKWARD, DOUBLE);
    UpdateSerial();
    }
  } 
  
  
  //Serial.write(data3);
  //Serial.println(data3);
  data2=0;
}

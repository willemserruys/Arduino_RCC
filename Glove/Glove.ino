#include <SoftwareSerial.h>
#include <VirtualWire.h>
#include <Timer.h>


const int xpin = A1;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis

float zero_G = 512.0; //ADC is 0~1023  the zero g output equal to Vs/2
                      //ADXL335 power supply by Vs 3.3V
float scale = 102.3;  //ADXL335330 Sensitivity is 330mv/g
                       //330 * 1024/3.3/1000  
                     
float thresholdValue = 5.00;

float xPrevValue = 0.00;
float yPrevValue = 0.00;

byte valuesToSend = 0;

const int ledVooruit = 2;
const int ledAchteruit = 3;

const int ledLinks = 4;
const int ledRechts = 5;

Timer t;
                      
void setup()
{
 // initialize the serial communications:
  Serial.begin(9600);
  
  vw_set_ptt_inverted(true);  // Required by the RF module
  vw_setup(2000);            // bps connection speed
  vw_set_tx_pin(13);         // Arduino pin to connect the receiver data pin

  
  pinMode(ledVooruit,OUTPUT);
  pinMode(ledAchteruit,OUTPUT);
  pinMode(ledLinks,OUTPUT);
  pinMode(ledRechts,OUTPUT);

  int lifeBeat = t.every(500, SendLifeBeat);
}
void loop()
{
  t.update();
 int x = analogRead(xpin);  //read from xpin
 
 int y = analogRead(ypin);  //read from ypin

 float xScaled = ((float)x - 331.5)/65*9.8;
 float yScaled = ((float)y - 329.5)/68.5*9.8;

  if (DetectMax(xScaled,xPrevValue)==1)
  {
    valuesToSend = valuesToSend | 1;
    UpdatePosition(valuesToSend);
  }

  
  if (DetectRestingPosition(xScaled,xPrevValue)==1)
  {
    valuesToSend = valuesToSend & 252;
    UpdatePosition(valuesToSend);
  }

    if (DetectMin(xScaled,xPrevValue)==1)
  {
    valuesToSend = valuesToSend | 2;
    UpdatePosition(valuesToSend);
  }

    if (DetectMax(yScaled,yPrevValue)==1)
  {
    valuesToSend = valuesToSend | 4;
    UpdatePosition(valuesToSend);
  }

  
  if (DetectRestingPosition(yScaled,yPrevValue)==1)
  {
    valuesToSend = valuesToSend & 243;
    UpdatePosition(valuesToSend);
  }

    if (DetectMin(yScaled,yPrevValue)==1)
  {
    valuesToSend = valuesToSend | 8;
    UpdatePosition(valuesToSend);
  }


  xPrevValue = xScaled;
  yPrevValue = yScaled;
  delay(50);
}


int DetectMax(float value, float prevValue)
{
  if(value > thresholdValue && prevValue < thresholdValue)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int DetectRestingPosition(float value, float prevValue)
{
  if((value < thresholdValue && prevValue > thresholdValue)||(value > thresholdValue *-1 && prevValue < thresholdValue*-1))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int DetectMin(float value, float prevValue)
{
  if(value < thresholdValue*-1 && prevValue > thresholdValue*-1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void UpdatePosition(byte values)
{
  digitalWrite(ledVooruit, bitRead(values,0));
  digitalWrite(ledAchteruit, bitRead(values,1));
  digitalWrite(ledLinks, bitRead(values,2));
  digitalWrite(ledRechts, bitRead(values,3));

  byte valuesToSend[1]= {values};

   vw_send(valuesToSend, 1);
   vw_wait_tx();        // We wait to finish sending the message
}

void SendLifeBeat()
{
    byte valuesToSend[1]= {128};

   vw_send(valuesToSend, 1);
   vw_wait_tx();        // We wait to finish sending the message
  
}





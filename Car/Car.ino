#include <VirtualWire.h>
#include <Timer.h>


//Alive timer: once per 700 ms, the program checks for connection
Timer  t;
boolean isAlive = true;
int dyingTimer;


//Pins for motor shield
int motor11;
int motor12;

int motor21;
int motor22;


void setup()
{
    //Pins for motor shield
    pinMode(motor11,OUTPUT);
    pinMode(motor21,OUTPUT);
    pinMode(motor12,OUTPUT);
    pinMode(motor22,OUTPUT);

    //Setup RC connection
    Serial.begin(9600);          // Configure the serial connection to the computer
    vw_set_ptt_inverted(true);  // Required by the RF module
    vw_setup(2000);            // bps connection speed
    vw_set_rx_pin(3);         // Arduino pin to connect the receiver data pin
    vw_rx_start();           // Start the receiver
}
 
void loop()
{
  //Needed for timer
  t.update();

  
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  //Receive messages
  if (vw_get_message(buf, &buflen))      // We check if we have received data
  {
   
    int i;
    // Message with proper check    
    for (i = 0; i < buflen; i++)
    {
      uint8_t data = buf[i];

      if(data == 128)
      {
        t.stop(dyingTimer);
        isAlive = true;
        dyingTimer = t.after(700,SetDead);
        Serial.println("Connection is alive!");
      }  
      else if(bitRead(data,0) == 1)
      {
        DriveForward();
      }

      else if(bitRead(data,1) == 1)
      {
        DriveBackward();
      }
      else if(bitRead(data,2) == 1)
      {
        DriveLeft();
      }

      else if(bitRead(data,3) == 1)
      {
        DriveRight();
      }
      else
      {
        StandStill();
      }                
    }
 
   
  }
      //No connection: motor stops
     if(isAlive == false)
     {
        Serial.println("Connection is Dead");
        StandStill();
     }
       
}

void SetDead()
{
  isAlive = false;
}

void DriveForward()
{
  Serial.println("Forward!");
}

void DriveBackward()
{
  Serial.println("Backward");
}

void DriveLeft()
{
  Serial.println("Left");
}

void DriveRight()
{
  Serial.println("Right");
}

void StandStill()
{
  Serial.println("Stand Still");
}

void Drive(int motorValue11,int motorValue12,int motorValue21, int motorValue22)
{
  
}


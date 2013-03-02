#if defined(ARDUINO) && ARDUINO >= 100

  #include "Arduino.h"

  #include "SoftwareSerial.h"

  //SoftwareSerial port(12,13);

#else // Arduino 0022 - use modified NewSoftSerial

  #include "WProgram.h"

  #include "NewSoftSerial.h"

  NewSoftSerial port(12,13);

#endif


#include "Streaming.h"
#include "EasyVR.h"

EasyVR easyvr(Serial);

SoftwareSerial xbee(2,3); 

SoftwareSerial xtend(4,5); 





//Groups and Commands

enum Groups

{

  GROUP_0  = 0,

  GROUP_1  = 1,

};



enum Group0 

{

  G0_START = 0,

};



enum Group1 

{

  G1_COFFEE_ON = 0,

  G1_COFFEE_OFF = 1,

  G1_LIGHT_ON = 2,

  G1_LIGHT_OFF = 3,

};





EasyVRBridge bridge;



int8_t group, idx, wake; 



void setup()

{

  // bridge mode?

  //if (bridge.check())

 // {

   // cli();

   // bridge.loop(0, 1, 12, 13);

//  }

  // run normally
  
  // xtend.begin(9600); 
  
   xbee.begin(9600); 
  
  
  Serial.begin(9600);
  
//  pinMode(6,OUTPUT); 
  
 // digitalWrite(6,HIGH); 



  if (!easyvr.detect())

  {

   // Serial.println("EasyVR not detected!");

    for (;;);

  }



  easyvr.setPinOutput(EasyVR::IO1, LOW);

  //Serial.println("EasyVR detected!");

  easyvr.setTimeout(5);

  easyvr.setLanguage(0);



  group = EasyVR::TRIGGER; //<-- start group (customize)
  
  easyvr.playSound(1, EasyVR::VOL_FULL);
  
  

}



void action();



void loop()

{
  
 
  
  xtend.write("Hello"); 
  
  
  easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)


  //Serial.print("Say a command in Group ");
  
  

  //Serial.println(group);

  easyvr.recognizeCommand(group);



  do

  {
     
     
    
    

    

  }

  while (!easyvr.hasFinished());

  

  easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off



  idx = easyvr.getWord();

  if (idx >= 0)

  {

    // built-in trigger (ROBOT)

    // group = GROUP_X; <-- jump to another group X

    return;

  }

  idx = easyvr.getCommand();

  if (idx >= 0)

  {

    // print debug message

    uint8_t train = 0;

    char name[32];

   // Serial.print("Command: ");

  //  Serial.print(idx);

    if (easyvr.dumpCommand(group, idx, name, train)) 

    
        
      
      
        xbee << name ;
        

    

    else

     

    easyvr.playSound(0, EasyVR::VOL_FULL);

    // perform some action

    action();

  }

  else // errors or timeout

  {

    if (easyvr.isTimeout()) {
    
      xbee << "Timeout" ; }
       
      //easyvr.playSound(4, EasyVR::VOL_FULL); }

      //Serial.println("Timed out, try again...");
      
    else{  

    int16_t err = easyvr.getError();

    if (err >= 0)

    {
      easyvr.playSound(4, EasyVR::VOL_FULL);

      //Serial.print("Error ");

     // Serial.println(err, HEX);
      
      xbee << "Error:" << err ; 
     // delay(1000);

    }
  }
    }

}



void action()

{

    switch (group)

    {

    case GROUP_0:

      switch (idx)

      {

      case G0_START:
          
   //      xbee << "Start" << endl ; 
      //   delay(1000);
         easyvr.playSound(2, EasyVR::VOL_FULL);
         

         group = 1; 

        break;

      }

      break;

    case GROUP_1:

      switch (idx)

      {

      case G1_COFFEE_ON:
          
      
        //  xbee << "Coffee on" << endl ;
           
          easyvr.playSound(5, EasyVR::VOL_FULL);
          
          
        // group = GROUP_X; <-- or jump to another group X for composite commands

        break;

      case G1_COFFEE_OFF:
     //   xbee << "Coffee off" << endl ;
      //  delay(300);
        easyvr.playSound(5, EasyVR::VOL_FULL);
        
         

        // group = GROUP_X; <-- or jump to another group X for composite commands

        break;

      case G1_LIGHT_ON:
       // xbee << "Lights_On" << endl ;
        easyvr.playSound(5, EasyVR::VOL_FULL);
         
   
         

        // group = GROUP_X; <-- or jump to another group X for composite commands

        break;

      case G1_LIGHT_OFF:
        //xbee << "Lights_Off" << endl ;
        //delay(1000);
        easyvr.playSound(5, EasyVR::VOL_FULL);

        // group = GROUP_X; <-- or jump to another group X for composite commands

        break;
        
        
      //default: 
        //easyvr.playSound(4, EasyVR::VOL_FULL);
        //break;          

      }
    
   // default:
    //    easyvr.playSound(4, EasyVR::VOL_FULL);
    //    break;   

      break;

    }

}

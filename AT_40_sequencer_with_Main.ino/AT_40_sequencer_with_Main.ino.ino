/* 
  Universal Software Controlled PWM function library  for Arduino nano
  --------------------------------------------------------------------
  Sequencer for Tri-cycle undercarriage with gear doors

                      needs version 1.04 of UniPWM library or higher
*/

#include <UniPWMControl.h>
#include <UniPWMMacros.h>
#include <Servo.h>

// Door and Gear pins 
#define PIN_MAIN_DOOR  9   // MainDoor
#define PIN_NOSE_DOOR   11   // NoseDoor
#define PIN_GEAR   10  // Gear


// Nose Door values
#define NoseDoorUpMax  45
#define NoseDoorDownMax  26.99
#define NoseDoorSpeed  50
#define NoseDoorDelayUp  300
#define NoseDoorDelayDown  250


// Main Door Valus
#define MainDoorUpMax  30
#define MainDoorDownMax  45
#define MainDoorSpeed  50
#define MainDoorDelayUp  250
#define MainDoorDelayDown  300


#define RECEIVER_GEAR    3   // receiver gear switch channel

SEQUENCE( GearDown )    = { PAUSE(50), HOLD(50) };
SEQUENCE( GearUp )     = { PAUSE(50), HOLD(25) };

SEQUENCE( NoseDoorDown ) = { RAMP(NoseDoorDownMax,NoseDoorUpMax,NoseDoorSpeed), CONST(NoseDoorUpMax,NoseDoorDelayDown), RAMP(NoseDoorUpMax,NoseDoorDownMax,NoseDoorSpeed), PAUSE(0)};
SEQUENCE( NoseDoorUp ) = { RAMP(NoseDoorDownMax,NoseDoorUpMax,NoseDoorSpeed), CONST(NoseDoorUpMax,NoseDoorDelayUp), RAMP(NoseDoorUpMax,NoseDoorDownMax,NoseDoorSpeed), PAUSE(0)};

SEQUENCE( MainDoorDown ) = { RAMP(MainDoorDownMax,MainDoorUpMax,MainDoorSpeed), CONST(MainDoorUpMax,MainDoorDelayDown), RAMP(MainDoorUpMax,MainDoorDownMax,MainDoorSpeed), CONST(MainDoorDownMax, 1000000), PAUSE(0)};
SEQUENCE( MainDoorUp ) = { RAMP(MainDoorDownMax,MainDoorUpMax,MainDoorSpeed), CONST(MainDoorUpMax,MainDoorDelayUp), RAMP(MainDoorUpMax,MainDoorDownMax,MainDoorSpeed), HOLD(MainDoorDownMax)};


// SEQUENCE( GearStart )    = { HOLD(25) };
// SEQUENCE( NoseDoorStart )     = { HOLD(NoseDoorDownMax) };
// SEQUENCE( MainDoorStart )     = { HOLD(MainDoorDownMax) };



// PWM Control Objekt
UniPWMControl ctrl;

enum { GEAR_UP, GEAR_DOWN, STARTUP };

void setup()
{

  ctrl.Init( 3, 1 ); // max 3 output channels, 2 input channels
  ctrl.SetInpChannelPin( RECEIVER_GEAR, UniPWMInpChannel::INP_NORMAL );   // first input pin is default when DoLoop is called w/o parameters

  // Gear up
  ctrl.AddSequence( GEAR_UP, PIN_NOSE_DOOR, ARRAY( NoseDoorUp ), UniPWMChannel::SOFTPWM_SERVO );
  ctrl.AddSequence( GEAR_UP, PIN_MAIN_DOOR, ARRAY( MainDoorUp ), UniPWMChannel::SOFTPWM_SERVO );
  ctrl.AddSequence( GEAR_UP, PIN_GEAR, ARRAY( GearUp ), UniPWMChannel::SOFTPWM_SERVO );


  
  // Gear Down
  ctrl.AddSequence( GEAR_DOWN, PIN_NOSE_DOOR, ARRAY( NoseDoorDown ), UniPWMChannel::SOFTPWM_SERVO );
  ctrl.AddSequence( GEAR_DOWN, PIN_MAIN_DOOR, ARRAY( MainDoorDown ), UniPWMChannel::SOFTPWM_SERVO );
  ctrl.AddSequence( GEAR_DOWN, PIN_GEAR, ARRAY( GearDown ), UniPWMChannel::SOFTPWM_SERVO );

     //Startup
//   ctrl.AddSequence( STARTUP, PIN_NOSE_DOOR, ARRAY( NoseDoorStart ), UniPWMChannel::SOFTPWM_SERVO );
//   ctrl.AddSequence( STARTUP, PIN_MAIN_DOOR, ARRAY( MainDoorStart ), UniPWMChannel::SOFTPWM_SERVO );
//   ctrl.AddSequence( STARTUP, PIN_GEAR, ARRAY( GearStart ), UniPWMChannel::SOFTPWM_SERVO );
  
  ctrl.AddInputSwitchPos( 0, 39, GEAR_UP );
  ctrl.AddInputSwitchPos( 40, 99, GEAR_DOWN );

//  ctrl.ActivatePattern( STARTUP ); // activate normal flight mode
}

void loop()
{
ctrl.DoLoop();
  } 


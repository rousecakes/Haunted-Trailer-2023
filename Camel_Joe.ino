/* Include Libraries */  
#include <AccelStepper.h>

/* AccelStepper Setup */
AccelStepper stepperHead(1, 12, 13);     // 1 = Driver Interface DM542T
                                         // Arduino Pin 12 Connected to STEP Pin of Driver
                                         // Arduino Pin 13 Connected to DIR Pin of Driver

/* DEFINE Consstraints/Pins */
#define home_switch 4               // Pin 4 Connected to Home Switch (Micro Switch)
#define spit_switch 3               // Pin 3 Connected to Spit Button (Micro Switch) (Interupt Pin) 
#define spit_Motor 5                // Pin 5 Connected to Spit Motor      

/* Stepper Travel Variables and Initializers */
long initial_homing = -1;                                      // Used to Home Stepper at Startup
const float STEPS_PER_REV = 800;                               // 1.8  degree step rotation - Motor steps per rotation

/* Set Variables and Arrays */
unsigned long currentTime = millis();                                     // Current Time
const unsigned long Positions_headMove[5] = { 285, 375, 415, 505, 620 };  // Positions to go to for Head Movement
const unsigned long Duration_headMove[3] = { 1000, 2500, 4000 };          // Duration Array for Head Movement (Seconds) 
const unsigned long Interval_spit[3] = { 3000, 6000, 9000 };              // Interval Array of time to set random numbers for camel Spit ** Lowest Number Must be Larger then Largest number in "Duration_spit" Array 
const unsigned long Duration_spit[3] = { 500, 1000, 1750 };               // Duration Array for Spit Time (Seconds)

volatile bool spitFlag = true;                                            // Flag for Spit routine
volatile bool spitState = false;                                          // Variable to turn off and on Spit Motor

/* Function ProtoTypes */
void Homing();
void headMove();
void spit();
void ISR_Spit();


void setup() {
  Serial.begin(9600);           // Start Serial Monitor with speed of 9600 Bauds
  randomSeed(3);                // Start Random Seed for use of Random Function Later

  /* Set Pins */
  pinMode(home_switch, INPUT_PULLUP);                                        // Set Homing Switch as Input - Internal Pull-up resistor on Pin
  pinMode(spit_switch, INPUT_PULLUP);                                        // Set Spit Switch as Input - Internal Pull-up resistor on Pin
  pinMode(spit_Motor, OUTPUT);                                               // Set Motor Pin to Output
  attachInterrupt(digitalPinToInterrupt(spit_switch), ISR_Spit, FALLING);    // Attach Interrupt to Spit Button

  delay(5);                              // Wait for Driver to startup

  Homing();                              // Start Homing procedure of Stepper Motor at startup
}

void loop() {
  currentTime = millis();                // Update current Time
  headMove();                            // Run Head Movements
  if (spitFlag == 1) {                   // Check to see if Camel should be Spitting
  spit();                                // ... If so, Run Spit Routine
  } else {                               // ... Otherwise ...
    digitalWrite(spit_Motor, LOW);       // ... Don't Spit (Turn off Spit Motor)
  }
  Serial.println(spitFlag);
} 

void headMove() {
  static unsigned long lastTime_HeadMove;                         // Variable to hold Last time action happened 
  static uint8_t randomNumber_headPosition;                       // Variable to hold random number for Head Position
  static uint8_t randomNumber_headDuration;                       // Variable to hold random number for Head Duration                

  if ((currentTime - lastTime_HeadMove) >= Duration_headMove[randomNumber_headDuration]) {   // Check to See if the Duration of Time the Head should stay in one place has passed
    stepperHead.moveTo(Positions_headMove[randomNumber_headPosition]);                       // Set Stepper Position to New Position
    if (spitState == true){                                                                  // If currently Spitting...
      return;                                                                                // ...Skip Movements and return to Main Loop Program...
    } else{                                                                                  // ...Otherwise...
        if (stepperHead.distanceToGo() != 0) {                                               // If Head is not at new position...
          stepperHead.run();                                                                 // ... advance the stepper a step.
        } else {                                                                              // If Stepper has reach Target Destination and...
          randomNumber_headDuration = random(3);                                             // Set New Random Number Head Duration
          randomNumber_headPosition = random(5);                                             // Set New Random Number Head Position
          lastTime_HeadMove = currentTime;                                                   // Reset Interval Clock
        }
    } 
  }  
}

void spit() {
  static unsigned long lastTime_spit;                             // Variable to hold Last time action happened 
  static unsigned long lastTime_spitDuration;                     // Variable to hold Last time action happened 
  static unsigned long randomNumber_spitInterval;                 // Variable to hold random number for Spit Interval
  static unsigned long randomNumber_spitDuration;                 // Variable to hold random number for Spit Duration 

  if ((currentTime - lastTime_spit) >= Interval_spit[randomNumber_spitInterval]) {           // Check to See if the Duration of Time has Passed in order to Spit Again
    if ((spitState == false) && (stepperHead.distanceToGo() == 0)) {                         // If Spit motor off (not spitting)... And Stepper is at Destination...
      digitalWrite(spit_Motor, HIGH);                                                        // ...Turn Spit motor on (start spitting)
      spitState = true;                                                                      // Turn on Spit Flag
      randomNumber_spitDuration = random(3);                                                 // Pick New Random Duration Camel should spit for
      lastTime_spitDuration = currentTime;                                                   // Reset Spit Duration Timer to start tracking Spit Duration
    }
  }
  if ((currentTime - lastTime_spitDuration) >= Duration_spit[randomNumber_spitDuration]) {   // Check to See if the Duration of Time has Passed for Length of Spit
    if (spitState == true) {                                                                 // If Spit Flag is on (still Spitting)...
      digitalWrite(spit_Motor, LOW);                                                         // ...Turn off Spit Motor
      spitState = false;                                                                     // Turn off Spit Flag
      randomNumber_spitInterval = random(3);                                                 // Pick New Random Interval to Wait for Next Spit Cycle
      lastTime_spit = currentTime;                                                           // Reset Spit Interval Timer to start tracking When to Spit Again
    }
  }
}

void ISR_Spit() {
  static unsigned long lastTime_Interrupt = 0;              // Variable to hold Last time action happened 
  unsigned long interruptTime = millis();                   // Update Current interrupt Time

  if ((interruptTime - lastTime_Interrupt) > 300) {         // If Debounce Time has passed....
    spitFlag = !spitFlag;                                   // ...Toggle the Spit Flag - Turn on or Turn off the Spit Function...
    spitState = false;                                      // ...Reset the Spit State to restart the cycle      
  }
  lastTime_Interrupt = interruptTime;                       // Update last Interrupt Time.
}

void Homing() {

  /* Set Max Speed and Acceleration of each Stepper at Startup for homing */
  stepperHead.setMaxSpeed(200.0);        // Set Max Speed of Stepper (Slower to get accuracy)
  stepperHead.setAcceleration(200.0);    // Set Acceleration of the Stepper

    Serial.print("Stepper is Homing........");

  while (digitalRead(home_switch)) {     // Make the Stepper move CCW until the switch is activated
    stepperHead.moveTo(initial_homing);  // Set the position to move to
    initial_homing--;                    // Decrease by 1 for the next move if needed
    stepperHead.run();                   // Start Moving the stepper - Move Stepper into Position by a step
    delay(5);
  }

  stepperHead.setCurrentPosition(0);     // Set the current position as zero for now
  stepperHead.setMaxSpeed(200.0);        // Set Max Speed of Stepper (Slower to get accuracy)
  stepperHead.setAcceleration(200.0);    // Set Acceleration of the Stepper
  initial_homing = 1;                    // Reverse Homing Direction

  while (!digitalRead(home_switch)) {    // Make the Stepper move CW until the switch is deactivated
    stepperHead.moveTo(initial_homing);  // Set the position to move to
    stepperHead.run();                   // Start Moving the stepper - Move Stepper into Position by a step
    initial_homing++;                    // Increase by 1 for the next move if needed
    delay(5);
  }

  stepperHead.setCurrentPosition(0);     // Once off Homing Switch, Set this Position as "0" or "Home"
    Serial.println("Homing Completed");
    Serial.println("");
  stepperHead.setMaxSpeed(1000.0);       // Set Max Speed of Stepper
  stepperHead.setAcceleration(1000.0);   // Set Acceleration of Stepper
}

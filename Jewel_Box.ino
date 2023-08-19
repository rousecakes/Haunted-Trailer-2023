/* Include Libraries */  
#include <Servo.h>

/* Set Pins */  
#define sensor_1_Pin 3         // Jewel Tube 1 Sensor
#define sensor_2_Pin 4         // Jewel Tube 2 Sensor
#define sensor_3_Pin 11        // Jewel Drop Sensor 1
#define sensor_4_Pin 12        // Jewel Drop Sensor 2
#define cylinder_1_Pin 5       // Jewel Drop Piston
#define cylinder_2_Pin 6       // Spider Run Piston
#define air_1_Pin 7            // Jewel Tube 1 Blow Off Air
#define air_2_Pin 8            // Jewel Tube 2 Blow Off Air
#define spider_1_Pin 9         // Spider Jump 1 Servo
#define spider_2_Pin 10        // Spider Jump 2 Servo

/* Set Servos */
Servo spiderServo_1;           // Name Servo 1
Servo spiderServo_2;           // Name Servo 2
uint16_t minAngle_1 = 0;       // Set Minimum Angle for Servo 1
uint16_t maxAngle_1 = 180;     // Set Max Angle for Servo 1
uint16_t minAngle_2 = 0;       // Set Minimum Angle for Servo 2
uint16_t maxAngle_2 = 90;      // Set Max Angle for Servo 2

const unsigned long Spider_Angle_1[4] = { 115, 90, 45, 15 };          // Possible Angles Spider can Jump at *** Dont Go higher than 120 Deg
const unsigned long Spider_Angle_2[4] = { 90, 75, 45, 25 };           // Possible Angles Spider can Jump at *** Dont Go higher than 90 Deg

/* Time Variables */
unsigned long currentTime = millis();                                 // Current Time
const unsigned long Interval_jewelDrop = 6500;                        // Duration of time to set random numbers for Jewel Drop Tube (6.5 Seconds)
const unsigned long Interval_spiderRun = 9000;                        // Duration of time to set random numbers for Spider Run Tube (9 Seconds)
const unsigned long Interval_spiderJump_1 = 7500;                     // Duration of time to set random numbers for Spider Jump 1 (7.5 Seconds)
const unsigned long Interval_spiderJump_2 = 9500;                     // Duration of time to set random numbers for Spider Jump 2 (9.5 Seconds)
const unsigned long Duration_jewelDrop[3] = { 3000, 2000, 1000 };     // Duration Array for Jewel Drop Tube (Seconds) ** Highest number must be less than "Interval_jewelDrop" Setting
const unsigned long Duration_spiderRun[3] = { 4500, 3000, 1000 };     // Duration Array for Spider Run Tube (Seconds) ** Highest number must be less than "Interval_spiderRun" Setting
const unsigned long Duration_spiderJump_1[3] = { 2500, 1500, 500};    // Duration Array for Spider Jump (Seconds) ** Highest number must be less than "Interval_spiderJump_1" Setting
const unsigned long Duration_spiderJump_2[3] = { 2500, 1500, 500};    // Duration Array for Spider Jump (Seconds) ** Highest number must be less than "Interval_spiderJump_2" Setting

uint8_t counter_jewelDrop_1 = 0;                // Counter 1 for Jewel Drop
uint8_t counter_jewelDrop_2 = 0;                // Counter 2 for Jewel Drop
uint8_t counter_jewelDrop_3 = 0;                // Counter 3 for Jewel Drop
uint8_t counter_spiderRun_1 = 0;                // Counter 1 for Spider Run                
uint8_t counter_spiderRun_2 = 0;                // Counter 2 for Spider Run 
uint8_t counter_spiderRun_3 = 0;                // Counter 3 for Spider Run 
uint8_t counter_spiderJump_1_1 = 0;             // Counter 1 for Spider Jump 1                
uint8_t counter_spiderJump_1_2 = 0;             // Counter 2 for Spider Jump 1                
uint8_t counter_spiderJump_1_3 = 0;             // Counter 3 for Spider Jump 1 
uint8_t counter_spiderJump_2_1 = 0;             // Counter 1 for Spider Jump 2                
uint8_t counter_spiderJump_2_2 = 0;             // Counter 2 for Spider Jump 2                
uint8_t counter_spiderJump_2_3 = 0;             // Counter 3 for Spider Jump 2 

/* Function Prototypes */
void tubeAir_1();
void tubeAir_2();
void jewelDrop();
void spiderRun();
void spiderJump_1();
void spiderJump_2();

void setup() {
/* Serial Setup */
  Serial.begin(9600);         // Setup Serial Monitor
  randomSeed(3);              // Start Random Seed for use of Random Function Later

/* Attach Servos */
  uint16_t spider_1_minAngle = map(minAngle_1, 0, 180, 544, 2400);                 // Set Variable for Minimum Angle on Spider 1
  uint16_t spider_1_maxAngle = map(maxAngle_1, 0, 180, 544, 2400);                 // Set Variable for Maximum Angle on Spider 1
  uint16_t spider_2_minAngle = map(minAngle_2, 0, 180, 544, 2400);                 // Set Variable for Minimum Angle on Spider 2
  uint16_t spider_2_maxAngle = map(maxAngle_2, 0, 180, 544, 2400);                 // Set Variable for Maximum Angle on Spider 2

  spiderServo_1.attach(spider_1_Pin, spider_1_minAngle, spider_1_maxAngle);        // Attach Servo to Pin and set Min/Max Angles
  spiderServo_2.attach(spider_2_Pin, spider_2_minAngle, spider_2_maxAngle);

/* Set Pin Modes */
  pinMode(sensor_1_Pin, INPUT_PULLUP);
  pinMode(sensor_2_Pin, INPUT_PULLUP);
  pinMode(sensor_3_Pin, INPUT_PULLUP);
  pinMode(sensor_4_Pin, INPUT_PULLUP);  
  pinMode(cylinder_1_Pin, OUTPUT);
  pinMode(cylinder_2_Pin, OUTPUT);
  pinMode(air_1_Pin, OUTPUT);
  pinMode(air_2_Pin, OUTPUT);

  digitalWrite(cylinder_1_Pin, HIGH);           // Pop Up Jewel in Tube to Start
  digitalWrite(cylinder_2_Pin, LOW);            // Send Spider to back of Tube to Start
  digitalWrite(air_1_Pin, LOW);                 // Turn off Tube 1 Air to Start
  digitalWrite(air_2_Pin, LOW);                 // Turn off Tube 2 Air to Start

}

void loop() {
  currentTime = millis(); 
  tubeAir_1();                        // Run Jewel Tube 1 
  tubeAir_2();                        // Run Jewel Tube 2
  jewelDrop();                        // Run Jewel Drop Tube
  spiderRun();                        // Run Spider Tube
  spiderJump_1();                     // Run Spider Jump 1
  spiderJump_2();                     // Run Spider Jump 2
}

void tubeAir_1 () {
  if (digitalRead(sensor_1_Pin) == LOW) {                 // Check to See if Sensor is Activated on Jewel Tube 1
    digitalWrite(air_1_Pin, HIGH);                        // If yes, Turn On Jewel Tube 1 Air
  } else {
    digitalWrite(air_1_Pin, LOW);                         // ... Otherwise turn Jewel Tube 1 Air Off
  }
}

void tubeAir_2 () {
  if (digitalRead(sensor_2_Pin) == LOW) {                 // Check to See if Sensor is Activated on Jewel Tube 2
    digitalWrite(air_2_Pin, HIGH);                        // If yes, Turn On Jewel Tube 2 Air
  } else {
    digitalWrite(air_2_Pin, LOW);                         // ... Otherwise turn Jewel Tube 2 Air Off
  }
}

void jewelDrop() {
  static unsigned long lastTime_Drop;         // Variable to hold Last time action happened 
  static uint8_t randomNumber_Drop;           // Variable to hold random number for Drop Timing

  if ((digitalRead(sensor_3_Pin) == LOW) || (digitalRead(sensor_4_Pin) == LOW)) {                 // Check to See if Sensor is Activated on Jewel Drop Tube 
    digitalWrite(cylinder_1_Pin, LOW);                                                            // If yes, Drop Jewel
  } else {
   // digitalWrite(cylinder_1_Pin, HIGH);                                                           // ... Otherwise Leave Jewel in Tube - TURN THIS ON FOR Sensor DROP Only
      if ((currentTime - lastTime_Drop) >= Interval_jewelDrop) {      // Resest Random Number for the Heads
        randomNumber_Drop = random(0, 2);                             // Set Sub Timers duration variable
        lastTime_Drop = currentTime;                                  // Reset Interval Clock
        counter_jewelDrop_1 = 0;                                      // Reset the Counters
        counter_jewelDrop_2 = 0;
        counter_jewelDrop_3 = 0;
     }

     switch (randomNumber_Drop) {
        case 0:                                  // Option Timing 1 - Longest Drop
          if (counter_jewelDrop_1 == 0) {               // If Starting Drop...
            digitalWrite(cylinder_1_Pin, LOW);          // ... Drop Jewel  
            counter_jewelDrop_1 = 1;                    // Go to Next Step
            lastTime_Drop = millis();                   // Reset Variable Clock
           }

          if ((currentTime - lastTime_Drop) > Duration_jewelDrop[0] && counter_jewelDrop_1 == 1) {      // If on this Step and Drop Time has Passed...
            digitalWrite(cylinder_1_Pin, HIGH);                                                         // Bring Jewel Back Up
            counter_jewelDrop_1 = 2;                                                                    // Moved on to Next Step (Wait for Counter Reset)
          } 
        break;

        case 1:                                  // Option Timing 2 - Medium Drop
          if (counter_jewelDrop_2 == 0) {               // If Starting Drop...
            digitalWrite(cylinder_1_Pin, LOW);          // ... Drop Jewel  
            counter_jewelDrop_2 = 1;                    // Go to Next Step
            lastTime_Drop = millis();                   // Reset Variable Clock
          }

          if ((currentTime - lastTime_Drop) > Duration_jewelDrop[1] && counter_jewelDrop_2 == 1) {      // If on this Step and Drop Time has Passed...
            digitalWrite(cylinder_1_Pin, HIGH);                                                         // Bring Jewel Back Up
            counter_jewelDrop_2 = 2;                                                                    // Moved on to Next Step (Wait for Counter Reset)
          } 
        break;

        case 2:                                  // Option Timing 3 - Shortest Drop
          if (counter_jewelDrop_3 == 0) {               // If Starting Drop...
            digitalWrite(cylinder_1_Pin, LOW);          // ... Drop Jewel  
            counter_jewelDrop_3 = 1;                    // Go to Next Step
            lastTime_Drop = millis();                   // Reset Variable Clock
          }

          if ((currentTime - lastTime_Drop) > Duration_jewelDrop[2] && counter_jewelDrop_3 == 1) {      // If on this Step and Drop Time has Passed...
            digitalWrite(cylinder_1_Pin, HIGH);                                                         // Bring Jewel Back Up
            counter_jewelDrop_3 = 2;                                                                    // Moved on to Next Step (Wait for Counter Reset)
          } 
        break;
      }
  } 
}

void spiderRun () {
  static unsigned long lastTime_Run;                      // Variable to hold Last time action happened
  static uint8_t randomNumber_Run;                        // Variable to hold random number for Spider Run Timing 

  if ((currentTime - lastTime_Run) >= Interval_spiderRun) {        // Resest Random Number for the Heads
    randomNumber_Run = random(0, 2);                               // Set Sub Timers duration variable
    lastTime_Run = currentTime;                                    // Reset Interval Clock
    counter_spiderRun_1 = 0;                                       // Reset the Counters
    counter_spiderRun_2 = 0;
    counter_spiderRun_3 = 0;
  }

  switch (randomNumber_Run) {
    case 0:                                  // Option Timing 1 - Longest Spider Send
      if (counter_spiderRun_1 == 0) {               // If Starting Spider Send...
        digitalWrite(cylinder_2_Pin, HIGH);         // ... Send Spider  
        counter_spiderRun_1 = 1;                    // Go to Next Step
        lastTime_Run = millis();                    // Reset Variable Clock
      }

      if ((currentTime - lastTime_Run) > Duration_spiderRun[0] && counter_spiderRun_1 == 1) {      // If on this Step and Run Time has Passed...
        digitalWrite(cylinder_2_Pin, LOW);                                                         // Send Spider Back
        counter_spiderRun_1 = 2;                                                                   // Moved on to Next Step (Wait for Counter Reset)
      } 
    break;

    case 1:                                  // Option Timing 2 - Medium Spider Send
      if (counter_spiderRun_2 == 0) {               // If Starting Spider Send...
        digitalWrite(cylinder_2_Pin, HIGH);         // ... Send Spider  
        counter_spiderRun_2 = 1;                    // Go to Next Step
        lastTime_Run = millis();                    // Reset Variable Clock
      }

      if ((currentTime - lastTime_Run) > Duration_spiderRun[1] && counter_spiderRun_2 == 1) {      // If on this Step and Run Time has Passed...
        digitalWrite(cylinder_2_Pin, LOW);                                                         // Send Spider Back
        counter_spiderRun_2 = 2;                                                                   // Moved on to Next Step (Wait for Counter Reset)
      } 
    break;

    case 2:                                  // Option Timing 3 - Shortest Spider Send
      if (counter_spiderRun_3 == 0) {               // If Starting Spider Send...
        digitalWrite(cylinder_2_Pin, HIGH);         // ... Send Spider  
        counter_spiderRun_3 = 1;                    // Go to Next Step
        lastTime_Run = millis();                    // Reset Variable Clock
      }

      if ((currentTime - lastTime_Run) > Duration_spiderRun[2] && counter_spiderRun_3 == 1) {      // If on this Step and Run Time has Passed...
        digitalWrite(cylinder_2_Pin, LOW);                                                         // Send Spider Back
        counter_spiderRun_3 = 2;                                                                   // Moved on to Next Step (Wait for Counter Reset)
      } 
    break;
  }
}

void spiderJump_1 () {
  static unsigned long lastTime_Jump_1;                     // Variable to hold Last time action happened 
  static uint8_t randomNumber_Jump_1;                       // Variable to hold random number for Spider Jump Timing
  static uint8_t randomAngle_Jump_1;                        // Variable to hold random number for Spider Jump Angle

  if ((currentTime - lastTime_Jump_1) >= Interval_spiderJump_1) {        // Resest Random Number for the Heads
    randomNumber_Jump_1 = random(0, 2);                                  // Set Sub Timers duration variable
    randomAngle_Jump_1 = random(0,3);                                    // Set Angle for Spider to Jump 
    lastTime_Jump_1 = currentTime;                                       // Reset Interval Clock
    counter_spiderJump_1_1 = 0;                                          // Reset the Counters
    counter_spiderJump_1_2 = 0;
    counter_spiderJump_1_2 = 0;
  }

  switch (randomNumber_Jump_1) {
    case 0:                                  // Option Timing 1 - Longest Spider Jump
      if (counter_spiderJump_1_1 == 0) {                          // If Starting Spider Jump...
        spiderServo_1.write(Spider_Angle_1[randomAngle_Jump_1]);  // ... Send Jumping Spider 
        counter_spiderJump_1_1 = 1;                               // Go to Next Step
        lastTime_Jump_1 = millis();                               // Reset Variable Clock
      }

      if ((currentTime - lastTime_Jump_1) > Duration_spiderJump_1[0] && counter_spiderJump_1_1 == 1) {      // If on this Step and Jump Time has Passed...
        spiderServo_1.write(minAngle_1);                                                                    // Send Spider Back
        counter_spiderJump_1_1 = 2;                                                                         // Moved on to Next Step (Wait for Counter Reset)
      } 
    break;

    case 1:                                  // Option Timing 2 - Medium Spider Jump
      if (counter_spiderJump_1_2 == 0) {                          // If Starting Spider Jump...
        spiderServo_1.write(Spider_Angle_1[randomAngle_Jump_1]);  // ... Send Jumping Spider 
        counter_spiderJump_1_2 = 1;                               // Go to Next Step
        lastTime_Jump_1 = millis();                               // Reset Variable Clock
      }

      if ((currentTime - lastTime_Jump_1) > Duration_spiderJump_1[1] && counter_spiderJump_1_2 == 1) {      // If on this Step and Jump Time has Passed...
        spiderServo_1.write(minAngle_1);                                                                    // Send Spider Back
        counter_spiderJump_1_2 = 2;                                                                         // Moved on to Next Step (Wait for Counter Reset)
      } 
    break;

    case 2:                                  // Option Timing 3 - Shortest Spider Jump
      if (counter_spiderJump_1_3 == 0) {                          // If Starting Spider Jump...
        spiderServo_1.write(Spider_Angle_1[randomAngle_Jump_1]);  // ... Send Jumping Spider 
        counter_spiderJump_1_3 = 1;                               // Go to Next Step
        lastTime_Jump_1 = millis();                               // Reset Variable Clock
      }

      if ((currentTime - lastTime_Jump_1) > Duration_spiderJump_1[2] && counter_spiderJump_1_3 == 1) {      // If on this Step and Jump Time has Passed...
        spiderServo_1.write(minAngle_1);                                                                    // Send Spider Back
        counter_spiderJump_1_3 = 2;                                                                         // Moved on to Next Step (Wait for Counter Reset)
      } 
    break;
  }
}

void spiderJump_2 () {
  static unsigned long lastTime_Jump_2;                     // Variable to hold Last time action happened 
  static uint8_t randomNumber_Jump_2;                       // Variable to hold random number for Spider Jump Timing
  static uint8_t randomAngle_Jump_2;                        // Variable to hold random number for Spider Jump Angle

  if ((currentTime - lastTime_Jump_2) >= Interval_spiderJump_2) {        // Resest Random Number for the Heads
    randomNumber_Jump_2 = random(0, 2);                                  // Set Sub Timers duration variable
    randomAngle_Jump_2 = random(0,3);                                    // Set Angle for Spider to Jump 
    lastTime_Jump_2 = currentTime;                                       // Reset Interval Clock
    counter_spiderJump_2_1 = 0;                                          // Reset the Counters
    counter_spiderJump_2_2 = 0;
    counter_spiderJump_2_2 = 0;
  }

  switch (randomNumber_Jump_2) {
    case 0:                                  // Option Timing 1 - Longest Spider Jump
      if (counter_spiderJump_2_1 == 0) {                          // If Starting Spider Jump...
        spiderServo_2.write(Spider_Angle_2[randomAngle_Jump_2]);  // ... Send Jumping Spider 
        counter_spiderJump_2_1 = 1;                               // Go to Next Step
        lastTime_Jump_2 = millis();                               // Reset Variable Clock
      }

      if ((currentTime - lastTime_Jump_2) > Duration_spiderJump_2[0] && counter_spiderJump_2_1 == 1) {      // If on this Step and Jump Time has Passed...
        spiderServo_2.write(minAngle_1);                                                                    // Send Spider Back
        counter_spiderJump_2_1 = 2;                                                                         // Moved on to Next Step (Wait for Counter Reset)
      } 
    break;

    case 1:                                  // Option Timing 2 - Medium Spider Jump
      if (counter_spiderJump_2_2 == 0) {                          // If Starting Spider Jump...
        spiderServo_2.write(Spider_Angle_2[randomAngle_Jump_2]);  // ... Send Jumping Spider 
        counter_spiderJump_2_2 = 1;                               // Go to Next Step
        lastTime_Jump_2 = millis();                               // Reset Variable Clock
      }

      if ((currentTime - lastTime_Jump_2) > Duration_spiderJump_2[1] && counter_spiderJump_2_2 == 1) {      // If on this Step and Jump Time has Passed...
        spiderServo_2.write(minAngle_1);                                                                    // Send Spider Back
        counter_spiderJump_2_2 = 2;                                                                         // Moved on to Next Step (Wait for Counter Reset)
      } 
    break;

    case 2:                                  // Option Timing 3 - Shortest Spider Jump
      if (counter_spiderJump_2_3 == 0) {                          // If Starting Spider Jump...
        spiderServo_2.write(Spider_Angle_2[randomAngle_Jump_2]);  // ... Send Jumping Spider 
        counter_spiderJump_2_3 = 1;                               // Go to Next Step
        lastTime_Jump_2 = millis();                               // Reset Variable Clock
      }

      if ((currentTime - lastTime_Jump_2) > Duration_spiderJump_2[2] && counter_spiderJump_2_3 == 1) {      // If on this Step and Jump Time has Passed...
        spiderServo_2.write(minAngle_1);                                                                    // Send Spider Back
        counter_spiderJump_2_3 = 2;                                                                         // Moved on to Next Step (Wait for Counter Reset)
      } 
    break;
  }
}

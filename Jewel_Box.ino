/* Include Libraries */  

/* Set Pins */  
#define sensor_1_Pin 3         // Jewel Tube 1 Sensor
#define sensor_2_Pin 4         // Jewel Tube 2 Sensor
#define cylinder_1_Pin 5       // Jewel Drop Piston
#define cylinder_2_Pin 6       // Spider Run Piston
#define air_1_Pin 7            // Jewel Tube 1 Blow Off Air
#define air_2_Pin 8            // Jewel Tube 2 Blow Off Air

/* Time Variables */
unsigned long currentTime = millis();                                 // Current Time
const unsigned long Interval_jewelDrop = 5000;                        // Duration of time to set random numbers for Jewel Drop Tube (5 Seconds)
const unsigned long Interval_spiderRun = 10000;                       // Duration of time to set random numbers for Spider Run Tube (10 Seconds)
const unsigned long Interval_spiderJump = 15000;                      // Duration of time to set random numbers for Spider Jump (15 Seconds)
const unsigned long Duration_jewelDrop[3] = { 3000, 2000, 1000 };     // Duration Array for Jewel Drop Tube (Seconds) ** Highest number must be less than "Interval_jewelDrop" Setting
const unsigned long Duration_spiderRun[3] = { 4500, 3000, 1000 };     // Duration Array for Spider Run Tube (Seconds) ** Highest number must be less than "Interval_spiderRun" Setting
const unsigned long Duration_spiderJump[3] = { 1000, 1000, 1000};     // Duration Array for Spider Jump (Seconds) ** Highest number must be less than "Interval_spiderJump" Setting

uint8_t counter_jewelDrop_1 = 0;                // Counter 1 for Jewel Drop
uint8_t counter_jewelDrop_2 = 0;                // Counter 2 for Jewel Drop
uint8_t counter_jewelDrop_3 = 0;                // Counter 3 for Jewel Drop
uint8_t counter_spiderRun_1 = 0;                // Counter 1 for Spider Run                
uint8_t counter_spiderRun_2 = 0;                // Counter 2 for Spider Run 
uint8_t counter_spiderRun_3 = 0;                // Counter 3 for Spider Run 

/* Function Prototypes */
void tubeAir_1();
void tubeAir_2();
void jewelDrop();
void spiderRun();
void spiderJump();

void setup() {
/* Serial Setup */
  Serial.begin(9600);                           // Setup Serial Monitor
  randomSeed(3);                                // Start Random Seed for use of Random Function Later

/* Set Pin Modes */
  pinMode(sensor_1_Pin, INPUT_PULLUP);
  pinMode(sensor_2_Pin, INPUT_PULLUP);
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
  spiderJump();                       // Run Spider Jump
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

void spiderRun () {
  static unsigned long lastTime_Run;                      // Variable to hold Last time action happened
  static uint8_t randomNumber_Run;                        // Variable to hold random number for Spider Run Timing 

    if ((currentTime - lastTime_Run) >= Interval_spiderRun) {      // Resest Random Number for the Heads
    randomNumber_Run = random(0, 2);                             // Set Sub Timers duration variable
    lastTime_Run = currentTime;                                  // Reset Interval Clock
    counter_spiderRun_1 = 0;                                      // Reset the Counters
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

void spiderJump () {
  static unsigned long lastTime_Jump;                     // Variable to hold Last time action happened 
  static uint8_t randomNumber_Jump;                       // Variable to hold random number for Spider Jump Timing

}

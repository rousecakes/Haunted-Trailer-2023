#include <AccelStepper.h>

/* AccelStepper Setup */
AccelStepper stepperS(1, 12, 13);     // 1 = Driver Interface (TB6600)*** Changed to DM542T 2023
                                    // Arduino Pin 12 Connected to STEP Pin of Driver
                                    // Arduino Pin 13 Connected to DIR Pin of Driver

/* DEFINE Consstraints/Pins */
#define home_switch 4               // Pin 4 Connected to Home Switch (Micro Switch)

/* Stepper Travel Variables and Initializers */
long initial_homing = -1;                                      // Used to Home Stepper at Startup
const float STEPS_PER_REV = 800;                               // 1.8  degree step rotation - Motor steps per rotation

uint8_t randomNumber_position;                                 // Variable for random position
uint8_t randomNumber_delay;                                    // Variable for random delay
uint16_t buffer_position[5] = { 285, 375, 415, 505, 620 };     // Positions to go to
uint16_t buffer_delay[3] = { 1000, 2500, 4000 };               // Delays to stay at position

unsigned long currentTime = millis();                          // Time for re-homing
unsigned long lastTime;                                        // Place to store Time on the Loop
unsigned long duration = 300000;                               // re Home every 5 Mins (300,000 Milliseconds)

/* Function ProtoTypes */
void Homing();


void setup() {
  Serial.begin(9600);                                // Start Serial Monitor with speed of 9600 Bauds
  randomSeed(3);                                     // Start Random Seed for use of Random Function Later

  /* Set Pins */
  pinMode(home_switch, INPUT_PULLUP);                // Set Homeing Switch as Input - Pull down resistor on Pin

  delay(5);                                          // Wait for Driver to startup

  Homing();                                          // Start Homing procedure of Stepper Motor at startup

}

void loop() {
  currentTime = millis();                                         // Update current Time
  randomNumber_position = random(0,5);                            // Pick Random Position
  randomNumber_delay = random(0,3);                               // Pick Random Delay

  stepperS.moveTo(buffer_position[randomNumber_position]);        // Set Position with Stepper
  stepperS.runToPosition();                                       // Go to set Position
  delay(buffer_delay[randomNumber_delay]);                        // Delay before next movement or cycle
  /* if ((currentTime - lastTime) >= duration) {
    Homing();
    lastTime = currentTime;
  } */
}

void Homing() {

  /* Set Max Speed and Acceleration of each Stepper at Startup for homing */
  stepperS.setMaxSpeed(200.0);        // Set Max Speed of Stepper (Slower to get accuracy)
  stepperS.setAcceleration(200.0);    // Set Acceleration of the Stepper

    Serial.print("Stepper is Homing........");

  while (digitalRead(home_switch)) {  // Make the Stepper move CCW until the switch is activated
    stepperS.moveTo(initial_homing);  // Set the position to move to
    initial_homing--;                 // Decrease by 1 for the next move if needed
    stepperS.run();                   // Start Moving the stepper
    delay(5);
  }

  stepperS.setCurrentPosition(0);     // Set the current position as zero for now
  stepperS.setMaxSpeed(200.0);        // Set Max Speed of Stepper (Slower to get accuracy)
  stepperS.setAcceleration(200.0);    // Set Acceleration of the Stepper
  initial_homing = 1;

  while (!digitalRead(home_switch)) { // Make the Stepper move CW until the switch is deactivated
    stepperS.moveTo(initial_homing);
    stepperS.run();
    initial_homing++;
    delay(5);
  }

  stepperS.setCurrentPosition(0);
    Serial.println("Homing Completed");
    Serial.println("");
  stepperS.setMaxSpeed(1000.0);       // Set Max Speed of Stepper
  stepperS.setAcceleration(1000.0);   // Set Acceleration of Stepper
}

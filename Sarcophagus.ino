#include <AccelStepper.h>


/* AccelStepper Setup */
AccelStepper stepperS(1, 12, 13);   // 1 = Driver Interface (TB6600)*** Changed to DM542T 05/2023 *** Changed to DM860T(V3.0) 07/2023
                                    // Arduino Pin 12 Connected to STEP Pin of Driver
                                    // Arduino Pin 13 Connected to DIR Pin of Driver
                                    // 800 Steps per Revolution - 1.8 Deg Per Step

/* DEFINE Consstraints/Pins */
#define lidPiston 4                 // Lid Piston actuacted on pin 4
#define handPiston 5                // Hand Piston Actuated on pin 5
#define strobePin 6                 // Strobe Actuated on Pin 6
#define eventButtonPin 7            // Pin for Button to Initiate Hand Event
#define smokePin 8                  // Pin For Sarcophagus Smoker

/* Stepper Travel Variables and Initializers */
const int motorSpeed = 450;                                    // Keep between 150-1000, Lower equal slower
unsigned long stepperVar = 1;                                  // A Place to hold Step location

uint8_t randomNumber_delay;                                    // Variable for random delay
uint16_t buffer_delay[3] = { 500, 1500, 2500 };                // Delays to stay at position (1/2 Seconds, 1-1/2 Seconds, 2-1/2 Seconds)

unsigned long currentTime = millis();                          // Set Time Variable
unsigned long lastTime_knockDelay;                             // Place to store Time on Knock Wheel Delay
unsigned long duration_knockDelay = 5000;                      // Delay Interval on Knock Wheel - 5 seconds
unsigned long lastTime_handEvent;                              // Place to store Time on the Hand Event
unsigned long duration_handEvent = 90000;                      // Interval Trigger for Hand Event - 1.5 Mins

unsigned long lidOpen_delay = 1000;                            // Lid Open Delay - 1 Second
unsigned long lidClose_delay = 500;                            // Lid Close Delay - 1/2 Second
unsigned long handExtend_delay = 1500;                         // Hand Extend Delay - 1-1/2 Seconds
unsigned long handRetract_delay = 1500;                        // Hand Retract Delay - 1-1/2 Seconds
unsigned long handDuration_delay_1 = 300;                      // Hand Duration 1 - 1/3 ish Seconds
unsigned long handDuration_delay_2 = 2000;                     // Hand Duration 2 - 2 Seconds
unsigned long strobe_delay_1 = 250;                             // Smoke Delay 1 - 1/4 Seconds
unsigned long strobe_delay_2 = 500;                             // Smoke Delay 2 - 1/2 Seconds
unsigned long smoke_delay_Space = 300;                         // Smoke Delay Space - 1/3 ish Seconds - Space between Blasts


/* Function ProtoTypes */
void handEvent();

void setup() {
  Serial.begin(9600);                                // Start Serial Monitor with speed of 9600 Bauds
  delay(5);                                          // Wait for Driver to startup
  stepperS.setCurrentPosition(0);                    // Set Current Position to Zero  
  stepperS.setMaxSpeed(motorSpeed + 1);              // Set Max Speed of Stepper (Slower to get accuracy)
  stepperS.setAcceleration(motorSpeed);              // Set Acceleration of the Stepper
  stepperS.setSpeed(motorSpeed);                     // Set Speed of Stepper
  randomSeed(3);                                     // Start Random Seed for use of Random Function Later

  /* Set Pins */
  pinMode(lidPiston, OUTPUT);                        // Set all pins as Outputs
  pinMode(handPiston, OUTPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(smokePin, OUTPUT);

  pinMode(eventButtonPin, INPUT_PULLUP);            // Set Bypass wait button as an input
}

void loop() {
  currentTime = millis();                                            // Update current Time
  //stepperS.moveTo(stepperVar);                                         // Set step to move to 1 step forward
  stepperS.runSpeed();                                               // Run Stepper 1 Step
  //stepperVar ++;
  if ((currentTime - lastTime_knockDelay) > duration_knockDelay){    // Check to see if enough time has passed to pick a random wheel delay
    randomNumber_delay = random(0,3);                                // Pick Random Delay
    delay(buffer_delay[randomNumber_delay]);                         // Delay before next movement or cycle
    lastTime_knockDelay = currentTime;                               // Reset Timer as event is complete
  }  
  if ((currentTime - lastTime_handEvent) > duration_handEvent) {     // Check to see if enough time has passed to trigger hand event
    handEvent();                                                     // Run Hand Event Function
    lastTime_handEvent = currentTime;                                // Reset Timer for hand event as it has just been completed
  }
  if (digitalRead(eventButtonPin) == LOW) {
    Serial.println("Button Pressed");
    handEvent();
  }
}

void handEvent() {
  digitalWrite(lidPiston, HIGH);                // Open Sarcophagus
  digitalWrite(smokePin, HIGH);                 // Turn on Smoker
  delay(lidOpen_delay);                         // Delay For Piston to Fire
  digitalWrite(strobePin, HIGH);                // Turn on Strobe
  delay(strobe_delay_1);                        // Strobe Delay 1 Short
  digitalWrite(handPiston, HIGH);               // Turn on Hand Extend
  delay(handExtend_delay);                      // Hand Delay
  digitalWrite(strobePin, LOW);                 // Strobe Off
  delay(handDuration_delay_1);                  // Leave hand out for a second
  digitalWrite(handPiston, LOW);                // Retract Hand
  delay(handDuration_delay_1);                  // Short hand Delay
  digitalWrite(handPiston, HIGH);               // Send Hand Back Out
  delay(handDuration_delay_1);                  // Short hand Delay
  digitalWrite(handPiston, LOW);                // Retract Hand
  delay(handDuration_delay_1);                  // Short hand Delay
  digitalWrite(handPiston, HIGH);               // Send Hand Back out
  delay(handDuration_delay_2);                  // Leave hand out longer
  digitalWrite(strobePin, HIGH);                // Turn on Strobe
  delay(strobe_delay_2);                        // Strobe Delay 2 Long
  digitalWrite(handPiston, LOW);                // Hand Retract
  delay(handRetract_delay);                     // Delay for hand retract
  digitalWrite(lidPiston, LOW);                 // Turn off Lid Piston
  digitalWrite(smokePin, LOW);                  // Turn of Smoker
  delay(strobe_delay_1);                        // Strobe Delay 1 Short
  digitalWrite(strobePin, LOW);                 // Strobe Off
  delay(lidClose_delay);                        // Lid Piston Delay
}

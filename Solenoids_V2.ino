/* Set Pins */                                                                                // Index ->    0       1        2        3        4
const uint8_t headPins[5] = { 3, 4, 5, 6, 7 };                                  // Relays for Heads 1-5 { Relay 1, Relay 2, Relay 3, Relay 4, Relay 5 }
const uint8_t NUMBER_OF_HEADPINS = sizeof(headPins) / sizeof(headPins[0]);
                                                                                                     // Index ->    0        1        2              
const uint8_t airPins[3] = { 8, 9, 10 };                                        // Relays for Air Blow Offs 1-3 { Relay 6, Relay 7, Relay 8 }
const uint8_t NUMBER_OF_AIRPINS = sizeof(airPins) / sizeof(airPins[0]);
                                                                                                              // Index ->    0     1       2
const uint8_t buttonPins[3] = { 11, 12, 13 };                                   // define the buttonPins that we'll use. { Air 1, Air 2, Air 3 }
const uint8_t NUMBER_OF_BUTTONS = sizeof(buttonPins) / sizeof(buttonPins[0]);   // determine how big the array up above is, by checking the size
                                                                                                              // Index ->    0       1       2       3       4 
const uint8_t latchPins[5] = { A0, A1, A2, A3, A4 };                            // define the Latch Pins that we'll use. { Head 1, Head 2, Head 3, Head 4, Head 5 }
const uint8_t NUMBER_OF_LATCHES = sizeof(latchPins) / sizeof(latchPins[0]);     // determine how big the array up above is, by checking the size

/* Time Variables */
unsigned long currentTime = millis();                    // Current Time
const unsigned long Interval_heads = 5000;               // Duration of time to set random numbers for Head (5 Seconds)
const unsigned long Interval_air = 2000;                 // Duration of time to set random numbers for Head (2 Seconds)
const unsigned long Duration_air = 500;                  // Duration of time for the Air to be on. (1/2 Second)
const unsigned long Duration_heads_1 = 4500;             // 4-1/2 Seconds
const unsigned long Duration_heads_2 = 3000;             // 3 Seconds
const unsigned long Duration_heads_3 = 1000;             // 1 Second

uint8_t counter_heads_1 = 0;
uint8_t counter_heads_2 = 0;
uint8_t counter_heads_3 = 0;
uint8_t counter_air = 0;

const unsigned long DEBOUNCE_DELAY = 50;                                // the debounce time; increase if the output flickers

uint8_t lastSteadyState_heads[5] = { LOW, LOW, LOW, LOW, LOW };         // the previous steady state from the input pin
uint8_t lastFlickerableState_heads[5] = { LOW, LOW, LOW, LOW, LOW };    // the previous flickerable state from the input pin
uint8_t currentState_heads[5];                                          // the current reading from the input pin
uint8_t Buffer_heads[5];                                                // Readings for air buffer - switch on or off
unsigned long lastDebounceTime_heads[5] = { 0, 0, 0, 0, 0 };            // the last time the output pin was toggled

uint8_t lastSteadyState_air[3] = { LOW, LOW, LOW };         // the previous steady state from the input pin
uint8_t lastFlickerableState_air[3] = { LOW, LOW, LOW };    // the previous flickerable state from the input pin
uint8_t currentState_air[3];                                // the current reading from the input pin
uint8_t Buffer_air[3];                                      // Readings for air buffer - switch on or off
unsigned long lastDebounceTime_air[3] = { 0, 0, 0 };        // the last time the output pin was toggled

/* Function Prototypes */
void buttonCheck();
void latchCheck();
void heads();
void air();

void setup() {
/* Serial Setup */
  Serial.begin(9600);                                     // Setup Serial Monitor
  randomSeed(3);                                          // Start Random Seed for use of Random Function Later

/* Set Pin Modes */
  for (uint8_t i = 0; i < NUMBER_OF_HEADPINS; i++) {
    pinMode(headPins[i], OUTPUT);                         // Set Head Pins as Outputs
    digitalWrite(headPins[i], LOW);                       // Initialize Head Pins to Low
  }

  for (uint8_t i = 0; i< NUMBER_OF_AIRPINS; i++) {
    pinMode(airPins[i], OUTPUT);                          // Set Air Pins as Outputs
    digitalWrite(airPins[i], LOW);                        // Initialize Air Pins to Low
  }

  for (uint8_t i=0; i < NUMBER_OF_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);                 // Set Button Pins as Inputs with pullup resistor (Internal)
    digitalWrite(buttonPins[i], HIGH);                    // Initialize Button Pins to High
  }

  for (uint8_t i=0; i < NUMBER_OF_LATCHES; i++) {
    pinMode(latchPins[i], INPUT_PULLUP);                 // Set Latch Pins as Inputs with pullup resistor (Internal)
    digitalWrite(latchPins[i], HIGH);                    // Initialize Button Pins to High
  }
}

void loop() {  
  currentTime = millis();
  buttonCheck();
  latchCheck();
  heads();        // Run Heads Routine - Pop Up
  air();          // Run Air Routine
}

void heads() {
  static unsigned long lastTime_1;                         // Variable to hold Last time action happened
  static uint8_t randomNumber_heads;                       // Variable to hole Random heads Number 
  static uint8_t randomNumber_heads_Delay;                 // Variable to hole Random heads Number 
  static unsigned long lastTime_heads_1;                   // Sub Timer 1 for heads duration
  static unsigned long lastTime_heads_2;                   // Sub Timer 2 for heads duration
  static unsigned long lastTime_heads_3;                   // Sub Timer 3 for heads duration  

  if ((currentTime - lastTime_1) >= Interval_heads) {      // Resest Random Number for the Heads
   randomNumber_heads = random(0, 5);                      // Pick the Head
   randomNumber_heads_Delay = random(0, 2);                // Set Sub Timers duration variable
   lastTime_1 = currentTime;
   counter_heads_1 = 0;                                    // Reset the Counters
   counter_heads_2 = 0;
   counter_heads_3 = 0;
  }

  for (uint8_t i; i < 5; i++) {                            // Check If Latch is Close or Random Number Turn on Corresponding Head 
   if ((Buffer_heads[i]) == LOW) {    
     digitalWrite(headPins[i], HIGH);
   } else if (randomNumber_heads != i) {
     digitalWrite(headPins[i], LOW);
   }
  }

  switch (randomNumber_heads_Delay) {
    case 0:
      if (counter_heads_1 == 0) {
        digitalWrite(headPins[randomNumber_heads], HIGH);
        counter_heads_1 = 1;
        lastTime_heads_1 = millis();
      }

      if ((currentTime - lastTime_heads_1) > Duration_heads_1 && counter_heads_1 == 1) {
        digitalWrite(headPins[randomNumber_heads], LOW);
        counter_heads_1 = 2;
      }
    break;

    case 1:
      if (counter_heads_2 == 0){
        digitalWrite(headPins[randomNumber_heads], HIGH);
        counter_heads_2 = 1;
        lastTime_heads_2 = millis();
      }
      if ((currentTime - lastTime_heads_2) > Duration_heads_2 && counter_heads_2 == 1) {
       digitalWrite(headPins[randomNumber_heads], LOW);
       counter_heads_2 = 2;
      }  
    break;

    case 2:
      if (counter_heads_3 == 0){
        digitalWrite(headPins[randomNumber_heads], HIGH);
        counter_heads_3 = 1;
        lastTime_heads_3 = millis();
      }
      if ((currentTime - lastTime_heads_3) > Duration_heads_3 && counter_heads_3 == 1) {
        digitalWrite(headPins[randomNumber_heads], LOW);
        counter_heads_3 = 2;
      }  
    break;
  }
}

void air() {
  static unsigned long lastTime_2;                         // Variable to hold Last time action happened  
  static uint8_t randomNumber_air;    
  static unsigned long lastTime_air;                       // Variable to hole Random Air Number 

  if ((currentTime - lastTime_2) >= Interval_air) {        // Resest Random Number for Air
   randomNumber_air = random(0,3);
   lastTime_2 = currentTime;
   counter_air = 0;
  }

  for (uint8_t i; i < 3; i++) {                            // Check If button is Closed or Random Number, Turn on Corresponding air blow off
   if ((Buffer_air[i]) == LOW) { 
     digitalWrite(airPins[i], HIGH);
   } else if (randomNumber_air != i) {
     digitalWrite(airPins[i], LOW);
   }
  }

  if (counter_air == 0){
    digitalWrite(airPins[randomNumber_air], HIGH);
    counter_air = 1;
    lastTime_air = millis();
  }
  if ((currentTime - lastTime_air) > Duration_air && counter_air == 1) {
    digitalWrite(airPins[randomNumber_air], LOW);
    counter_air = 2;
  }  
}

void buttonCheck() {
  for (uint8_t i; i < 3; i++) {
    currentState_air[i] = digitalRead(buttonPins[i]);                                      // read the state of the switch/button:

    if (currentState_air[i] != lastFlickerableState_air[i]) {                              // If the switch/button changed, due to noise or pressing:
      lastDebounceTime_air[i] = currentTime;                                               // reset the debouncing timer
      lastFlickerableState_air[i] = currentState_air[i];                                   // save the the last flickerable state
    }

    if ((currentTime - lastDebounceTime_air[i]) > DEBOUNCE_DELAY) {                        // whatever the reading is at, it's been there for longer than the debounce delay, so take it as the actual current state:
     if (lastSteadyState_air[i] == HIGH && currentState_air[i] == LOW) {                   // if the button state has changed:
       Buffer_air[i] = LOW;
     } else if (lastSteadyState_air[i] == LOW && currentState_air[i] == HIGH) {
       Buffer_air[i] = HIGH;
     }  
     lastSteadyState_air[i] = currentState_air[i];                                         // save the the last steady state
    }
  }
}

void latchCheck() {
  for (uint8_t i; i < 5; i++) {
    currentState_heads[i] = digitalRead(latchPins[i]);                                      // read the state of the switch/button:

    if (currentState_heads[i] != lastFlickerableState_heads[i]) {                           // If the switch/button changed, due to noise or pressing:
      lastDebounceTime_air[i] = currentTime;                                                // reset the debouncing timer
      lastFlickerableState_heads[i] = currentState_heads[i];                                // save the the last flickerable state
    }

    if ((currentTime - lastDebounceTime_heads[i]) > DEBOUNCE_DELAY) {                       // whatever the reading is at, it's been there for longer than the debounce delay, so take it as the actual current state:
     if (lastSteadyState_heads[i] == HIGH && currentState_heads[i] == LOW) {                // if the button state has changed:
       Buffer_heads[i] = LOW;
     } else if (lastSteadyState_heads[i] == LOW && currentState_heads[i] == HIGH) {
       Buffer_heads[i] = HIGH;
     }  
     lastSteadyState_heads[i] = currentState_heads[i];                                      // save the the last steady state
    }
  }
}


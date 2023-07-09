/* Set Pins */                                                                                // Index ->    0       1        2        3        4
const uint8_t headPins[5] = { 3, 4, 5, 6, 7 };                                  // Relays for Heads 1-5 { Relay 1, Relay 2, Relay 3, Relay 4, Relay 5 }
const uint8_t NUMBER_OF_HEADPINS = sizeof(headPins) / sizeof(headPins[0]);
                                                                                              // Index ->    0       1        2        3        
const uint8_t airPins[3] = { 8, 9, 10 };                                        // Relays for Air Blow Offs 1-3 { Relay 6, Relay 7, Relay 8 }
const uint8_t NUMBER_OF_AIRPINS = sizeof(airPins) / sizeof(airPins[0]);
                                                                                                              // Index ->    0     1       2
const uint8_t buttonPins[3] = { 11, 12, 13 };                                   // define the buttonPins that we'll use. { Air 1, Air 2, Air 3 }
const uint8_t NUMBER_OF_BUTTONS = sizeof(buttonPins) / sizeof(buttonPins[0]);   // determine how big the array up above is, by checking the size
                                                                                                              // Index ->    0       1       2       3       4 
const uint8_t latchPins[5] = { A0, A1, A2, A3, A4 };                            // define the Latch Pins that we'll use. { Head 1, Head 2, Head 3, Head 4, Head 5 }
const uint8_t NUMBER_OF_LATCHES = sizeof(latchPins) / sizeof(latchPins[0]);     // determine how big the array up above is, by checking the size

/* Button Initializers and settings */
//track if a button is just pressed, just released, or 'currently pressed' 
uint8_t pressed[NUMBER_OF_BUTTONS], justpressed[NUMBER_OF_BUTTONS], justreleased[NUMBER_OF_BUTTONS];
uint8_t previous_keystate[NUMBER_OF_BUTTONS], current_keystate[NUMBER_OF_BUTTONS];
const uint8_t DEBOUNCE = 10;        // how many ms to debounce, 5+ ms is usually plenty

/* Time Variables */
unsigned long currentTime = millis();                    // Current Time
const unsigned long Interval_heads = 5000;               // Duration of time to set random numbers for Head
const unsigned long Interval_air = 2000;                 // Duration of time to set random numbers for Head
const unsigned long Duration_air = 500;                  // Duration of time for the Air to be on.
const unsigned long Duration_heads_1 = 4000;
const unsigned long Duration_heads_2 = 3000;
const unsigned long Duration_heads_3 = 1000;

uint8_t counter_heads_1 = 0;
uint8_t counter_heads_2 = 0;
uint8_t counter_heads_3 = 0;
uint8_t counter_air = 0;

  

/* Function Prototypes */
void ButtonRoutine();
void check_switches();
uint8_t thisSwitch_justPressed();
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

  for (uint8_t i=0; i < NUMBER_OF_BUTTONS; i++)   {
    pinMode(buttonPins[i], INPUT_PULLUP);                 // Set Button Pins as Inputs with pullup resistor (Internal)
    digitalWrite(buttonPins[i], HIGH);                    // Initialize Button Pins to High
  }

  for (uint8_t i=0; i < NUMBER_OF_LATCHES; i++)   {
    pinMode(latchPins[i], INPUT_PULLUP);                 // Set Latch Pins as Inputs with pullup resistor (Internal)
    digitalWrite(latchPins[i], HIGH);                    // Initialize Button Pins to High
  }

  Serial.print("Button checker with ");
  Serial.print(NUMBER_OF_BUTTONS, DEC);
  Serial.println(" buttonPins");

}

void loop() {  
  currentTime = millis();
  heads();        // Run Heads Routine - Pop Up
  air();          // Run Air Routine

}

void heads() {
  static unsigned long lastTime_1;                         // Variable to hold Last time action happened
  static uint8_t randomNumber_heads;                       // Variable to hole Random heads Number 
  static uint8_t randomNumber_heads_Delay;                 // Variable to hole Random heads Number 
  unsigned long lastTime_heads_1;                          // Sub Timer 1 for heads duration
  unsigned long lastTime_heads_2;                          // Sub Timer 2 for heads duration
  unsigned long lastTime_heads_3;                          // Sub Timer 3 for heads duration  

  if ((currentTime - lastTime_1) >= Interval_heads) {      // Resest Random Number for the Heads
   randomNumber_heads = random(0, 6);                      // Pick the Head
   randomNumber_heads_Delay = random(0, 3);                // Set Sub Timers duration variable
   lastTime_1 = currentTime;
   counter_heads_1 = 0;                                    // Reset the Counters
   counter_heads_2 = 0;
   counter_heads_3 = 0;
  }

  for (uint8_t i; i < 5; i++) {                            // Check If Latch is Close or Random Number Turn on Corresponding Head 
   if ((digitalRead(latchPins[i]) == LOW)){    
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
  unsigned long lastTime_air;                              // Variable to hole Random Air Number 

  if ((currentTime - lastTime_2) >= Interval_air) {        // Resest Random Number for Air
   randomNumber_air = random(0,3);
   lastTime_2 = currentTime;
   counter_air = 0;
  }

  for (uint8_t i; i < 3; i++) {                            // Check If button is Closed or Random Number, Turn on Corresponding air blow off
   if ((digitalRead(buttonPins[i]) == LOW)){ 
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

void ButtonRoutine() {
  uint8_t thisSwitch = thisSwitch_justPressed();
  switch(thisSwitch) {  
    case 0: 
     Serial.println("switch 1 just pressed"); 
      if (digitalRead(buttonPins[0] == LOW)) {
      digitalWrite(airPins[0], HIGH);
     } else {
      digitalWrite(airPins[0], LOW);
     }
    break;
    case 1: 
     Serial.println("switch 2 just pressed"); 
     if (digitalRead(buttonPins[1] == LOW)) {
      digitalWrite(airPins[1], HIGH);
     } else {
      digitalWrite(airPins[1], LOW);
     }
    break;
    case 2: 
     Serial.println("switch 3 just pressed"); 
     if (digitalRead(buttonPins[2] == LOW)) {
      digitalWrite(airPins[2], HIGH);
     } else {
      digitalWrite(airPins[2], LOW);
     }
    break;
  }
}

void check_switches() {
  static uint8_t previousstate[NUMBER_OF_BUTTONS];
  static uint8_t currentstate[NUMBER_OF_BUTTONS];
  static long lasttime;
  uint8_t index;
  
  if (currentTime < lasttime) {
    // we wrapped around, lets just try again
    lasttime = currentTime;
  }
  if ((lasttime + DEBOUNCE) > currentTime) {
    // not enough time has passed to debounce
    return;
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = currentTime;
  for (index = 0; index < NUMBER_OF_BUTTONS; index++) {
    justpressed[index] = 0;  //when we start, we clear out the "just" indicators
    justreleased[index] = 0;
    currentstate[index] = digitalRead(buttonPins[index]);  //read the button
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
        // just pressed
        justpressed[index] = 1;
      } else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
        justreleased[index] = 1;  // just released
      }
      pressed[index] = !currentstate[index];  //remember, digital HIGH means NOT pressed
    }
    previousstate[index] = currentstate[index];  //keep a running tally of the buttonPins
  }
}

uint8_t thisSwitch_justPressed() {
  uint8_t thisSwitch = 255;
  check_switches();  //check the switches &amp; get the current state
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
    current_keystate[i] = justpressed[i];
    if (current_keystate[i] != previous_keystate[i]) {
      if (current_keystate[i]) thisSwitch = i;
    }
    previous_keystate[i] = current_keystate[i];
  }
  return thisSwitch;
}

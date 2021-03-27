/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/


const int PIN_BUTTON_DOWN = 11;
const int PIN_BUTTON_PLUG = 10;
const int PIN_BUTTON_UP = 9;
const int PIN_BUTTON_LIGHT = 8;

const int PIN_OUT_LED_DOWN = 7;
const int PIN_OUT_LED_PLUG = 12;
const int PIN_OUT_LED_UP = 6;
const int PIN_OUT_LED_LIGHT = A5;

const int PIN_OUT_RELAY_RES1 = 2;
const int PIN_OUT_RELAY_RES2 = 3;
const int PIN_OUT_RELAY_PLUG = 4;
const int PIN_OUT_RELAY_LIGHT = 5;
const int PIN_OUT_RELAY_UP = A3;
const int PIN_OUT_RELAY_DOWN = A4;


long lastDebounceTime = 0;
long debounceDelay = 50;
char motorState = 0;
char lightState = 0;
char plugState = 0;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
int buttonDownPreviousValue = 1;
int buttonDownDebounceHoldValue = 1;
int buttonUpPreviousValue = 1;
int buttonUpDebounceHoldValue = 1;
int buttonLightPreviousValue = 1;
int buttonLightDebounceHoldValue = 1;
int buttonPlugPreviousValue = 1;
int buttonPlugDebounceHoldValue = 1;
long motorTimer = 0;
long motorTimerDuration = 15000;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(PIN_BUTTON_DOWN , INPUT_PULLUP);
  pinMode(PIN_BUTTON_PLUG , INPUT_PULLUP);
  pinMode(PIN_BUTTON_UP , INPUT_PULLUP);
  pinMode(PIN_BUTTON_LIGHT , INPUT_PULLUP);
  pinMode(PIN_OUT_LED_DOWN , OUTPUT);
  pinMode(PIN_OUT_LED_PLUG , OUTPUT);
  pinMode(PIN_OUT_LED_UP , OUTPUT);
  pinMode(PIN_OUT_LED_LIGHT , OUTPUT);
  pinMode(PIN_OUT_RELAY_RES1 , OUTPUT);
  pinMode(PIN_OUT_RELAY_RES2 , OUTPUT);
  pinMode(PIN_OUT_RELAY_PLUG , OUTPUT);
  pinMode(PIN_OUT_RELAY_LIGHT , OUTPUT);
  pinMode(PIN_OUT_RELAY_UP , OUTPUT);
  pinMode(PIN_OUT_RELAY_DOWN , OUTPUT);

  Serial.println("X");
  lightSet(0);
  plugSet(0);
  setMotor(0);
}

void loop() {
  int buttonDownValue = digitalRead(PIN_BUTTON_DOWN);
  int buttonUpValue = digitalRead(PIN_BUTTON_UP);
  int buttonLightValue = digitalRead(PIN_BUTTON_LIGHT);
  int buttonPlugValue = digitalRead(PIN_BUTTON_PLUG);
  

  if ((buttonDownValue != buttonDownDebounceHoldValue)
      || (buttonUpValue != buttonUpDebounceHoldValue) 
      || (buttonLightValue != buttonLightDebounceHoldValue) 
      || (buttonPlugValue != buttonPlugDebounceHoldValue) ) {
    lastDebounceTime = millis();
  }

  if ( (millis() - lastDebounceTime) > debounceDelay) {
    checkButtonDown(buttonDownValue);
    checkButtonUp(buttonUpValue);
    checkButtonLight(buttonLightValue);
    checkButtonPlug(buttonPlugValue);
  }

  buttonDownDebounceHoldValue = buttonDownValue;
  buttonUpDebounceHoldValue = buttonUpValue;
  buttonLightDebounceHoldValue = buttonLightValue;
  buttonPlugDebounceHoldValue = buttonPlugValue;

  if (Serial.available()) {
    switch((char)Serial.read()) {
      case 'U':
        setMotor(1);
        break;
      case 'D':
        setMotor(-1);
        break;
      case 'S':
        setMotor(0);
        break;
      case 'L':
        lightSet(1);
        break;
      case 'l':
        lightSet(0);
        break;
      case 'P':
        plugSet(1);
        break;
      case 'p':
        plugSet(0);
        break;
      
    }
  }

  long current = millis();
  if (motorTimer != 0 && motorTimer < current) {
    setMotor(0);
  }
}

inline void checkButtonDown(int newValue) {
  if (newValue != buttonDownPreviousValue) {
    buttonDownPreviousValue = newValue;
    if (newValue == 0) {
      motorDownAction();
    }
  }
}

inline void checkButtonUp(int newValue) {
  if (newValue != buttonUpPreviousValue) {
    buttonUpPreviousValue = newValue;
    if (newValue == 0) {
      motorUpAction();
    }
  }
}

inline void checkButtonLight(int newValue) {
  if (newValue != buttonLightPreviousValue) {
    buttonLightPreviousValue = newValue;
    if (newValue == 0) {
      lightSwitchAction();
    }
  }
}

inline void checkButtonPlug(int newValue) {
  if (newValue != buttonPlugPreviousValue) {
    buttonPlugPreviousValue = newValue;
    if (newValue == 0) {
     plugSwitchAction();
    }
  }
}

void motorDownAction() {
   setMotor(motorState == 0 ? -1 : 0);
}

void motorUpAction() {
    setMotor(motorState == 0 ? 1 : 0);
}

void setMotor(char state) {
  if ( state == 0 ) {
      motorTimer = 0;
      Serial.println("S");
      digitalWrite(PIN_OUT_RELAY_UP, 0);
      digitalWrite(PIN_OUT_RELAY_DOWN, 0);
      digitalWrite(PIN_OUT_LED_DOWN, 0);
      digitalWrite(PIN_OUT_LED_UP, 0);
    motorState = 0;
  } else {
    motorTimer = millis() + motorTimerDuration;
    if (state > 0) {
      Serial.println("U");
      digitalWrite(PIN_OUT_LED_UP, 1);
      digitalWrite(PIN_OUT_LED_DOWN, 0);
      digitalWrite(PIN_OUT_RELAY_UP, 1);
      digitalWrite(PIN_OUT_RELAY_DOWN, 0);
      motorState = 1;
    } else {
      Serial.println("D");
      digitalWrite(PIN_OUT_LED_UP, 0);
      digitalWrite(PIN_OUT_LED_DOWN, 1);
      digitalWrite(PIN_OUT_RELAY_UP, 0);
      digitalWrite(PIN_OUT_RELAY_DOWN, 1);
      motorState = 1;
    }
  }
   
}

inline void lightSwitchAction() {
  lightSet(lightState == 0 ? 1 : 0);
}

void lightSet(char state) {
   if (state != 0) {
    Serial.println("L");
    digitalWrite(PIN_OUT_LED_LIGHT, 0);
    digitalWrite(PIN_OUT_RELAY_LIGHT, 1);
    lightState = 1;
  }
  else {
    Serial.println("l");
    digitalWrite(PIN_OUT_LED_LIGHT, 1);
    digitalWrite(PIN_OUT_RELAY_LIGHT, 0);
    lightState = 0;
  } 
}

inline void plugSwitchAction() {
  plugSet(plugState == 0 ? 1 : 0);
}

void plugSet(char state) {
   if (state != 0) {
    Serial.println("P");
    digitalWrite(PIN_OUT_LED_PLUG, 1);
    digitalWrite(PIN_OUT_RELAY_PLUG, 1);
    plugState = 1;
  }
  else {
    Serial.println("p");
    digitalWrite(PIN_OUT_LED_PLUG, 0);
    digitalWrite(PIN_OUT_RELAY_PLUG, 0);
    plugState = 0;
  } 
}

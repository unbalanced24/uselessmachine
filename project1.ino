#include<SoftRcPulseOut.h>

// Pins
#define SERVO_PIN 0
#define LED_BUILTIN 1 
#define SWITCH_PIN 2    
#define SWITCH_OUT 3    


#define STOP 96
#define FORWARD 50
#define BACKWARD 122
#define BACKWARD_TIME 500
#define BACKWARD_TIME2 2000

// States für State-Machine
#define STATE_START 0
#define STATE_FORWARD 1
#define STATE_BACKWARD 2
int anzahl_loops = 0;


SoftRcPulseOut servo;
int state;
unsigned long backward_start_time;


void setup()
{
  servo.attach(SERVO_PIN);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(SWITCH_OUT, OUTPUT);
  digitalWrite(SWITCH_OUT, LOW);
  
  pinMode(LED_BUILTIN, OUTPUT);
  state = 0;
}


void loop(){

  int switch_state = digitalRead(SWITCH_PIN);

  unsigned long now = millis();


  if (switch_state == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  
  // ==================================== //
  // State machine                        //
  // ==================================== //
  if (state == STATE_START) {
    if (switch_state == LOW) {
      anzahl_loops += 1;
      state = STATE_FORWARD;
    }
  } 
    
  else if (state == STATE_FORWARD) {
    if (switch_state == HIGH) {
      if (anzahl_loops == 4) {
        delay(1000);
        state = STATE_BACKWARD;
        backward_start_time = now;
      }
      if (anzahl_loops == 5) {
        delay(2000);
        state = STATE_BACKWARD;
        backward_start_time = now;
      }
      else {
        state = STATE_BACKWARD;
        backward_start_time = now;
      }
    }
   }
  
  else if (state = STATE_BACKWARD) {
      if (switch_state == LOW) {        // LOW bedeutet Schalter ist ON
        state = STATE_FORWARD;
       }
      if (anzahl_loops == 4 or anzahl_loops == 5) {
        if (now - backward_start_time > BACKWARD_TIME2) {
          state = STATE_START;
      }
      }
      else if (now - backward_start_time > BACKWARD_TIME) {
        state = STATE_START;
      }
  }  

  
  // ==================================== //
  // Servo motor                          //
  // ==================================== //
  if (state == STATE_FORWARD) {
    if (anzahl_loops < 3) {
      servo.write(FORWARD);
    }
    else if (anzahl_loops < 4) {
      servo.write(50);
      delay(200);
    }
    else if (anzahl_loops < 5) {
      servo.write(50);
    }
    else if (anzahl_loops == 6) {
      servo.write(0);
      delay(100);
    }
    else if (anzahl_loops == 7) {
      servo.write(0);
      delay(50);
    }
    else if (anzahl_loops > 4) {
      servo.write(FORWARD);
    }
    if (anzahl_loops == 8) {
      anzahl_loops = 0;
    }
  } 
  else if (state == STATE_BACKWARD) {
    servo.write(BACKWARD);
    }
  else {
    servo.write(STOP);
  }
      
  SoftRcPulseOut::refresh();
  delay(20);
  
  }

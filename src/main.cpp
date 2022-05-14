#include <Arduino.h>

#define MOTOR_LEFT_1 2
#define MOTOR_LEFT_2 3
#define MOTOR_RIGHT_1 4
#define MOTOR_RIGHT_2 5

#define MOTOR_LEFT_ENA 9
#define MOTOR_RIGHT_ENA 10

#define SHARP_FRONT_LEFT A0
#define SHARP_FRONT_RIGHT A1
#define SHARP_BACK A2

#define LINE_SENSOR_FRONT A3
#define LINE_SENSOR_BACK A4
#define MODULE_START 11

int module_start_val;
int line_sensor_front_val;
int line_sensor_back_val;
int sharp_front_left_val;
int sharp_front_right_val;
int sharp_back_val;
boolean initpos = true;

void StopBot() {
  digitalWrite(MOTOR_LEFT_1, LOW);
  digitalWrite(MOTOR_LEFT_2, LOW);
  digitalWrite(MOTOR_RIGHT_1, LOW);
  digitalWrite(MOTOR_RIGHT_2, LOW);
}


void readSensors() {
  module_start_val = digitalRead(MODULE_START);
  line_sensor_front_val = analogRead(LINE_SENSOR_FRONT);
  line_sensor_back_val = analogRead(LINE_SENSOR_BACK);
  sharp_front_left_val = analogRead(SHARP_FRONT_LEFT)*0.0048828125;
  sharp_front_right_val = analogRead(SHARP_FRONT_RIGHT)*0.0048828125;
  sharp_back_val = analogRead(SHARP_BACK)*0.0048828125;
}

void go_forward(int speed_left = 255, int speed_right = 255) {
  digitalWrite(MOTOR_LEFT_1, HIGH);
  digitalWrite(MOTOR_LEFT_2, LOW);
  analogWrite(MOTOR_LEFT_ENA, speed_left);

  digitalWrite(MOTOR_RIGHT_1, HIGH);
  digitalWrite(MOTOR_RIGHT_2, LOW);
  analogWrite(MOTOR_RIGHT_ENA, speed_right);
}

void go_backward(int speed_left = 255, int speed_right = 255) {
  digitalWrite(MOTOR_LEFT_1, LOW);
  digitalWrite(MOTOR_LEFT_2, HIGH);
  analogWrite(MOTOR_LEFT_ENA, speed_left);

  digitalWrite(MOTOR_RIGHT_1, LOW);
  digitalWrite(MOTOR_RIGHT_2, HIGH);
  analogWrite(MOTOR_RIGHT_ENA, speed_right);
}

void go_right() {
  go_forward(255, 20);
}

void go_left() {
  go_forward(20, 255);
}

void initbattle() {
  go_left();
  delay(500);
}

void demi_tour() {
  analogWrite(MOTOR_RIGHT_ENA, 255);
  analogWrite(MOTOR_LEFT_ENA, 255);
  long last_time = millis();
  while(millis() - last_time < 400) {
    readSensors();
    if((sharp_front_left_val or sharp_front_right_val) and !(line_sensor_front_val < 100)) {
      break;
    }
    if(millis() - last_time < 100) {
      go_backward();
    }
    else {
      // demi tour
      digitalWrite(MOTOR_LEFT_1, LOW);
      digitalWrite(MOTOR_LEFT_2, HIGH);
      digitalWrite(MOTOR_RIGHT_1, HIGH);
      digitalWrite(MOTOR_RIGHT_2, LOW);
    }
    
  }
    
}

void setup() {
  pinMode(MOTOR_LEFT_1, OUTPUT);
  pinMode(MOTOR_LEFT_2, OUTPUT);
  pinMode(MOTOR_RIGHT_1, OUTPUT);
  pinMode(MOTOR_RIGHT_2, OUTPUT);
  pinMode(MOTOR_LEFT_ENA, OUTPUT);
  pinMode(MOTOR_RIGHT_ENA, OUTPUT);
  pinMode(MODULE_START, INPUT);
  // Serial.begin(9600);
  
}



void loop() {
  readSensors();
  // Serial.println(line_sensor_front_val);
  if (!module_start_val) {StopBot(); return; }
  
  if (line_sensor_front_val < 100) {
    demi_tour();
  }
  else {
    //if(initpos) {initbattle(); initpos = false;}
    if(sharp_front_right_val and !sharp_front_left_val) {
      go_right();
    }
    else if(sharp_front_left_val and !sharp_front_right_val) {
      go_left();
    }
    else {
      go_forward();
    }
  }
}



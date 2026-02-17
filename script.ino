// Receiver Pins
const int ch1_steering = 2; 
const int ch2_throttle = 3; 

// Motor Driver Pins
const int enA = 9;  
const int in1 = 8;  
const int in2 = 7;
const int enB = 10; 
const int in3 = 5;  
const int in4 = 4;

// Deadzone to prevent motor hum
const int deadzone = 25; // Adjust

void setup() {
  pinMode(ch1_steering, INPUT);
  pinMode(ch2_throttle, INPUT);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  // Read RC signals (1000 to 2000 microseconds)
  int pulse_steer = pulseIn(ch1_steering, HIGH);
  int pulse_throttle = pulseIn(ch2_throttle, HIGH);

  // Map to -255 to 255 range
  int steer_val = map(pulse_steer, 1000, 2000, -255, 255);
  int throttle_val = map(pulse_throttle, 1000, 2000, -255, 255);

  // Differential Drive Calculation
  int leftSpeed = throttle_val + steer_val;
  int rightSpeed = throttle_val - steer_val;

  // Constrain to PWM limits
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  controlMotor(leftSpeed, enA, in1, in2);
  controlMotor(rightSpeed, enB, in3, in4);
}

void controlMotor(int speed, int pwmPin, int pin1, int pin2) {
  if (abs(speed) < deadzone) {
    // Stop motor
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    analogWrite(pwmPin, 0);
  } else if (speed > 0) {
    // Forward
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    analogWrite(pwmPin, speed);
  } else {
    // Backward
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    analogWrite(pwmPin, abs(speed));
  }
}
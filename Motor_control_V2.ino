#define XMOTOR 0
#define YMOTOR 1

// OpenCV updates these
volatile float motor_x_angle = 0;  // degrees from center
volatile float motor_y_angle = 0;
volatile float distance = 0;
volatile bool target_detected = false; //volatile type since it will be changing

String data = "";
// Motor pins
const int dirPinx = 2;
const int stepPinx = 4;
const int dirPiny = 3;
const int stepPiny = 5;

// Stepper parameters
const int MOTOR_STEPS = 200;  // 1.8° motor
const int MICROSTEPS = 16;
const int STEPS_PER_REV = MOTOR_STEPS * MICROSTEPS;

// Track motor positions (updated by our code)
float current_x_angle = 0;
float current_y_angle = 0;

void serial_decode(){
  if (Serial.available()) {
    data = Serial.readStringUntil('\n');

    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);
    int thirdComma = data.indexOf(',', secondComma + 1);
    if (firstComma > 0 && //just incase the parsing doesnt notice all of the commas
      secondComma > firstComma && 
      thirdComma > secondComma) {
  
      motor_x_angle = data.substring(0, firstComma).toInt();
      motor_y_angle = data.substring(firstComma + 1, secondComma).toInt();
      distance = data.substring(secondComma + 1, thirdComma).toInt();
      target_detected = data.substring(thirdComma + 1).toInt();
      Serial.println(target_detected);
      Serial.println(motor_x_angle);
      Serial.println(motor_y_angle);
      Serial.println(distance);
  }
    
  }
}
// Step conversion
long degreesToSteps(float degrees){
  return (long)(degrees * STEPS_PER_REV / 360.0);
}

// One step in correct direction
void stepMotorOnce(int dirPin, int stepPin, bool dir){
  digitalWrite(dirPin, dir ? HIGH : LOW);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(1000);
}

void setup() {
  Serial.begin(115200);
  pinMode(dirPinx, OUTPUT);
  pinMode(stepPinx, OUTPUT);
  pinMode(dirPiny, OUTPUT);
  pinMode(stepPiny, OUTPUT);
}

void loop() {
  serial_decode();
  // Compute incremental movement
  float target_x = motor_x_angle;
  float target_y = motor_y_angle;

  float error_x = target_x - current_x_angle;
  float error_y = target_y - current_y_angle;

  const float step_increment = 0.5; // degrees per loop (adjust speed)

  // Move X motor
  if (abs(error_x) >= step_increment) {
    bool dir = (error_x > 0);
    stepMotorOnce(dirPinx, stepPinx, dir);
    current_x_angle += dir ? step_increment : -step_increment; 
    //above is a condensed if else statement, reads if dir == true, current angle += step_increment, else current_angle += -step_increment
  }

  // Move Y motor
  if (abs(error_y) >= step_increment) {
    bool dir = (error_y > 0);
    stepMotorOnce(dirPiny, stepPiny, dir);
    current_y_angle += dir ? step_increment : -step_increment;
  }

  // If no target, slowly scan X
  if (!target_detected) {
    stepMotorOnce(dirPinx, stepPinx, true); // scan in positive X
    current_x_angle += step_increment;
  }

  // Short delay to control loop speed
  delay(1);  // non-blocking, still allows OpenCV updates
}

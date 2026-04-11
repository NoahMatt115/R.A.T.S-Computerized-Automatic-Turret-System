// Motor pins
const int dirPinx = 2;
const int stepPinx = 4;
const int dirPiny = 3;
const int stepPiny = 5;

// Stepper parameters
const int MOTOR_STEPS = 200;  // 1.8° motor
const int MICROSTEPS = 16;
const int STEPS_PER_REV = MOTOR_STEPS * MICROSTEPS;

// OpenCV updates these via serial
volatile float motor_x_angle = 0;
volatile float motor_y_angle = 0;
volatile float distance = 0;
volatile bool target_detected = false;

// Track current physical motor positions
float current_x_angle = 0;
float current_y_angle = 0;

String data = "";

const float step_increment = 0.5;  // degrees per step increment

// Convert degrees to number of steps
long degreesToSteps(float degrees) {
  return (long)(degrees * STEPS_PER_REV / 360.0);
}

// Execute a given number of steps in a direction
void stepMotor(int dirPin, int stepPin, bool dir, long steps) {
  digitalWrite(dirPin, dir ? HIGH : LOW);
  for (long i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
}

void serial_decode() {
  if (Serial.available()) {
    data = Serial.readStringUntil('\n');
    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);
    int thirdComma = data.indexOf(',', secondComma + 1);

    if (firstComma > 0 &&
        secondComma > firstComma &&
        thirdComma > secondComma) {

      motor_x_angle = data.substring(0, firstComma).toFloat();
      motor_y_angle = data.substring(firstComma + 1, secondComma).toFloat();
      distance = data.substring(secondComma + 1, thirdComma).toFloat();
      target_detected = data.substring(thirdComma + 1).toInt();
    }
  }
}

// Scan bounds
const float SCAN_MIN = -90.0;
const float SCAN_MAX = 90.0;
bool scan_direction = true;  // true = positive X

void setup() {
  Serial.begin(115200);
  pinMode(dirPinx, OUTPUT);
  pinMode(stepPinx, OUTPUT);
  pinMode(dirPiny, OUTPUT);
  pinMode(stepPiny, OUTPUT);
}

void loop() {
  serial_decode();

  if (target_detected) {
    // --- Tracking mode ---
    float error_x = motor_x_angle - current_x_angle;
    float error_y = motor_y_angle - current_y_angle;

    // Move X toward target
    if (abs(error_x) >= step_increment) {
      bool dir = (error_x > 0);
      long steps = degreesToSteps(step_increment);
      stepMotor(dirPinx, stepPinx, dir, steps);
      current_x_angle += dir ? step_increment : -step_increment;
    }

    // Move Y toward target
    if (abs(error_y) >= step_increment) {
      bool dir = (error_y > 0);
      long steps = degreesToSteps(step_increment);
      stepMotor(dirPiny, stepPiny, dir, steps);
      current_y_angle += dir ? step_increment : -step_increment;
    }

  } else {
    // --- Scan mode ---
    // Reverse direction at bounds
    if (current_x_angle >= SCAN_MAX) {
      scan_direction = false;
    } else if (current_x_angle <= SCAN_MIN) {
      scan_direction = true;
    }

    long steps = degreesToSteps(step_increment);
    stepMotor(dirPinx, stepPinx, scan_direction, steps);
    current_x_angle += scan_direction ? step_increment : -step_increment;
  }

  delay(1);
}

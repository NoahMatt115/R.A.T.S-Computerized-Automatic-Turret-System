#define XMOTOR 0
#define YMOTOR 1
//This code is supposed to co-exist with an opencv program
//The opencv program returns motorx, motory, which are angles from the center in degrees
// it also returns if a target exists in a boolean variable
int target_detected = false; //checks if a target is detected
//x and y values in degrees from the center
int motor_x_angle = 0;
int motor_y_angle = 0; 

const int dirPinx = 2; //the pin of the motor that decides direction
const int dirPiny = 3;
const int stepPinx = 4;
const int stepPiny = 5;
const int MOTOR_STEPS = 200;     // 1.8° motor
const int MICROSTEPS = 16;
//Calculates how many steps per revolution      
const int STEPS_PER_REV = MOTOR_STEPS * MICROSTEPS;
long degreesToSteps(float degrees){
  return(long)(degrees * STEPS_PER_REV / 360.0);
}

void setup() {
pinMode(dirPinx, OUTPUT);
pinMode(stepPinx, OUTPUT);
pinMode(dirPiny, OUTPUT);
pinMode(stepPiny, OUTPUT);
}

//functions to rotate the motors a certain amount of degrees
void motor_rotate_pos(int motor, float degrees){
  if(motor == YMOTOR){
    digitalWrite(dirPiny, HIGH);
    long steps = degreesToSteps(degrees);
    for(long i = 0; i < steps; i++){
      digitalWrite(stepPiny, HIGH);
      delayMicroseconds(1000);                  // Adjust for speed
      digitalWrite(stepPiny, LOW);
      delayMicroseconds(1000);
      }
  }
  else if(motor == XMOTOR){
    digitalWrite(dirPinx, HIGH);
    long steps = degreesToSteps(degrees);
    for(long i = 0; i < steps; i++){
      digitalWrite(stepPinx, HIGH);
      delayMicroseconds(1000);                  // Adjust for speed
      digitalWrite(stepPinx, LOW);
      delayMicroseconds(1000);
      }
}
}

void motor_rotate_neg(int motor, float degrees){
 if(motor == YMOTOR){
    digitalWrite(dirPiny, LOW);
    long steps = degreesToSteps(degrees);
    for(long i = 0; i < steps; i++){
      digitalWrite(stepPiny, HIGH);
      delayMicroseconds(1000);                  // Adjust for speed
      digitalWrite(stepPiny, LOW);
      delayMicroseconds(1000);
      }
  }
  else if(motor == XMOTOR){
    digitalWrite(dirPinx, LOW);
    long steps = degreesToSteps(degrees);
    for(long i = 0; i < steps; i++){
      digitalWrite(stepPinx, HIGH);
      delayMicroseconds(1000);                  // Adjust for speed
      digitalWrite(stepPinx, LOW);
      delayMicroseconds(1000);
      }
}

}
void loop() {
  //loops to see if a target is detected or not
  if (target_detected == true){
    if(motor_x_angle > 0){
      motor_rotate_pos(XMOTOR, motor_x_angle);
      }
    else if(motor_x_angle < 0){
      motor_rotate_neg(XMOTOR, abs(motor_x_angle));
      }
    if(motor_y_angle > 0){
      motor_rotate_pos(YMOTOR, motor_y_angle);
    }
    else if(motor_y_angle < 0){
      motor_rotate_neg(YMOTOR, abs(motor_y_angle));

  }
  }//if a target isnt detected, it  keeps the motor in a x rotation loop
  else if(target_detected == false){
      digitalWrite(dirPinx, HIGH);
    while(!target_detected){
      digitalWrite(stepPinx, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPinx, LOW);
      delayMicroseconds(1000);
      //Opencv code needs to update target_detected
      }
      
  }
  
}

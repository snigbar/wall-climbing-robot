#define REMOTEXY_MODE__HARDSERIAL
#include <RemoteXY.h>
// RemoteXY connection settings
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 9600
// RemoteXY configurate
#pragma pack(push, 1)

uint8_t RemoteXY_CONF[] =   // 104 bytes
  { 255,5,0,0,0,97,0,16,219,0,5,36,66,28,33,33,2,26,31,4,
  128,3,14,52,5,2,26,2,1,64,14,30,7,2,74,31,31,99,108,101,
  97,110,101,114,32,111,102,102,0,99,108,101,97,110,101,114,32,111,110,0,
  2,1,5,43,36,9,2,26,31,31,79,78,0,79,70,70,0,129,0,30,
  4,39,4,31,87,97,108,108,32,67,108,105,109,98,105,110,103,32,82,111,
  98,111,116,0 };

// this structure defines all the variables and events of your control interface
struct {
// input variables
 int8_t joystick_1_x; // =-100..100 x-coordinate joystick position
int8_t joystick_1_y; // =-100..100 y-coordinate joystick position
 int8_t slider_1; // =0..100 slider position
//  cleaning switch
 uint8_t switch_1; // =1 if state is ON, else =0
//  main power switch
 uint8_t switch_2; // =1 if switch ON and =0 if OFF
 // other variable
 uint8_t connect_flag; // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)

#include <RemoteXY.h>
#include <Servo.h>
// bldc motor
Servo bldcMotor;
// servo motor
Servo myservo;


// Motor control pins
#define right_wheel_A 6
#define right_wheel_B 7

#define left_wheel_A 8
#define left_wheel_B 9


#define PIN_SWITCH_1 13  // Define PIN_SWITCH_1 here

uint8_t RightMotor[2] = {right_wheel_A, right_wheel_B};
uint8_t LeftMotor[2] = {left_wheel_A, left_wheel_B};


void setup() {
  RemoteXY_Init();
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  myservo.attach(3);
  myservo.write(90);
  delay(1000);
  myservo.write(180);
  delay(2000);
  myservo.write(90);
  delay(1000);
  myservo.write(0);

  pinMode(PIN_SWITCH_1, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);

  // Initialize motor control pins
  pinMode(right_wheel_A, OUTPUT);
  pinMode(right_wheel_B, OUTPUT);
  pinMode(left_wheel_A, OUTPUT);
  pinMode(left_wheel_B, OUTPUT);
}

void loop() {
  RemoteXY_Handler();

  if (RemoteXY.switch_2 == 1) {
    // Manage the right and left motors

  controlNavigation (RightMotor, RemoteXY.joystick_1_y - RemoteXY.joystick_1_x);
  controlNavigation (LeftMotor, RemoteXY.joystick_1_y + RemoteXY.joystick_1_x);
    // bldc motor
    bldcMotor.attach(11, 1000, 2000);
    bldcMotor.write(RemoteXY.slider_1 * 1.7);
    
     if (RemoteXY.switch_1 == 1) {
      myservo.write(0);
      digitalWrite(4, LOW);
    } else {
      moveServo();
      digitalWrite(4, HIGH);
    }
  }


  }


void controlNavigation(uint8_t* motor, int v) {
  if (v > 100) v = 100;
  if (v < -100) v = -100;
  
  if (v > 0) {
    digitalWrite(motor[0], HIGH);
    digitalWrite(motor[1], LOW);
  } else if (v < 0) {
    digitalWrite(motor[0], LOW);
    digitalWrite(motor[1], HIGH);
  } else {
    digitalWrite(motor[0], LOW);
    digitalWrite(motor[1], LOW);
  }
}


void moveServo() {
  static unsigned long servoLastMoveTime = 0;
  static int servoState = 0;

  unsigned long currentTime = millis();

  if (currentTime - servoLastMoveTime >= 1000) {
    if (servoState == 0) {
      myservo.write(180);
      servoState = 1;
    } else {
      myservo.write(0);
      servoState = 0;
    }
    servoLastMoveTime = currentTime;
  }
}

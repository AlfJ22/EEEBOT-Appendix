#include <Wire.h>        // include Wire library
#include <Encoder.h>    //include Encoder library
#include <Servo.h>      //include Servo library

Encoder rearRight(3,12);
Encoder rearLeft(2,11);
#define enA 5 
#define enB 6 
#define INa A0  
#define INb A1  
#define INc A2  
#define INd A3 
byte speedSetting = 0;
#define servoPin 4
Servo myservo;        // create servo object to control a servo
float servoLeft = 8, servoRight = 110, servoCentre = 59;

String message;

void setup() {
  Wire.begin(0x08);             // join i2c bus with address 8
  Wire.onReceive(receiveEvent); // create a receive event
  Serial.begin(115200);           // start serial to visualise data 
                               // received via serial monitor in the IDE
  //configure the motor control pins as outputs
  pinMode(INa, OUTPUT);
  pinMode(INb, OUTPUT);
  pinMode(INc, OUTPUT);
  pinMode(INd, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT); 

  motors(120, 153);   //discrepencies in the motors mean one has to be set to a lower speed setting
  //motors have also been set 2/3 of the maximum speed setting to help lessen the effect any potential lag in the system 
  myservo.attach(servoPin);
}

void loop() {
}

void receiveEvent() {
  message = "";
  while (Wire.available()) {  // loop whilst bus is busy
    char c = Wire.read();     // receive data byte by byte
    message += c;             // form complete string
  }

  if (message == "forward"){  //if the message recieved is "forward"
    goForwards();             //call the "goForwards" Function
  }
  else if (message == "stop"){  //if the message recieved is "stop"
    stopMoving();               //call the "stopMoving" function
  }
  else if (message == "left"){  //if the message recieved is "left"
    myservo.write(servoLeft);   //tells the servo to steer left
  }
  else if (message == "right"){  //if the message recieved is "right"
    myservo.write(servoRight);   //tells the servo to steer right
  }
  else if (message == "centre"){  //if the message recieved is "centre"
    myservo.write(servoCentre); //centres the servo    
  }
  else if (message == "backward"){  //if the message recieved is "centre"
    goBackwards();        //the "goBackwards function is called"    
  }
  
} 


void motors(int leftSpeed, int rightSpeed) {
  //set individual motor speed
  //direction is set separately

  analogWrite(enA, leftSpeed);
  analogWrite(enB, rightSpeed);
}

void goForwards() {
  digitalWrite(INa, HIGH);
  digitalWrite(INb, LOW);
  digitalWrite(INc, LOW);
  digitalWrite(INd, HIGH);
}

void goBackwards() {
  digitalWrite(INa, LOW);
  digitalWrite(INb, HIGH);
  digitalWrite(INc, HIGH);
  digitalWrite(INd, LOW);
}

void stopMoving() {
  digitalWrite(INa, LOW);
  digitalWrite(INb, LOW);
  digitalWrite(INc, LOW);
  digitalWrite(INd, LOW);
}


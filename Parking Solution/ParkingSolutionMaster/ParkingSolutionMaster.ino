#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

const int trigPin = 5;
const int echoPin = 18;
#define SOUND_SPEED 0.034

void setup() {
  Wire.begin();             // join i2c bus
  Serial.begin(115200);     //begins serial communication
  mpu6050.begin();        //initialises the MPU6050
  mpu6050.calcGyroOffsets(true);    //automatic calibration of the gyroscope   
  pinMode(trigPin, OUTPUT); // sets trigger (5) as output from esp to sensor
  pinMode(echoPin, INPUT); // sets echo pin (18) as input to esp from the sensor
  parking();
}

void loop() {
  
}

void parking() {
  float angleZ, angleZCurrent = 0;
  Wire.beginTransmission(0x08);
  Wire.write("centre");     //transmits "centre" to slave, making the wheels centred before use
  Wire.endTransmission();
  Wire.beginTransmission(0x08); //begins transmission to device 8
  Wire.write("forward");    //transmits "forward" to slave
  Wire.endTransmission();       //ends transmission
  delay(1000);                //delay of 1s
  Wire.beginTransmission(0x08);
  Wire.write("stop");     //transmits "stop" to slave
  Wire.endTransmission();
  delay(1000);            //delay of 1s

  Wire.beginTransmission(0x08);
  Wire.write("left");     //transmits "leftTurn" to slave
  Wire.endTransmission();
  Wire.beginTransmission(0x08); 
  Wire.write("forward");    //transmits "forward" to slave
  Wire.endTransmission();
  mpu6050.update();  
  angleZ = mpu6050.getAngleZ();
  while (angleZCurrent < (angleZ + 172)){    //anticlockwise rotation causes a positive on the gyroscope
    mpu6050.update();  
    angleZCurrent = mpu6050.getAngleZ();   //updates the angle until it reaches 180 degrees
  }
  Wire.beginTransmission(0x08);
  Wire.write("stop");       //transmits the stop command to slave
  Wire.endTransmission();
  Wire.beginTransmission(0x08); 
  Wire.write("centre");    //transmits "centre" to slave
  Wire.endTransmission();
  delay(1000);

  Wire.beginTransmission(0x08); 
  Wire.write("backward");    //transmits "backwards" to slave
  Wire.endTransmission();
  objectDetection();    //will loop until an object is detected within 10 cm
  Wire.beginTransmission(0x08); 
  Wire.write("stop");    //transmits "stop" to slave 
  Wire.endTransmission();
  delay(1000);
  
  Wire.beginTransmission(0x08); 
  Wire.write("right");    //transmits "right" to slave 
  Wire.endTransmission();
  Wire.beginTransmission(0x08); 
  Wire.write("forward");    //transmits "forward" to slave 
  Wire.endTransmission();
  angleZ = mpu6050.getAngleZ();
  while (angleZCurrent > (angleZ - 82)){    //clock rotation causes a negative on the gyroscope
    mpu6050.update();  
    angleZCurrent = mpu6050.getAngleZ();   //updates the angle until it reaches 180 degrees
    Serial.print("\n");
    Serial.print(angleZ);
  }
  Wire.beginTransmission(0x08); 
  Wire.write("stop");    //transmits "stop" to slave 
  Wire.endTransmission();
  Wire.beginTransmission(0x08); 
  Wire.write("centre");    //transmits "centre" to slave 
  Wire.endTransmission();
  delay(1000);

  Wire.beginTransmission(0x08); 
  Wire.write("backward");    //transmits "backward" to slave 
  Wire.endTransmission();
  objectDetection(); //to detect an object within 10cm
  Wire.beginTransmission(0x08); 
  Wire.write("stop");    //transmits "stop" to slave 
  Wire.endTransmission();

}

//A function that will keep looping as long as there is not an obh=ject detected within 10cm of the HC-SR04
void objectDetection(){
  long duration;        
  float distance = 14;      //defining initial distance to above the maximum distance value so the while loop starts
  while (distance >= 13){
    delay(20);   
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distance
    distance = duration * SOUND_SPEED/2;

  }
  
}

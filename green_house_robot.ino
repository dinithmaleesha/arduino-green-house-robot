// Green House robot - Final Code

#include <Servo.h>                // For Servo Motor
#include <DHT.h>                  // For Humidity Sensor
#include <Wire.h>                 // For I2C communication
#include <LiquidCrystal_I2C.h>    // For LCD display

//Motor Driver
int enA = 3;
int in1 = 4;
int in2 = 5;
int enB = 6;
int in3 = 7;
int in4 = 8;

// Ultrasoinc Sensor
int echoPin = 9;
int trigPin = 10;

//Digital Input for IR
// int IR_L = 10;
// int IR_R = 11;
// int IR_M = 9;

//Analog input for IR
int IR_LL = A0;
int IR_L = A1;
int IR_M = A2;
int IR_R = A3;
int IR_RR = A4;

// Variable for IR sensors
int left_IR;
int right_IR;
int m_IR;
int LL_IR;
int RR_IR;

// Soil Moisture sensors
int soilMoistureLeft = A5;
int soilMoistureRight = A6;

// Servo Motor
Servo servoLeft;
Servo servoRight;

int servoLeftPin = 11;
int servoRightPin = 12;

// motor Speed
int forwardSpeed = 50;
int backwardSpeed = 60;
int leftSpeed = 0;
int rightSpeed = 0;

// Array for Condition() function
unsigned char s[4];
int data;

// Calculate Distance
long distance;
long duration;

// Water pump
int leftPump = 22;
int rightPump = 23;

// Humidity Sensor
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LCD display
LiquidCrystal_I2C lcd(0x25, 16, 2);

// Moisture value
int soilMoistureValue;
int moisturePercentage;

void setup() {

  //Ultasonic pin
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  //Motor Driver pin
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // IR sensor pin
  pinMode(IR_LL, INPUT);
  pinMode(IR_L, INPUT);
  pinMode(IR_M, INPUT);
  pinMode(IR_R, INPUT);
  pinMode(IR_RR, INPUT);

  //Soil Moisture sensor pin
  pinMode(soilMoistureLeft, INPUT);
  pinMode(soilMoistureRight, INPUT);

  //Servo pin
  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);

  //Water pump pin
  pinMode(leftPump, OUTPUT);
  pinMode(rightPump, OUTPUT);

  //Humidty Sensor
  dht.begin();

  //LCD display
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);

  //Servo position
  servoLeft.write(150);
  servoRight.write(0);

  Serial.begin(9600);
}

void loop() {
  readSensor();
  condition();
  checkTemperature();
}

void forward(){
  Serial.println("Forward 1");
  analogWrite(enA, 70);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(enB, 70);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void moveForward() {
  Serial.println("Forward");
  int startSpeed = 125;
  int endSpeed = 40;
  int speedStep = -1;

  for (int speed = startSpeed; speed >= endSpeed; speed += speedStep) {
    analogWrite(enA, speed);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    analogWrite(enB, speed);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    //delay(100);  
  }

  // final speed
  analogWrite(enA, endSpeed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(enB, endSpeed);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}


void turnLeft(){
    Serial.println("Turn Left");
  analogWrite(enA, 90);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  analogWrite(enB, 90);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}


void turnRight() {
  Serial.println("Turn Right");  
  analogWrite(enA, 90);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(enB, 90);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void backward(){
  Serial.println("Reverse");
  analogWrite(enA, 70);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  analogWrite(enB, 70);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stop(){
  Serial.println("Stop");
  analogWrite(enA, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  analogWrite(enB, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}


// Read sensor value
void readSensor(){
  LL_IR = analogRead(IR_LL);
  left_IR = analogRead(IR_L);
  right_IR = analogRead(IR_R);
  RR_IR = analogRead(IR_RR);


  if(LL_IR > 300){
    s[0] = 1;
  }
  else{
    s[0] = 0;
  }
  if(left_IR > 300){
    s[1] = 1;
  }
  else{
    s[1] = 0;
  }
  if(right_IR > 300){
    s[2] = 1;
  }
  else{
    s[2] = 0;
  }
  if(RR_IR > 300){
    s[3] = 1;
  }
  else{
    s[3] = 0;
  }

  data = (s[0]*8) + (s[1]*4) + (s[2]*2) + (s[3]*1);

}

void readDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration= pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Serial.print(distance);
  // Serial.println("");
}

void waterLeft(){
  Serial.println("Left Water");
  int leftStart = 150;
  int leftEnd = 0;
  
  // Rotate left servo
  for (int i = leftStart; i >= leftEnd; i--) {
    servoLeft.write(i);
  }

  //check soil moisture value
  for(int c = 0; c < 5; c++){
    soilMoistureValue = analogRead(soilMoistureLeft);
    delay(200);
  }

  soilMoistureValue /= 5;

  moisturePercentage = map(soilMoistureValue, 0, 1023, 0, 100);

  Serial.print("Soil Moisture Level: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  // Rotate servo back to 0
  for (int j = leftEnd; j <= leftStart; j++) {
    servoLeft.write(j);
    delay(30);
  }

  if(moisturePercentage < 80){
    digitalWrite(leftPump, HIGH);
    delay(2000);
    digitalWrite(leftPump, LOW);
  }
  
  delay(1000);
  // end watering
  forward();
}

void waterRight() {
  Serial.println("Right Water");
  int rightStart = 0;
  int rightEnd = 150;
  
  // Rotate right servo
  for (int i = 0; i <= rightEnd; i++) {
    servoRight.write(i);
    // delay(30);
  }

  //check soil moisture value
  for(int c = 0; c < 5; c++){
    soilMoistureValue = analogRead(soilMoistureRight);
    delay(200);
  }
  soilMoistureValue /= 5;

  moisturePercentage = map(soilMoistureValue, 0, 1023, 0, 100);

  Serial.print("Soil Moisture Level: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  // Rotate servo back to 0
  for (int j = rightEnd; j >= rightStart; j--) {
    servoRight.write(j);
    delay(10);
  }

  if(moisturePercentage < 80){
    digitalWrite(rightPump, HIGH);
    delay(2000);
    digitalWrite(rightPump, LOW);
  }
  
  delay(1000);
  forward();
  // End watering
}

void condition(){
  readDistance();
  if(distance > 30){
    if(data == 0){
      stop();
    }
    else if(data == 1){
      stop();
    }
    else if(data == 2){
      stop();
    }
    else if(data == 3){
      stop();
      delay(500);
      waterLeft();
    }
    else if(data == 4){
      stop();
    }
    else if(data == 5){
      stop();
    }
    else if(data == 6){
      forward();
    }
    else if(data == 7){
      forward();
    }
    else if(data == 8){
      stop();
    }
    else if(data == 9){
      stop();
    }
    else if(data == 10){
      stop();
    }
    else if(data == 11){
      turnLeft();
      while(data == 11){
        readSensor();
        turnLeft();
      }
    }
    else if(data == 12){
      stop();
      delay(500);
      waterRight();
    }
    else if(data == 13){
      turnRight();
      while(data == 13){
        readSensor();
        turnRight();
      }
    }
    else if(data == 14){
      forward();
    }
    else if(data == 15){
      forward();
    }    
    else{
      stop();
    }
  }
  else{
    stop();
  }
}

void checkTemperature(){
  // Read humidity value
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();

  if (!isnan(humidity)) {
    // Display the temperature
    lcd.setCursor(0, 0);
    lcd.print("Temp : ");
    lcd.print(temp, 1);  // one decimal place
    lcd.print(" °C");

    // Display the humidity value
    lcd.setCursor(0, 1);
    lcd.print("Humi : ");
    lcd.print(humidity, 1);  // one decimal place
    lcd.print(" %");

    if(dht.readTemperature() > 27.5){
      // turn on fan
    }
    else{
      // turn off fan
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Temp : Error");
    lcd.setCursor(0, 1);
    lcd.print("Humi : Error");
  }
}

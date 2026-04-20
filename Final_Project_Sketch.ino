#include <Servo.h>

Servo myServo;

const int servoPin = 3;
const int trigPin = 11;
const int echoPin = 12;

const int redPin = 5;
const int greenPin = 6;
const int bluePin = 7;

bool commonAnode = false;

int angle = 0;
int step = 1;
int maxDistance = 75;

unsigned long previousServoMillis = 0;
unsigned long servoInterval = 30;

long duration;
int distance;

void setup() {
  myServo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

unsigned long previousSerialMillis = 0;
unsigned long serialInterval = 50; // 20Hz

void loop() {
  unsigned long currentMillis = millis();

  // Servo movement
  if (millis() - previousServoMillis >= servoInterval) {
    previousServoMillis = millis();
    
    angle += step;

    if (angle >= 180 || angle <= 0) {
      step = -step;
    }

    digitalWrite(2, HIGH);

    myServo.write(angle);

    distance = getDistance();

    Serial.print(angle);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);

  int dist = duration * 0.034 / 2;

  int lowThreshold  = maxDistance * 0.33;
  int midThreshold  = maxDistance * 0.67;
  
  
  if(dist <= lowThreshold) {
    setColor(255, 0, 0); //Red
  }
  else if (dist <= midThreshold) {
    setColor(0, 255, 0); //Green
  }
  else if(dist <= maxDistance) {
    setColor(0, 0, 255); //Blue
  }
  else {
    setColor(255, 255, 255); //White
  }
  
  if (dist > 0 || dist <= maxDistance) {
    return dist;
  }
  else {
    return -1;
  }
}

void setColor(int r, int g, int b) {
  if(commonAnode) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
  }
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}
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
int smoothedDistance = 0;
int currentZone = -1;

unsigned long previousServoMillis = 0;
unsigned long servoInterval = 50;
unsigned long previousColorMillis = 0;
unsigned long colorInterval = 10;

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

void loop() {
  unsigned long currentMillis = millis();

  if (millis() - previousServoMillis >= servoInterval) {
    previousServoMillis = millis();
    
    angle += step;

    if (angle >= 175 || angle <= 5) {
      step = -step;
      angle = constrain(angle, 5, 175);
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

  smoothedDistance = (smoothedDistance * 0.8) + (dist * 0.2);

  unsigned long currentMillis = millis();
  if (currentMillis - previousColorMillis >= colorInterval) {
    previousColorMillis = currentMillis;
    updateColor(smoothedDistance);
  }

  return (dist > 5 && dist <= maxDistance) ? dist : -1;
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

void updateColor(int dist) {
  int low  = maxDistance * 0.33;
  int mid  = maxDistance * 0.67;
  
  int newZone;
  if (dist <= low) { newZone = 0; } // red
  else if (dist <= mid) { newZone = 1; } // green
  else if (dist <= maxDistance) { newZone = 2; } // blue
  else { newZone = 3; } // white

  if (newZone == currentZone) return;

  bool clearOfLow  = dist < (low - 3)  || dist > (low + 3);
  bool clearOfMid  = dist < (mid - 3)  || dist > (mid + 3);

  if ((newZone == 0 || newZone == 1) && !clearOfLow) return;
  if ((newZone == 1 || newZone == 2) && !clearOfMid) return;

  currentZone = newZone;
  switch (currentZone) {
    case 0: setColor(255, 0,   0);   break; // red
    case 1: setColor(0,   255, 0);   break; // green
    case 2: setColor(0,   0,   255); break; // blue
    case 3: setColor(255, 255, 255); break; // white
  }
}

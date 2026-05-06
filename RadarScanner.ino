#include <Servo.h>
Servo myServo;

const int servoPin = 3;
const int trigPin = 11;
const int echoPin = 12;

const int redPin = 5;
const int greenPin = 6;
const int bluePin = 7;

const int minAngle = 1;
const int maxAngle = 179;

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

const int max = 6;
int values[max] = {0};
int index = 0;
int count = 0;

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

    if (angle >= maxAngle || angle <= minAngle) {
      step = -step;
      angle = constrain(angle, minAngle, maxAngle);
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

  if (duration == 0) {
    setColor(255, 255, 255); 
    return -1;
  }

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

  values[index] = dist;
  index = (index + 1) % max;

  if (count < max) { count++; }

  double avg = 0;
  for (int i = 0; i < count; i++) {
    avg += values[i];
  }
  avg /= count;

  if (avg <= low) { setColor(255, 0,   0); } // red
  else if (avg <= mid) { setColor(0, 255,   0);; } // green
  else if (avg <= maxDistance) { setColor(0, 0, 255); } // blue
  else { setColor(255, 255, 255); }
}

#include <Servo.h>

Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripServo;

bool busy = false;

// Track current positions
int basePos = 90;
int shoulderPos = 100;
int elbowPos = 10;
int gripPos = 30;

void setup() {
  Serial.begin(115200);

  baseServo.attach(D5);
  shoulderServo.attach(D6);
  elbowServo.attach(D7);
  gripServo.attach(D1);

  Serial.println("Type r, b, or g");

  goHome();
}

void loop() {
  if (Serial.available() > 0 && !busy) {
    char c = Serial.read();

    if (c == '\n' || c == '\r' || c == ' ') return;

    busy = true;

    if (c == 'r') moveColour(120, false);
    else if (c == 'b') moveColour(150, false);
    else if (c == 'g') moveColour(150, true);

    busy = false;
    Serial.println("READY");
  }
}

// -------- SMOOTH MOVE FUNCTION --------

void smoothMove(Servo &servo, int &currentPos, int targetPos, int speedDelay) {

  if (currentPos < targetPos) {
    for (int pos = currentPos; pos <= targetPos; pos++) {
      servo.write(pos);
      delay(speedDelay);
    }
  } else {
    for (int pos = currentPos; pos >= targetPos; pos--) {
      servo.write(pos);
      delay(speedDelay);
    }
  }

  currentPos = targetPos;
}

// -------- HOME --------

void goHome() {
  smoothMove(baseServo, basePos, 90, 10);
  smoothMove(shoulderServo, shoulderPos, 100, 10);
  smoothMove(elbowServo, elbowPos, 10, 10);
  smoothMove(gripServo, gripPos, 30, 10);
}

// -------- MAIN MOVEMENT --------

void moveColour(int finalBaseAngle, bool elbowActive) {

  // PICK
  smoothMove(baseServo, basePos, 30, 10);
  smoothMove(shoulderServo, shoulderPos, 70, 12);

  delay(200);

  smoothMove(gripServo, gripPos, 70, 15);

  delay(300);

  smoothMove(shoulderServo, shoulderPos, 100, 12);

  // CENTER
  smoothMove(baseServo, basePos, 90, 10);

  // FINAL BOX
  smoothMove(baseServo, basePos, finalBaseAngle, 10);

  // PLACE
  if (elbowActive) {
    smoothMove(elbowServo, elbowPos, 60, 10);
  }

  smoothMove(shoulderServo, shoulderPos, 70, 15);

  delay(200);

  smoothMove(gripServo, gripPos, 30, 20);  // slower open = stable drop

  delay(300);

  smoothMove(shoulderServo, shoulderPos, 100, 12);

  if (elbowActive) {
    smoothMove(elbowServo, elbowPos, 10, 10);
  }

  smoothMove(baseServo, basePos, 90, 10);

  goHome();
}

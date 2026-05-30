#define BLYNK_TEMPLATE_ID "TMPL3xuehkRRn"
#define BLYNK_TEMPLATE_NAME "sortingbotttt"
#define BLYNK_AUTH_TOKEN "99TbFF8V5U-V4llBr3dspYJ9iVq09bGs"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <ctype.h>

// WiFi
char ssid[] = "test";
char pass[] = "12345678";

// ESP8266 (NodeMCU) pin mapping
// D7 = GPIO13, D5 = GPIO14, D6 = GPIO12, D4 = GPIO2
#define BASE_SERVO_PIN     D7
#define SHOULDER_SERVO_PIN D5
#define ELBOW_SERVO_PIN    D6
#define GRIP_SERVO_PIN     D4

Servo baseServo;
Servo shoulderServo;
Servo elbowServo;
Servo gripServo;

bool busy = false;

// Counters for Blynk
int redCount = 0;
int blueCount = 0;
int greenCount = 0;

void goHome();
void moveColour(int finalBaseAngle, bool elbowActive);

void setup() {
  Serial.begin(115200);  // MUST match Python baud

  baseServo.attach(BASE_SERVO_PIN);
  shoulderServo.attach(SHOULDER_SERVO_PIN);
  elbowServo.attach(ELBOW_SERVO_PIN);
  gripServo.attach(GRIP_SERVO_PIN);

  Serial.println("Commands: r=red, b=blue, g=green, h=home");
  goHome();

  // WiFi connect
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pass);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Blynk.virtualWrite(V7, 1);  // WiFi status
  } else {
    Serial.println("\nWiFi Failed");
    Blynk.virtualWrite(V7, 0);  // WiFi status
  }

  // Blynk connect
  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);
  if (Blynk.connect(10000)) {
    Serial.println("Blynk Connected");
    Blynk.virtualWrite(V8, 1);  // Blynk status
    Blynk.virtualWrite(V0, redCount);
    Blynk.virtualWrite(V1, blueCount);
    Blynk.virtualWrite(V2, greenCount);
  } else {
    Serial.println("Blynk Failed");
    Blynk.virtualWrite(V8, 0);  // Blynk status
  }
}

void loop() {
  Blynk.run();

  if (Serial.available() > 0 && !busy) {
    char c = (char)Serial.read();
    c = tolower(c);

    if (c == '\n' || c == '\r' || c == ' ') return;

    Serial.print("Received: ");
    Serial.println(c);

    busy = true;
    Blynk.virtualWrite(V6, 1);  // arm moving

    if (c == 'r') {
      Serial.println("RED command received");
      moveColour(120, false);
      redCount++;
      Blynk.virtualWrite(V0, redCount);
    } else if (c == 'b') {
      Serial.println("BLUE command received");
      moveColour(150, false);
      blueCount++;
      Blynk.virtualWrite(V1, blueCount);
    } else if (c == 'g') {
      Serial.println("GREEN command received");
      moveColour(150, true);
      greenCount++;
      Blynk.virtualWrite(V2, greenCount);
    } else if (c == 'h') {
      Serial.println("HOME command received");
      goHome();
    } else {
      Serial.println("Invalid command. Use r, b, g, or h.");
    }

    busy = false;
    Blynk.virtualWrite(V6, 0);  // arm idle
    Serial.println("READY for next command");
  }
}

// ---------------- FUNCTIONS ----------------

void goHome() {
  baseServo.write(90);
  delay(1000);

  shoulderServo.write(100);
  elbowServo.write(10);
  delay(1000);

  gripServo.write(30);
  delay(1000);
}

void moveColour(int finalBaseAngle, bool elbowActive) {
  baseServo.write(30);
  delay(1000);

  shoulderServo.write(60);
  delay(1000);

  gripServo.write(85);
  delay(1000);

  shoulderServo.write(100);
  delay(1000);

  baseServo.write(90);
  delay(1000);

  baseServo.write(finalBaseAngle);
  delay(1000);

  if (elbowActive) {
    Serial.println("ELBOW MOVING FOR PLACING");
    elbowServo.write(60);
    delay(500);
  }

  shoulderServo.write(60);
  delay(1000);

  gripServo.write(30);
  delay(1000);

  if (!elbowActive) {
    shoulderServo.write(100);
    delay(1000);

    baseServo.write(90);
    delay(1000);

    goHome();
    return;
  }

  shoulderServo.write(100);
  delay(1000);

  elbowServo.write(10);
  delay(500);

  baseServo.write(90);
  delay(1000);

  goHome();
}

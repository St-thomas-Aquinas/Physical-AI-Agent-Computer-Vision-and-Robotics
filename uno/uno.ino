#include <Servo.h>

Servo servo1;   // First servo (arm movement)
const int servo1Pin = 9;

// L293D motor control pins
const int motorPin1 = 10;   // Motor direction 1
const int motorPin2 = 11;   // Motor direction 2

// Starting positions
int restPos = 0;
int activePos = 90;

void setup() {
  Serial.begin(9600);

  // Servo setup
  servo1.attach(servo1Pin);
  delay(500);                // Allow time for servo power to stabilize
  servo1.write(restPos);     // Move to safe start position

  // Motor driver pins setup
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);

  Serial.println("✅ System ready! Type '1' in Serial Monitor to activate servo + motor sequence.");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == '1') {
      Serial.println("🔥 Command received: Activating servo and motor sequence!");

      // --- Step 1: Servo action ---
      servo1.write(activePos);
      delay(800);
      servo1.write(restPos);
      delay(400);

      // --- Step 2: Motor forward (pin 10 HIGH) ---
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      Serial.println("⚙️ Motor spinning forward...");
      delay(4000);   // 1 second forward
      digitalWrite(motorPin1, LOW);  // stop
      delay(300);

      // --- Step 3: Motor reverse (pin 11 HIGH) ---
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, HIGH);
      Serial.println("🔁 Motor spinning reverse...");
      delay(1500);   // 1 second reverse
      digitalWrite(motorPin2, LOW);  // stop

      Serial.println("✅ Sequence complete — servo and motor reset.");
    }
  }
}

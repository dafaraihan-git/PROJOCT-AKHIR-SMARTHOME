#define SENSOR_PIN 2
#define LED_PIN 13

bool ledState = false;
bool lastState = LOW;

unsigned long lastClapTime = 0;
const unsigned long debounceDelay = 100;

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
}

void loop() {
  bool currentState = digitalRead(SENSOR_PIN);

  // Deteksi perubahan LOW -> HIGH
  if (currentState == HIGH && lastState == LOW) {

    if (millis() - lastClapTime > debounceDelay) {
      ledState = !ledState;          // Toggle LED
      digitalWrite(LED_PIN, ledState);

      lastClapTime = millis();
    }
  }

  lastState = currentState;
}
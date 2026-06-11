#include <Servo.h>

// ======================
// PIN
// ======================
#define TRIG_PIN    5
#define ECHO_PIN    3
#define LED_LDR     4
#define LED_CLAP    13
#define SERVO_PIN   9
#define LDR_PIN     A0
#define SENSOR_PIN  2

// ======================
// SERVO
// ======================
Servo gerbang;

// ======================
// PENGATURAN
// ======================
const int batasJarak              = 10;
const int batasLDR                = 8;
const int delayTutup              = 650;
const unsigned long debounceDelay = 50;
const unsigned long intervalSensor = 60; // baca jarak setiap 60ms

// ======================
// VARIABEL GERBANG
// ======================
bool gerbangTerbuka                = false;
unsigned long waktuTerakhirDeteksi = 0;
unsigned long waktuSensorTerakhir  = 0;
float jarakTerakhir                = 0;

// ======================
// VARIABEL TEPUK TANGAN
// ======================
bool ledClapState   = false;
bool lastSoundState = LOW;
unsigned long lastClapTime = 0;

// ======================
// SETUP
// ======================
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN,   OUTPUT);
  pinMode(ECHO_PIN,   INPUT);
  pinMode(LED_LDR,    OUTPUT);
  pinMode(LED_CLAP,   OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

  gerbang.attach(SERVO_PIN);
  gerbang.write(90);

  digitalWrite(LED_LDR,  LOW);
  digitalWrite(LED_CLAP, LOW);

  Serial.println("=== SISTEM SIAP ===");
}

// ======================
// LOOP
// ======================
void loop() {

  // ======================
  // TEPUK TANGAN
  // (selalu dicek setiap iterasi, tanpa blocking)
  // ======================
  bool currentSoundState = digitalRead(SENSOR_PIN);

  if (currentSoundState == HIGH && lastSoundState == LOW) {
    if (millis() - lastClapTime > debounceDelay) {
      ledClapState = !ledClapState;
      digitalWrite(LED_CLAP, ledClapState);
      Serial.println(ledClapState ? ">> LED Clap ON" : ">> LED Clap OFF");
      lastClapTime = millis();
    }
  }
  lastSoundState = currentSoundState;

  // ======================
  // BACA HC-SR04
  // (hanya setiap 60ms, tidak blocking terus)
  // ======================
  if (millis() - waktuSensorTerakhir >= intervalSensor) {
    waktuSensorTerakhir = millis();

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long durasi = pulseIn(ECHO_PIN, HIGH, 15000); // timeout 15ms max
    jarakTerakhir = durasi * 0.034 / 2;

    Serial.print("Jarak : ");
    Serial.print(jarakTerakhir);
    Serial.println(" cm");

    // Orang mendekat → buka gerbang
    if (jarakTerakhir > 0 && jarakTerakhir < batasJarak) {
      if (!gerbangTerbuka) {
        Serial.println(">> Orang terdeteksi!");
        Serial.println(">> Gerbang terbuka.");
        gerbang.write(0);
        gerbangTerbuka = true;
      }
      waktuTerakhirDeteksi = millis();
    }

    // ======================
    // BACA LDR
    // ======================
    int nilaiLDR = analogRead(LDR_PIN);
    Serial.print("LDR : ");
    Serial.println(nilaiLDR);

    if (nilaiLDR < batasLDR) {
      digitalWrite(LED_LDR, HIGH);
    } else {
      digitalWrite(LED_LDR, LOW);
    }
  }

  // Orang menjauh → tutup gerbang
  // (dicek setiap iterasi, bukan cuma saat baca sensor)
  if (gerbangTerbuka &&
      millis() - waktuTerakhirDeteksi >= delayTutup) {
    Serial.println(">> Gerbang tertutup.");
    gerbang.write(90);
    gerbangTerbuka = false;
  }
}
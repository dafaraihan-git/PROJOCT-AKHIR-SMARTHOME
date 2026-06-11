#include <Servo.h>

// ======================
// PIN
// ======================
#define TRIG_PIN  5
#define ECHO_PIN  3
#define SERVO_PIN 9

// ======================
// SERVO
// ======================
Servo gerbang;

// ======================
// PENGATURAN
// ======================
const int batasJarak               = 10;  // cm
const int delayTutup               = 650; // ms setelah objek pergi
const unsigned long intervalSensor = 60;  // baca jarak setiap 60ms

// ======================
// VARIABEL
// ======================
bool gerbangTerbuka                = false;
unsigned long waktuTerakhirDeteksi = 0;
unsigned long waktuSensorTerakhir  = 0;
float jarakTerakhir                = 0;

// ======================
// SETUP
// ======================
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  gerbang.attach(SERVO_PIN);
  gerbang.write(90); // posisi awal tertutup

  Serial.println("=== SISTEM SIAP ===");
}

// ======================
// LOOP
// ======================
void loop() {

  // ======================
  // BACA HC-SR04
  // (setiap 60ms, timeout 15ms)
  // ======================
  if (millis() - waktuSensorTerakhir >= intervalSensor) {
    waktuSensorTerakhir = millis();

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long durasi = pulseIn(ECHO_PIN, HIGH, 15000);
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
  }

  // ======================
  // TUTUP GERBANG
  // (dicek setiap iterasi)
  // ======================
  if (gerbangTerbuka &&
      millis() - waktuTerakhirDeteksi >= delayTutup) {
    Serial.println(">> Gerbang tertutup.");
    gerbang.write(90);
    gerbangTerbuka = false;
  }
}
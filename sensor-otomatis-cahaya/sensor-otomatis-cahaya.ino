// ======================
// PIN
// ======================
#define LED_PIN 4
#define LDR_PIN A0

// ======================
// PENGATURAN
// ======================
const int batasLDR             = 8;   // sesuaikan kondisi LDR
const unsigned long intervalLDR = 200; // baca LDR setiap 200ms

// ======================
// VARIABEL
// ======================
unsigned long waktuLDRTerakhir = 0;

// ======================
// SETUP
// ======================
void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("=== SISTEM SIAP ===");
}

// ======================
// LOOP
// ======================
void loop() {

  // ======================
  // BACA LDR
  // (setiap 200ms)
  // ======================
  if (millis() - waktuLDRTerakhir >= intervalLDR) {
    waktuLDRTerakhir = millis();

    int nilaiLDR = analogRead(LDR_PIN);

    Serial.print("LDR : ");
    Serial.println(nilaiLDR);

    // Gelap → LED nyala
    if (nilaiLDR < batasLDR) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println(">> Gelap, LED nyala.");
    } else {
      digitalWrite(LED_PIN, LOW);
      Serial.println(">> Terang, LED mati.");
    }
  }
}

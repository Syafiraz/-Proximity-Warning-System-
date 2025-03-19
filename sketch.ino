#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define TRIG_PIN A1
#define ECHO_PIN A2
#define PIR_PIN A0
#define LED_DEKAT 13
#define LED_SEDANG 12
#define LED_JAUH 11

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(LED_DEKAT, OUTPUT);
    pinMode(LED_SEDANG, OUTPUT);
    pinMode(LED_JAUH, OUTPUT);

    Serial.begin(9600);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 gagal diinisialisasi");
        for (;;);
    }
    display.clearDisplay();
}

long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}

void loop() {
    bool pirStatus = digitalRead(PIR_PIN);
    long distance = getDistance();
    
    // Reset LED
    digitalWrite(LED_DEKAT, LOW);
    digitalWrite(LED_SEDANG, LOW);
    digitalWrite(LED_JAUH, LOW);
    
    String statusPir = pirStatus ? "Terdeteksi" : "Tidak Terdeteksi";
    String statusJarak;
    if (distance < 50) {
        statusJarak = "DEKAT";
        digitalWrite(LED_DEKAT, HIGH);
    } else if (distance >= 50 && distance <= 125) {
        statusJarak = "SEDANG";
        digitalWrite(LED_SEDANG, HIGH);
    } else {
        statusJarak = "JAUH";
        digitalWrite(LED_JAUH, HIGH);
    }
    
    // Tampilkan di Serial Monitor
    Serial.print("PIR: ");
    Serial.print(statusPir);
    Serial.print(" | Jarak: ");
    Serial.print(distance);
    Serial.print(" cm | Status: ");
    Serial.println(statusJarak);
    
    // Tampilkan di OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("PIR: ");
    display.println(statusPir);
    display.print("Jarak: ");
    display.print(distance);
    display.println(" cm");
    display.print("Status: ");
    display.println(statusJarak);
    display.display();
    
    delay(500);
}
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>  // Ganti dengan <ESP8266WiFi.h> jika menggunakan ESP8266
#include <HTTPClient.h>

// Konfigurasi untuk LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Konfigurasi untuk DHT11
#define DHTPIN 4        // Pin data DHT11
#define DHTTYPE DHT11   // Jenis sensor DHT11

// Konfigurasi untuk relay
#define RELAY_PIN 5     // Pin untuk mengontrol relay

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Inisialisasi serial monitor
  Serial.begin(115200);

  // Inisialisasi LCD
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  // Inisialisasi sensor DHT11
  dht.begin();

  // Inisialisasi pin relay sebagai output
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Pastikan relay dalam keadaan mati

  // Tampilkan pesan di layar
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
}

void loop() {
  // Baca suhu dan kelembaban
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Cek apakah pembacaan berhasil
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error");
    return;
   // Tampilkan suhu dan kelembaban di serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  }
  // Tampilkan suhu dan kelembaban di LCD
  lcd.setCursor(0, 0);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%   ");
  lcd.setCursor(0, 1);
  lcd.print("Tem: ");
  lcd.print(temperature);
  lcd.print((char)223); // Karakter derajat
  lcd.print("C   ");

  // Kontrol relay berdasarkan suhu
  if (temperature > 30.0) {
    digitalWrite(RELAY_PIN, HIGH); // Nyalakan relay
  } else {
    digitalWrite(RELAY_PIN, LOW); // Matikan relay
  }

  // Tunggu sebelum pembacaan berikutnya
  delay(1000); // Delay 2 detik
}

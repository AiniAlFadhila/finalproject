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

// Konfigurasi WiFi
const char* ssid = "Redmi 9"; // Ganti dengan SSID WiFi Anda
const char* password = "123456879"; // Ganti dengan password WiFi Anda

// URL server untuk menerima data
const char* serverName = "http://192.168.50.33:5000/api/data";

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
  lcd.print("Connecting...");

  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  lcd.clear();
  lcd.print("Connected!");
}

void loop() {
  // Baca suhu dan kelembaban
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Cek apakah pembacaan berhasil
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error");
    return;
  }

  // Tampilkan suhu dan kelembaban di serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

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

  // Kirim data ke server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);

    // Set header untuk permintaan HTTP POST
    http.addHeader("Content-Type", "application/json");

    // Data yang dikirim ke server
    String httpRequestData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode); // Kode respon HTTP
      Serial.println(response); // Respon dari server
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }

  // Tunggu sebelum pembacaan berikutnya
  delay(1000); // Delay 1 detik
}

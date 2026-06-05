/*
  Meb_Renk - Renk Sınıflandırması Örneği
  
  Bu örnek yumurtaları rengine göre sınıflandırır.
  
  Bağlantı:
  - TCS34725 VCC  -> Arduino 5V
  - TCS34725 GND  -> Arduino GND
  - TCS34725 SDA  -> Arduino A4 (Uno/Nano) or 20 (Mega)
  - TCS34725 SCL  -> Arduino A5 (Uno/Nano) or 21 (Mega)
*/

#include <Wire.h>
#include "MebRenk.h"

MebRenk colorSensor;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("\n\n=== Meb_Renk Renk Sınıflandırması ===");
  Serial.println("TCS34725 Sensörü Başlatılıyor...");
  
  // Sensörü başlat
  if (!colorSensor.begin()) {
    Serial.println("HATA: Sensör başlatılamadı!");
    while (1);
  }
  
  Serial.println("\nSensör başarıyla başlatıldı.");
  Serial.println("Kalibrasyonu başlatmak için 'C' tuşuna basın...");
}

void loop() {
  // Serial komutlarını kontrol et
  if (Serial.available()) {
    char command = Serial.read();
    
    if (command == 'C' || command == 'c') {
      colorSensor.calibrate();
    }
    
    if (command == 'T' || command == 't') {
      testColors();
    }
  }
  
  // Rengi ölç
  colorSensor.measureColor();
  
  // Sonuçları yazdır
  Serial.print("Renk: ");
  Serial.print(colorSensor.getColorName());
  Serial.print(" | RGB: (");
  
  uint8_t r = map(colorSensor.getRed(), 0, 65535, 0, 255);
  uint8_t g = map(colorSensor.getGreen(), 0, 65535, 0, 255);
  uint8_t b = map(colorSensor.getBlue(), 0, 65535, 0, 255);
  
  Serial.print(r);
  Serial.print(", ");
  Serial.print(g);
  Serial.print(", ");
  Serial.print(b);
  Serial.println(")");
  
  delay(1000);
}

void testColors() {
  Serial.println("\n=== Renk Test Modu ===");
  Serial.println("Farklı renkli yumurtaları sensörün üzerine yerleştirin...");
  Serial.println("Her bir renk için 5 ölçüm yapılacak...");
  Serial.println("-----------------------------------------");
  
  // Her renk için test
  String colors[] = {"KIRMIZI", "YESIL", "MAVI", "SARI"};
  
  for (int c = 0; c < 4; c++) {
    Serial.print("\n");
    Serial.print(colors[c]);
    Serial.println(" rengini yerleştirin ve 10 saniye bekleyin...");
    delay(10000);
    
    for (int i = 0; i < 5; i++) {
      colorSensor.measureColor();
      
      uint8_t r = map(colorSensor.getRed(), 0, 65535, 0, 255);
      uint8_t g = map(colorSensor.getGreen(), 0, 65535, 0, 255);
      uint8_t b = map(colorSensor.getBlue(), 0, 65535, 0, 255);
      
      Serial.print("Ölçüm ");
      Serial.print(i + 1);
      Serial.print(": RGB(");
      Serial.print(r);
      Serial.print(", ");
      Serial.print(g);
      Serial.print(", ");
      Serial.print(b);
      Serial.println(")");
      
      delay(500);
    }
  }
  
  Serial.println("\nTest Tamamlandı");
}

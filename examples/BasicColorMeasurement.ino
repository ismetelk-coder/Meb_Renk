/*
  Meb_Renk - Temel Renk Ölçümü Örneği
  
  Bu örnek basit bir şekilde RGB değerlerini ölçer ve yazdırır.
*/

#include <Wire.h>
#include "MebRenk.h"

MebRenk colorSensor;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("Meb_Renk - Temel Renk Ölçümü");
  Serial.println("Sensör başlatılıyor...");
  
  if (!colorSensor.begin()) {
    Serial.println("HATA: Sensör bulunamadı!");
    while (1) delay(1000);
  }
  
  Serial.println("Sensör başarılı, kalibrasyonu başlatın.");
  colorSensor.calibrate();
}

void loop() {
  // Rengi ölç
  colorSensor.measureColor();
  
  // Normalize et (0-255)
  uint8_t r = map(colorSensor.getRed(), 0, 65535, 0, 255);
  uint8_t g = map(colorSensor.getGreen(), 0, 65535, 0, 255);
  uint8_t b = map(colorSensor.getBlue(), 0, 65535, 0, 255);
  uint8_t brightness = map(colorSensor.getBrightness(), 0, 65535, 0, 255);
  
  // Yazdır
  Serial.print("R: ");
  Serial.print(r);
  Serial.print(" G: ");
  Serial.print(g);
  Serial.print(" B: ");
  Serial.print(b);
  Serial.print(" Brightness: ");
  Serial.println(brightness);
  
  delay(500);
}

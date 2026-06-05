/*
  Meb_Renk - İleri Ayarlar Örneği
  
  Bu örnek hassasiyet ayarlarını ve kazancı değiştirme şeklini gösterir.
*/

#include <Wire.h>
#include "MebRenk.h"

MebRenk colorSensor;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("\n\nMeb_Renk - İleri Ayarlar");
  
  // Sensörü başlat
  if (!colorSensor.begin()) {
    Serial.println("HATA: Sensör başlatılamadı!");
    while (1) delay(1000);
  }
  
  Serial.println("Sensör başarıyla başlatıldı.\n");
  
  // İleri Ayarlar
  Serial.println("=== Ayarlar ===");
  
  // Kazancı Ayarla (1x, 4x, 16x, 60x)
  Serial.println("Kazanç: 4x olarak ayarlandı");
  colorSensor.setGain(TCS34725_GAIN_4X);
  
  // İntegrasyon Zamanını Ayarla
  Serial.println("İntegrasyon Zamanı: 154ms olarak ayarlandı");
  colorSensor.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
  
  // Renk Eşiğini Ayarla (daha hassas = düşük değer)
  Serial.println("Renk Eşiği: 20 olarak ayarlandı (daha hassas)");
  colorSensor.setColorThreshold(20);
  
  // Kalibrasyonu yap
  Serial.println("\nLütfen Kalibrasyonu yapın...");
  colorSensor.calibrate();
  
  Serial.println("\n=== Ayarlar Tamamlandı ===");
}

void loop() {
  // Rengi ölç
  colorSensor.measureColor();
  
  // Sonuçları göster
  Serial.print("Renk: ");
  Serial.println(colorSensor.getColorName());
  
  // Ham değerleri göster
  colorSensor.printRawValues();
  
  Serial.println();
  delay(1000);
}

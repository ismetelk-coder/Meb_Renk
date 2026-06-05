/*
  Meb_Renk - Kalibrasyonu Örneği
  
  Bu örnek sensörün doğru kalibrasyonunu gösterir.
  Kalibrasyonu yapıldıktan sonra çok daha doğru renk tespiti sağlanır.
*/

#include <Wire.h>
#include "MebRenk.h"

MebRenk colorSensor;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("\n\nMeb_Renk - Kalibrasyonu Örneği");
  Serial.println("================================\n");
  
  // Sensörü başlat
  if (!colorSensor.begin()) {
    Serial.println("HATA: Sensör başlatılamadı!");
    while (1) delay(1000);
  }
  
  Serial.println("Sensör başarıyla başlatıldı.");
  Serial.println("\nOtomatik Kalibrasyonu Başlatılıyor...");
  Serial.println("Lütfen talimatları izleyin.");
  
  // Otomatik kalibrasyonu çalıştır
  colorSensor.calibrate();
  
  Serial.println("\n=== Kalibrasyonu Tamamlandı ===");
  Serial.println("Şimdi renkler ölçülmeye hazır!");
}

void loop() {
  // Rengi ölç
  colorSensor.measureColor();
  
  // Sonuçları göster
  Serial.print("Algılanan Renk: ");
  Serial.println(colorSensor.getColorName());
  
  // Ham değerleri de göster
  Serial.print("Ham RGB: (");
  Serial.print(colorSensor.getRed());
  Serial.print(", ");
  Serial.print(colorSensor.getGreen());
  Serial.print(", ");
  Serial.print(colorSensor.getBlue());
  Serial.println(")");
  
  Serial.println("-----------");
  
  delay(1000);
}

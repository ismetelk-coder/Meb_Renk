#include "MebRenk.h"

// Constructor
MebRenk::MebRenk() {
  // Varsayılan değerler
  r = g = b = c = 0;
  whiteRefR = 255;
  whiteRefG = 255;
  whiteRefB = 255;
  blackRefR = 0;
  blackRefG = 0;
  blackRefB = 0;
  colorThreshold = 30;
  gain = TCS34725_GAIN_4X;
  integrationTime = TCS34725_INTEGRATIONTIME_154MS;
  brightness = 100;
  calibrated = false;
}

// Sensörü başlat
boolean MebRenk::begin() {
  Wire.begin();
  
  // Sensör ID'sini kontrol et
  uint8_t id = read8(TCS34725_ID);
  if (id != 0x44) {
    Serial.println("HATA: TCS34725 Sensörü Bulunamadı!");
    return false;
  }
  
  Serial.println("TCS34725 Sensörü Başarıyla Başlatıldı");
  
  // Sensörü yapılandır
  setGain(gain);
  setIntegrationTime(integrationTime);
  enable();
  
  delay(500);
  
  return true;
}

// Otomatik Kalibrasyonu
void MebRenk::calibrate() {
  Serial.println("\n=== Kalibrasyonu Başladı ===");
  Serial.println("Lütfen sensörün üzerine BEYAZ bir yüzey yerleştirin ve Enter tuşuna basın...");
  
  while (Serial.available() == 0) {
    delay(100);
  }
  Serial.read();
  
  // Beyaz referansını ölç
  int sumR = 0, sumG = 0, sumB = 0;
  for (int i = 0; i < 10; i++) {
    measureColor();
    sumR += r;
    sumG += g;
    sumB += b;
    delay(100);
  }
  
  whiteRefR = sumR / 10;
  whiteRefG = sumG / 10;
  whiteRefB = sumB / 10;
  
  Serial.print("Beyaz Referansı: R=");
  Serial.print(whiteRefR);
  Serial.print(" G=");
  Serial.print(whiteRefG);
  Serial.print(" B=");
  Serial.println(whiteRefB);
  
  // Siyah referansını ölç
  Serial.println("\nLütfen sensörün üzerine SİYAH bir yüzey yerleştirin ve Enter tuşuna basın...");
  
  while (Serial.available() == 0) {
    delay(100);
  }
  Serial.read();
  
  sumR = sumG = sumB = 0;
  for (int i = 0; i < 10; i++) {
    measureColor();
    sumR += r;
    sumG += g;
    sumB += b;
    delay(100);
  }
  
  blackRefR = sumR / 10;
  blackRefG = sumG / 10;
  blackRefB = sumB / 10;
  
  Serial.print("Siyah Referansı: R=");
  Serial.print(blackRefR);
  Serial.print(" G=");
  Serial.print(blackRefG);
  Serial.print(" B=");
  Serial.println(blackRefB);
  
  calibrated = true;
  Serial.println("=== Kalibrasyonu Tamamlandı ===");
}

// Manuel Beyaz Referansı Ayarla
void MebRenk::setWhiteReference(uint16_t r, uint16_t g, uint16_t b) {
  whiteRefR = r;
  whiteRefG = g;
  whiteRefB = b;
}

// Manuel Siyah Referansı Ayarla
void MebRenk::setBlackReference(uint16_t r, uint16_t g, uint16_t b) {
  blackRefR = r;
  blackRefG = g;
  blackRefB = b;
}

// Renk Ölçümü Yap
void MebRenk::measureColor() {
  // Clear kanal
  c = read16(TCS34725_CDATAL);
  // Red kanal
  r = read16(TCS34725_RDATAL);
  // Green kanal
  g = read16(TCS34725_GDATAL);
  // Blue kanal
  b = read16(TCS34725_BDATAL);
}

// Kırmızı Değeri
uint16_t MebRenk::getRed() {
  return r;
}

// Yeşil Değeri
uint16_t MebRenk::getGreen() {
  return g;
}

// Mavi Değeri
uint16_t MebRenk::getBlue() {
  return b;
}

// Parlaklık Değeri
uint16_t MebRenk::getBrightness() {
  return c;
}

// Renk Adını Getir
String MebRenk::getColorName() {
  ColorCategory category = getColorCategory();
  
  switch (category) {
    case COLOR_RED:
      return "KIRMIZI";
    case COLOR_GREEN:
      return "YESIL";
    case COLOR_BLUE:
      return "MAVI";
    case COLOR_YELLOW:
      return "SARI";
    case COLOR_WHITE:
      return "BEYAZ";
    case COLOR_BROWN:
      return "KAHVEREND";
    default:
      return "BILINMIYOR";
  }
}

// Renk Kategorisini Getir
ColorCategory MebRenk::getColorCategory() {
  // Değerleri normalize et (0-255 aralığına)
  uint8_t normR = map(r, 0, 65535, 0, 255);
  uint8_t normG = map(g, 0, 65535, 0, 255);
  uint8_t normB = map(b, 0, 65535, 0, 255);
  
  // Renk Sınıflandırması
  // Beyaz: Tüm değerler yüksek
  if (normR > 200 && normG > 200 && normB > 200) {
    return COLOR_WHITE;
  }
  
  // Kırmızı: R > G ve R > B
  if (normR > 150 && normR > (normG + colorThreshold) && normR > (normB + colorThreshold)) {
    return COLOR_RED;
  }
  
  // Yeşil: G > R ve G > B
  if (normG > 150 && normG > (normR + colorThreshold) && normG > (normB + colorThreshold)) {
    return COLOR_GREEN;
  }
  
  // Mavi: B > R ve B > G
  if (normB > 150 && normB > (normR + colorThreshold) && normB > (normG + colorThreshold)) {
    return COLOR_BLUE;
  }
  
  // Sarı: R ve G yüksek, B düşük
  if (normR > 150 && normG > 150 && normB < 100) {
    return COLOR_YELLOW;
  }
  
  // Kahverengi: R yüksek, G orta, B düşük
  if (normR > 120 && normG > 60 && normG < 150 && normB < 100) {
    return COLOR_BROWN;
  }
  
  return COLOR_UNKNOWN;
}

// Renk Eşiğini Ayarla
void MebRenk::setColorThreshold(int threshold) {
  colorThreshold = threshold;
}

// Kazancı Ayarla
void MebRenk::setGain(uint8_t g) {
  gain = g;
  write8(TCS34725_CONTROL, g);
}

// İntegrasyon Zamanını Ayarla
void MebRenk::setIntegrationTime(uint8_t time) {
  integrationTime = time;
  write8(TCS34725_ATIME, time);
}

// Parlaklığı Ayarla
void MebRenk::setBrightness(uint8_t b) {
  brightness = b;
}

// Ham Değerleri Yazdır
void MebRenk::printRawValues() {
  Serial.print("R=");
  Serial.print(r);
  Serial.print(" G=");
  Serial.print(g);
  Serial.print(" B=");
  Serial.print(b);
  Serial.print(" C=");
  Serial.println(c);
}

// I2C 8-bit Okuma
uint8_t MebRenk::read8(uint8_t address) {
  Wire.beginTransmission(TCS34725_ADDRESS);
  Wire.write(address | 0x80);
  Wire.endTransmission();
  
  Wire.requestFrom(TCS34725_ADDRESS, 1);
  return Wire.read();
}

// I2C 16-bit Okuma
uint16_t MebRenk::read16(uint8_t address) {
  uint16_t data = 0;
  
  Wire.beginTransmission(TCS34725_ADDRESS);
  Wire.write(address | 0xA0); // 0xA0 = 16-bit otomatik increment
  Wire.endTransmission();
  
  Wire.requestFrom(TCS34725_ADDRESS, 2);
  data = Wire.read();
  data |= (Wire.read() << 8);
  
  return data;
}

// I2C 8-bit Yazma
void MebRenk::write8(uint8_t address, uint8_t value) {
  Wire.beginTransmission(TCS34725_ADDRESS);
  Wire.write(address | 0x80);
  Wire.write(value);
  Wire.endTransmission();
}

// Sensörü Aç
void MebRenk::enable() {
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  delay(2);
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
}

// Sensörü Kapat
void MebRenk::disable() {
  uint8_t reg = read8(TCS34725_ENABLE);
  write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}

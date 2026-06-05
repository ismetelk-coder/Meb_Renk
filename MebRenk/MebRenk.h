#ifndef MebRenk_h
#define MebRenk_h

#include "Arduino.h"
#include "Wire.h"

// TCS34725 I2C Adresi
#define TCS34725_ADDRESS         0x29

// TCS34725 Registerları
#define TCS34725_ENABLE          0x00
#define TCS34725_ATIME           0x01
#define TCS34725_WTIME           0x03
#define TCS34725_AILTL           0x04
#define TCS34725_AILTH           0x05
#define TCS34725_AIHTL           0x06
#define TCS34725_AIHTH           0x07
#define TCS34725_PERS            0x0C
#define TCS34725_CONFIG          0x0D
#define TCS34725_CONTROL         0x0F
#define TCS34725_ID              0x12
#define TCS34725_STATUS          0x13
#define TCS34725_CDATAL          0x14
#define TCS34725_CDATAH          0x15
#define TCS34725_RDATAL          0x16
#define TCS34725_RDATAH          0x17
#define TCS34725_GDATAL          0x18
#define TCS34725_GDATAH          0x19
#define TCS34725_BDATAL          0x1A
#define TCS34725_BDATAH          0x1B

// Enable Register Bitleri
#define TCS34725_ENABLE_AIEN     0x10
#define TCS34725_ENABLE_WEN      0x08
#define TCS34725_ENABLE_AEN      0x04
#define TCS34725_ENABLE_PON      0x01

// Gain Değerleri
#define TCS34725_GAIN_1X         0x00
#define TCS34725_GAIN_4X         0x01
#define TCS34725_GAIN_16X        0x02
#define TCS34725_GAIN_60X        0x03

// İntegrasyon Zamanı Değerleri
#define TCS34725_INTEGRATIONTIME_2_4MS   0xFF
#define TCS34725_INTEGRATIONTIME_24MS    0xF6
#define TCS34725_INTEGRATIONTIME_50MS    0xEB
#define TCS34725_INTEGRATIONTIME_101MS   0xD5
#define TCS34725_INTEGRATIONTIME_154MS   0xC0
#define TCS34725_INTEGRATIONTIME_700MS   0x00

// Renk Kategorileri
enum ColorCategory {
  COLOR_RED,
  COLOR_GREEN,
  COLOR_BLUE,
  COLOR_YELLOW,
  COLOR_WHITE,
  COLOR_BROWN,
  COLOR_UNKNOWN
};

class MebRenk {
public:
  MebRenk();
  
  // Başlatma ve Kalibrasyonu
  boolean begin();
  void calibrate();
  void setWhiteReference(uint16_t r, uint16_t g, uint16_t b);
  void setBlackReference(uint16_t r, uint16_t g, uint16_t b);
  
  // Ölçüm
  void measureColor();
  
  // Veri Okuma
  uint16_t getRed();
  uint16_t getGreen();
  uint16_t getBlue();
  uint16_t getBrightness();
  String getColorName();
  ColorCategory getColorCategory();
  
  // Hassasiyet Ayarlaması
  void setColorThreshold(int threshold);
  void setGain(uint8_t gain);
  void setIntegrationTime(uint8_t time);
  void setBrightness(uint8_t brightness);
  
  // Debug
  void printRawValues();
  
private:
  // I2C İşlevleri
  uint8_t read8(uint8_t address);
  uint16_t read16(uint8_t address);
  void write8(uint8_t address, uint8_t value);
  void enable();
  void disable();
  
  // Renk Değerleri
  uint16_t r, g, b, c;
  
  // Referans Değerleri
  uint16_t whiteRefR, whiteRefG, whiteRefB;
  uint16_t blackRefR, blackRefG, blackRefB;
  
  // Hassasiyet Değerleri
  int colorThreshold;
  uint8_t gain;
  uint8_t integrationTime;
  uint8_t brightness;
  
  // Kalibrasyonu Durumu
  boolean calibrated;
};

#endif

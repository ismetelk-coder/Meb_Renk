# Meb_Renk - TCS34725 Renk Sensörü Kütüphanesi

🤖 MEb Robot Yumurta Toplama Kategorisi için Renk Sınıflandırması

## Açıklama

Bu kütüphane TCS34725 renk sensörünü Arduino platformunda kullanarak yumurtaları renklerine göre sınıflandırmayı sağlar. Kütüphane kalibrasyonu içerir ve doğru renk tespiti yapar.

## Özellikler

✅ **Renk Algılama**: RGB renkleri doğru şekilde ölçer
✅ **Renk Sınıflandırması**: Yumurtaları kategorilere ayırır (Kırmızı, Yeşil, Mavi, Sarı, Beyaz, Kahverengi)
✅ **Kalibrasyonu**: Ortam ışığına uyum sağlar
✅ **Hassas Tespiti**: Renk eşikleri yapılandırılabilir
✅ **I2C İletişim**: Basit bağlantı

## Donanım Gereksinimleri

- **Arduino** (Uno, Mega, Nano vb.)
- **TCS34725 Renk Sensörü**
- **Pullup Dirençleri**: 10kΩ (SDA ve SCL için)

## Bağlantı Şeması

```
TCS34725    Arduino
---------   -------
VCC      -> 5V
GND      -> GND
SDA      -> A4 (Uno/Nano) veya 20 (Mega)
SCL      -> A5 (Uno/Nano) veya 21 (Mega)
```

## Kurulum

1. `MebRenk` klasörünü Arduino/libraries klasörüne kopyalayın
2. Arduino IDE'yi yeniden başlatın
3. Sketch → Include Library menüsünde `MebRenk` görülecektir

## Hızlı Başlangıç

### Temel Kullanım

```cpp
#include <MebRenk.h>

MebRenk colorSensor;

void setup() {
  Serial.begin(9600);
  colorSensor.begin();
  colorSensor.calibrate(); // Kalibrasyonu çalıştırın
}

void loop() {
  // Renk ölçümü yap
  colorSensor.measureColor();
  
  // Rengi sınıflandır
  String colorName = colorSensor.getColorName();
  
  Serial.println("Algılanan Renk: " + colorName);
  delay(1000);
}
```

### Kalibrasyonu Ayarlama

```cpp
// Kalibrasyonu başlat (Serial monitörü açık tutun)
colorSensor.calibrate();

// Kalibrasyonu manuel ayarla
colorSensor.setWhiteReference(250, 245, 240);
colorSensor.setBlackReference(10, 8, 6);
```

### RGB Değerlerini Okuma

```cpp
colorSensor.measureColor();

int red = colorSensor.getRed();
int green = colorSensor.getGreen();
int blue = colorSensor.getBlue();
int brightness = colorSensor.getBrightness();

Serial.print("R:");
Serial.print(red);
Serial.print(" G:");
Serial.print(green);
Serial.print(" B:");
Serial.println(blue);
```

### Renk Sınıfları

| Renk | RGB Aralığı |
|------|-------------|
| **Kırmızı** | R > 150, G < 100, B < 100 |
| **Yeşil** | G > 150, R < 100, B < 100 |
| **Mavi** | B > 150, R < 100, G < 100 |
| **Sarı** | R > 150, G > 150, B < 100 |
| **Beyaz** | R > 150, G > 150, B > 150 |
| **Kahverengi** | R > 100, G < 100, B < 100 |
| **Bilinmeyen** | Yukarıdaki kategorilere uymayan |

## API Referansı

### Başlatma ve Kalibrasyonu

```cpp
void begin()                    // Sensörü başlat
void calibrate()               // Otomatik kalibrasyonu çalıştır
void setWhiteReference(...)    // Beyaz referansını ayarla
void setBlackReference(...)    // Siyah referansını ayarla
```

### Ölçüm

```cpp
void measureColor()            // Renk ölçümü yap
```

### Veri Okuma

```cpp
int getRed()                   // Kırmızı değeri (0-255)
int getGreen()                 // Yeşil değeri (0-255)
int getBlue()                  // Mavi değeri (0-255)
int getBrightness()           // Parlaklık değeri (0-255)
String getColorName()          // Renk adı (String)
```

### Hassasiyet Ayarlaması

```cpp
void setColorThreshold(int threshold)  // Renk tanıma eşiğini ayarla
void setGain(uint8_t gain)             // Kazancı ayarla (1, 4, 16, 60)
void setIntegrationTime(uint8_t time)  // İntegrasyon zamanını ayarla
```

## Örnek Projeler

### 1. Renk Monitörü

Serial monitörde RGB değerlerini gösterir.

### 2. Yumurta Sınıflandırıcı

Yumurtaları rengine göre sınıflandırır.

### 3. Otomatik Kalibrasyonu

Ortak ışık koşullarına göre kalibrasyonu otomatik ayarlar.

## Sorun Giderme

### Sensör Bağlantı Sorunu
```
❌ "Sensör Bulunamadı" hatası
✅ I2C bağlantısını kontrol edin
✅ Pullup dirençlerini kontrol edin
✅ SDA/SCL pinlerini doğru bağladığınızı kontrol edin
```

### Yanlış Renk Tespiti
```
❌ Renkler yanlış tespit ediliyor
✅ Kalibrasyonu çalıştırın
✅ Aydınlatmayı kontrol edin
✅ Renk eşiğini ayarlayın
```

## Lisans

MIT License

## Katkılar

Bug bildirimi ve önerileriniz için issue açabilirsiniz.

---

**Yapımcı**: ismetelk-coder
**Tarih**: 2026
**Kategori**: MEb Robot Yumurta Toplama 🤖🥚

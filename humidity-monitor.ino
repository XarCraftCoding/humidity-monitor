#include "DHT.h"
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE, 6);

char str[10];

void loading(void) {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(5, 20, "Loading...");
}

void setup() {
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

  Serial.begin(9600);

  dht.begin();
  u8g.firstPage();
  do {
    loading();
  } while (u8g.nextPage());
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    u8g.setFont(u8g_font_helvB08);
    u8g.firstPage();
    do {
      u8g.drawStr(5, 20, "Cannot collect data");
      u8g.drawStr(5, 35, "from DHT11 sensor!");
    } while(u8g.nextPage());
    Serial.println("Cannot collect data from DHT11 sensor!");
    return;
  }

  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C / ");
  Serial.print(f);
  Serial.print(" °F\t");

  Serial.print("Heat Index: ");
  Serial.print(hi);
  Serial.println(" °F");

  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_helvB08);
    u8g.drawStr(5, 15, "Humidity:");
    u8g.drawStr(85, 15, dtostrf(h, 5, 2, str));
    u8g.drawStr(115, 15, "% ");

    u8g.drawStr(5, 30, "Temperature:");
    u8g.drawStr(85, 30, dtostrf(t, 5, 2, str));
    u8g.drawStr(115, 30, "\260C ");

    u8g.drawStr(85, 45, dtostrf(f, 5, 2, str));
    u8g.drawStr(115, 45, "\260F ");

    u8g.drawStr(5, 60, "Heat Index:");
    u8g.drawStr(85, 60, dtostrf(hi, 5, 2, str));
    u8g.drawStr(115, 60, "\260F ");

  } while(u8g.nextPage());
}
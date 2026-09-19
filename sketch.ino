#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ============================================================
// Smart Agriculture for Hilly Regions - Wokwi Simulation
// Based on the project report:
// ESP32 + soil moisture + DHT22 + rain + water level
// + relay/pump + OLED + status LEDs/buzzer.
// ============================================================

// ---------- Pin assignments ----------
#define SOIL_PIN       34
#define RAIN_PIN       27
#define WATER_LEVEL_PIN 35
#define DHT_PIN        4
#define DHT_TYPE       DHT22

#define RELAY_PIN      26
#define GREEN_LED_PIN  25
#define RED_LED_PIN    33
#define BUZZER_PIN     32

#define OLED_SDA       21
#define OLED_SCL       22
#define OLED_ADDR      0x3C

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// Report thresholds
const int SOIL_DRY_THRESHOLD = 40;       // below 40% -> irrigate
const int TANK_LOW_THRESHOLD = 20;       // below 20% -> stop + alert
const float TEMP_ALERT_THRESHOLD = 35.0;  // above 35 C -> heat alert
const float HUMIDITY_ALERT_THRESHOLD = 40.0;

bool pumpState = false;
bool rainState = false;
bool lowTank = false;

unsigned long lastRead = 0;
const unsigned long READ_INTERVAL = 15000; // report: 15 s cloud/data interval

// Convert raw soil ADC to relative moisture percentage.
// Calibrated using the report's approximate dry=3100 and wet=1200 values.
int soilMoisturePercent(int raw) {
  int pct = map(raw, 3100, 1200, 0, 100);
  return constrain(pct, 0, 100);
}

// Convert water-level ADC to approximate tank percentage.
// Report: empty ~200, full ~3500.
int waterLevelPercent(int raw) {
  int pct = map(raw, 200, 3500, 0, 100);
  return constrain(pct, 0, 100);
}

void setPump(bool on) {
  pumpState = on;
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
  digitalWrite(GREEN_LED_PIN, on ? HIGH : LOW);
  digitalWrite(RED_LED_PIN, on ? LOW : HIGH);
}

void showStatus(int soilPct, float temp, float hum, int tankPct, bool rain) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("SMART AGRICULTURE");

  display.setCursor(0, 12);
  display.print("Soil: ");
  display.print(soilPct);
  display.print("%");

  display.setCursor(68, 12);
  display.print("Tank: ");
  display.print(tankPct);
  display.print("%");

  display.setCursor(0, 24);
  display.print("Temp: ");
  display.print(temp, 1);
  display.print(" C");

  display.setCursor(68, 24);
  display.print("Hum: ");
  display.print(hum, 0);
  display.print("%");

  display.setCursor(0, 36);
  display.print("Rain: ");
  display.print(rain ? "YES" : "NO");

  display.setCursor(68, 36);
  display.print("Pump: ");
  display.print(pumpState ? "ON" : "OFF");

  display.setCursor(0, 50);
  if (lowTank) {
    display.print("LOW TANK - ALERT");
  } else if (rain) {
    display.print("RAIN - PAUSED");
  } else if (pumpState) {
    display.print("IRRIGATION ACTIVE");
  } else {
    display.print("IRRIGATION OFF");
  }

  display.display();
}

void readAndControl() {
  int soilRaw = analogRead(SOIL_PIN);
  int tankRaw = analogRead(WATER_LEVEL_PIN);

  int soilPct = soilMoisturePercent(soilRaw);
  int tankPct = waterLevelPercent(tankRaw);

  // Typical rain module behavior in the report:
  // HIGH = no rain, LOW = rain.
  rainState = (digitalRead(RAIN_PIN) == LOW);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Avoid NaN in display/logic if DHT read fails.
  if (isnan(temp)) temp = 0.0;
  if (isnan(hum)) hum = 0.0;

  lowTank = (tankPct < TANK_LOW_THRESHOLD);

  // Decision priority follows the report flowchart:
  // Rain -> pump OFF
  // Low tank -> pump OFF
  // Dry soil and no rain + sufficient tank -> pump ON
  // Otherwise -> pump OFF
  if (rainState) {
    setPump(false);
  } else if (lowTank) {
    setPump(false);
  } else if (soilPct < SOIL_DRY_THRESHOLD) {
    setPump(true);
  } else {
    setPump(false);
  }

  // Heat / humidity alert indication.
  bool environmentalAlert =
      (temp > TEMP_ALERT_THRESHOLD) ||
      (hum > 0 && hum < HUMIDITY_ALERT_THRESHOLD);

  digitalWrite(BUZZER_PIN, (lowTank || environmentalAlert) ? HIGH : LOW);

  showStatus(soilPct, temp, hum, tankPct, rainState);

  // Serial output acts as the simulation's cloud/debug log.
  Serial.println("------------------------------------------------");
  Serial.print("Soil raw: "); Serial.print(soilRaw);
  Serial.print(" | Soil moisture: "); Serial.print(soilPct); Serial.println("%");

  Serial.print("Tank raw: "); Serial.print(tankRaw);
  Serial.print(" | Tank level: "); Serial.print(tankPct); Serial.println("%");

  Serial.print("Temperature: "); Serial.print(temp, 1);
  Serial.print(" C | Humidity: "); Serial.print(hum, 1); Serial.println("%");

  Serial.print("Rain: "); Serial.println(rainState ? "DETECTED" : "NO RAIN");
  Serial.print("Pump: "); Serial.println(pumpState ? "ON" : "OFF");

  if (lowTank) Serial.println("ALERT: LOW WATER LEVEL");
  if (temp > TEMP_ALERT_THRESHOLD) Serial.println("ALERT: HIGH TEMPERATURE");
  if (hum > 0 && hum < HUMIDITY_ALERT_THRESHOLD) Serial.println("ALERT: LOW HUMIDITY");

  Serial.println("Cloud simulation: sensor data updated.");
}

void setup() {
  Serial.begin(115200);

  pinMode(RAIN_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  setPump(false);
  digitalWrite(BUZZER_PIN, LOW);

  dht.begin();

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED initialization failed.");
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println("SMART AGRICULTURE");
    display.setCursor(0, 25);
    display.println("ESP32 INITIALIZING");
    display.display();
  }

  delay(1500);
  readAndControl();
  lastRead = millis();
}

void loop() {
  if (millis() - lastRead >= READ_INTERVAL) {
    lastRead = millis();
    readAndControl();
  }
}

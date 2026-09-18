#  Smart Agriculture for Efficient Cultivation in Hilly Regions

An **ESP32-based IoT Smart Agriculture System** designed to automate irrigation and monitor environmental conditions in hilly agricultural regions. The system uses multiple sensors to make irrigation decisions automatically and provides local and remote monitoring.

##  Project Overview

Agriculture in hilly regions faces challenges such as uneven terrain, irregular rainfall, limited water availability, and labour-intensive manual irrigation.

This project proposes a **low-cost IoT-based smart irrigation system** that uses an ESP32 microcontroller and multiple sensors to monitor field conditions and automatically control irrigation.

The system follows a simple decision-making approach:

**Sense → Decide → Act → Monitor → Alert**

The project was developed as part of the **Summer Research Internship Program (SIP)** at **Amrita Vishwa Vidyapeetham, Chennai Campus** and is aligned with **Smart India Hackathon 2025 Problem Statement ID 25062**, proposed by the Government of Sikkim.

---

##  Objectives

* Design and implement an ESP32-based smart irrigation system.
* Monitor soil moisture, temperature, humidity, rainfall, and water level.
* Automatically control the irrigation pump based on soil moisture.
* Stop irrigation when rainfall is detected.
* Monitor rainwater/storage tank levels.
* Provide real-time monitoring through IoT platforms.
* Provide SMS/app-based alerts for important conditions.
* Develop a low-cost and scalable solution suitable for hilly farms.

---

##  System Architecture

```text
                    ┌─────────────────────┐
                    │   Soil Moisture     │
                    │      Sensor         │
                    └──────────┬──────────┘
                               │
                    ┌──────────▼──────────┐
                    │       DHT22         │
                    │ Temp & Humidity      │
                    └──────────┬──────────┘
                               │
                    ┌──────────▼──────────┐
                    │    Rain Sensor      │
                    └──────────┬──────────┘
                               │
                    ┌──────────▼──────────┐
                    │  Water Level Sensor │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │        ESP32        │
                    │  Decision & Control │
                    └──────────┬──────────┘
                               │
              ┌────────────────┼────────────────┐
              │                │                │
              ▼                ▼                ▼
       ┌────────────┐   ┌────────────┐   ┌─────────────┐
       │   Relay    │   │    OLED    │   │   Buzzer /  │
       │   Module   │   │  Display   │   │    LEDs     │
       └─────┬──────┘   └────────────┘   └─────────────┘
             │
             ▼
       ┌────────────┐
       │ Water Pump │
       └────────────┘

             ESP32
               │
       ┌───────┴────────┐
       │                │
       ▼                ▼
  ThingSpeak          Blynk
  IoT Cloud       Mobile Dashboard

               │
               ▼
           GSM Module
            SIM800L
               │
               ▼
          SMS Alerts
```

---

##  Hardware Components

| Component                       | Purpose                                     |
| ------------------------------- | ------------------------------------------- |
| ESP32 Development Board         | Main microcontroller and Wi-Fi connectivity |
| Capacitive Soil Moisture Sensor | Measures soil moisture                      |
| DHT22                           | Measures temperature and humidity           |
| Rain Sensor                     | Detects rainfall                            |
| Water Level Sensor              | Monitors tank/storage level                 |
| 2-Channel Relay Module          | Controls pump and valve                     |
| Mini DC Water Pump              | Provides irrigation water                   |
| OLED Display                    | Displays sensor readings and system status  |
| SIM800L GSM Module              | Sends SMS alerts                            |
| LEDs                            | Visual status indication                    |
| Buzzer                          | Audio alerts                                |
| Sprinkler Nozzles               | Distributes water                           |
| Breadboard & Jumper Wires       | Prototyping and connections                 |

---

##  Software & Technologies

* **ESP32**
* **Arduino IDE**
* **Embedded C/C++**
* **Wi-Fi**
* **MQTT**
* **ThingSpeak**
* **Blynk IoT**
* **Arduino libraries**
* **Wokwi Simulator**

### Required Arduino Libraries

```text
Adafruit GFX Library
Adafruit SSD1306
DHT sensor library
Adafruit Unified Sensor
```

---

##  Working Principle

The ESP32 continuously reads the values from the connected sensors.

### 1. Soil Moisture Monitoring

The soil moisture sensor determines whether the soil is sufficiently wet.

The project uses:

```text
Soil moisture < 40%
        ↓
    Soil is dry
        ↓
   Check rain/tank
        ↓
     Pump ON
```

If the soil moisture is above the threshold, irrigation remains OFF.

### 2. Rain Detection

If rainfall is detected:

```text
Rain detected
      ↓
Pump OFF
      ↓
Irrigation paused
```

This prevents unnecessary water usage during rainfall.

### 3. Water Level Monitoring

The storage/rainwater tank is monitored using a water-level sensor.

If the tank level falls below **20%**, irrigation is stopped and a low-water alert is generated.

### 4. Temperature & Humidity

The DHT22 monitors environmental temperature and humidity.

The project uses a temperature alert threshold of:

```text
Temperature > 35°C
```

A low-humidity alert condition is also included.

---

##  Irrigation Decision Logic

```text
             Read Sensors
                  │
                  ▼
           Is rain detected?
             /          \
           YES           NO
            │             │
            ▼             ▼
        Pump OFF     Tank < 20%?
                         /    \
                       YES     NO
                        │       │
                        ▼       ▼
                    Pump OFF  Soil < 40%?
                               /     \
                             YES      NO
                              │        │
                              ▼        ▼
                          Pump ON    Pump OFF
```

The system repeats the monitoring and decision process continuously.

---

## 📊 Reported System Performance

The project report documented the following prototype results:

| Parameter                  | Result                         |
| -------------------------- | ------------------------------ |
| Pump response time         | < 2 seconds                    |
| Cloud data update          | Every 15 seconds               |
| Dashboard update latency   | < 5 seconds under stable Wi-Fi |
| SMS delivery               | 8–12 seconds average           |
| Soil moisture accuracy     | ±2% relative moisture          |
| DHT22 temperature accuracy | ±0.5°C                         |
| DHT22 humidity accuracy    | ±2% RH                         |
| System uptime during test  | 100% over 8 hours              |
| Estimated water saving     | ~35–40%                        |

---

##  Wokwi Simulation

This repository also contains a **Wokwi simulation** of the core smart irrigation system.

### Files

```text
├── sketch.ino
├── diagram.json
├── libraries.txt
└── README.md
```

### Simulation Components

The Wokwi version includes:

* ESP32
* DHT22
* Soil moisture input
* Water-level input
* Rain detection switch
* Relay
* Pump-control output
* OLED display
* LEDs
* Buzzer

The potentiometers in the simulation represent the analog outputs of the **soil moisture and water-level sensors**.

The rain switch represents the rain sensor.

---

##  Running the Wokwi Simulation

1. Open a new ESP32 project in Wokwi.
2. Copy `sketch.ino` into the project.
3. Copy `diagram.json` into the project.
4. Add the required libraries.
5. Start the simulation.
6. Adjust the soil moisture and tank-level controls to test different conditions.
7. Use the rain switch to simulate rainfall.
8. Observe the relay/pump status, OLED display and Serial Monitor.

---

##  Repository Structure

```text
smart-agriculture-esp32/
│
├── sketch.ino
├── diagram.json
├── libraries.txt
├── README.md
│
└── docs/
    └── project-report.pdf
```

---

##  IoT Monitoring

The proposed system uses:

### ThingSpeak

Used for:

* Sensor data logging
* Real-time graphs
* Historical analysis
* IoT data monitoring

### Blynk

Used for:

* Mobile monitoring
* Dashboard visualization
* Remote pump control

### MQTT

Used as the lightweight communication protocol between the ESP32 and IoT services.

### GSM

The SIM800L module provides SMS-based alerts for important conditions.

---

##  Alert Conditions

The system can generate alerts for:

```text
 Soil too dry
 Tank water level low
 High temperature
 Rain detected
Irrigation started/stopped
```

---

## 📈 Future Improvements

The project can be further developed by adding:

*  Solar-powered operation
*  LoRa/LoRaWAN communication
*  Multiple sensor nodes for different terrace plots
*  Machine-learning-based predictive irrigation
*  Drone-based crop health monitoring
*  Edge AI using ESP32
* Long-term field testing
* Real-farm pilot deployment

---

##  Sustainable Development Goals

The project supports the following **UN Sustainable Development Goals (SDGs)**:

* **SDG 2** – Zero Hunger
* **SDG 6** – Clean Water and Sanitation
* **SDG 9** – Industry, Innovation and Infrastructure
* **SDG 12** – Responsible Consumption and Production
* **SDG 13** – Climate Action
* **SDG 15** – Life on Land

---

## Project Information

**Project:** Implementation of Smart Agriculture for Efficient Cultivation in Hilly Regions

**Program:** Summer Research Internship Program (SIP)

**Institution:** Kumaraguru College of Technology, Coimbatore

**Research Institution:** Amrita Vishwa Vidyapeetham, Chennai Campus

**Theme:** Hardware | Agriculture, Food Tech & Rural Development

**Problem Statement:** Smart India Hackathon 2025 – Problem ID 25062

**Supervisors:**

* Dr. S. G. Rahul
* Dr. M. Gayathri

---

##  Author

**Arun.N**

Electrical and Electronics Engineering (EEE)

Kumaraguru College of Technology, Coimbatore

---

##  License

This project is intended for **educational, academic, research, and prototype development purposes**.

You are welcome to modify and extend the project for further experimentation and research.

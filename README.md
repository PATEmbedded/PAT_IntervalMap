# PAT_IntervalMap Library

The `PAT_IntervalMap` library provides a versatile and efficient way to manage interval-based key-value mappings. It allows for assigning values to key ranges, retrieving values for individual keys, and offers easy visualization of intervals.

## Features

- **Range Assignments**: Assign values to key intervals, with all keys in the interval sharing the same value.
- **Efficient Lookups**: Retrieve values for individual keys, returning a default value if no value has been assigned to that key.
- **Interval Printing**: Print intervals and their associated values in a user-friendly format.

## Installation

1. Clone the repository:

<<<<<<< HEAD
    ```bash
    git clone https://github.com/PAT-IOT/PAT_IntervalMap.git
    ```
=======
   ```bash
   git clone https://github.com/PAT-IOT/PAT_IntervalMap.git
   ```
>>>>>>> f44d9f5 (Initial commit: add PAT IntervalMap library and examples)

2. Copy the `PAT_IntervalMap` directory to your Arduino `libraries` folder.

## Example Usage

## expected Serial Output:

<<<<<<< HEAD
- **Heart Rate**:  55 bpm - Status: Bradycardia Alert.
- **SpO2 Level**: 92% - Status: Low Oxygen Alert.
- **ALERT**: Monitor patient closely; consider intervention.
- **Heart Rate**:  150 bpm - Status: Critical Tachycardia.
- **SpO2 Level**: 88% - Status: Critical Hypoxia Alert.
- **Blood Pressure**: 160/100 mmHg - Status: Critical Hypertension.
- **EMERGENCY**: Immediate medical intervention required!.
- **Heart Rate**:  72 bpm - Status: Normal Heart Rate.
=======
- **Heart Rate**: 55 bpm - Status: Bradycardia Alert.
- **SpO2 Level**: 92% - Status: Low Oxygen Alert.
- **ALERT**: Monitor patient closely; consider intervention.
- **Heart Rate**: 150 bpm - Status: Critical Tachycardia.
- **SpO2 Level**: 88% - Status: Critical Hypoxia Alert.
- **Blood Pressure**: 160/100 mmHg - Status: Critical Hypertension.
- **EMERGENCY**: Immediate medical intervention required!.
- **Heart Rate**: 72 bpm - Status: Normal Heart Rate.
>>>>>>> f44d9f5 (Initial commit: add PAT IntervalMap library and examples)
- **SpO2 Level**: 89% - Status: Critical Hypoxia Alert.
- **Blood Pressure**: 140/90 mmHg - Status: Critical Hypertension.
- **EMERGENCY**: Immediate medical intervention required!.

```cpp
#include <Arduino.h>
#include "../src/PAT_IntervalMap.h"

// Create interval maps for each vital sign parameter
IntervalMap<int, String> heartRateMap("Unknown Status");
IntervalMap<int, String> spo2Map("Unknown Status");
IntervalMap<std::pair<int, int>, String> bloodPressureMap("Unknown Status");

// Initialize vital sign thresholds and corresponding alerts
void initializeMaps()
{
    // Heart rate in beats per minute (bpm)
    heartRateMap[{30, 60}] = "Bradycardia Alert";      // Very low heart rate
    heartRateMap[{60, 100}] = "Normal Heart Rate";     // Normal range
    heartRateMap[{100, 130}] = "Tachycardia Alert";    // Elevated heart rate
    heartRateMap[{130, 200}] = "Critical Tachycardia"; // Dangerously high heart rate

    // SpO₂ levels in percentage
    spo2Map[{85, 90}] = "Critical Hypoxia Alert"; // Immediate attention needed
    spo2Map[{90, 95}] = "Low Oxygen Alert";       // Below normal but less critical
    spo2Map[{95, 100}] = "Normal Oxygen Level";   // Healthy range

    // Blood pressure in systolic/diastolic mmHg using std::pair
    bloodPressureMap[{{80, 50}, {90, 60}}] = "Hypotension Alert";        // Low BP
    bloodPressureMap[{{90, 60}, {120, 80}}] = "Normal Blood Pressure";   // Normal BP
    bloodPressureMap[{{120, 80}, {140, 90}}] = "Hypertension Alert";     // High BP
    bloodPressureMap[{{140, 90}, {180, 120}}] = "Critical Hypertension"; // Dangerously high BP
}

// Function to simulate ICU monitoring and print out statuses
void monitorPatient(int heartRate, int spo2, std::pair<int, int> bloodPressure)
{
    String heartRateStatus = heartRateMap[heartRate];
    String spo2Status = spo2Map[spo2];
    String bloodPressureStatus = bloodPressureMap[bloodPressure];

    Serial.print("Heart Rate: ");
    Serial.print(heartRate);
    Serial.print(" bpm - Status: ");
    Serial.println(heartRateStatus);

    Serial.print("SpO2 Level: ");
    Serial.print(spo2);
    Serial.print("% - Status: ");
    Serial.println(spo2Status);

    Serial.print("Blood Pressure: ");
    Serial.print(bloodPressure.first);
    Serial.print("/");
    Serial.print(bloodPressure.second);
    Serial.print(" mmHg - Status: ");
    Serial.println(bloodPressureStatus);

    // Combining statuses for emergency response
    if (heartRateStatus == "Critical Tachycardia" || spo2Status == "Critical Hypoxia Alert" || bloodPressureStatus == "Critical Hypertension")
    {
        Serial.println("EMERGENCY: Immediate medical intervention required!");
    }
    else if (heartRateStatus == "Tachycardia Alert" || spo2Status == "Low Oxygen Alert" || bloodPressureStatus == "Hypertension Alert")
    {
        Serial.println("ALERT: Monitor patient closely; consider intervention.");
    }
    else if (heartRateStatus == "Normal Heart Rate" && spo2Status == "Normal Oxygen Level" && bloodPressureStatus == "Normal Blood Pressure")
    {
        Serial.println("Patient stable with no intervention required.");
    }
    else
    {
        Serial.println("Patient condition requires further evaluation.");
    }
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    // Initialize vital sign ranges in the maps
    initializeMaps();

    // Simulate patient readings
    monitorPatient(55, 92, {120, 80});   // Example: Normal patient
    monitorPatient(150, 88, {160, 100}); // Example: Critical alert
    monitorPatient(72, 89, {140, 90});   // Example: Alert level
}

void loop()
{
    // Real-time monitoring would be placed here for continuous updates
}
<<<<<<< HEAD
=======
```

# 🕒 WeeklyInterval & DailyInterval Library

The **WeeklyInterval** and **DailyInterval** libraries provide a flexible and modular scheduling system embedded systems.  
They allow developers to define, manage, and query **time-based tasks** across days and weeks using simple APIs, supporting multiple data types and complex schedules.

---

## 📦 Features

- Manage multiple **tasks per day** with automatic handling of overlaps
- Query the **current active task** or value based on RTC time
- Supports **different data types**: `bool`, `int`, `float`, `String` via templates
- JSON export support for web dashboards or logging
- Lightweight, memory-efficient, and easy to integrate

---

## 🧠 Example Use Case

Smart home automation:

- Lights scheduled ON/OFF at specific hours
- HVAC system with temperature setpoints for morning and evening
- Fan speed control using integer levels
- Text notifications for active tasks or reminders

All of this can be handled **without hardcoding complex time logic**, using only these libraries.

---

## 💡 Example Code

```cpp
#include <Arduino.h>
#include "RTClib.h"
#include "PAT_WeeklyInterval.h"

// -------------------
// Create weekly schedules for multiple task types
// -------------------
WeeklyInterval<bool> lightsSchedule;   // true = ON, false = OFF
WeeklyInterval<int> fanSchedule;       // Fan speed 0-5
WeeklyInterval<float> heaterSchedule;  // Temperature setpoints
WeeklyInterval<String> messageSchedule; // Notifications

void setup() {
  Serial.begin(115200);

  // -------------------
  // Boolean schedule: Lights ON/OFF
  // -------------------
  for (int day = 0; day <= 4; ++day) {
    lightsSchedule.addTaskToDay(day, "18:00:00", "23:00:00", true);
  }
  lightsSchedule.addTaskToDay(5, "19:00:00", "23:59:59", true);
  lightsSchedule.addTaskToDay(6, "00:00:00", "01:00:00", true);

  // -------------------
  // Integer schedule: Fan speed
  // -------------------
  for (int day = 0; day <= 6; ++day) {
    fanSchedule.addTaskToDay(day, "08:00:00", "12:00:00", 3);
    fanSchedule.addTaskToDay(day, "12:00:00", "16:00:00", 5);
  }

  // -------------------
  // Float schedule: Heater temperature
  // -------------------
  for (int day = 0; day <= 6; ++day) {
    heaterSchedule.addTaskToDay(day, "06:00:00", "09:00:00", 21.5f);
    heaterSchedule.addTaskToDay(day, "18:00:00", "22:00:00", 23.0f);
  }

  // -------------------
  // String schedule: Notifications
  // -------------------
  for (int day = 0; day <= 6; ++day) {
    messageSchedule.addTaskToDay(day, "18:00:00", "23:00:00", "Lights are ON, enjoy!");
  }

  // Print all schedules for debugging
  lightsSchedule.printdays();
  fanSchedule.printdays();
  heaterSchedule.printdays();
  messageSchedule.printdays();
}

void loop() {
  DateTime now = DateTime(F(__DATE__), F(__TIME__));

  bool lightState = lightsSchedule.getCurrentValue(now);
  int fanSpeed = fanSchedule.getCurrentValue(now);
  float heaterTemp = heaterSchedule.getCurrentValue(now);
  String notification = messageSchedule.getCurrentValue(now);

  Serial.print("Now: ");
  Serial.print(now.timestamp(DateTime::TIMESTAMP_TIME));
  Serial.print(" | Light: ");
  Serial.print(lightState ? "ON" : "OFF");
  Serial.print(" | Fan Speed: ");
  Serial.print(fanSpeed);
  Serial.print(" | Heater Temp: ");
  Serial.print(heaterTemp);
  Serial.print("°C | Message: ");
  Serial.println(notification);

  delay(5000);
}
```
>>>>>>> f44d9f5 (Initial commit: add PAT IntervalMap library and examples)

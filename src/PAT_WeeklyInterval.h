#ifndef _PAT_WeeklyInterval_H_
#define _PAT_WeeklyInterval_H_

#include <Arduino.h>
#include <vector>
#include <algorithm>
#include "PAT_DailyInterval.h"
#include <ArduinoJson.h>

template <typename T> // Template type T for the WeeklyInterval class
class WeeklyInterval
{
private:
      std::vector<DailyInterval<T>> days;

public:
      // Constructor to initialize the weekly schedule
      WeeklyInterval()
      {
            // jsonDoc = new (ps_malloc(sizeof(DynamicJsonDocument))) DynamicJsonDocument(20000);
            for (int i = 0; i < 7; ++i)
            {
                  days.push_back(DailyInterval<T>(T())); // Initialize with default value of type T
            }
      }

      // Method to clear the weekly schedule
      void clean()
      {
            for (int i = 0; i < 7; ++i)
            {
                  days[i].clear();
            }
      }

      // Method to set all tasks for a specific day to a value
      void setDayTasks(int day, T value)
      {
            if (day < 0 || day > 6)
            {
#ifdef PAT_DEBUG_ENABLED
                  Serial.println("Invalid day of the week.");
#endif
                  return;
            }
            days[day].setAllTasks(value); // Set all tasks for the day
      }

      // Method to check if there are any tasks for a specific day
      bool hasTasksForDay(int day)
      {
            if (day < 0 || day > 6)
            {
#ifdef PAT_DEBUG_ENABLED
                  Serial.println("Invalid day of the week.");
#endif
                  return false;
            }
            return days[day].hasTasks(); // Check if there are any tasks for the day
      }

      // Method to add a task to a specific day
      void addTaskToDay(int day, const String &startStr, const String &endStr, T value)
      {
            if (day < 0 || day > 6)
            {
#ifdef PAT_DEBUG_ENABLED
                  Serial.println("Invalid day of the week.");
#endif
                  return;
            }
            days[day].addTask(startStr, endStr, value);
      }

      // Method to remove a specific task from a day
      void removeTaskFromDay(int day, const String &startStr, const String &endStr)
      {
            if (day < 0 || day > 6)
            {
#ifdef PAT_DEBUG_ENABLED
                  Serial.println("Invalid day of the week.");
#endif
                  return;
            }
            days[day].removeTask(startStr, endStr); // Remove the task from the day
      }

      // Method to update a specific task
      void updateTaskOnDay(int day, const String &oldStartStr, const String &oldEndStr, const String &newStartStr, const String &newEndStr, T newValue)
      {
            if (day < 0 || day > 6)
            {
#ifdef PAT_DEBUG_ENABLED
                  Serial.println("Invalid day of the week.");
#endif
                  return;
            }
            days[day].updateTask(oldStartStr, oldEndStr, newStartStr, newEndStr, newValue); // Update the task on the day
      }

      // Method to get the value for a specific day and time
      // T getCurrentValue(int day, String currentTimeString)
      // {
      //       if (day < 0 || day > 6)
      //       {
      //             Serial.println("Invalid day of the week.");
      //             return T(); // Default value if invalid
      //       }
      //       return days[day].getCurrentValue(currentTimeString);
      // }
      //----------------------------------------------
      T getCurrentValue(DateTime dt)
      {
            int day = int(dt.dayOfTheWeek());
            String clock = dt.getString("hh:mm");

            return days[day].getCurrentValue(clock);
      }
      //----------------------------------------------
      structCurrentTask<T> getCurrentTask(DateTime dt)
      {
            int day = int(dt.dayOfTheWeek());
            String clock = dt.getString("hh:mm");

            return days[day].getCurrentTask(clock);
      }
      //----------------------------------------------
      // Method to print the schedule for the entire week
      void printdays()
      {
#ifdef PAT_DEBUG_ENABLED
            // std::lock_guard<std::mutex> guard(mtx);
            const char *daysOfWeek[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
            for (int i = 0; i < 7; ++i)
            {

                  Serial.print(daysOfWeek[i]);
                  Serial.println(":");
                  days[i].print();
            }
#endif
      }

      void printWeeklyJson(T inval = true)
      {
#ifdef PAT_DEBUG_ENABLED
            // std::lock_guard<std::mutex> guard(mtx);
            serializeJsonPretty(getWeeklyJson(inval), Serial);
#endif
      }

      // Get Weekly JSON with the provided value
      DynamicJsonDocument &getWeeklyJson(T inval)
      {

            DynamicJsonDocument *jsonDoc;
            jsonDoc = new (ps_malloc(sizeof(DynamicJsonDocument))) DynamicJsonDocument(20000);

            // std::lock_guard<std::mutex> guard(mtx);

            // lock();
            JsonArray weeklyArray = jsonDoc->createNestedArray("schedule");

            for (int i = 0; i < days.size(); ++i)
            {
                  for (const auto &task : days.at(i).getTasks())
                  {
                        if (String(task.value) == String(inval))
                        {
                              JsonObject obj = weeklyArray.createNestedObject();
                              obj["day"] = i;

                              char timeBuffer[5];
                              sprintf(timeBuffer, "%02d:%02d", task.start / 3600, (task.start % 3600) / 60);
                              obj["t1"] = String(timeBuffer);

                              sprintf(timeBuffer, "%02d:%02d", task.end / 3600, (task.end % 3600) / 60); // task.end.load() % 60
                              obj["t2"] = String(timeBuffer);
                        }
                  }
            }

            // unlock();
            return *jsonDoc; // Return the populated JSON document
      }
};

#endif // _PAT_WeeklyInterval_H_

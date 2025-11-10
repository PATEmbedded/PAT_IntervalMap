

#ifndef _PAT_DailyInterval_H_
#define _PAT_DailyInterval_H_

#include <Arduino.h>
#include <vector>
#include <algorithm>
#include "RTClib.h"

template <typename T>
struct structCurrentTask
{
      DateTime start; // Start time in seconds
      DateTime end;   // End time in seconds
      T value;        // Task value
};

template <typename T>
class DailyInterval
{
private:
      int parseTimeString(const String &timeStr)
      {
            int hours = 0, minutes = 0, seconds = 0;
            sscanf(timeStr.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);
            return constrain(hours * 3600 + minutes * 60 + seconds, 0, 24 * 3600);
      }

protected:
      const T defaultValue;
      struct Task
      {
            int start; // Start time in seconds
            int end;   // End time in seconds
            T value;   // Task value
      };
      std::vector<Task> tasks;

      T defaultTaskValue; // Default task value
      int currentStart;
      int currentEnd;
      //------------

      void resolveOverlaps()
      {
            std::vector<Task> resolved;

            for (const auto &newTask : tasks)
            {
                  std::vector<Task> temp;
                  for (const auto &existingTask : resolved)
                  {
                        if (newTask.end <= existingTask.start || newTask.start >= existingTask.end)
                        {
                              temp.push_back(existingTask); // No overlap
                        }
                        else
                        {
                              // Handle overlap
                              if (newTask.start > existingTask.start)
                              {
                                    temp.push_back({existingTask.start, newTask.start, existingTask.value});
                              }
                              if (newTask.end < existingTask.end)
                              {
                                    temp.push_back({newTask.end, existingTask.end, existingTask.value});
                              }
                        }
                  }
                  temp.push_back(newTask);
                  resolved = std::move(temp);
            }

            std::sort(resolved.begin(), resolved.end(), [](const Task &a, const Task &b)
                      { return a.start < b.start; });
            tasks = std::move(resolved);
      }
      void mergeTasks()
      {
            std::vector<Task> mergedTasks;
            std::sort(tasks.begin(), tasks.end(), [](const Task &a, const Task &b)
                      { return (a.value < b.value || (a.value == b.value && a.start < b.start)); });

            for (const auto &currentTask : tasks)
            {
                  if (mergedTasks.empty())
                  {
                        mergedTasks.push_back(currentTask);
                  }
                  else
                  {
                        Task &lastMergedTask = mergedTasks.back();
                        if (lastMergedTask.value == currentTask.value &&
                            lastMergedTask.end >= currentTask.start)
                        {
                              lastMergedTask.end = std::max(lastMergedTask.end, currentTask.end);
                        }
                        else
                        {
                              mergedTasks.push_back(currentTask);
                        }
                  }
            }
            std::sort(mergedTasks.begin(), mergedTasks.end(), [](const Task &a, const Task &b)
                      { return a.start < b.start; });
            tasks = std::move(mergedTasks);
      }

public:
      DailyInterval(const T &defaultValueIn) : defaultValue(defaultValueIn), defaultTaskValue(defaultValueIn)
      {
            // Add a default task covering the entire day
            addTask("00:00:00", "24:00:00", defaultTaskValue);
      }
      void clear()
      {
            tasks.clear();
            tasks.shrink_to_fit();
            defaultTaskValue = defaultValue;
            int currentStart = 0;
            int currentEnd = 0;
            addTask("00:00:00", "24:00:00", defaultTaskValue);
      }
      void addTask(const String &startStr, const String &endStr, const T &value)
      {
            int start = parseTimeString(startStr);
            if (start < 0)
                  start = 0;
            if (start > 86399)
                  start = 86399;

            int end = parseTimeString(endStr);
            if (end < 0)
                  end = 0;
            if (end > 86399)
                  end = 86399;

            Task newTask = {start, end, value};
            tasks.push_back(newTask);
            resolveOverlaps();
            mergeTasks();
      }
      void addTask(const int &startStr, const int &endStr, const T &value)
      {
            int start = startStr;
            if (start < 0)
                  start = 0;
            if (start > 86400)
                  start = 86400;

            int end = endStr;
            if (end < 0)
                  end = 0;
            if (end > 86400)
                  end = 86400;

            Task newTask = {start, end, value};
            tasks.push_back(newTask);
            resolveOverlaps();
            mergeTasks();
      }
      // Operator to add tasks using [] syntax
      DailyInterval &operator[](const std::pair<String, String> &timeRange)
      {
            currentStart = parseTimeString(timeRange.first);
            currentEnd = parseTimeString(timeRange.second);
            return *this;
      }
      // Operator to add tasks using [] syntax
      DailyInterval &operator[](const std::pair<int, int> &timeRange)
      {
            currentStart = timeRange.first;
            currentEnd = timeRange.second;
            return *this;
      }
      // Overload operator= to set the task value
      void operator=(const T &value)
      {
            String startStr = String(currentStart / 3600) + ":" + String((currentStart % 3600) / 60) + ":" + String(currentStart % 60);
            String endStr = String(currentEnd / 3600) + ":" + String((currentEnd % 3600) / 60) + ":" + String(currentEnd % 60);
            addTask(startStr, endStr, value);
      }

      void print() const
      {
#ifdef PAT_DEBUG_ENABLED
            Serial.println("Current Schedule:");

            for (const auto &task : tasks)
            {
                  int startH = task.start / 3600, startM = (task.start % 3600) / 60, startS = task.start % 60;
                  int endH = task.end / 3600, endM = (task.end % 3600) / 60, endS = task.end % 60;
                  Serial.printf("%02d:%02d:%02d - %02d:%02d:%02d -> %s\n", startH, startM, startS, endH, endM, endS, task.value.c_str());
            }
#endif
      }
      //-------------------------------------
      void printJson() const
      {
            DynamicJsonDocument jsonDoc(2048);
            JsonArray tasksArray = jsonDoc.createNestedArray("schedule");
            for (const auto &task : tasks)
            {
                  if (task.value)
                  {
                        JsonObject taskObject = tasksArray.createNestedObject();
                        taskObject["t1"] = String(task.start / 3600) + ":" + String((task.start % 3600) / 60) + ":" + String(task.start % 60);
                        taskObject["t2"] = String(task.end / 3600) + ":" + String((task.end % 3600) / 60) + ":" + String(task.end % 60);
                  }
            }
#ifdef PAT_DEBUG_ENABLED
            serializeJsonPretty(jsonDoc, Serial);
            Serial.println();
#endif
      }
      //-------------------------------------
      std::vector<Task> &getTasks(void)
      {
            return tasks;
      }
      //-------------------------------------
      bool isTimeInRange(int timeInSeconds) const
      {
            for (const auto &task : tasks)
            {
                  if (timeInSeconds >= task.start && timeInSeconds < task.end)
                  {
                        return true;
                  }
            }
            return false;
      }
      //-------------------------------------
      T getCurrentValue(int currentTimeInSeconds) const
      {
#ifdef PAT_DEBUG_ENABLED
            // Serial.printf("getCurrentValue = %d\n", currentTimeInSeconds);
#endif
            int currentTime = currentTimeInSeconds;
            if (currentTime < 0)
                  currentTime = 0;
            if (currentTime > 86400)
                  currentTime = 86400;

            for (const auto &task : tasks)
            {
                  if (currentTime >= task.start && currentTime < task.end)
                  {
#ifdef PAT_DEBUG_ENABLED
                        // Serial.printf(" task.value = %d\n", task.value);
#endif

                        return task.value;
                  }
            }
#ifdef PAT_DEBUG_ENABLED
            // Serial.printf(" defaultTaskValue = %d\n", defaultTaskValue);
#endif

            return defaultTaskValue;
      }
      //-------------------------------------
      structCurrentTask<T> getCurrentTask(int currentTimeInSeconds) const
      {
            int currentTime = currentTimeInSeconds;
            if (currentTime < 0)
                  currentTime = 0;
            if (currentTime > 86400)
                  currentTime = 86400;

            structCurrentTask<T> currentTask; // Specify <T> here

            for (const auto &task : tasks)
            {
                  if (currentTime >= task.start && currentTime < task.end)
                  {
                        DateTime t1(task.start + SECONDS_FROM_1970_TO_2000);
                        DateTime t2(task.end + SECONDS_FROM_1970_TO_2000);

                        currentTask.start = t1;
                        currentTask.end = t2;
                        currentTask.value = task.value;
                        return currentTask;
                  }
            }

            return currentTask;
      }

      //-------------------------------------
      T getCurrentValue(String currentTimeString) const
      {
            String timeStr = currentTimeString;
            int hours = 0, minutes = 0, seconds = 0;
            sscanf(timeStr.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);
            int result = constrain(hours * 3600 + minutes * 60 + seconds, 0, 24 * 3600);
            return getCurrentValue(result);
      }
      //-------------------------------------
      structCurrentTask<T> getCurrentTask(String currentTimeString) const
      {
            String timeStr = currentTimeString;
            int hours = 0, minutes = 0, seconds = 0;
            sscanf(timeStr.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);
            int result = constrain(hours * 3600 + minutes * 60 + seconds, 0, 24 * 3600);
            return getCurrentTask(result);
      }
      //-------------------------------------
};

#endif // _PAT_DailyInterval_H_

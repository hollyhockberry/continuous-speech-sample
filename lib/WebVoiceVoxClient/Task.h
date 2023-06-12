// Copyright (c) 2023 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#ifndef TASK_H_
#define TASK_H_

#include <RTOS.h>

class Task {
  const char* _taskName;
  const uint32_t _stackSize;
 public:
  Task() : Task(nullptr, 1024) {
  }
  Task(const char* taskName, uint32_t stackSize) : _taskName(taskName), _stackSize(stackSize) {
  }
  void begin() {
    ::xTaskCreate(main, _taskName == nullptr ? "" : _taskName, _stackSize, this, 1, nullptr);
  }
 protected:
  virtual void loop() = 0;
 private:
  static void main(void* arg) {
    Task* task = (Task*)arg;
    while (true) {
      task->loop();
      vTaskDelay(10);
    }
  }
  Task(const Task&) = delete;
  void operator=(const Task&) = delete;
};

#endif  // TASK_H_

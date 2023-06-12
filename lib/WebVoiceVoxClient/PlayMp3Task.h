#ifndef PLAYMP3TASK_H_
#define PLAYMP3TASK_H_

#include <WString.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutput.h>
#include "Task.h"

class PlayMp3Task : public Task {
  AudioGeneratorMP3 _mp3;
  AudioOutput& _output;
  QueueHandle_t _queue = nullptr;
  const char* _rootCACertificate;
  uint8_t* _buffer;

 public:
  PlayMp3Task(AudioOutput& output, QueueHandle_t recvQueue, const char* rootCACertificate);

 protected:
  void loop() override;
};

#endif  // PLAYMP3TASK_H_

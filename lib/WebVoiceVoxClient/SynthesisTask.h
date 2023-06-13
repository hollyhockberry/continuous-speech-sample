#ifndef SYNTHESISTASK_H_
#define SYNTHESISTASK_H_

#include <queue>
#include <vector>
#include <WString.h>
#include "Task.h"

class SynthesisTask : public Task {
  const char* _apiKey;
  const char* _rootCACertificate;
  QueueHandle_t _sendQueue = nullptr;
  std::queue<String> _queue;
  int _speaker = 3;

 public:
  SynthesisTask(const char* apiKey, QueueHandle_t sendQueue, const char* rootCACertificate);

  int Speaker() const;
  void Speaker(int speaker);

  void Queue(String& message);

 protected:
  void loop() override;
};

#endif  // SYNTHESISTASK_H_

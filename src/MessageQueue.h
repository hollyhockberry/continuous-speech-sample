// Copyright (c) 2023 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include <Task.h>
#include <vector>
#include <WString.h>
#include <WebVoiceVoxClient.h>

class MessageQueue : public Task {
  QueueHandle_t _queue;
  String _temporary;
  std::vector<String> _delimiters;
  WebVoiceVoxClient& _voiceVox;

 public:
  explicit MessageQueue(WebVoiceVoxClient& voiceVox);

  void add(const char* chunk);
  void flush();

 protected:
  void loop() override;
};

#endif  // MESSAGEQUEUE_H_

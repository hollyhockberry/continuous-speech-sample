// Copyright (c) 2023 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "MessageQueue.h"

#include <HardwareSerial.h>

MessageQueue::MessageQueue(WebVoiceVoxClient& voiceVox)
: Task("message", 4096), _temporary(), _voiceVox(voiceVox) {
  _queue = ::xQueueCreate(20, sizeof(void*));
  // _delimiters = { "。", "！", "？", "、", ".", ",", "!", "?" };
  _delimiters = { "。", "！", "？", "!", "?" };
}

void MessageQueue::add(const char* chunk) {
  _temporary += chunk;

  String str(chunk);
  for (const auto& delimiter : _delimiters) {
    const auto period = str.indexOf(delimiter) >= 0;
    if (period) {
      const auto th = _voiceVox.busy() ? 300 : 50;
      if (_temporary.length() > th) {
Serial.printf("** MessageQueue) queue: %d\r\n", th);
        const auto idx = _temporary.lastIndexOf(delimiter);
        auto sub = _temporary.substring(0, idx + delimiter.length());
        auto message = new String(sub);
        _temporary.remove(0, idx + delimiter.length());
        ::xQueueSend(_queue, &message, 0);
      } else {
        Serial.printf("Hold: (%d) %s\r\n", _temporary.length(), _temporary.c_str());
      }
      break;
    }
  }
}

void MessageQueue::flush() {
  if (_temporary.length() > 0) {
Serial.printf("** MessageQueue) flush\r\n");
    auto message = new String(_temporary);
    ::xQueueSend(_queue, &message, 0);
    _temporary.clear();
  }
}

void MessageQueue::loop() {
  String* message;
  auto ret = ::xQueueReceive(_queue, &message, portMAX_DELAY);
Serial.printf("** MessageQueue) begin: %s\r\n", message->c_str());
  _voiceVox.Queue(*message);
Serial.printf("** MessageQueue) end\r\n");
  delete message;
}

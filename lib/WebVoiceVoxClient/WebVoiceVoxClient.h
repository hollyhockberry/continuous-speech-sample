// Copyright (c) 2023 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#ifndef WEBVOICEVOXCLIENT_H_
#define WEBVOICEVOXCLIENT_H_

#include <AudioOutput.h>
#include "SynthesisTask.h"
#include "PlayMp3Task.h"

class WebVoiceVoxClient {
  AudioOutput& _output;
  const char* _apiKey;
  const char* _rootCACertificate;
  SynthesisTask* _synthesis = nullptr;
  PlayMp3Task* _playMp3 = nullptr;

 public:
  WebVoiceVoxClient(AudioOutput& output, const char* apiKey, const char* rootCACertificate = nullptr);

  void begin();

  bool busy() const;

  int Speaker() const;
  void Speaker(int speaker);

  void Queue(String message);

 private:
  WebVoiceVoxClient(const WebVoiceVoxClient&) = delete;
  void operator=(const WebVoiceVoxClient&) = delete;
};

#endif  // WEBVOICEVOXCLIENT_H_

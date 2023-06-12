// Copyright (c) 2023 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <M5Unified.h>
#include <WiFi.h>
#include <WebVoiceVoxClient.h>

#include "AudioOutputM5Speaker.h"

static const char* VOICEVOX_APIKEY = "*** YOUR API KEY ***";

AudioOutputM5Speaker audioOutput(&M5.Speaker, 0);
WebVoiceVoxClient voiceVox(audioOutput, VOICEVOX_APIKEY);

void setup() {
  M5.begin();
  {
    auto spk_cfg = M5.Speaker.config();
    spk_cfg.sample_rate = 96000;
    spk_cfg.task_pinned_core = APP_CPU_NUM;
    M5.Speaker.config(spk_cfg);
    M5.Speaker.begin();
  }

  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED) {
    ::delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  voiceVox.begin();
}

void loop() {
  m5gfx::touch_point_t t;
  if (!M5.Display.getTouch(&t)) {
    return;
  }
  while (M5.Display.getTouch(&t)) {
    ::delay(1);
  }
  voiceVox.Queue("こんにちは。お元気ですか？私は元気です");
  voiceVox.Queue("ところで今日は何曜日ですか？知ってます？");
  voiceVox.Queue("あ、時間だ。それではさようなら");
  voiceVox.Queue("またいつかお会いしましょうね！");
}

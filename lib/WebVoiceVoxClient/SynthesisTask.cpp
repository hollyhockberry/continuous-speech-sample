// Copyright (c) 2023 Inaba (@hollyhockberry)
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>
#include "SynthesisTask.h"

namespace {

String URLEncode(const char* msg) {
  const char *hex = "0123456789ABCDEF";
  String encodedMsg = "";

  while (*msg != '\0') {
    if ( ('a' <= *msg && *msg <= 'z')
         || ('A' <= *msg && *msg <= 'Z')
         || ('0' <= *msg && *msg <= '9')
         || *msg  == '-' || *msg == '_' || *msg == '.' || *msg == '~' ) {
      encodedMsg += *msg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[*msg >> 4];
      encodedMsg += hex[*msg & 0xf];
    }
    msg++;
  }
  return encodedMsg;
}

DynamicJsonDocument doc(1024);

bool GetStreamingUrl(const String& url, String& out, const char* rootCA, WiFiClientSecure* client, HTTPClient* https) {
  client->setCACert(rootCA);
  if (!https->begin(*client, url)) {
    return false;
  }
  const auto code = https->GET();
  if (code != HTTP_CODE_OK) {
    https->end();
    return false;
  }
  const String payload = https->getString();
  https->end();
  if (::deserializeJson(doc, payload.c_str())) {
    return false;
  }
  const char* mp3StreamingUrl = doc["mp3StreamingUrl"];
  out = String(mp3StreamingUrl);
  return true;
}

bool GetStreamingUrl(const String& url, String& out, const char* rootCA) {
  auto client = new WiFiClientSecure();
  auto https = new HTTPClient();
  auto ret = GetStreamingUrl(url, out, rootCA, client, https);
  delete client;
  delete https;
  return ret;
}
}  // namespace

SynthesisTask::SynthesisTask(const char* apiKey, QueueHandle_t sendQueue, const char* rootCACertificate)
: Task("synthesis", 4096), _apiKey(apiKey), _rootCACertificate(rootCACertificate), _sendQueue(sendQueue) {
}

void SynthesisTask::Queue(String& message) {
Serial.printf("Synthesis) queue: %s\r\n", message.c_str());
  _queue.push(message);
}

int SynthesisTask::Speaker() const {
  return _speaker;
}

void SynthesisTask::Speaker(int speaker) {
  _speaker = speaker;
}

void SynthesisTask::loop() {
  if (_queue.size() <= 0) {
    return;
  }
  if (::uxQueueSpacesAvailable(_sendQueue) <= 0) {
    return;
  }
  _busy = true;
  String message = _queue.front();
Serial.printf("---- Synthesis) process: %s\r\n", message.c_str());
  String api = String("https://api.tts.quest/v3/voicevox/synthesis?key=") + _apiKey;
  api += String("&text=") +  URLEncode(message.c_str());
  if (_speaker >= 0) {
    api += String("&speaker=") + String(_speaker);
  }
  auto url = new String();
  if (!GetStreamingUrl(api, *url, _rootCACertificate)) {
Serial.printf("---- Synthesis) error!\r\n");
    //TODO:　エラーの場合、待ちを要求されてるかもなのでケアしたい
    std::queue<String> empty;
    std::swap(_queue, empty);
    _queue.empty();
    delete url;
    _busy = false;
    return;
  }
Serial.printf("---- Synthesis) finish\r\n");
  ::xQueueSend(_sendQueue, &url, 0);
  _queue.pop();
  _busy = false;
}

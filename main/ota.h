#pragma once

// system includes
#include <expected>

// 3rdparty lib includes
#include <espasyncota.h>
#include <delayedconstruction.h>

extern cpputils::DelayedConstruction<EspAsyncOta> asyncOta;
extern bool asyncOtaTaskStarted;

void initOta();
void handleOta();
std::expected<void, std::string> triggerOta(std::string_view url);

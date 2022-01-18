#pragma once

// 3rdparty lib includes
#include <espasyncota.h>
#include <delayedconstruction.h>

extern cpputils::DelayedConstruction<EspAsyncOta> asyncOta;
extern bool asyncOtaTaskStarted;

void initOta();
void handleOta();
tl::expected<void, std::string> triggerOta(std::string_view url);

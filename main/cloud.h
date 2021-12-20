#pragma once

// 3rdparty lib includes
#include <wrappers/websocket_client.h>
#include <espwifistack.h>
#include <esphttpdutils.h>
#include <fmt/core.h>
#include <tickchrono.h>

// local includes
#include "globals.h"
#include "utils.h"

#ifdef FEATURE_CLOUD
extern espcpputils::websocket_client cloudClient;
extern bool cloudStarted;
extern espchrono::millis_clock::time_point lastCreateTry;
extern espchrono::millis_clock::time_point lastStartTry;
extern std::string cloudBuffer;

void createCloud();
void destroyCloud();
void startCloud();

void initCloud();
void updateCloud();
void cloudCollect();
void cloudSend();
#endif

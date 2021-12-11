#pragma once

// 3rdparty lib includes
#include <esphttpdutils.h>
#include <espwifistack.h>
#include <fmt/core.h>
#include <tickchrono.h>
#include <wrappers/websocket_client.h>

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
void cloudCollect();
void cloudSend();
#endif

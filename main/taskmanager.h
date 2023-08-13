#pragma once

// system includes
#include <optional>
#include <expected>

// 3rdparty lib includes
#include <arrayview.h>

// local includes
#include "bobbyschedulertask.h"

extern cpputils::ArrayView<BobbySchedulerTask> schedulerTasks;

extern const BobbySchedulerTask &drivingModeTask;

void sched_pushStats(bool printTasks);

std::expected<bool, std::string> checkInitializedByName(const std::string& name);

bool checkEnabledByName(const std::string& name);

void reload_tasks();

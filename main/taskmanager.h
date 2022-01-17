#pragma once

// 3rdparty lib includes
#include <arrayview.h>

// local includes
#include "bobbyschedulertask.h"

extern cpputils::ArrayView<BobbySchedulerTask> schedulerTasks;

extern const BobbySchedulerTask &drivingModeTask;

void sched_pushStats(bool printTasks);

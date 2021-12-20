#pragma once

// 3rdparty lib includes
#include <arrayview.h>

// forward declares
namespace espcpputils { class SchedulerTask; }

extern cpputils::ArrayView<espcpputils::SchedulerTask> schedulerTasks;

extern const espcpputils::SchedulerTask &drivingModeTask;

void sched_pushStats(bool printTasks);

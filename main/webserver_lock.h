#pragma once

// 3rdparty lib includes
#include <delayedconstruction.h>
#include <wrappers/mutex_semaphore.h>

extern cpputils::DelayedConstruction<espcpputils::mutex_semaphore> webserver_lock;

#pragma once

// 3rdparty lib includes
#include <delayedconstruction.h>
#include <wrappers/mutex_semaphore.h>

#ifndef FEATURE_IS_MIR_EGAL_OB_DER_WEBSERVER_KORREKT_ARBEITET
extern cpputils::DelayedConstruction<espcpputils::mutex_semaphore> webserver_lock;
#endif

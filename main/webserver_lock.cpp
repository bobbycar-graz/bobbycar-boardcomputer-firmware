#include "webserver_lock.h"

#ifndef FEATURE_IS_MIR_EGAL_OB_DER_WEBSERVER_KORREKT_ARBEITET
cpputils::DelayedConstruction<espcpputils::mutex_semaphore> webserver_lock;
#endif

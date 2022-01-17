#include "webserver_lock.h"

cpputils::DelayedConstruction<espcpputils::mutex_semaphore> webserver_lock;

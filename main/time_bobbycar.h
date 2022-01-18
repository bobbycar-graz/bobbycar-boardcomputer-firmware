#pragma once

#include "sdkconfig.h"

// 3rdparty lib includes
#include <espchrono.h>

espchrono::time_zone get_default_timezone() noexcept;

void initTime();
void updateTime();

void time_set_now(espchrono::utc_clock::time_point now);

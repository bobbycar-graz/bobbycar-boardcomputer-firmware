#pragma once

// system includes
#include <cstdint>
#include <optional>

// 3rdparty lib includes
#include <espchrono.h>

// local includes
#include "bobbycar-common.h"
#include "modeinterface.h"
#include "globals.h"
#include "utils.h"
#include "defaultmode.h"

struct RemoteCommand
{
    int16_t frontLeft{};
    int16_t frontRight{};
    int16_t backLeft{};
    int16_t backRight{};
};

class RemoteControlMode : public ModeInterface
{
    using Base = ModeInterface;

public:
    void update() override;

    const char *displayName() const override { return "RemoteControl"; }

    void setCommand(const RemoteCommand &command);

    std::optional<RemoteCommand> m_remoteCommand;
    espchrono::millis_clock::time_point m_timestamp;
};

namespace modes {
extern RemoteControlMode remoteControlMode;
} // namespace modes

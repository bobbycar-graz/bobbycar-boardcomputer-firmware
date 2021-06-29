#pragma once

#include <string>

namespace {
class ModeInterface {
public:
    virtual ~ModeInterface() = default;

    virtual void start() {};
    virtual void update() = 0;
    virtual void stop() {};

    virtual const char *displayName() const = 0;
};
}

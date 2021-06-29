#pragma once

#include "actioninterface.h"

namespace {
class DummyAction : public virtual ActionInterface
{
public:
    void triggered() override {}
};
}

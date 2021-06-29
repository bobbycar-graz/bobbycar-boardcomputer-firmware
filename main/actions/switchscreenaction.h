#pragma once

#include "actioninterface.h"
#include "utils.h"

namespace {
template<typename Tscreen, typename ...Targs>
class SwitchScreenAction : public virtual ActionInterface
{
public:
    void triggered() override { switchScreen<Tscreen>(std::make_unique<Targs>()...); }
};
}

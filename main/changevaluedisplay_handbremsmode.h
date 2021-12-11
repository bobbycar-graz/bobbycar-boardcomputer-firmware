#pragma once

// 3rdparty lib includes
#include "actions/backproxyaction.h"
#include "actions/setvalueaction.h"
#include "changevaluedisplay.h"
#include "icons/back.h"
#include "menudisplay.h"

// local includes
#include "handbremse.h"

namespace espgui
{
template<>
class ChangeValueDisplay<HandbremseMode> :
    public MenuDisplay,
    public virtual AccessorInterface<HandbremseMode>,
    public virtual ActionInterface
{
  using Base = MenuDisplay;

public:
  ChangeValueDisplay();

  void start() override;
};
} // namespace espgui

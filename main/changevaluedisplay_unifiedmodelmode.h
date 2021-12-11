#pragma once

// 3rdparty lib includes
#include "actions/backproxyaction.h"
#include "actions/setvalueaction.h"
#include "changevaluedisplay.h"
#include "icons/back.h"
#include "menudisplay.h"

// local includes
#include "unifiedmodelmode.h"

namespace espgui
{
template<>
class ChangeValueDisplay<UnifiedModelMode> :
    public MenuDisplay,
    public virtual AccessorInterface<UnifiedModelMode>,
    public virtual ActionInterface
{
  using Base = MenuDisplay;

public:
  ChangeValueDisplay();

  void start() override;
};
} // namespace espgui

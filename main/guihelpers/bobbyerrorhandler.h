#pragma once

// 3rdparty lib includes
#include <errorhandlerinterface.h>

namespace bobby {

struct BobbyErrorHandler : public virtual espgui::ErrorHandlerInterface
{
    void errorOccurred(std::string &&error) override;
};

} // namespace bobby

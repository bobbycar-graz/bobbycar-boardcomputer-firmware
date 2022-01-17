#pragma once

// local includes
#include "textinterface.h"

struct CloudCreatedText : public virtual espgui::TextInterface
{
public:
    std::string text() const override;
};

struct CloudStartedText : public virtual espgui::TextInterface
{
public:
    std::string text() const override;
};

struct CloudConnectedText : public virtual espgui::TextInterface
{
public:
    std::string text() const override;
};

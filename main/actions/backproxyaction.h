#pragma once

#include "actioninterface.h"

namespace {
class BackProxyAction : public virtual ActionInterface
{
public:
    BackProxyAction(BackInterface &backInterface) :
        m_backInterface{backInterface}
    {
    }

    void triggered() override
    {
        m_backInterface.back();
    }

private:
    BackInterface &m_backInterface;
};
}

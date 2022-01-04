#ifdef FEATURE_CAN

#include "candebugmenu.h"

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <fmt/core.h>

// 3rdparty lib includes
#include <menuitem.h>
#include <actions/switchscreenaction.h>
#include <actions/dummyaction.h>
#include <icons/back.h>
#include <screenmanager.h>

// local includes
#include "displays/menus/debugmenu.h"

namespace {
constexpr char TAG[] = "CANDEBUG";

constexpr char TEXT_CANDEBUG[] = "CAN Debug";
constexpr char TEXT_BACK[] = "Back";

class CanStatusText : public virtual espgui::TextInterface
{
public:
    explicit CanStatusText(const tl::expected<twai_status_info_t, esp_err_t> &last_can_status_info) :
        m_last_can_status_info{last_can_status_info}
    {
    }

    std::string text() const override
    {
        if (m_last_can_status_info)
            return fmt::format("{}: &f{}", canStatusName(), canStatusText(*m_last_can_status_info));
        else
            return fmt::format("{}: &f&1{}", canStatusName(), esp_err_to_name(m_last_can_status_info.error()));
    }

protected:
    virtual std::string canStatusName() const = 0;
    virtual std::string canStatusText(const twai_status_info_t &can_status_info) const = 0;

private:
    const tl::expected<twai_status_info_t, esp_err_t> &m_last_can_status_info;
};

class CanStatusStateText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "state"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.state); }
};

class CanStatusMsgsToTxText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "msgs_to_tx"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.msgs_to_tx); }
};

class CanStatusMsgsToRxText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "msgs_to_rx"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.msgs_to_rx); }
};

class CanStatusTxErrorCounterText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "&stx_error_counter"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.tx_error_counter); }
};

class CanStatusRxErrorCounterText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "&srx_error_counter"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.rx_error_counter); }
};

class CanStatusTxFailedCountText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "&stx_failed_count"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.tx_failed_count); }
};

class CanStatusRxFailedCountText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "&srx_missed_count"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.rx_missed_count); }
};

class CanStatusRxOverrunCountText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "&srx_overrun_count"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.rx_overrun_count); }
};

class CanStatusArbLostCountText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "&sarb_lost_count"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.arb_lost_count); }
};

class CanStatusBusErrorCountText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "&sbus_error_count"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.bus_error_count); }
};

class CanPendingTxCountText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        unsigned pending{};
        if (const auto result = twai_get_pending_tx(&pending); result == ESP_OK)
            return fmt::format("pending_TX: {}", pending);
        else
            return fmt::format("pending_TX: &1{}", esp_err_to_name(result));
    }
};

class CanPendingRxCountText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        unsigned pending{};
        if (const auto result = twai_get_pending_rx(&pending); result == ESP_OK)
            return fmt::format("pending_RX: {}", pending);
        else
            return fmt::format("pending_RX: &1{}", esp_err_to_name(result));
    }
};
} // namespace

CanDebugMenu::CanDebugMenu()
{
    using namespace espgui;
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusStateText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusMsgsToTxText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusMsgsToRxText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusTxErrorCounterText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusRxErrorCounterText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusTxFailedCountText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusRxFailedCountText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusRxOverrunCountText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusArbLostCountText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusBusErrorCountText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanPendingTxCountText, DummyAction>>();
    constructMenuItem<makeComponentArgs<MenuItem, CanPendingRxCountText, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<DebugMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string CanDebugMenu::text() const
{
    return TEXT_CANDEBUG;
}

void CanDebugMenu::update()
{
    Base::update();

    twai_status_info_t status_info;
    if (const auto result = twai_get_status_info(&status_info); result != ESP_OK)
    {
        ESP_LOGE(TAG, "twai_get_status_info() failed with %s", esp_err_to_name(result));
        m_last_can_status_info = tl::make_unexpected(result);
    }
    else
    {
        ESP_LOGD(TAG, "state=%i msgs_to_tx=%u tx_error_counter=%u tx_failed_count=%u arb_lost_count=%u bus_error_count=%u",
                 status_info.state, status_info.msgs_to_tx, status_info.tx_error_counter,
                 status_info.tx_failed_count, status_info.arb_lost_count, status_info.bus_error_count);
        m_last_can_status_info = status_info;
    }
}

void CanDebugMenu::back()
{
    espgui::switchScreen<DebugMenu>();
}

#endif

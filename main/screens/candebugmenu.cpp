#ifdef FEATURE_CAN

#include "candebugmenu.h"

// system includes
#include <utility>

// esp-idf includes
#include "esp_log.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <fmt/core.h>
#include <menuitem.h>
#include <screenmanager.h>

// local includes
#include "guihelpers/bobbyerrorhandler.h"
#include "icons/back.h"

namespace bobby {

namespace {
constexpr char TAG[] = "CANDEBUG";

constexpr char TEXT_CANDEBUG[] = "CAN Debug";
constexpr char TEXT_TWAI_INITIATE_RECOVERY[] = "twai_initiate_recovery()";
constexpr char TEXT_TWAI_STOP[] = "twai_stop()";
constexpr char TEXT_TWAI_START[] = "twai_start()";
constexpr char TEXT_TWAI_UNINSTALL[] = "twai_drv_uninstall()";
constexpr char TEXT_TWAI_INSTALL[] = "twai_drv_install()";
constexpr char TEXT_BACK[] = "Back";

std::string toString(twai_state_t val)
{
    switch (val)
    {
    case TWAI_STATE_STOPPED:    return "STOPPED";
    case TWAI_STATE_RUNNING:    return "RUNNING";
    case TWAI_STATE_BUS_OFF:    return "BUS_OFF";
    case TWAI_STATE_RECOVERING: return "RECOVERING";
    default:
        ESP_LOGW(TAG, "unknown twai_state_t(%i)", std::to_underlying(val));
        return fmt::format("Unknown twai_state_t({})", std::to_underlying(val));
    }
}

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
    std::string canStatusText(const twai_status_info_t &can_status_info) const override
    {
        return fmt::format("{}{}",
                           can_status_info.state==TWAI_STATE_RUNNING?"":"&1",
                           toString(can_status_info.state));
    }
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

#ifndef OLD_IDF
class CanStatusRxOverrunCountText : public CanStatusText
{
public:
    using CanStatusText::CanStatusText;

protected:
    std::string canStatusName() const override { return "&srx_overrun_count"; }
    std::string canStatusText(const twai_status_info_t &can_status_info) const override { return std::to_string(can_status_info.rx_overrun_count); }
};
#endif

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

class CanInitiateRecoveryAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        const auto result = twai_initiate_recovery();
        ESP_LOGI(TAG, "twai_initiate_recovery() returned %s", esp_err_to_name(result));
        BobbyErrorHandler{}.errorOccurred(fmt::format("twai_initiate_recovery() returned {}", esp_err_to_name(result)));
    }
};

class CanStopAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        const auto result = twai_stop();
        ESP_LOGI(TAG, "twai_stop() returned %s", esp_err_to_name(result));
        BobbyErrorHandler{}.errorOccurred(fmt::format("twai_stop() returned {}", esp_err_to_name(result)));
    }
};

class CanStartAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        const auto result = twai_start();
        ESP_LOGI(TAG, "twai_start() returned %s", esp_err_to_name(result));
        BobbyErrorHandler{}.errorOccurred(fmt::format("twai_start() returned {}", esp_err_to_name(result)));
    }
};

class CanUninstallAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        const auto result = twai_driver_uninstall();
        ESP_LOGI(TAG, "twai_driver_uninstall() returned %s", esp_err_to_name(result));
        BobbyErrorHandler{}.errorOccurred(fmt::format("twai_driver_uninstall() returned {}", esp_err_to_name(result)));
    }
};

class CanInstallAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, TWAI_MODE_NORMAL);
        twai_timing_config_t t_config TWAI_TIMING_CONFIG_250KBITS();
        twai_filter_config_t f_config TWAI_FILTER_CONFIG_ACCEPT_ALL();

        const auto result = twai_driver_install(&g_config, &t_config, &f_config);
        ESP_LOGI(TAG, "twai_driver_install() returned %s", esp_err_to_name(result));
        BobbyErrorHandler{}.errorOccurred(fmt::format("twai_driver_install() returned {}", esp_err_to_name(result)));
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
#ifndef OLD_IDF
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusRxOverrunCountText, DummyAction>>(m_last_can_status_info);
#endif
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusArbLostCountText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponentArgs<MenuItem, CanStatusBusErrorCountText, DummyAction>>(m_last_can_status_info);
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TWAI_INITIATE_RECOVERY>, CanInitiateRecoveryAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TWAI_STOP>, CanStopAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TWAI_START>, CanStartAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TWAI_UNINSTALL>, CanUninstallAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_TWAI_INSTALL>, CanInstallAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
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
        ESP_LOGD(TAG, "state=%i msgs_to_tx=%lu tx_error_counter=%lu tx_failed_count=%lu arb_lost_count=%lu bus_error_count=%lu",
                 status_info.state, status_info.msgs_to_tx, status_info.tx_error_counter,
                 status_info.tx_failed_count, status_info.arb_lost_count, status_info.bus_error_count);
        m_last_can_status_info = status_info;
    }
}

void CanDebugMenu::back()
{
    espgui::popScreen();
}
} // namespace bobby

#endif

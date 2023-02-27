#include "calibratevoltagedisplay.h"

// 3rdparty lib includes
#include <menuitem.h>
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <widgets/label.h>
#include <fmt/core.h>

// Local includes
#include "accessors/settingsaccessors.h"
#include "batterymenu.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "newsettings.h"
#include "utils.h"

namespace bobby {

namespace {
constexpr char TEXT_BATTERY_CALIBRATE[] = "Calibrate Voltages";
constexpr char TEXT_VOLTAGECALIBRATION_30V[] = "Calibrate 30.0V";
constexpr char TEXT_VOLTAGECALIBRATION_50V[] = "Calibrate 50.0V";
constexpr char TEXT_VOLTAGECALIBRATION_VALUE_30_FRONT[] = "30V Front";
constexpr char TEXT_VOLTAGECALIBRATION_VALUE_30_BACK[] = "30V Back";
constexpr char TEXT_VOLTAGECALIBRATION_VALUE_50_FRONT[] = "50V Front";
constexpr char TEXT_VOLTAGECALIBRATION_VALUE_50_BACK[] = "50V Back";
constexpr char TEXT_BATTERY_APPLYCALIB[] = "Apply calibration";
constexpr char TEXT_VOLTAGECALIBRATION_RESET[] = "Reset calibration";
constexpr char TEXT_BACK[] = "Back";

class Save30VCalibrationAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        configs.write_config(configs.battery.front30VoltCalibration, controllers.front.feedback.batVoltage);
        configs.write_config(configs.battery.back30VoltCalibration, controllers.back.feedback.batVoltage);
    }
};

class Save50VCalibrationAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        configs.write_config(configs.battery.front50VoltCalibration, controllers.front.feedback.batVoltage);
        configs.write_config(configs.battery.back50VoltCalibration, controllers.back.feedback.batVoltage);
    }
};

class ResetCalibrationAction : public virtual espgui::ActionInterface
{
public:
    void triggered() override
    {
        configs.reset_config(configs.battery.front30VoltCalibration);
        configs.reset_config(configs.battery.back30VoltCalibration);
        configs.reset_config(configs.battery.front50VoltCalibration);
        configs.reset_config(configs.battery.back50VoltCalibration);
    }
};

float convertToFloat(int16_t value)
{
    return value / 100.f;
}

class BatteryVoltageCalibrationFront30VText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("30V Front: {}", convertToFloat(configs.battery.front30VoltCalibration.value()));
    }
};

class BatteryVoltageCalibrationBack30VText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("30V Back: {}", convertToFloat(configs.battery.back30VoltCalibration.value()));
    }
};

class BatteryVoltageCalibrationFront50VText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("50V Front: {}", convertToFloat(configs.battery.front50VoltCalibration.value()));
    }
};

class BatteryVoltageCalibrationBack50VText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        return fmt::format("50V Back: {}", convertToFloat(configs.battery.back50VoltCalibration.value()));
    }
};

class BatteryVoltageCalibratedText : public virtual espgui::TextInterface
{
public:
    std::string text() const override
    {
        if (configs.battery.applyCalibration.value())
            return fmt::format("F{:.2f}V B{:.2f}", controllers.front.getCalibratedVoltage(), controllers.back.getCalibratedVoltage());
        else
            return "Not activated";
    }
};
} // namespace

CalibrateVoltageDisplay::CalibrateVoltageDisplay()
{
    using namespace espgui;
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_VOLTAGECALIBRATION_30V>,          Save30VCalibrationAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_VOLTAGECALIBRATION_50V>,          Save50VCalibrationAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BATTERY_APPLYCALIB>,              BobbyCheckbox, BatteryApplyCalibrationAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>,                            PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();

    constructMenuItem<makeComponent<MenuItem, EmptyText,                                        DummyAction>>();

    constructMenuItem<makeComponent<MenuItem, BatteryVoltageCalibrationFront30VText,            DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BatteryVoltageCalibrationBack30VText,             DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BatteryVoltageCalibrationFront50VText,            DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BatteryVoltageCalibrationBack50VText,             DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, BatteryVoltageCalibratedText,                     DisabledColor, DummyAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_VOLTAGECALIBRATION_RESET>,        ResetCalibrationAction>>();
}

std::string CalibrateVoltageDisplay::text() const
{
    return TEXT_BATTERY_CALIBRATE;
}

void CalibrateVoltageDisplay::back()
{
    espgui::popScreen();
}
} // namespace bobby

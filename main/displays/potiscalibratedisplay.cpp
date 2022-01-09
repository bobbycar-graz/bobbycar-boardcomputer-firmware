#include "potiscalibratedisplay.h"

// 3rdparty lib includes
#include <tftinstance.h>
#include <screenmanager.h>

// local includes
#include "displays/menus/mainmenu.h"
#include "displays/statusdisplay.h"
#include "displays/menus/boardcomputerhardwaresettingsmenu.h"
#include "newsettings.h"
#include "utils.h"
#include "globals.h"

namespace {
constexpr char TEXT_CALIBRATE[] = "Potis Calibrate";
} // namespace

std::string PotisCalibrateDisplay::text() const
{
    return TEXT_CALIBRATE;
}

void PotisCalibrateDisplay::start()
{
    Base::start();

    m_oldMode = currentMode;
    currentMode = &m_mode;
    m_selectedButton = 0;
    m_status = Status::Begin;
    copyFromSettings();
    m_gas = std::nullopt;
    m_brems = std::nullopt;
}

void PotisCalibrateDisplay::initScreen()
{
    Base::initScreen();

    espgui::tft.setTextFont(4);
    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    espgui::tft.drawString("gas:", 25, 47);
    espgui::tft.drawString("brems:", 25, 147);

    for (auto &label : m_labels)
        label.start();

    for (auto &progressBar : m_progressBars)
        progressBar.start();

    m_renderedButton = -1;
}

void PotisCalibrateDisplay::update()
{
    Base::update();

    if (raw_gas)
        m_gas = cpputils::mapValueClamped<float>(*raw_gas, m_gasMin, m_gasMax, 0., 1000.);
    else
        m_gas = std::nullopt;

    if (raw_brems)
        m_brems = cpputils::mapValueClamped<float>(*raw_brems, m_bremsMin, m_bremsMax, 0., 1000.);
    else
        m_brems = std::nullopt;
}

void PotisCalibrateDisplay::redraw()
{
    Base::redraw();

    m_labels[0].redraw(m_gas ? fmt::format("{:.02f}", *m_gas) : "?");
    m_labels[1].redraw(raw_gas ? std::to_string(*raw_gas) : "?");
    if (m_status == Status::GasMin)
        espgui::tft.setTextColor(TFT_RED, TFT_BLACK);
    m_labels[2].redraw(std::to_string(m_gasMin));
    if (m_status == Status::GasMin)
        espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);
    if (m_status == Status::GasMax)
        espgui::tft.setTextColor(TFT_RED, TFT_BLACK);
    m_labels[3].redraw(std::to_string(m_gasMax));
    if (m_status == Status::GasMax)
        espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    m_progressBars[0].redraw(m_gas ? *m_gas : 0);

    m_labels[4].redraw(m_brems ? fmt::format("{:.02f}", *m_brems) : "?");
    m_labels[5].redraw(raw_brems ? std::to_string(*raw_brems) : "?");
    if (m_status == Status::BremsMin)
        espgui::tft.setTextColor(TFT_RED, TFT_BLACK);
    m_labels[6].redraw(std::to_string(m_bremsMin));
    if (m_status == Status::BremsMin)
        espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);
    if (m_status == Status::BremsMax)
        espgui::tft.setTextColor(TFT_RED, TFT_BLACK);
    m_labels[7].redraw(std::to_string(m_bremsMax));
    if (m_status == Status::BremsMax)
        espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);

    m_progressBars[1].redraw(m_brems ? *m_brems : 0);

    m_labels[8].redraw([&](){
        switch (m_status)
        {
        case Status::Begin: return "Start calibrating?";
#ifdef FEATURE_JOYSTICK
        case Status::Mitte: return "Release joystick";
#endif
        case Status::GasMin: return "Release gas";
        case Status::GasMax: return "Press gas";
        case Status::BremsMin: return "Release brems";
        case Status::BremsMax: return "Press brems";
        case Status::Confirm: return "Verify";
        }
        __builtin_unreachable();
    }());

    {
        const auto failed = !m_gas || !m_brems || (m_status == Status::Confirm && (*m_gas > 100 || *m_brems > 100));
        const auto color = failed ? TFT_DARKGREY : TFT_WHITE;
        espgui::tft.setTextColor(color, TFT_BLACK);
        m_labels[9].redraw([&](){
            switch (m_status)
            {
            case Status::Begin: return "Yes";
#ifdef FEATURE_JOYSTICK
            case Status::Mitte:
#endif
            case Status::GasMin:
            case Status::GasMax:
            case Status::BremsMin:
            case Status::BremsMax: return "Next";
            case Status::Confirm: return "Save";
            }
            __builtin_unreachable();
        }());

        if (m_selectedButton != m_renderedButton && (m_selectedButton == 0 || m_renderedButton == 0))
            espgui::tft.drawRect(3, 275, 100, 27, m_selectedButton == 0 ? color : TFT_BLACK);
    }

    espgui::tft.setTextColor(TFT_WHITE, TFT_BLACK);
    m_labels[10].redraw([&](){
        switch (m_status)
        {
        case Status::Begin: return "No";
#ifdef FEATURE_JOYSTICK
        case Status::Mitte:
#endif
        case Status::GasMin:
        case Status::GasMax:
        case Status::BremsMin:
        case Status::BremsMax:
        case Status::Confirm: return "Abort";
        }
        __builtin_unreachable();
    }());

    if (m_selectedButton != m_renderedButton && (m_selectedButton == 1 || m_renderedButton == 1))
        espgui::tft.drawRect(123, 275, 100, 27, m_selectedButton == 1 ? TFT_WHITE : TFT_BLACK);

    m_renderedButton = m_selectedButton;
}

void PotisCalibrateDisplay::stop()
{
    Base::stop();

    if (currentMode == &m_mode)
    {
        // to avoid crash after deconstruction
        m_mode.stop();
        lastMode = nullptr;

        currentMode = m_oldMode;
    }
}

void PotisCalibrateDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Up:
        m_selectedButton--;

        if (m_selectedButton < 0)
            m_selectedButton = 1;

        break;
    case Button::Down:
        m_selectedButton++;

        if (m_selectedButton > 1)
            m_selectedButton = 0;

        break;
    case Button::Left:
    back:
        switch (m_status)
        {
        case Status::Begin:
            if (m_bootup)
                espgui::switchScreen<StatusDisplay>();
            else if (configs.lockscreen.keepLockedAfterReboot.value && configs.lockscreen.locked.value)
            {
                espgui::switchScreen<MainMenu>();
                configs.write_config(configs.lockscreen.locked, false);
            }
            else
                espgui::switchScreen<BoardcomputerHardwareSettingsMenu>();
            break;
#ifdef FEATURE_JOYSTICK
        case Status::Mitte:
#endif
        case Status::GasMin:
        case Status::GasMax:
        case Status::BremsMin:
        case Status::BremsMax:
        case Status::Confirm:
            m_selectedButton = 0;
            m_status = Status::Begin;
            copyFromSettings();
        }

        break;
    case Button::Right:
        switch (m_selectedButton)
        {
        case 0: // left button pressed
            if (!raw_gas || !raw_brems || !m_gas || !m_brems)
                return;

            switch (m_status)
            {
#ifndef FEATURE_JOYSTICK
            case Status::Begin:
                m_status = Status::GasMin;
                break;
#else
            case Status::Begin:
                m_status = Status::Mitte;
                break;

            case Status::Mitte:
                m_gasMitte = *raw_gas;
                m_bremsMitte = *raw_brems;
                m_status = Status::GasMin;
                break;
#endif
            case Status::GasMin:
                m_gasMin = *raw_gas;
                m_status = Status::GasMax;
                break;
            case Status::GasMax:
                m_gasMax = *raw_gas;
                m_status = Status::BremsMin;
                {
                    const auto dead = (m_gasMax - m_gasMin)/20;
                    m_gasMin += dead;
                    m_gasMax -= dead;
                }
                break;
            case Status::BremsMin:
                m_bremsMin = *raw_brems;
                m_status = Status::BremsMax;
                break;
            case Status::BremsMax:
                m_bremsMax = *raw_brems;
                m_status = Status::Confirm;
                {
                    const auto dead = (m_bremsMax - m_bremsMin)/20;
                    m_bremsMin += dead;
                    m_bremsMax -= dead;
                }
                break;
            case Status::Confirm:
                if (*m_gas > 100 || *m_brems > 100)
                    return;
                copyToSettings();
                saveProfileSettings();
                if (m_bootup)
                    espgui::switchScreen<StatusDisplay>();
                else
                    espgui::switchScreen<BoardcomputerHardwareSettingsMenu>();
            }
            break;
        case 1: // right button pressed
            goto back;
        }
        break;
    }
}

void PotisCalibrateDisplay::copyFromSettings()
{
#ifdef FEATURE_JOYSTICK
    m_gasMitte = configs.gasMitte.value;
    m_bremsMitte = configs.bremsMitte.value;
#endif
    m_gasMin = configs.gasMin.value;
    m_gasMax = configs.gasMax.value;
    m_bremsMin = configs.bremsMin.value;
    m_bremsMax = configs.bremsMax.value;
}

void PotisCalibrateDisplay::copyToSettings()
{
#ifdef FEATURE_JOYSTICK
    configs.write_config(configs.gasMitte, m_gasMitte);
    configs.write_config(configs.bremsMitte, m_bremsMitte);
#endif
    configs.write_config(configs.gasMin, m_gasMin);
    configs.write_config(configs.gasMax, m_gasMax);
    configs.write_config(configs.bremsMin, m_bremsMin);
    configs.write_config(configs.bremsMax, m_bremsMax);
}

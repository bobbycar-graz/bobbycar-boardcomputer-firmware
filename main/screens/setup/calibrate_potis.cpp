#include "calibrate_potis.h"

// 3rdparty lib includes
#include <cpputils.h>
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "screens/setup/ask_setup_clouds.h"
#include "globals.h"
#include "setup.h"

namespace bobby {

using namespace espgui;

void SetupCalibratePotisDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.drawString("gas:", 25, 47, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    tft.drawString("brems:", 25, 147, espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    for (auto &label : m_labels)
        label.start(tft);

    for (auto &progressBar : m_progressBars)
        progressBar.start(tft);

    m_renderedButton = -1;
}

void SetupCalibratePotisDisplay::start()
{
    Base::start();

    setup::lock();

    m_selectedButton = 0;
    m_status = Status::Begin;
    copyFromSettings();
    m_gas = std::nullopt;
    m_brems = std::nullopt;
}
void SetupCalibratePotisDisplay::update()
{
    Base::update();

    if (raw_gas)
        m_gas = cpputils::mapValueClamped<float>(*raw_gas, m_gasMin, m_gasMax, 0.f, 1000.f);
    else
        m_gas = std::nullopt;

    if (raw_brems)
        m_brems = cpputils::mapValueClamped<float>(*raw_brems, m_bremsMin, m_bremsMax, 0.f, 1000.f);
    else
        m_brems = std::nullopt;
}

void SetupCalibratePotisDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    m_labels[0].redraw(tft, m_gas ? fmt::format("{:.02f}", *m_gas) : "?", espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_labels[1].redraw(tft, raw_gas ? std::to_string(*raw_gas) : "?", espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    m_labels[2].redraw(tft, std::to_string(m_gasMin), (m_status == Status::GasMin) ? espgui::TFT_RED : espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_labels[3].redraw(tft, std::to_string(m_gasMax), (m_status == Status::GasMax) ? espgui::TFT_RED : espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    m_progressBars[0].redraw(tft, m_gas ? *m_gas : 0);

    m_labels[4].redraw(tft, m_brems ? fmt::format("{:.02f}", *m_brems) : "?", espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_labels[5].redraw(tft, raw_brems ? std::to_string(*raw_brems) : "?", espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    m_labels[6].redraw(tft, std::to_string(m_bremsMin), (m_status == Status::BremsMin) ? espgui::TFT_RED : espgui::TFT_WHITE, espgui::TFT_BLACK, 4);
    m_labels[7].redraw(tft, std::to_string(m_bremsMax), (m_status == Status::BremsMax) ? espgui::TFT_RED : espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    m_progressBars[1].redraw(tft, m_brems ? *m_brems : 0);

    m_labels[8].redraw(tft, [&](){
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
    }(), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    {
        const auto failed = !m_gas || !m_brems || (m_status == Status::Confirm && (*m_gas > 100 || *m_brems > 100));
        const auto color = failed ? espgui::TFT_DARKGREY : espgui::TFT_WHITE;
        m_labels[9].redraw(tft, [&](){
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
        }(), color, espgui::TFT_BLACK, 4);

        if (m_selectedButton != m_renderedButton && (m_selectedButton == 0 || m_renderedButton == 0))
            tft.drawRect(3, 275, 100, 27, m_selectedButton == 0 ? color : espgui::TFT_BLACK);
    }

    m_labels[10].redraw(tft, [&](){
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
    }(), espgui::TFT_WHITE, espgui::TFT_BLACK, 4);

    if (m_selectedButton != m_renderedButton && (m_selectedButton == 1 || m_renderedButton == 1))
        tft.drawRect(123, 275, 100, 27, m_selectedButton == 1 ? espgui::TFT_WHITE : espgui::TFT_BLACK);

    m_renderedButton = m_selectedButton;
}

void SetupCalibratePotisDisplay::stop()
{
    if (m_early_return)
    {
        setup::unlock();
    }

    Base::stop();
}

void SetupCalibratePotisDisplay::buttonPressed(espgui::Button button)
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
            if (m_early_return)
                espgui::popScreen();
            else
                espgui::switchScreen<SetupAskSetupCloudsDisplay>();
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

                if (m_early_return)
                    espgui::popScreen();
                else
                    espgui::switchScreen<SetupAskSetupCloudsDisplay>();
            }
            break;
        case 1: // right button pressed
            goto back;
        }
        break;
    }
};

std::string SetupCalibratePotisDisplay::text() const
{
    return "Calibrate Potis";
}

void SetupCalibratePotisDisplay::copyFromSettings()
{
#ifdef FEATURE_JOYSTICK
    m_gasMitte = configs.gasMitte.value();
    m_bremsMitte = configs.bremsMitte.value();
#endif
    m_gasMin = configs.gasMin.value();
    m_gasMax = configs.gasMax.value();
    m_bremsMin = configs.bremsMin.value();
    m_bremsMax = configs.bremsMax.value();
}

void SetupCalibratePotisDisplay::copyToSettings() const
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
} // namespace bobby

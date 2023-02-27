#include "basic_buttons.h"

// system includes
#include <esp_log.h>

// 3rdparty lib includes
#include <screenmanager.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "guihelpers/bobbyerrorhandler.h"
#include "screens/setup/calibrate_potis.h"
#include "setup.h"
#include "utils.h"

namespace bobby {

namespace {
constexpr char const buttonText[] = "Please press the highlighted\n buttons!";
constexpr const char * const TAG = "SETUP-BUTTONS";
}

void SetupBasicButtonsDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    drawLargeText(tft, buttonText);
    drawButtons(tft, m_button_cal_status);
}

void SetupBasicButtonsDisplay::start()
{
    Base::start();

    setup::lock();

    m_lastButton = std::nullopt;
    m_button_cal_status = LEFT;
    m_button_cal_finished = false;
}

void SetupBasicButtonsDisplay::update()
{
    if (m_button_cal_finished)
    {
        m_button_cal_finished = false;
        saveButtons();

        setup::unlock();

        if (m_early_return)
        {
            espgui::popScreen();
        }
        else
        {
            espgui::switchScreen<SetupCalibratePotisDisplay>();
        }
    }

    Base::update();
}

void SetupBasicButtonsDisplay::rawButtonPressed(uint8_t button)
{
    if (m_button_cal_status == FINISHED)
    {
        if (button == m_rightButton)
        {
            m_button_cal_finished = true;
        }
    }
    else if (!m_lastButton || *m_lastButton != button)
        m_lastButton = button;
    else
    {
        switch (m_button_cal_status)
        {
        case LEFT:
            m_leftButton = button;
            m_lastButton = std::nullopt;
            m_button_cal_status = RIGHT;
            break;
        case RIGHT:
            m_rightButton = button;
            m_lastButton = std::nullopt;
            m_button_cal_status = UP;
            break;
        case UP:
            m_upButton = button;
            m_lastButton = std::nullopt;
            m_button_cal_status = DOWN;
            break;
        case DOWN:
            m_downButton = button;
            m_lastButton = std::nullopt;
            m_button_cal_status = FINISHED;
            break;
        default:;
        }
    }

    m_button_rerender_needed = true;

    Base::rawButtonPressed(button);
}

void SetupBasicButtonsDisplay::rawButtonReleased(uint8_t button)
{
    // Base::rawButtonReleased(button);
}

void SetupBasicButtonsDisplay::buttonPressed(espgui::Button button)
{
    // Base::buttonPressed(button);
}

void SetupBasicButtonsDisplay::buttonReleased(espgui::Button button)
{
    // Base::buttonReleased(button);
}

std::string SetupBasicButtonsDisplay::text() const
{
    return "Calibrate Buttons";
}

void SetupBasicButtonsDisplay::saveButtons() const
{
    if (auto result = configs.write_config(configs.dpadMappingLeft, m_leftButton); !result)
    {
        BobbyErrorHandler{}.errorOccurred(std::move(result).error());
        return;
    }
    else
    {
        ESP_LOGI(TAG, "Left button set to %d", m_leftButton);
    }

    if (auto result = configs.write_config(configs.dpadMappingRight, m_rightButton); !result)
    {
        BobbyErrorHandler{}.errorOccurred(std::move(result).error());
        return;
    }
    else
    {
        ESP_LOGI(TAG, "Right button set to %d", m_rightButton);
    }
    if (auto result = configs.write_config(configs.dpadMappingUp, m_upButton); !result)
    {
        BobbyErrorHandler{}.errorOccurred(std::move(result).error());
        return;
    }
    else
    {
        ESP_LOGI(TAG, "Up button set to %d", m_upButton);
    }
    if (auto result = configs.write_config(configs.dpadMappingDown, m_downButton); !result)
    {
        BobbyErrorHandler{}.errorOccurred(std::move(result).error());
        return;
    }
    else
    {
        ESP_LOGI(TAG, "Down button set to %d", m_downButton);
    }
}

void SetupBasicButtonsDisplay::redraw(espgui::TftInterface &tft)
{
    if (m_button_rerender_needed)
    {
        m_button_rerender_needed = false;
        drawButtons(tft, m_button_cal_status);
    }
}

void SetupBasicButtonsDisplay::drawButtons(espgui::TftInterface &tft, const SetupBasicButtonsDisplay::CurrentButton button)
{
    using namespace espgui;

    const int16_t x_mid = tft.width() / 2;
    const int16_t y_mid = tft.height() / 2;

    const auto offset = 40;
    const auto radius = 15;
    const auto subtract = 2;

    const auto up_x = x_mid;
    const auto up_y = y_mid - offset;

    const auto down_x = x_mid;
    const auto down_y = y_mid + offset;

    const auto left_x = x_mid - offset;
    const auto left_y = y_mid;

    const auto right_x = x_mid + offset;
    const auto right_y = y_mid;

    tft.fillCircle(up_x,    up_y,    radius-subtract, espgui::TFT_BLACK);
    tft.fillCircle(down_x,  down_y,  radius-subtract, espgui::TFT_BLACK);
    tft.fillCircle(left_x,  left_y,  radius-subtract, espgui::TFT_BLACK);
    tft.fillCircle(right_x, right_y, radius-subtract, espgui::TFT_BLACK);

    tft.drawCircle(up_x,    up_y,    radius, espgui::TFT_WHITE);
    tft.drawCircle(down_x,  down_y,  radius, espgui::TFT_WHITE);
    tft.drawCircle(left_x,  left_y,  radius, espgui::TFT_WHITE);
    tft.drawCircle(right_x, right_y, radius, espgui::TFT_WHITE);

    if (m_button_cal_finished)
    {
        return;
    }

    switch(button)
    {
    case UP:
        tft.fillCircle(up_x, up_y, radius-subtract, m_lastButton ? espgui::TFT_YELLOW : espgui::TFT_WHITE);
        break;
    case DOWN:
        tft.fillCircle(down_x,  down_y,  radius-subtract, m_lastButton ? espgui::TFT_YELLOW : espgui::TFT_WHITE);
        break;
    case LEFT:
        tft.fillCircle(left_x,  left_y,  radius-subtract, m_lastButton ? espgui::TFT_YELLOW : espgui::TFT_WHITE);
        break;
    case RIGHT:
        tft.fillCircle(right_x, right_y, radius-subtract, m_lastButton ? espgui::TFT_YELLOW : espgui::TFT_WHITE);
        break;
    default:;
    }

    if (m_button_cal_status == FINISHED)
    {
        tft.fillCircle(right_x, right_y, radius-subtract, espgui::TFT_GREEN);
    }
}
} // namespace bobby

#include "updatedisplay.h"

// system includes
#include <array>
#include <string>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#include <screenmanager.h>
#include <espasyncota.h>
#include <esp_ota_ops.h>

// local includes
#include "globals.h"
#include "ota.h"
#include "newsettings.h"

void UpdateDisplay::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);

    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString("Update", 5, 5, 4);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.drawString("Status:", 20, m_statusLabel.y());
    m_statusLabel.start(tft);

    tft.drawString("Progress:", 20, m_progressLabel.y());
    m_progressLabel.start(tft);

    tft.drawString("Total:", 20, m_totalLabel.y());
    m_totalLabel.start(tft);

    m_messageLabel.start(tft);

    m_progressBar.start(tft);

    if (const esp_app_desc_t *app_desc = esp_ota_get_app_description())
    {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
        tft.drawString(app_desc->version, 20, 250);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }

    m_newVersionLabel.start(tft);
}

void UpdateDisplay::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);

    if (asyncOta)
    {
        m_statusLabel.redraw(toString(asyncOta->status()));
        const auto progress = asyncOta->progress();
        m_progressLabel.redraw(std::to_string(progress));
        if (const auto totalSize = asyncOta->totalSize(); totalSize && *totalSize > 0)
        {
            m_totalLabel.redraw(std::to_string(*totalSize));
            m_progressBar.redraw(float(progress) / *totalSize * 100);
        }
        else
        {
            m_totalLabel.clear();
            m_progressBar.redraw(0);
        }
        m_messageLabel.redraw(asyncOta->message());

        if (const auto &appDesc = asyncOta->appDesc())
        {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
            m_newVersionLabel.redraw(appDesc->version);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        else
            m_newVersionLabel.clear();
    }
    else
    {
        m_statusLabel.clear();
        m_progressLabel.clear();
        m_totalLabel.clear();
        m_messageLabel.clear();

        m_progressBar.redraw(0);

        m_newVersionLabel.clear();
    }
}

void UpdateDisplay::buttonPressed(espgui::Button button)
{
    Base::buttonPressed(button);

    switch (button)
    {
    using espgui::Button;
    case Button::Left:
        espgui::popScreen();
        break;
    case Button::Right:
        if (const auto result = triggerOta(configs.otaUrl.value()); !result)
            ESP_LOGE("BOBBY", "triggerOta() failed with %.*s", result.error().size(), result.error().data());
        break;
    default:;
    }
}

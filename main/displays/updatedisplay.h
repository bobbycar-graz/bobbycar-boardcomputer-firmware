#pragma once

// system includes
#include <array>
#include <string>

// esp-idf includes
#include <esp_log.h>

// 3rdparty lib includes
#ifdef FEATURE_OTA
#include <espasyncota.h>
#include <esp_ota_ops.h>
#endif

// local includes
#include "display.h"
#include "actions/switchscreenaction.h"
#include "globals.h"
#include "texts.h"
#include "widgets/label.h"
#include "widgets/progressbar.h"
#include "ota.h"

namespace {
class OtaMenu;
}

namespace {
#ifdef FEATURE_OTA
class UpdateDisplay : public Display, public BackActionInterface<SwitchScreenAction<OtaMenu>>
{
public:
    void start() override;
    void initScreen() override;
    void redraw() override;
    void confirm() override;

private:
    Label m_statusLabel{120, 75};
    Label m_progressLabel{120, 100};
    Label m_totalLabel{120, 125};
    Label m_messageLabel{20, 150};

    ProgressBar m_progressBar{20, 200, 200, 10, 0, 100};

    Label m_newVersionLabel{20, 275};
};

void UpdateDisplay::start()
{
}

void UpdateDisplay::initScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(4);
    tft.setTextColor(TFT_YELLOW);

    tft.drawString(TEXT_UPDATE, 5, 5, 4);

    tft.fillRect(0, 34, tft.width(), 3, TFT_WHITE);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.drawString("Status:", 20, m_statusLabel.y());
    m_statusLabel.start();

    tft.drawString("Progress:", 20, m_progressLabel.y());
    m_progressLabel.start();

    tft.drawString("Total:", 20, m_totalLabel.y());
    m_totalLabel.start();

    m_messageLabel.start();

    m_progressBar.start();

    if (const esp_app_desc_t *app_desc = esp_ota_get_app_description())
    {
        tft.setTextColor(TFT_ORANGE, TFT_BLACK);
        tft.drawString(app_desc->version, 20, 250);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }

    m_newVersionLabel.start();
}

void UpdateDisplay::redraw()
{
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

void UpdateDisplay::confirm()
{
    if (const auto result = triggerOta(stringSettings.otaUrl); !result)
        ESP_LOGE("BOBBY", "triggerOta() failed with %.*s", result.error().size(), result.error().data());
}
#endif
}

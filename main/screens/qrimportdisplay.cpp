#include "qrimportdisplay.h"

// 3rdparty lib includes
#include <espwifistack.h>
#include <tftcolors.h>

namespace bobby {

namespace {
constexpr const char * const TAG = "qrimport";
} // namespace

void QrImportDisplay::initScreen(espgui::TftInterface &tft)
{
    using namespace espgui;
    Base::initScreen(tft);

    m_statuslabel.start(tft);

    if (!m_requestStarted)
        start_request();
}

void QrImportDisplay::update()
{
    using namespace espgui;
    Base::update();

    if (!m_waitingForResult)
        return;

    if (qrimport::get_request_running())
        return;

    m_waitingForResult = false;

    m_result = qrimport::check_request();
    if (m_result)
    {
        ESP_LOGI(TAG, "%.*s => %.*s", m_nvs_key.size(), m_nvs_key.data(), m_result->size(), m_result->data());
        if (const auto result = qrimport::set_qr_code(m_nvs_key, *m_result); !result)
            m_result = tl::make_unexpected(fmt::format("saving qr failed: {}", esp_err_to_name(result.error())));
    }
    else
    {
        ESP_LOGW(TAG, "failed %.*s => %.*s", m_nvs_key.size(), m_nvs_key.data(), m_result.error().size(),
                 m_result.error().data());
    }
}

void QrImportDisplay::redraw(espgui::TftInterface &tft)
{
    using namespace espgui;
    Base::redraw(tft);

    if (m_waitingForResult)
    {
        m_statuslabel.redraw(tft, "In progress", TFT_YELLOW, TFT_BLACK, 4);
    }
    else if (!m_result && !m_result.error().empty())
    {
        BobbyErrorHandler{}.errorOccurred(fmt::format("&1Error: {}&6", m_result.error()));
        m_statuslabel.redraw(tft, fmt::format("Error: {}", m_result.error()), TFT_RED, TFT_BLACK, 4);
        m_result.error().clear();
    }
    else if (m_result && !m_result->empty())
    {
        m_statuslabel.redraw(tft, "OK", TFT_GREEN, TFT_BLACK, 4);
        popScreen();
    }
}

void QrImportDisplay::buttonPressed(espgui::Button button)
{
    using namespace espgui;
    Base::buttonPressed(button);

    switch (button)
    {
        using espgui::Button;
    case Button::Left:
        if (!m_waitingForResult)
            popScreen();
        else
            ESP_LOGW(TAG, "tried to leave while waiting for result");
        break;
    default:;
    }
}

void QrImportDisplay::start_request()
{
    if (wifi_stack::get_sta_status() != wifi_stack::WiFiStaStatus::CONNECTED)
    {
        m_result = tl::make_unexpected(std::string{"not connected to wifi"});
        return;
    }

    m_requestStarted = true;

    qrimport::setup_request();

    if (const auto result = qrimport::start_qr_request(); result)
    {
        ESP_LOGI(TAG, "started request, waiting for result");
        m_waitingForResult = true;
    }
    else
    {
        ESP_LOGE(TAG, "could not start request: %.*s", result.error().size(), result.error().data());
        m_result = tl::make_unexpected(result.error());
    }
}
} // namespace bobby

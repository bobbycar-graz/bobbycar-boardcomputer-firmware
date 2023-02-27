#include "greenpassmenu.h"

// system includes
#include <esp_log.h>

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <actions/pushscreenaction.h>
#include <fmt/core.h>

// local includes
#include "actions/qraction.h"
#include "guihelpers/bobbycheckbox.h"
#include "icons/back.h"
#include "qrimport.h"
#include "screens/qrdisplay.h"
#include "screens/qrimportdisplay.h"

namespace bobby {

namespace {
constexpr char TEXT_GREENPASS[] = "Green Pass";
constexpr char TEXT_ADDCERT[] = "Add cert";
constexpr char TEXT_DELCERT[] = "Delete cert mode";
constexpr char TEXT_BACK[] = "Back";

bool deleteMode;
struct DeleteModeAccessor : espgui::RefAccessor<bool> { bool &getRef() const override { return deleteMode; } };

class ErrorMenuItem : public espgui::MenuItem
{
public:
    std::string text() const override { return m_text; }
    void setText(std::string &&text) { m_text = std::move(text); }
    void setText(const std::string &text) { m_text = text; }
    void triggered() override{}
    int color() const override { return TFT_RED; }
private:
    std::string m_text;
};

class CertMenuItem : public espgui::MenuItem
{
public:
    CertMenuItem(qraction::QrMenu qrmenu) : m_qrmenu{qrmenu} {}
    std::string text() const override { return m_qrmenu.text; }
    void triggered() override
    {
        if (deleteMode)
        {
            qrimport::delete_qr_code(m_qrmenu.text);
            espgui::popScreen();
        }
        else
        {
            espgui::pushScreen<QrDisplay>(m_qrmenu.message);
        }
    }
private:
    qraction::QrMenu m_qrmenu;
};

} // namespace

GreenPassMenu::GreenPassMenu()
{
    using namespace espgui;

    for (uint8_t index = 0; index < 4; index++)
    {
        std::string nvs_key = fmt::format("covidcert-{}", index);
        ESP_LOGI("greenpassmenu", "Checking key %s", nvs_key.c_str());
        if (qrimport::has_qr_code(nvs_key))
        {
            if (const auto certTxt = qrimport::get_qr_code(nvs_key); certTxt)
            {
                constructMenuItem<CertMenuItem>(qraction::QrMenu{.message=*certTxt, .text=nvs_key });
            }
            else
            {
                auto &menuitem = constructMenuItem<ErrorMenuItem>();
                menuitem.setText(esp_err_to_name(certTxt.error()));
            }
        }
        else
        {
            constructMenuItem<makeComponentArgs<MenuItem, PushQrImportDisplayAction, StaticText<TEXT_ADDCERT>>>(std::move(nvs_key));
        }
    }

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DELCERT>, BobbyCheckbox, DeleteModeAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string GreenPassMenu::text() const
{
    return TEXT_GREENPASS;
}

void GreenPassMenu::back()
{
    espgui::popScreen();
}

} // namespace bobby

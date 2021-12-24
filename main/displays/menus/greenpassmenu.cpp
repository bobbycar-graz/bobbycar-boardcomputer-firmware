#include "greenpassmenu.h"

// 3rd party includes
#include <esp_err.h>
#include <esp_log.h>

// local includes
#include "actions/switchscreenaction.h"
#include "actions/qraction.h"
#include "actions/dummyaction.h"
#include "actions/toggleboolaction.h"
#include "displays/menus/mainmenu.h"
#include "displays/qrdisplay.h"
#include "displays/qrimportdisplay.h"
#include "icons/back.h"
#include "checkboxicon.h"
#include "utilities/qrimport.h"

using namespace espgui;

namespace {

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
            switchScreen<GreenPassMenu>();
        }
        else
        {
            switchScreen<QrDisplay>(m_qrmenu.message, m_qrmenu.ver);
        }
    }
private:
    qraction::QrMenu m_qrmenu;
};

} // namespace

GreenPassMenu::GreenPassMenu()
{
    for (uint8_t index = 0; index < 4; index++)
    {
        const std::string nvs_key = fmt::format("covidcert-{}", index);
        ESP_LOGI("greenpassmenu", "Checking key %s", nvs_key.c_str());
        if (qrimport::has_qr_code(nvs_key))
        {
            if (const auto certTxt = qrimport::get_qr_code(nvs_key); certTxt)
            {
                constructMenuItem<CertMenuItem>(qraction::QrMenu{.message=*certTxt, .text=nvs_key , .ver=15});
            }
            else
            {
                auto &menuitem = constructMenuItem<ErrorMenuItem>();
                menuitem.setText(esp_err_to_name(certTxt.error()));
            }
        }
        else
        {
            constructMenuItem<makeComponentArgs<MenuItem, SwitchQrImportDisplayAction, StaticText<TEXT_ADDCERT>>>(nvs_key);
        }
    }
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_DELCERT>, ToggleBoolAction, CheckboxIcon, DeleteModeAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<MainMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void GreenPassMenu::back()
{
    switchScreen<MainMenu>();
}

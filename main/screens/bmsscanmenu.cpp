#include "bmsscanmenu.h"

// 3rdparty lib includes
#include <actions/popscreenaction.h>
#include <fmt/format.h>
#include <menuitem.h>
#include <richtextrenderer.h>
#include <screenmanager.h>

// local includes
#include "actions/bmsclearscanaction.h"
#include "actions/bmsscanaction.h"
#include "bmsutils.h"
#include "guihelpers/bobbypopupdisplay.h"
#include "icons/back.h"
#include "newsettings.h"

namespace bobby {

using namespace espgui;

namespace {
constexpr char TEXT_STARTSCAN[] = "Start scan";
constexpr char TEXT_CLEARRESULTS[] = "Clear results";
constexpr char TEXT_BACK[] = "Back";

class BMSScanMenuItem : public MenuItem
{
public:
    BMSScanMenuItem(const scanResult_t &info) : m_info{info} {}

    void setInfo(const scanResult_t &info) { m_info = info; }

    void triggered() override;
    std::string text() const override;

private:
    scanResult_t m_info;
};

constexpr const size_t extraItemsAtBeginning = 2; // Scan and clear
} // namespace

BMSScanMenu::BMSScanMenu()
{
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_STARTSCAN>, BMSScanAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLEARRESULTS>, BMSClearScanAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string BMSScanMenu::text() const
{
    const bool scanStatus = bmsutils::antBms.getScanStatus();
    const auto &results = bmsutils::antBms.getScanResults();

    auto text = fmt::format("Scan {}{}",
                            [&](){
                                if (scanStatus) return "&4";
                                else return "&2";
                            }(),
                            [&](){
                                if (scanStatus) return "Scanning";
                                else return "Finished";
                            }());

    if (!scanStatus && results)
        text += fmt::format("&c ({} found)", results->entries.size());
    else if (!scanStatus && !results)
        text += fmt::format("&c (0 found)");

    return text;
}

void BMSScanMenu::back()
{
    popScreen();
}

void BMSScanMenu::start()
{
    Base::start();

    m_wasScanning = true;
}

void BMSScanMenu::update()
{
    const auto scanStatus = bmsutils::antBms.getScanStatus();

    if (scanStatus && !m_wasScanning)
    {
        m_wasScanning = true;
    }
    else if (!scanStatus && m_wasScanning)
    {
        m_wasScanning = false;

        auto backButton = takeLastMenuItem();

        const auto &result = bmsutils::antBms.getScanResults();

        for (std::size_t i = 0; i < (result ? result->entries.size() : 0); i++)
        {
            if (menuItemCount() - extraItemsAtBeginning <= i)
                constructMenuItem<BMSScanMenuItem>(result->entries[i]);
            else
                ((BMSScanMenuItem*)(&getMenuItem(i + extraItemsAtBeginning)))->setInfo(result->entries[i]);
        }

        while (menuItemCount() - extraItemsAtBeginning > (result ? result->entries.size() : 0))
            takeLastMenuItem();

        emplaceMenuItem(std::move(backButton));
    }

    Base::update();
}

namespace {

void BMSScanMenuItem::triggered()
{
    configs.write_config(configs.bmsAddress, m_info.address);
    popScreen();
}

std::string BMSScanMenuItem::text() const
{
    return fmt::format("{}{}",
                       [&](){
                           if (m_info.name.find("ANT-BLE") != std::string::npos || m_info.address == configs.bmsAddress.value())
                               return "&2";
                           return "";
                       }(),
                       m_info.name.empty() ? m_info.address.toString() : m_info.name
                       );
}

} // namespace
} // namespace bobby

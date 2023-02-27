#include "extrabuttoncalibratemenu.h"

// system includes
#include <esp_log.h>

// 3rd party includes
#include <actions/popscreenaction.h>
#include <menuitem.h>
#include <textwithvaluehelper.h>
#include <tftcolors.h>
#include <tftinterface.h>

// local includes
#include "accessors/settingsaccessors.h"
#include "globals.h"
#include "guihelpers/bobbyerrorhandler.h"
#include "icons/back.h"
#include "screens/boardcomputerhardwaresettingsmenu.h"

namespace bobby {

using namespace espgui;

namespace {
Status currentStatus;

constexpr const char TAG[] = "BUTTON";

constexpr char TEXT_EXTRABUTTONCALIBRATEMENU[] = "Calibrate Buttons";
constexpr char TEXT_BUTTON_LEFT2[]  = "Left2";
constexpr char TEXT_BUTTON_RIGHT2[] = "Right2";
constexpr char TEXT_BUTTON_UP2[]    = "Up2";
constexpr char TEXT_BUTTON_DOWN2[]  = "Down2";

constexpr char TEXT_BUTTON_PROFILE0[] = "Profile0";
constexpr char TEXT_BUTTON_PROFILE1[] = "Profile1";
constexpr char TEXT_BUTTON_PROFILE2[] = "Profile2";
constexpr char TEXT_BUTTON_PROFILE3[] = "Profile3";

constexpr char TEXT_BUTTON_EXTRA1[] = "Extra1";
constexpr char TEXT_BUTTON_EXTRA2[] = "Extra2";
constexpr char TEXT_BUTTON_EXTRA3[] = "Extra3";
constexpr char TEXT_BUTTON_EXTRA4[] = "Extra4";

constexpr char TEXT_BACK[] = "Back";

template<const char *Tprefix, typename Taccessor, Status status>
class ButtonCalibrateMenuItem :
        public MenuItem,
        public TextWithValueHelper<Tprefix, Taccessor>
{
    using StandardText = TextWithValueHelper<Tprefix, Taccessor>;
public:
    void triggered() override
    {
        ESP_LOGI(TAG, "Pressed menu item for %s", Tprefix);
        currentStatus = status;
    }

    int color() const override
    {
        if ((status == currentStatus) || (currentStatus == Idle))
            return espgui::TFT_WHITE;
        else
            return espgui::color565(50,50,50);
    }

    std::string text() const override
    {
        if (status == currentStatus)
        {
            return fmt::format("Press {}", Tprefix);
        }
        else
        {
            return StandardText::text();
        }
    }
};

} // namespace

ExtraButtonCalibrateMenu::ExtraButtonCalibrateMenu()
{
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_LEFT2, ButtonLeft2Accessor, Status::WaitingLeft2>>();
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_RIGHT2, ButtonRight2Accessor, Status::WaitingRight2>>();
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_UP2, ButtonUp2Accessor, Status::WaitingUp2>>();
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_DOWN2, ButtonDown2Accessor, Status::WaitingDown2>>();

    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_PROFILE0, ButtonProfile0Accessor, Status::WaitingProfile0>>();
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_PROFILE1, ButtonProfile1Accessor, Status::WaitingProfile1>>();
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_PROFILE2, ButtonProfile2Accessor, Status::WaitingProfile2>>();
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_PROFILE3, ButtonProfile3Accessor, Status::WaitingProfile3>>();

    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_EXTRA1, ButtonExtra1Accessor, Status::WaitingExtra1>>();
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_EXTRA2, ButtonExtra2Accessor, Status::WaitingExtra2>>();
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_EXTRA3, ButtonExtra3Accessor, Status::WaitingExtra3>>();
    constructMenuItem<ButtonCalibrateMenuItem<TEXT_BUTTON_EXTRA4, ButtonExtra4Accessor, Status::WaitingExtra4>>();

    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

void ExtraButtonCalibrateMenu::start()
{
    Base::start();
    m_oldMode = currentMode;
    currentMode = &m_mode;
    currentStatus = Idle;
}

void ExtraButtonCalibrateMenu::stop()
{
    Base::stop();

    if (currentMode == &m_mode)
    {
        m_mode.stop();
        lastMode = nullptr;
        currentMode = m_oldMode;
    }
}

void ExtraButtonCalibrateMenu::rawButtonPressed(uint8_t button)
{
    if (currentStatus == Idle)
    {
        Base::rawButtonPressed(button);
    }
    else if (validateNewButton(button))
    {
        ESP_LOGI(TAG, "Valid new button: %i", button);
        switch(currentStatus)
        {
        case WaitingUp2:
            if (auto result = configs.write_config(configs.dpadMappingUp2, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingDown2:
            if (auto result = configs.write_config(configs.dpadMappingDown2, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingLeft2:
            if (auto result = configs.write_config(configs.dpadMappingLeft2, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingRight2:
            if (auto result = configs.write_config(configs.dpadMappingRight2, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingProfile0:
            if (auto result = configs.write_config(configs.dpadMappingProfile0, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingProfile1:
            if (auto result = configs.write_config(configs.dpadMappingProfile1, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingProfile2:
            if (auto result = configs.write_config(configs.dpadMappingProfile2, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingProfile3:
            if (auto result = configs.write_config(configs.dpadMappingProfile3, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingExtra1:
            if (auto result = configs.write_config(configs.dpadMappingExtra1, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingExtra2:
            if (auto result = configs.write_config(configs.dpadMappingExtra2, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingExtra3:
            if (auto result = configs.write_config(configs.dpadMappingExtra3, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        case WaitingExtra4:
            if (auto result = configs.write_config(configs.dpadMappingExtra4, button); !result)
            {
                BobbyErrorHandler{}.errorOccurred(std::move(result).error());
                break;
            }
            break;
        default:
            break;
        }
        currentStatus = Idle;
    }
    else
    {
        ESP_LOGE(TAG, "Invalid new button: %i", button);
        currentStatus = Idle;
    }
}

void ExtraButtonCalibrateMenu::buttonPressed(espgui::Button button)
{
    if (currentStatus == Idle)
    {
        Base::buttonPressed(button);
    }
    else
    {
        switch (button)
        {
        case Left:
            ESP_LOGI(TAG, "Canceling procedure");
            currentStatus = Idle;
            break;
        default:
            break;
        }
    }
}

std::string ExtraButtonCalibrateMenu::text() const
{
    return TEXT_EXTRABUTTONCALIBRATEMENU;
}

void ExtraButtonCalibrateMenu::back()
{
    espgui::popScreen();
}

bool ExtraButtonCalibrateMenu::validateNewButton(uint8_t button)
{
    return (
            (button != configs.dpadMappingDown.value()) &&
            (button != configs.dpadMappingUp.value()) &&
            (button != configs.dpadMappingLeft.value()) &&
            (button != configs.dpadMappingRight.value())
    );
}

} // namespace bobby

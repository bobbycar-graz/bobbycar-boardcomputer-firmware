#include "buttons.h"
#include "modes/defaultmode.h"

using namespace std::chrono_literals;

int rotated{};
bool requestFullRedraw{};

bool confirmButtonPressed{};
bool confirmButtonLongPressed{};
bool backButtonPressed{};
bool backButtonLongPressed{};

bool profileButtonDisabled{};

std::optional<espchrono::millis_clock::time_point> upPressedSince;
int upPressRepeat{};
std::optional<espchrono::millis_clock::time_point> downPressedSince;
int downPressRepeat{};

void InputDispatcher::init()
{

}

void InputDispatcher::update()
{
    if (upPressedSince && espchrono::ago(*upPressedSince) > (upPressRepeat > 2 ? 50ms : 400ms))
    {
        upPressedSince = espchrono::millis_clock::now();
        upPressRepeat++;
        rotated -= 1;
    }

    if (downPressedSince && espchrono::ago(*downPressedSince) > (downPressRepeat > 2 ? 50ms : 400ms))
    {
        downPressedSince = espchrono::millis_clock::now();
        downPressRepeat++;
        rotated += 1;
    }
}

void InputDispatcher::rotate(int offset)
{
    rotated += offset;
}

void InputDispatcher::profileButton(uint8_t index, bool pressed)
{
    if (!pressed)
        return;

    if (profileButtonDisabled)
        return;

    settingsutils::switchProfile(index);
}

void InputDispatcher::upButton(bool pressed)
{
    ButtonHandlers[settings.buttonMapping.button3](pressed);
}

void InputDispatcher::downButton(bool pressed)
{
    ButtonHandlers[settings.buttonMapping.button4](pressed);
}

void InputDispatcher::confirmButton(bool pressed)
{
    ButtonHandlers[settings.buttonMapping.button1](pressed);
}

void InputDispatcher::backButton(bool pressed)
{
    ButtonHandlers[settings.buttonMapping.button2](pressed);
}

void InputDispatcher::blinkLeftButton(bool pressed)
{
    ButtonHandlers[settings.buttonMapping.button5](pressed);
}

void InputDispatcher::blinkRightButton(bool pressed)
{
    ButtonHandlers[settings.buttonMapping.button6](pressed);
}

void InputDispatcher::quickActionButtonDown(bool pressed)
{
    ButtonHandlers[settings.buttonMapping.button8](pressed);
}

void InputDispatcher::quickActionButtonUp(bool pressed)
{
    ButtonHandlers[settings.buttonMapping.button7](pressed);
}

// handlers
void InputDispatcher::handleUp(bool pressed)
{
    if (pressed)
    {
        upPressedSince = espchrono::millis_clock::now();
        upPressRepeat = 0;
        rotated -= 1;
    }
    else
    {
        upPressedSince = std::nullopt;
    }
}
void InputDispatcher::handleDown(bool pressed)
{
    if (pressed)
    {
        downPressedSince = espchrono::millis_clock::now();
        downPressRepeat = 0;
        rotated += 1;
    }
    else
    {
        downPressedSince = std::nullopt;
    }
}
void InputDispatcher::handleConfirm(bool pressed)
{
    static espchrono::millis_clock::time_point pressBegin{};

    const auto now = espchrono::millis_clock::now();

    if (pressed)
        pressBegin = now;
    else
    {
        const auto duration = now - pressBegin;

        if (duration < 500ms)
            confirmButtonPressed = true;
        else if (duration < 2000ms)
            confirmButtonLongPressed = true;
        else
            requestFullRedraw = true;

        pressBegin = {};
    }
}
void InputDispatcher::handleBack(bool pressed)
{
    static espchrono::millis_clock::time_point pressBegin{};

    const auto now = espchrono::millis_clock::now();

    if (pressed)
        pressBegin = now;
    else
    {
        const auto duration = now - pressBegin;

        if (duration < 500ms)
            backButtonPressed = true;
        else
            backButtonLongPressed = true;

        pressBegin = {};
    }
}
void InputDispatcher::handleBlinkLeft(bool pressed)
{
    if(!pressed)return;
#ifdef FEATURE_LEDSTRIP
    if(blinkAnimation == LEDSTRIP_OVERWRITE_NONE){ //transition from off to right
        blinkAnimation = LEDSTRIP_OVERWRITE_BLINKRIGHT;
    }
    else if(blinkAnimation == LEDSTRIP_OVERWRITE_BLINKLEFT){ // transition to warning
        blinkAnimation = LEDSTRIP_OVERWRITE_BLINKBOTH;
    }
    else{ // transition to off
        blinkAnimation = LEDSTRIP_OVERWRITE_NONE;
    }
#endif
}
void InputDispatcher::handleBlinkRight(bool pressed)
{
    if(!pressed)return;

#ifdef FEATURE_LEDSTRIP
    if(blinkAnimation == LEDSTRIP_OVERWRITE_NONE){ //transition from off to left
        blinkAnimation = LEDSTRIP_OVERWRITE_BLINKLEFT;
    }
    else if(blinkAnimation == LEDSTRIP_OVERWRITE_BLINKRIGHT){ // transition to warning
        blinkAnimation = LEDSTRIP_OVERWRITE_BLINKBOTH;
    }
    else{ // transition to off
        blinkAnimation = LEDSTRIP_OVERWRITE_NONE;
    }
#endif
}
void InputDispatcher::handleQuickActionUp(bool pressed)
{

}
void InputDispatcher::handleQuickActionDown(bool pressed)
{
    using namespace handbremse;
    if(!pressed)return;

    if (settings.handbremse.enable)
    {
        if (stateWish == StateWish::brake || angezogen)
            stateWish = StateWish::release;
        else
            stateWish = StateWish::brake;
        wishTimer = espchrono::millis_clock::now();
    }
}

// This should have the same order as Button enum in buttons.h
std::array<std::function<void(bool)>, 8> InputDispatcher::ButtonHandlers = {
    &handleConfirm,
    &handleBack,
    &handleUp,
    &handleDown,
    &handleBlinkLeft,
    &handleBlinkRight,
    &handleQuickActionUp,
    &handleQuickActionDown
};

#include "serial_bobby.h"

// local includes
#include "globals.h"
#include "screens.h"

#ifdef FEATURE_SERIAL

void initSerial()
{
    bootLabel.redraw("front Serial begin");
    controllers.front.serial.get().begin(38400, SERIAL_8N1, PINS_RX1, PINS_TX1);

    bootLabel.redraw("back Serial begin");
    controllers.back.serial.get().begin(38400, SERIAL_8N1, PINS_RX2, PINS_TX2);

    bootLabel.redraw("swap front back");
    updateSwapFrontBack();
}

void updateSerial()
{
    for (Controller &controller : controllers)
        controller.parser.update();
}

#endif

#include "modes.h"

// local includes
#include "globals.h"

void initDrivingMode()
{

}

void updateDrivingMode()
{
    if (lastMode != currentMode)
    {
        if (lastMode)
            lastMode->stop();
        lastMode = currentMode;
        if (currentMode)
            currentMode->start();
    }

    if (currentMode)
        currentMode->update();
}

#pragma once

// system includes
#include <bitset>
#include <memory>

// local includes
#include "bobbydisplay.h"

class GameOfLifeDisplay : public BobbyDisplay
{
    using Base = BobbyDisplay;

public:
    void start() override;
    void initScreen() override;
    void redraw() override;
    void stop() override;

    void buttonPressed(espgui::Button button) override;

private:

    //Draws the grid on the display
    void drawGrid();

    //Initialise Grid
    void initGrid();

    // Check the Moore neighborhood
    int getNumberOfNeighbors(int x, int y);

    //Compute the CA. Basically everything related to CA starts here
    void computeCA();

    //static const constexpr auto GRIDX = 80;
    //static const constexpr auto GRIDY = 60;
    //static const constexpr auto CELLXY = 4;

    static const constexpr auto GRIDX = 160;
    static const constexpr auto GRIDY = 120;
    static const constexpr auto CELLXY = 2;

    static const constexpr auto GEN_DELAY = 0;

    int index(int x, int y)
    {
        if (x >= GRIDX)
        {
            //Serial.printf("x: %i\r\n", x);
            return 0;
        }
        if (y >= GRIDY)
        {
            //Serial.printf("y: %i\r\n", x);
            return 0;
        }
        const auto result = (x * GRIDY) + y;
        if (result >= GRIDX*GRIDY)
        {
            //Serial.printf("x:%i y:%i x*y:%i\r\n", x, y, result);
            return 0;
        }
        return result;
    }

    std::unique_ptr<std::bitset<GRIDX*GRIDY>> m_grid;
    std::unique_ptr<std::bitset<GRIDX*GRIDY>> m_newgrid;

    int gen = 0;
};

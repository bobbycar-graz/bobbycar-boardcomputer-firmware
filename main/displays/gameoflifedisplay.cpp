#include "gameoflifedisplay.h"

// 3rdparty lib includes
#include <randomutils.h>
#include <esprandom.h>
#include <tftinstance.h>
#include <screenmanager.h>

void GameOfLifeDisplay::start()
{
    m_grid = std::make_unique<std::bitset<GRIDX*GRIDY>>();
    m_newgrid = std::make_unique<std::bitset<GRIDX*GRIDY>>();
}

void GameOfLifeDisplay::initScreen()
{
    espgui::tft.setRotation(3);
    espgui::tft.fillScreen(TFT_BLACK);
}

void GameOfLifeDisplay::redraw()
{
    if (gen == 0)
    {
        espgui::tft.fillScreen(TFT_BLACK);
        initGrid();
    }

    computeCA();
    drawGrid();

    *m_grid = *m_newgrid;

    if (++gen == 500)
        gen = 0;
}

void GameOfLifeDisplay::stop()
{
    espgui::tft.setRotation(0);
    m_grid = nullptr;
    m_newgrid = nullptr;
}

void GameOfLifeDisplay::confirm()
{

}

void GameOfLifeDisplay::back()
{

}

void GameOfLifeDisplay::drawGrid()
{
    uint16_t color = TFT_WHITE;
    for (int16_t x = 1; x < GRIDX - 1; x++) {
        for (int16_t y = 1; y < GRIDY - 1; y++) {
            if (((*m_grid)[index(x,y)]) != ((*m_newgrid)[index(x,y)])) {
                if ((*m_newgrid)[index(x,y)] == 1)
                    color = 0xFFFF; //random(0xFFFF);
                else
                    color = 0;
                espgui::tft.fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY, color);
            }
        }
    }
}

void GameOfLifeDisplay::initGrid()
{
    for (int16_t x = 0; x < GRIDX; x++) {
        for (int16_t y = 0; y < GRIDY; y++) {
            (*m_newgrid)[index(x,y)] = 0;

            if (x == 0 || x == GRIDX - 1 || y == 0 || y == GRIDY - 1)
                (*m_grid)[index(x,y)] = 0;
            else
            {
                if (cpputils::randomNumber<uint8_t>(4, espcpputils::esp_random_device{}) == 1)
                    (*m_grid)[index(x,y)] = 1;
                else
                    (*m_grid)[index(x,y)] = 0;
            }

        }
    }
}

int GameOfLifeDisplay::getNumberOfNeighbors(int x, int y)
{
    int n{};
    for (auto xOffset : {-1,0,1})
        for (auto yOffset : {-1,0,1})
        {
            if (xOffset == 0 && yOffset == 0)
                continue;

            const auto new_x = x+xOffset;
            const auto new_y = y+yOffset;

            if (new_x >= 0 && new_y >= 0 &&
                new_x < GRIDX && new_y < GRIDY)
                n += (*m_grid)[index(new_x, new_y)];
        }

    return n;
}

void GameOfLifeDisplay::computeCA()
{
    for (int16_t x = 1; x < GRIDX; x++) {
        for (int16_t y = 1; y < GRIDY; y++) {
            int neighbors = getNumberOfNeighbors(x, y);
            if ((*m_grid)[index(x,y)] == true && (neighbors == 2 || neighbors == 3 ))
                (*m_newgrid)[index(x,y)] = true;
            else if ((*m_grid)[index(x,y)] == 1)
                (*m_newgrid)[index(x,y)] = false;
            if ((*m_grid)[index(x,y)] == false && (neighbors == 3))
                (*m_newgrid)[index(x,y)] = true;
            else if ((*m_grid)[index(x,y)] == 0)
                (*m_newgrid)[index(x,y)] = false;
        }
    }
}

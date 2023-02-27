#pragma once

// system includes
#include <string_view>

// 3rdparty lib includes
#include <widgets/label.h>

namespace bobby {

extern espgui::Label bootLabel;

void tft_init();
void initScreen();
void updateDisplay();
void updateRotation();
void redrawDisplay();
void disableScreenFlip(bool enable);

void set_boot_msg(std::string_view msg);
uint16_t getScreenWidth();
uint16_t getScreenHeight();
} // namespace bobby

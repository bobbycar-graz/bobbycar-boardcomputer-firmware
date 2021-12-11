#pragma once

// system includes
#include <optional>

// 3rdparty lib includes
#include <screenmanager.h>
#include <widgets/label.h>

extern espgui::Label bootLabel;

void initScreen();
void updateDisplay();
void redrawDisplay();

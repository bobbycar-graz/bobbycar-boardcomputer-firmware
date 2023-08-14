#include "menudisplaywithtime.h"

// 3rdparty lib includes
#include <fmt/core.h>
#include <fontrenderer.h>

// local includes
#include "utils.h"

using namespace espgui;

namespace bobbygui {
void MenuDisplayWithTime::initScreen(espgui::TftInterface &tft)
{
    Base::initScreen(tft);
    m_label_currentTime.start(tft);
}

void MenuDisplayWithTime::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);
    m_label_currentTime.redraw(tft, fmt::format("&7Time: {}", local_clock_string()), espgui::TFT_WHITE, espgui::TFT_BLACK, use_big_font() ? 4 : 2);
}

} // namespace

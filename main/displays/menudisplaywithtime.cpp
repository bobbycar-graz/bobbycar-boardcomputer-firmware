#include "menudisplaywithtime.h"

// 3rdparty lib includes
#include <fmt/core.h>

// local includes
#include "utils.h"

using namespace espgui;

namespace bobbygui {
void MenuDisplayWithTime::start()
{
    Base::start();
    m_label_currentTime.start();
}

void MenuDisplayWithTime::redraw(espgui::TftInterface &tft)
{
    Base::redraw(tft);
    tft.setTextFont(use_big_font() ? 4 : 2);
    m_label_currentTime.redraw(fmt::format("&7Time: {}", local_clock_string()));
}

} // namespace

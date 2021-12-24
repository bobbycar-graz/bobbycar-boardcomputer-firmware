#include "menudisplaywithtime.h"
#include "tftinstance.h"
#include "utilities/utils.h"

using namespace espgui;

namespace bobbygui {
void MenuDisplayWithTime::start()
{
    Base::start();
    m_label_currentTime.start();
}

void MenuDisplayWithTime::redraw()
{
    Base::redraw();
    tft.setTextFont(use_big_font() ? 4 : 2);
    m_label_currentTime.redraw(fmt::format("&7Time: {}", local_clock_string()));
}

} // namespace

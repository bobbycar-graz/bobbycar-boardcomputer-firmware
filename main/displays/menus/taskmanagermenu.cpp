#include "taskmanagermenu.h"

// 3rdparty lib includes
#include <icons/back.h>
#include <actions/switchscreenaction.h>
#include <actions/dummyaction.h>
#include <schedulertask.h>
#include <fmt/core.h>

// local includes
#include "displays/menus/debugmenu.h"
#include "taskmanager.h"

using namespace espgui;

namespace {

class TaskText : public virtual espgui::TextInterface
{
public:
    TaskText(const espcpputils::SchedulerTask &task) : m_task{task} {}

    std::string text() const override
    {
        const std::string_view name{m_task.name()};
        return fmt::format("{}{}&f &2{} &1{}ms", name.size() > 6 ? "&s" : "", name, m_task.callCount(), m_task.maxElapsed().count());
    }

private:
    const espcpputils::SchedulerTask &m_task;
};
} // namespace

TaskmanagerMenu::TaskmanagerMenu()
{
    for (const auto &task : schedulerTasks)
        constructMenuItem<makeComponentArgs<MenuItem, TaskText, DummyAction>>(task);
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, SwitchScreenAction<DebugMenu>, StaticMenuItemIcon<&espgui::icons::back>>>();
}

void TaskmanagerMenu::back()
{
    switchScreen<DebugMenu>();
}

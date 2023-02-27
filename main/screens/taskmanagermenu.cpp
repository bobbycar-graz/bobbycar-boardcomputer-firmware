#include "taskmanagermenu.h"

// 3rdparty lib includes
#include <actions/dummyaction.h>
#include <actions/popscreenaction.h>
#include <fmt/core.h>
#include <schedulertask.h>

// local includes
#include "icons/back.h"
#include "taskmanager.h"

namespace bobby {

using namespace espgui;
using namespace std::chrono_literals;

namespace {
constexpr char TEXT_TASKMANAGER[] = "Taskmanager";
constexpr char TEXT_BACK[] = "Back";

class TaskText : public virtual espgui::TextInterface
{
public:
    TaskText(const espcpputils::SchedulerTask &task) : m_task{task} {}

    std::string text() const override
    {
        const std::string_view name{m_task.name()};
        return fmt::format("{}{}&f &2{} &1{}ms", name.size() > 6 ? "&s" : "", name, m_task.callCount(), m_task.maxElapsed() / 1ms);
    }

private:
    const espcpputils::SchedulerTask &m_task;
};
} // namespace

TaskmanagerMenu::TaskmanagerMenu()
{
    for (const auto &task : schedulerTasks)
        constructMenuItem<makeComponentArgs<MenuItem, TaskText, DummyAction>>(task);
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&bobbyicons::back>>>();
}

std::string TaskmanagerMenu::text() const
{
    return TEXT_TASKMANAGER;
}

void TaskmanagerMenu::back()
{
    popScreen();
}
} // namespace bobby

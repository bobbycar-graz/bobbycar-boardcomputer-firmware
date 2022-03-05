#include "taskmanagermenu.h"

// 3rdparty lib includes
#include <icons/back.h>
#include <actions/popscreenaction.h>
#include <actions/dummyaction.h>
#include <schedulertask.h>
#include <fmt/core.h>

// local includes
#include "taskmanager.h"

using namespace espgui;

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
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&espgui::icons::back>>>();
}

std::string TaskmanagerMenu::text() const
{
    return TEXT_TASKMANAGER;
}

void TaskmanagerMenu::back()
{
    popScreen();
}

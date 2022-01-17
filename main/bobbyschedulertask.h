#pragma once

// 3rdparty lib includes
#include <schedulertask.h>

class BobbySchedulerTask : public espcpputils::SchedulerTask {
public:
    using SchedulerTask::SchedulerTask;
    void setup() const { SchedulerTask::setup(); m_wasInitialized = true; }
    bool isInitialized() { return m_wasInitialized; }
private:
    mutable bool m_wasInitialized{false};
};

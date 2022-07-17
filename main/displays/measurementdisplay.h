#pragma once

// 3rdparty lib includes
#include <widgets/label.h>
#include <widgets/progressbar.h>
#include <espchrono.h>
#include <optional>

// local includes
#include "bobbydisplaywithtitle.h"
#include "modes/ignoreinputmode.h"
#include "bobbymenudisplay.h"

class MeasurementDisplay : public BobbyMenuDisplay
{
    using Base = BobbyMenuDisplay;

public:
    MeasurementDisplay();

    std::string text() const override;

    void back() override;
    void start() override;
    void update() override;


    void clearMeasurements();

    struct Result {
        std::chrono::milliseconds time;
        float distance;
        float speed;
    };
    std::vector<Result> m_results;

    std::optional<float> distance_at_line; // measurement should start af 0.30m from starting line https://en.wikipedia.org/wiki/Rollout_(drag_racing)
    std::optional<float> distance_at_start;

    std::optional<espchrono::millis_clock::time_point> m_start_time;
    float m_max_speed;
};

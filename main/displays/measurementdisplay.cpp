#include "measurementdisplay.h"

// system includes
#include <algorithm>

// 3rdparty lib includes
#include <menuitem.h>
#include <actions/pushscreenaction.h>
#include <actions/popscreenaction.h>
#include <icons/back.h>
#include <screenmanager.h>
#include <fmt/format.h>
#include <strutils.h>
#include <espchrono.h>

// local includes
#include "displays/bobbychangevaluedisplay.h"
#include "globals.h"
#include "newsettings.h"
#include "drivingstatistics.h"
#include "accessors/settingsaccessors.h"
#include "bobbycheckbox.h"

using namespace espgui;

namespace {
    constexpr char TEXT_INTERVAL[] = "Set interval";
    constexpr char TEXT_INTERVAL_KMH[] = "km/h Measurement Interval";
    constexpr char TEXT_CLEARRESULTS[] = "Start Measurement";
    constexpr char TEXT_ACCELERATION[] = "Show Acceleration";
    constexpr char TEXT_BACK[] = "Back";

    bool showAcceleration;
    struct ShowAccelerationAccessor : espgui::RefAccessor<bool> { bool &getRef() const override { return showAcceleration; } };


    class ClearMeasurementsAction : public virtual espgui::ActionInterface {
    public:
        void triggered() override {
            //TODO: call clearMeasurements()
        }
    };



    class MeasurementDisplayItem : public MenuItem {
    public:
        MeasurementDisplayItem(const MeasurementDisplay::Result result) : m_result{result} {}

        void triggered() override;

        std::string text() const override;

    private:
        MeasurementDisplay::Result m_result;
    };

    void MeasurementDisplayItem::triggered()
    {
        //pushScreen<MeasurementDetails>(m_result);
    }

    std::string MeasurementDisplayItem::text() const
    {
        if(showAcceleration){
            return fmt::format("{}: {}ms {:.2f}m/s² ", m_result.speed, m_result.time.count(),
                               m_result.speed / (m_result.time.count() * 1e-3));
        }else{
            return fmt::format("{}: {}ms {:.2f}m ", m_result.speed, m_result.time.count(), m_result.distance);
        }
    }

    constexpr const size_t extraItemsAtBeginning = 3;

    using MeasurementIntervalChangeScreen = espgui::makeComponent<
            BobbyChangeValueDisplay<uint8_t>,
            espgui::StaticText<TEXT_INTERVAL_KMH>,
            MeasurementIntervalAccessor,
            espgui::ConfirmActionInterface<espgui::PopScreenAction>,
            espgui::BackActionInterface<espgui::PopScreenAction>
    >;
} // namespace


MeasurementDisplay::MeasurementDisplay() {
    clearMeasurements();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_INTERVAL>, PushScreenAction<MeasurementIntervalChangeScreen>>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_CLEARRESULTS>, ClearMeasurementsAction>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_ACCELERATION>, BobbyCheckbox, ShowAccelerationAccessor>>();
    constructMenuItem<makeComponent<MenuItem, StaticText<TEXT_BACK>, PopScreenAction, StaticMenuItemIcon<&icons::back>>>();

}

std::string MeasurementDisplay::text() const {
    return "Measurement Mode";
}

void MeasurementDisplay::back() {
    popScreen();
}

void MeasurementDisplay::start() {
    Base::start();
}

void MeasurementDisplay::update() {
    if (distance_at_line && !distance_at_start) {
        if (drivingStatistics.meters_driven > *distance_at_line + 0.3) {
            //start measurement
            distance_at_start = drivingStatistics.meters_driven;
            m_start_time = espchrono::millis_clock::now();
        }
    }

    //remove excess menu items
    if(menuItemCount() - extraItemsAtBeginning - 1 > m_results.size()) {
        auto backButton = takeLastMenuItem();
        while(menuItemCount() - extraItemsAtBeginning - 1 > m_results.size()) {
            takeLastMenuItem();
        }
        emplaceMenuItem(std::move(backButton));
    }

    //take measurement and add menu items
    while (distance_at_start && avgSpeedKmh + configs.measurementMode.measurementInterval.value() > m_max_speed) {
        m_max_speed += configs.measurementMode.measurementInterval.value();

        Result result = {
                .time = std::chrono::floor<std::chrono::milliseconds>(espchrono::millis_clock::now() - *m_start_time),
                .distance = drivingStatistics.meters_driven - *distance_at_start,
                .speed = m_max_speed
        };
        m_results.push_back(result);

        auto backButton = takeLastMenuItem();
        constructMenuItem<MeasurementDisplayItem>(result);
        emplaceMenuItem(std::move(backButton));
    }

    Base::update();
}

void MeasurementDisplay::clearMeasurements() {
    distance_at_line = drivingStatistics.meters_driven;
    distance_at_start = std::nullopt;
    m_max_speed = 0;
    m_start_time = std::nullopt;
}


#pragma once

#include "textinterface.h"
#include "globals.h"
#include "utils.h"

namespace {
template<typename controller>
struct ControllerTexts
{
    ControllerTexts() = delete;
    ~ControllerTexts() = delete;

    struct BuzzerFreqText : public virtual TextInterface { public: String text() const override { return String{"buzzerFreq: "} + toString(controller::get().command.buzzer.freq); } };
    struct BuzzerPatternText : public virtual TextInterface { public: String text() const override { return String{"buzzerPattern: "} + toString(controller::get().command.buzzer.pattern); } };
    struct PoweroffText : public virtual TextInterface { public: String text() const override { return String{"poweroff: "} + toString(controller::get().command.poweroff); } };
    struct LedText : public virtual TextInterface { public: String text() const override { return String{"led: "} + toString(controller::get().command.led); } };

private:
    struct LeftCommandGetter { static const MotorState &get() { return controller::get().command.left; } };
    struct RightCommandGetter { static const MotorState &get() { return controller::get().command.right; } };

    template<typename MotorStateGetter>
    struct CommandTexts
    {
        CommandTexts() = delete;
        ~CommandTexts() = delete;

        struct EnableText : public virtual TextInterface { public: String text() const override { return String{"enable: "} + toString(MotorStateGetter::get().enable); } };
        struct PwmText : public virtual TextInterface { public: String text() const override { return String{"pwm: "} + toString(MotorStateGetter::get().pwm); } };
        struct CtrlTypText : public virtual TextInterface { public: String text() const override { return String{"ctrlTyp: "} + toString(MotorStateGetter::get().ctrlTyp); } };
        struct CtrlModText : public virtual TextInterface { public: String text() const override { return String{"ctrlMod: "} + toString(MotorStateGetter::get().ctrlMod); } };
        struct IMotMaxText : public virtual TextInterface { public: String text() const override { return String{"iMotMax: "} + toString(MotorStateGetter::get().iMotMax); } };
        struct IDcMaxText : public virtual TextInterface { public: String text() const override { return String{"iDcMax: "} + toString(MotorStateGetter::get().iDcMax); } };
        struct NMotMaxText : public virtual TextInterface { public: String text() const override { return String{"nMotMax: "} + toString(MotorStateGetter::get().nMotMax); } };
        struct FieldWeakMaxText : public virtual TextInterface { public: String text() const override { return String{"fieldWeakMax: "} + toString(MotorStateGetter::get().fieldWeakMax); } };
        struct PhaseAdvMaxText : public virtual TextInterface { public: String text() const override { return String{"phaseAdvMax: "} + toString(MotorStateGetter::get().phaseAdvMax); } };
    };

public:
    using LeftCommand = CommandTexts<LeftCommandGetter>;
    using RightCommand = CommandTexts<LeftCommandGetter>;

    struct BatVoltageText : public virtual TextInterface { public: String text() const override { auto line = String{"batVoltage: "}; if (controller::get().feedbackValid) line += toString(controller::get().feedback.batVoltage); return line; } };
    struct BatVoltageFixedText : public virtual TextInterface { public: String text() const override { auto line = String{"batVoltage: "}; if (controller::get().feedbackValid) line += toString(fixBatVoltage(controller::get().feedback.batVoltage)) + 'V'; return line; } };
    struct BoardTempText : public virtual TextInterface { public: String text() const override { auto line = String{"boardTemp: "}; if (controller::get().feedbackValid) line += toString(controller::get().feedback.boardTemp); return line; } };
    struct BoardTempFixedText : public virtual TextInterface { public: String text() const override { auto line = String{"boardTemp: "}; if (controller::get().feedbackValid) line += toString(fixBoardTemp(controller::get().feedback.boardTemp)) + 'C'; return line; } };
    struct TimeoutCntSerialText : public virtual TextInterface { public: String text() const override { auto line = String{"timeoutCntSerial: "}; if (controller::get().feedbackValid) line += toString(controller::get().feedback.timeoutCntSerial); return line; } };

private:
    struct LeftFeedbackGetter { static const MotorFeedback &get() { return controller::get().feedback.left; } };
    struct RightFeedbackGetter { static const MotorFeedback &get() { return controller::get().feedback.right; } };

    template<typename MotorFeedbackGetter>
    struct FeedbackTexts
    {
        FeedbackTexts() = delete;
        ~FeedbackTexts() = delete;

        struct AngleText : public virtual TextInterface { public: String text() const override { auto line = String{"angle: "}; if (controller::get().feedbackValid) line += toString(MotorFeedbackGetter::get().angle); return line; } };
        struct SpeedText : public virtual TextInterface { public: String text() const override { auto line = String{"speed: "}; if (controller::get().feedbackValid) line += toString(MotorFeedbackGetter::get().speed); return line; } };
        struct SpeedKmhText : public virtual TextInterface { public: String text() const override { auto line = String{"speed kmh: "}; if (controller::get().feedbackValid) line += toString(convertToKmh(MotorFeedbackGetter::get().speed)); return line; } };
        struct ErrorText : public virtual TextInterface { public: String text() const override { auto line = String{"error: "}; if (controller::get().feedbackValid) line += toString(MotorFeedbackGetter::get().error); return line; } };
        struct CurrentText : public virtual TextInterface { public: String text() const override { auto line = String{"current: "}; if (controller::get().feedbackValid) line += toString(MotorFeedbackGetter::get().current); return line; } };
        struct CurrentFixedText : public virtual TextInterface { public: String text() const override { auto line = String{"current: "}; if (controller::get().feedbackValid) line += toString(fixCurrent(MotorFeedbackGetter::get().current)) + 'A'; return line; } };
        struct ChopsText : public virtual TextInterface { public: String text() const override { auto line = String{"chops: "}; if (controller::get().feedbackValid) line += toString(MotorFeedbackGetter::get().chops); return line; } };
        struct HallText : public virtual TextInterface { public: String text() const override { auto line = String{"hall: "}; if (controller::get().feedbackValid) line += hallString(MotorFeedbackGetter::get()); return line; } };
        struct AngleDeltaText : public virtual TextInterface { public: String text() const override { auto line = String{"angleDelta: "}; if (controller::get().feedbackValid) line += toString(MotorFeedbackGetter::get().angleDelta); return line; } };
    };

public:
    using LeftFeedback = FeedbackTexts<LeftFeedbackGetter>;
    using RightFeedback = FeedbackTexts<RightFeedbackGetter>;
};

using FrontTexts = ControllerTexts<FrontControllerGetter>;
using BackTexts = ControllerTexts<BackControllerGetter>;
}

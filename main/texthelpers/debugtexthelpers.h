#pragma once

// 3rdparty lib includes
#include "fmt/core.h"

// local includes
#include "textinterface.h"
#include "globals.h"
#include "utils.h"

namespace {
template<typename controller>
struct ControllerTexts
{
    ControllerTexts() = delete;
    ~ControllerTexts() = delete;

    struct BuzzerFreqText : public virtual TextInterface { public: std::string text() const override { return fmt::format("buzzerFreq: {}", controller::get().command.buzzer.freq); } };
    struct BuzzerPatternText : public virtual TextInterface { public: std::string text() const override { return fmt::format("buzzerPattern: {}", controller::get().command.buzzer.pattern); } };
    struct PoweroffText : public virtual TextInterface { public: std::string text() const override { return fmt::format("poweroff: {}", controller::get().command.poweroff); } };
    struct LedText : public virtual TextInterface { public: std::string text() const override { return fmt::format("led: {}", controller::get().command.led); } };

private:
    struct LeftCommandGetter { static const bobbycar::protocol::serial::MotorState &get() { return controller::get().command.left; } };
    struct RightCommandGetter { static const bobbycar::protocol::serial::MotorState &get() { return controller::get().command.right; } };

    template<typename MotorStateGetter>
    struct CommandTexts
    {
        CommandTexts() = delete;
        ~CommandTexts() = delete;

        struct EnableText : public virtual TextInterface { public: std::string text() const override { return fmt::format("enable: {}", MotorStateGetter::get().enable); } };
        struct PwmText : public virtual TextInterface { public: std::string text() const override { return fmt::format("pwm: {}", MotorStateGetter::get().pwm); } };
        struct CtrlTypText : public virtual TextInterface { public: std::string text() const override { return fmt::format("ctrlTyp: {}", to_string(MotorStateGetter::get().ctrlTyp)); } };
        struct CtrlModText : public virtual TextInterface { public: std::string text() const override { return fmt::format("ctrlMod: {}", to_string(MotorStateGetter::get().ctrlMod)); } };
        struct IMotMaxText : public virtual TextInterface { public: std::string text() const override { return fmt::format("iMotMax: {}", MotorStateGetter::get().iMotMax); } };
        struct IDcMaxText : public virtual TextInterface { public: std::string text() const override { return fmt::format("iDcMax: {}", MotorStateGetter::get().iDcMax); } };
        struct NMotMaxText : public virtual TextInterface { public: std::string text() const override { return fmt::format("nMotMax: {}", MotorStateGetter::get().nMotMax); } };
        struct FieldWeakMaxText : public virtual TextInterface { public: std::string text() const override { return fmt::format("fieldWeakMax: {}", MotorStateGetter::get().fieldWeakMax); } };
        struct PhaseAdvMaxText : public virtual TextInterface { public: std::string text() const override { return fmt::format("phaseAdvMax: {}", MotorStateGetter::get().phaseAdvMax); } };
        struct CruiseCtrlEnaText : public virtual TextInterface { public: std::string text() const override { return fmt::format("cruiseCtrlEna: {}", MotorStateGetter::get().cruiseCtrlEna); } };
        struct NCruiseMotTgtText : public virtual TextInterface { public: std::string text() const override { return fmt::format("nCruiseMotTgt: {}", MotorStateGetter::get().nCruiseMotTgt); } };
    };

public:
    using LeftCommand = CommandTexts<LeftCommandGetter>;
    using RightCommand = CommandTexts<LeftCommandGetter>;

    //struct BatVoltageText : public virtual TextInterface { public: std::string text() const override { std::string line{"batVoltage: "}; if (controller::get().feedbackValid) line += std::to_string(controller::get().feedback.batVoltage); return line; } };
    struct BatVoltageFixedText : public virtual TextInterface { public: std::string text() const override { std::string line{"batVoltage: "}; if (controller::get().feedbackValid) line += fmt::format("{:.2f}V", controller::get().getCalibratedVoltage()); return line; } };
    //struct BoardTempText : public virtual TextInterface { public: std::string text() const override { std::string line{"boardTemp: "}; if (controller::get().feedbackValid) line += std::to_string(controller::get().feedback.boardTemp); return line; } };
    struct BoardTempFixedText : public virtual TextInterface { public: std::string text() const override { std::string line{"boardTemp: "}; if (controller::get().feedbackValid) line += fmt::format("{:.2f}C", fixBoardTemp(controller::get().feedback.boardTemp)); return line; } };
    struct TimeoutCntSerialText : public virtual TextInterface { public: std::string text() const override { std::string line{"timeoutCntSerial: "}; if (controller::get().feedbackValid) line += std::to_string(controller::get().feedback.timeoutCntSerial); return line; } };

private:
    struct LeftFeedbackGetter { static const bobbycar::protocol::serial::MotorFeedback &get() { return controller::get().feedback.left; } };
    struct RightFeedbackGetter { static const bobbycar::protocol::serial::MotorFeedback &get() { return controller::get().feedback.right; } };

    template<typename MotorFeedbackGetter>
    struct FeedbackTexts
    {
        FeedbackTexts() = delete;
        ~FeedbackTexts() = delete;

        struct AngleText : public virtual TextInterface { public: std::string text() const override { std::string line{"angle: "}; if (controller::get().feedbackValid) line += std::to_string(MotorFeedbackGetter::get().angle); return line; } };
        //struct SpeedText : public virtual TextInterface { public: std::string text() const override { std::string line{"speed: "}; if (controller::get().feedbackValid) line += std::to_string(MotorFeedbackGetter::get().speed); return line; } };
        struct SpeedKmhText : public virtual TextInterface { public: std::string text() const override { std::string line{"speed: "}; if (controller::get().feedbackValid) line += fmt::format("{:.2f}kmh", convertToKmh(MotorFeedbackGetter::get().speed)); return line; } };
        struct ErrorText : public virtual TextInterface { public: std::string text() const override { std::string line{"error: "}; if (controller::get().feedbackValid) line += std::to_string(MotorFeedbackGetter::get().error); return line; } };
        //struct DcLinkText : public virtual TextInterface { public: std::string text() const override { std::string line{"dcLink: "}; if (controller::get().feedbackValid) line += std::to_string(MotorFeedbackGetter::get().dcLink); return line; } };
        struct DcLinkFixedText : public virtual TextInterface { public: std::string text() const override { std::string line{"dcLink: "}; if (controller::get().feedbackValid) line += fmt::format("{:.2f}A", fixCurrent(MotorFeedbackGetter::get().dcLink)); return line; } };
        //struct DcPhaAText : public virtual TextInterface { public: std::string text() const override { std::string line{"dcPhaA: "}; if (controller::get().feedbackValid) line += std::to_string(MotorFeedbackGetter::get().dcPhaA); return line; } };
        struct DcPhaAFixedText : public virtual TextInterface { public: std::string text() const override { std::string line{"dcPhaA: "}; if (controller::get().feedbackValid) line += fmt::format("{:.2f}A", fixCurrent(MotorFeedbackGetter::get().dcPhaA)); return line; } };
        //struct DcPhaBText : public virtual TextInterface { public: std::string text() const override { std::string line{"dcPhaB: "}; if (controller::get().feedbackValid) line += std::to_string(MotorFeedbackGetter::get().dcPhaB); return line; } };
        struct DcPhaBFixedText : public virtual TextInterface { public: std::string text() const override { std::string line{"dcPhaB: "}; if (controller::get().feedbackValid) line += fmt::format("{:.2f}A", fixCurrent(MotorFeedbackGetter::get().dcPhaB)); return line; } };
        //struct DcPhaCText : public virtual TextInterface { public: std::string text() const override { std::string line{"dcPhaC: "}; if (controller::get().feedbackValid) line += std::to_string(MotorFeedbackGetter::get().dcPhaC); return line; } };
        struct DcPhaCFixedText : public virtual TextInterface { public: std::string text() const override { std::string line{"dcPhaC: "}; if (controller::get().feedbackValid) line += fmt::format("{:.2f}A", fixCurrent(MotorFeedbackGetter::get().dcPhaC)); return line; } };
        struct ChopsText : public virtual TextInterface { public: std::string text() const override { std::string line{"chops: "}; if (controller::get().feedbackValid) line += std::to_string(MotorFeedbackGetter::get().chops); return line; } };
        struct HallText : public virtual TextInterface { public: std::string text() const override { std::string line{"hall: "}; if (controller::get().feedbackValid) line += hallString(MotorFeedbackGetter::get()); return line; } };
        struct IdText : public virtual TextInterface { public: std::string text() const override { std::string line{"id: "}; if (controller::get().feedbackValid) line += fmt::format("{:.2f}A", fixCurrent(MotorFeedbackGetter::get().id)); return line; } };
        struct IqText : public virtual TextInterface { public: std::string text() const override { std::string line{"iq: "}; if (controller::get().feedbackValid) line += fmt::format("{:.2f}A", fixCurrent(MotorFeedbackGetter::get().iq)); return line; } };
    };

public:
    using LeftFeedback = FeedbackTexts<LeftFeedbackGetter>;
    using RightFeedback = FeedbackTexts<RightFeedbackGetter>;
};

using FrontTexts = ControllerTexts<FrontControllerGetter>;
using BackTexts = ControllerTexts<BackControllerGetter>;
}

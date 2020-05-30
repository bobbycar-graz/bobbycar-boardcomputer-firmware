#pragma once

#include "textinterface.h"
#include "globals.h"
#include "utils.h"

#ifdef GLUMP_CONTROLLER
namespace {
template<Controller &controller>
struct ControllerTexts
{
    ControllerTexts() = delete;
    ~ControllerTexts() = delete;

    struct BuzzerFreqText : public virtual TextInterface { public: String text() const override { return String{"buzzerFreq: "} + toString(controller.command.buzzer.freq); } };
    struct BuzzerPatternText : public virtual TextInterface { public: String text() const override { return String{"buzzerPattern: "} + toString(controller.command.buzzer.pattern); } };
    struct PoweroffText : public virtual TextInterface { public: String text() const override { return String{"poweroff: "} + toString(controller.command.poweroff); } };
    struct LedText : public virtual TextInterface { public: String text() const override { return String{"led: "} + toString(controller.command.led); } };

    struct LeftCommand
    {
        LeftCommand() = delete;
        ~LeftCommand() = delete;

        struct EnableText : public virtual TextInterface { public: String text() const override { return String{"enable: "} + toString(controller.command.left.enable); } };
        struct PwmText : public virtual TextInterface { public: String text() const override { return String{"pwm: "} + toString(controller.command.left.pwm); } };
        struct CtrlTypText : public virtual TextInterface { public: String text() const override { return String{"ctrlTyp: "} + toString(controller.command.left.ctrlTyp); } };
        struct CtrlModText : public virtual TextInterface { public: String text() const override { return String{"ctrlMod: "} + toString(controller.command.left.ctrlMod); } };
        struct IMotMaxText : public virtual TextInterface { public: String text() const override { return String{"iMotMax: "} + toString(controller.command.left.iMotMax); } };
        struct IDcMaxText : public virtual TextInterface { public: String text() const override { return String{"iDcMax: "} + toString(controller.command.left.iDcMax); } };
        struct NMotMaxText : public virtual TextInterface { public: String text() const override { return String{"nMotMax: "} + toString(controller.command.left.nMotMax); } };
        struct FieldWeakMaxText : public virtual TextInterface { public: String text() const override { return String{"fieldWeakMax: "} + toString(controller.command.left.fieldWeakMax); } };
        struct PhaseAdvMaxText : public virtual TextInterface { public: String text() const override { return String{"phaseAdvMax: "} + toString(controller.command.left.phaseAdvMax); } };
    };

    struct RightCommand
    {
        RightCommand() = delete;
        ~RightCommand() = delete;

        struct EnableText : public virtual TextInterface { public: String text() const override { return String{"enable: "} + toString(controller.command.right.enable); } };
        struct PwmText : public virtual TextInterface { public: String text() const override { return String{"pwm: "} + toString(controller.command.right.pwm); } };
        struct CtrlTypText : public virtual TextInterface { public: String text() const override { return String{"ctrlTyp: "} + toString(controller.command.right.ctrlTyp); } };
        struct CtrlModText : public virtual TextInterface { public: String text() const override { return String{"ctrlMod: "} + toString(controller.command.right.ctrlMod); } };
        struct IMotMaxText : public virtual TextInterface { public: String text() const override { return String{"iMotMax: "} + toString(controller.command.right.iMotMax); } };
        struct IDcMaxText : public virtual TextInterface { public: String text() const override { return String{"iDcMax: "} + toString(controller.command.right.iDcMax); } };
        struct NMotMaxText : public virtual TextInterface { public: String text() const override { return String{"nMotMax: "} + toString(controller.command.right.nMotMax); } };
        struct FieldWeakMaxText : public virtual TextInterface { public: String text() const override { return String{"fieldWeakMax: "} + toString(controller.command.right.fieldWeakMax); } };
        struct PhaseAdvMaxText : public virtual TextInterface { public: String text() const override { return String{"phaseAdvMax: "} + toString(controller.command.right.phaseAdvMax); } };
    };

    struct BatVoltageText : public virtual TextInterface { public: String text() const override { auto line = String{"batVoltage: "}; if (controller.feedbackValid) line += toString(controller.feedback.batVoltage); return line; } };
    struct BatVoltageFixedText : public virtual TextInterface { public: String text() const override { auto line = String{"batVoltage: "}; if (controller.feedbackValid) line += toString(fixBatVoltage(controller.feedback.batVoltage)) + 'V'; return line; } };
    struct BoardTempText : public virtual TextInterface { public: String text() const override { auto line = String{"boardTemp: "}; if (controller.feedbackValid) line += toString(controller.feedback.boardTemp); return line; } };
    struct BoardTempFixedText : public virtual TextInterface { public: String text() const override { auto line = String{"boardTemp: "}; if (controller.feedbackValid) line += toString(fixBoardTemp(controller.feedback.boardTemp)) + 'C'; return line; } };
    struct TimeoutCntSerialText : public virtual TextInterface { public: String text() const override { auto line = String{"timeoutCntSerial: "}; if (controller.feedbackValid) line += toString(controller.feedback.timeoutCntSerial); return line; } };

    struct LeftFeedback
    {
        LeftFeedback() = delete;
        ~LeftFeedback() = delete;

        struct AngleText : public virtual TextInterface { public: String text() const override { auto line = String{"angle: "}; if (controller.feedbackValid) line += toString(controller.feedback.left.angle); return line; } };
        struct SpeedText : public virtual TextInterface { public: String text() const override { auto line = String{"speed: "}; if (controller.feedbackValid) line += toString(controller.feedback.left.speed); return line; } };
        struct SpeedKmhText : public virtual TextInterface { public: String text() const override { auto line = String{"speed kmh: "}; if (controller.feedbackValid) line += toString(convertToKmh(controller.feedback.left.speed)); return line; } };
        struct ErrorText : public virtual TextInterface { public: String text() const override { auto line = String{"error: "}; if (controller.feedbackValid) line += toString(controller.feedback.left.error); return line; } };
        struct CurrentText : public virtual TextInterface { public: String text() const override { auto line = String{"current: "}; if (controller.feedbackValid) line += toString(controller.feedback.left.current); return line; } };
        struct CurrentFixedText : public virtual TextInterface { public: String text() const override { auto line = String{"current: "}; if (controller.feedbackValid) line += toString(fixCurrent(controller.feedback.left.current)) + 'A'; return line; } };
        struct ChopsText : public virtual TextInterface { public: String text() const override { auto line = String{"chops: "}; if (controller.feedbackValid) line += toString(controller.feedback.left.chops); return line; } };
        struct HallText : public virtual TextInterface { public: String text() const override { auto line = String{"hall: "}; if (controller.feedbackValid) line += hallString(controller.feedback.left); return line; } };
    };

    struct RightFeedback
    {
        RightFeedback() = delete;
        ~RightFeedback() = delete;

        struct AngleText : public virtual TextInterface { public: String text() const override { auto line = String{"angle: "}; if (controller.feedbackValid) line += toString(controller.feedback.right.angle); return line; } };
        struct SpeedText : public virtual TextInterface { public: String text() const override { auto line = String{"speed: "}; if (controller.feedbackValid) line += toString(controller.feedback.right.speed); return line; } };
        struct SpeedKmhText : public virtual TextInterface { public: String text() const override { auto line = String{"speed kmh: "}; if (controller.feedbackValid) line += toString(convertToKmh(controller.feedback.right.speed)); return line; } };
        struct ErrorText : public virtual TextInterface { public: String text() const override { auto line = String{"error: "}; if (controller.feedbackValid) line += toString(controller.feedback.right.error); return line; } };
        struct CurrentText : public virtual TextInterface { public: String text() const override { auto line = String{"current: "}; if (controller.feedbackValid) line += toString(controller.feedback.right.current); return line; } };
        struct CurrentFixedText : public virtual TextInterface { public: String text() const override { auto line = String{"current: "}; if (controller.feedbackValid) line += toString(fixCurrent(controller.feedback.right.current)) + 'A'; return line; } };
        struct ChopsText : public virtual TextInterface { public: String text() const override { auto line = String{"chops: "}; if (controller.feedbackValid) line += toString(controller.feedback.right.chops); return line; } };
        struct HallText : public virtual TextInterface { public: String text() const override { auto line = String{"hall: "}; if (controller.feedbackValid) line += hallString(controller.feedback.right); return line; } };
    };
};

using FrontTexts = ControllerTexts<front>;
using BackTexts = ControllerTexts<back>;
}
#endif

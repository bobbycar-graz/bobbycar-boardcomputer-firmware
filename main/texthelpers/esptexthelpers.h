#pragma once

// esp-idf includes
#include "esp_chip_info.h"
#include "esp_heap_caps.h"
#include "esp_idf_version.h"

// 3rdparty lib includes
#include "espstrutils.h"
#include "fmt/core.h"

// local includes
#include "can.h"
#include "globals.h"
#include "textinterface.h"
#include "utils.h"

//template<const char *Ttext, typename TreturnType, TreturnType (EspClass::*Tmethod)()>
//using EspStatusTextHelper = StatusTextHelper<Ttext, EspClass, &ESP, TreturnType, Tmethod>;

class HeapTotal8Text : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("HeapTotal8: {}", heap_caps_get_total_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)); }};

class HeapFree8Text : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("HeapFree8: {}", heap_caps_get_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)); }};

class HeapMinFree8Text : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("HeapMinFree8: {}", heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)); }};

class HeapLargest8Text : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("HeapLargest8: {}", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)); }};

class HeapTotal32Text : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("HeapTotal32: {}", heap_caps_get_total_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_32BIT)); }};

class HeapFree32Text : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("HeapFree32: {}", heap_caps_get_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_32BIT)); }};

class HeapMinFree32Text : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("HeapMinFree32: {}", heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL|MALLOC_CAP_32BIT)); }};

class HeapLargest32Text : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("HeapLargest32: {}", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL|MALLOC_CAP_32BIT)); }};

class LastRebootReasonText : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("Last Reboot Reason: {}", espcpputils::toString(esp_reset_reason())); }};

#ifdef FEATURE_CAN
class CanIcCrashText : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("CAN IC reseted: {}", can::can_total_error_cnt); }};
#endif

constexpr char TEXT_ESPCHIPREVISION[] = "Chip revision: ";
class EspChipRevisionText : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("{}{}", TEXT_ESPCHIPREVISION, chip_info.revision);
}};

constexpr char TEXT_ESPCHIPMODEL[] = "Chip Model: ";
class EspChipModelText : public virtual espgui::TextInterface { public: std::string text() const override {
        return fmt::format("{}{}", TEXT_ESPCHIPMODEL, toString(chip_info.model));
    }};

constexpr char TEXT_ESPMAXCPUFREQMHZ[] = "Cpu max freq MHz: ";
class EspCpuMaxFreqMHzText : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("{}{}", TEXT_ESPMAXCPUFREQMHZ, pm_config.max_freq_mhz);
}};

constexpr char TEXT_ESPMINCPUFREQMHZ[] = "Cpu max freq MHz: ";
class EspCpuMinFreqMHzText : public virtual espgui::TextInterface { public: std::string text() const override {
        return fmt::format("{}{}", TEXT_ESPMINCPUFREQMHZ, pm_config.min_freq_mhz);
}};

constexpr char TEXT_ESPSDKVERSION[] = "Sdk version: ";
class EspSdkVersionText : public virtual espgui::TextInterface { public: std::string text() const override {
    return fmt::format("{}{}", TEXT_ESPSDKVERSION, esp_get_idf_version());
}};

constexpr char TEXT_ESPFLASHCHIPSIZE[] = "Flash chip size: ";
using EspFlashChipSizeText = espgui::StaticText<TEXT_ESPFLASHCHIPSIZE>; //EspStatusTextHelper<TEXT_ESPFLASHCHIPSIZE, uint32_t, &EspClass::getFlashChipSize>;

constexpr char TEXT_ESPFLASHCHIPSPEED[] = "Flash chip speed: ";
using EspFlashChipSpeedText = espgui::StaticText<TEXT_ESPFLASHCHIPSPEED>; //EspStatusTextHelper<TEXT_ESPFLASHCHIPSPEED, uint32_t, &EspClass::getFlashChipSpeed>;

constexpr char TEXT_ESPFLASHCHIPMODE[] = "Flash chip mode: ";
using EspFlashChipModeText = espgui::StaticText<TEXT_ESPFLASHCHIPMODE>; //EspStatusTextHelper<TEXT_ESPFLASHCHIPMODE, FlashMode_t, &EspClass::getFlashChipMode>; // TODO: improve stringifying

constexpr char TEXT_ESPSKETCHSIZE[] = "Sketch size: ";
using EspSketchSizeText = espgui::StaticText<TEXT_ESPSKETCHSIZE>; //StaticallyCachedText<EspStatusTextHelper<TEXT_ESPSKETCHSIZE, uint32_t, &EspClass::getSketchSize>>; // caching because of slow

constexpr char TEXT_ESPSKETCHMD5[] = "Sketch MD5: ";
using EspSketchMd5Text = espgui::StaticText<TEXT_ESPSKETCHMD5>; //StaticallyCachedText<EspStatusTextHelper<TEXT_ESPSKETCHMD5, String, &EspClass::getSketchMD5>>; // caching because of slow

constexpr char TEXT_ESPFREESKETCHSPACE[] = "Free sketch space: ";
using EspFreeSketchSpaceText = espgui::StaticText<TEXT_ESPFREESKETCHSPACE>; //EspStatusTextHelper<TEXT_ESPFREESKETCHSPACE, uint32_t, &EspClass::getFreeSketchSpace>;


#pragma once

// esp-idf includes
#include <esp_heap_caps.h>

// 3rdparty lib includes
#include <fmt/core.h>
#include <espstrutils.h>

// local includes
#include "can.h"
#include "textinterface.h"

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
using EspChipRevisionText = espgui::StaticText<TEXT_ESPCHIPREVISION>; //EspStatusTextHelper<TEXT_ESPCHIPREVISION, uint8_t, &EspClass::getChipRevision>;

constexpr char TEXT_ESPCPUFREQMHZ[] = "Cpu freq MHz: ";
using EspCpuFreqMHzText = espgui::StaticText<TEXT_ESPCPUFREQMHZ>; //EspStatusTextHelper<TEXT_ESPCPUFREQMHZ, uint32_t, &EspClass::getCpuFreqMHz>;

constexpr char TEXT_ESPCYCLECOUNT[] = "Cycle count: ";
using EspCycleCountText = espgui::StaticText<TEXT_ESPCYCLECOUNT>; //EspStatusTextHelper<TEXT_ESPCYCLECOUNT, uint32_t, &EspClass::getCycleCount>;

constexpr char TEXT_ESPSDKVERSION[] = "Sdk version: ";
using EspSdkVersionText = espgui::StaticText<TEXT_ESPSDKVERSION>; //EspStatusTextHelper<TEXT_ESPSDKVERSION, const char *, &EspClass::getSdkVersion>;

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


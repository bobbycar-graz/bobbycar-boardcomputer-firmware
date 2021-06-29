#pragma once

#include "textinterface.h"

namespace {
//template<const char *Ttext, typename TreturnType, TreturnType (EspClass::*Tmethod)()>
//using EspStatusTextHelper = StatusTextHelper<Ttext, EspClass, &ESP, TreturnType, Tmethod>;

constexpr char TEXT_ESPHEAPSIZE[] = "Heap size: ";
using EspHeapSizeText = StaticText<TEXT_ESPHEAPSIZE>; //EspStatusTextHelper<TEXT_ESPHEAPSIZE, uint32_t, &EspClass::getHeapSize>;

constexpr char TEXT_ESPFREEHEAP[] = "Free heap: ";
using EspFreeHeapText = StaticText<TEXT_ESPFREEHEAP>; //EspStatusTextHelper<TEXT_ESPFREEHEAP, uint32_t, &EspClass::getFreeHeap>;

constexpr char TEXT_ESPMINFREEHEAP[] = "Min free heap: ";
using EspMinFreeHeapText = StaticText<TEXT_ESPMINFREEHEAP>; //EspStatusTextHelper<TEXT_ESPMINFREEHEAP, uint32_t, &EspClass::getMinFreeHeap>;

constexpr char TEXT_ESPMAXALLOCHEAP[] = "Max alloc heap: ";
using EspMaxAllocHeapText = StaticText<TEXT_ESPMAXALLOCHEAP>; //EspStatusTextHelper<TEXT_ESPMAXALLOCHEAP, uint32_t, &EspClass::getMaxAllocHeap>;

constexpr char TEXT_ESPPSRAMSIZE[] = "psram size: ";
using EspPsramSizeText = StaticText<TEXT_ESPPSRAMSIZE>; //EspStatusTextHelper<TEXT_ESPPSRAMSIZE, uint32_t, &EspClass::getPsramSize>;

constexpr char TEXT_ESPFREEPSRAM[] = "Free psram: ";
using EspFreePsramText = StaticText<TEXT_ESPFREEPSRAM>; //EspStatusTextHelper<TEXT_ESPFREEPSRAM, uint32_t, &EspClass::getFreePsram>;

constexpr char TEXT_ESPMINFREEPSRAM[] = "Min free psram: ";
using EspMinFreePsramText = StaticText<TEXT_ESPMINFREEPSRAM>; //EspStatusTextHelper<TEXT_ESPMINFREEPSRAM, uint32_t, &EspClass::getMinFreePsram>;

constexpr char TEXT_ESPMAXALLOCPSRAM[] = "Max alloc psram: ";
using EspMaxAllocPsramText = StaticText<TEXT_ESPMAXALLOCPSRAM>; //EspStatusTextHelper<TEXT_ESPMAXALLOCPSRAM, uint32_t, &EspClass::getMaxAllocPsram>;

constexpr char TEXT_ESPCHIPREVISION[] = "Chip revision: ";
using EspChipRevisionText = StaticText<TEXT_ESPCHIPREVISION>; //EspStatusTextHelper<TEXT_ESPCHIPREVISION, uint8_t, &EspClass::getChipRevision>;

constexpr char TEXT_ESPCPUFREQMHZ[] = "Cpu freq MHz: ";
using EspCpuFreqMHzText = StaticText<TEXT_ESPCPUFREQMHZ>; //EspStatusTextHelper<TEXT_ESPCPUFREQMHZ, uint32_t, &EspClass::getCpuFreqMHz>;

constexpr char TEXT_ESPCYCLECOUNT[] = "Cycle count: ";
using EspCycleCountText = StaticText<TEXT_ESPCYCLECOUNT>; //EspStatusTextHelper<TEXT_ESPCYCLECOUNT, uint32_t, &EspClass::getCycleCount>;

constexpr char TEXT_ESPSDKVERSION[] = "Sdk version: ";
using EspSdkVersionText = StaticText<TEXT_ESPSDKVERSION>; //EspStatusTextHelper<TEXT_ESPSDKVERSION, const char *, &EspClass::getSdkVersion>;

constexpr char TEXT_ESPFLASHCHIPSIZE[] = "Flash chip size: ";
using EspFlashChipSizeText = StaticText<TEXT_ESPFLASHCHIPSIZE>; //EspStatusTextHelper<TEXT_ESPFLASHCHIPSIZE, uint32_t, &EspClass::getFlashChipSize>;

constexpr char TEXT_ESPFLASHCHIPSPEED[] = "Flash chip speed: ";
using EspFlashChipSpeedText = StaticText<TEXT_ESPFLASHCHIPSPEED>; //EspStatusTextHelper<TEXT_ESPFLASHCHIPSPEED, uint32_t, &EspClass::getFlashChipSpeed>;

constexpr char TEXT_ESPFLASHCHIPMODE[] = "Flash chip mode: ";
using EspFlashChipModeText = StaticText<TEXT_ESPFLASHCHIPMODE>; //EspStatusTextHelper<TEXT_ESPFLASHCHIPMODE, FlashMode_t, &EspClass::getFlashChipMode>; // TODO: improve stringifying

constexpr char TEXT_ESPSKETCHSIZE[] = "Sketch size: ";
using EspSketchSizeText = StaticText<TEXT_ESPSKETCHSIZE>; //StaticallyCachedText<EspStatusTextHelper<TEXT_ESPSKETCHSIZE, uint32_t, &EspClass::getSketchSize>>; // caching because of slow

constexpr char TEXT_ESPSKETCHMD5[] = "Sketch MD5: ";
using EspSketchMd5Text = StaticText<TEXT_ESPSKETCHMD5>; //StaticallyCachedText<EspStatusTextHelper<TEXT_ESPSKETCHMD5, String, &EspClass::getSketchMD5>>; // caching because of slow

constexpr char TEXT_ESPFREESKETCHSPACE[] = "Free sketch space: ";
using EspFreeSketchSpaceText = StaticText<TEXT_ESPFREESKETCHSPACE>; //EspStatusTextHelper<TEXT_ESPFREESKETCHSPACE, uint32_t, &EspClass::getFreeSketchSpace>;
}

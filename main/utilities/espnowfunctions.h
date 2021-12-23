#pragma once
#ifdef FEATURE_ESPNOW
#include <cstdint>
#include <deque>
#include <string>
#include <string_view>
#include <vector>
#include <esp_now.h>

namespace espnow {
extern uint16_t lastYear;
constexpr const uint8_t broadcast_address[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
struct esp_now_message_t
{
    std::string content;
    std::string type;
};

extern bool receiveTimeStamp;
extern bool receiveTsFromOtherBobbycars;

extern std::deque<esp_now_message_t> message_queue;
extern std::vector<esp_now_peer_info_t> peers;

void initESPNow();
void handle();
void onRecvTs(uint64_t millis, bool isFromBobbycar = false);
esp_err_t send_espnow_message(std::string_view message);
} // namespace espnow
#endif

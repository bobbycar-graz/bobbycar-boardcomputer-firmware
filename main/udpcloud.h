#pragma once

// system includes
#include <string>

#ifdef FEATURE_UDPCLOUD
// Little "flash" on statusdisplay when udp stuff is happening
extern bool visualSendUdpPacket;

void udpCloudInit();
void udpCloudUpdate();

void spamUdpBroadcast();
std::string buildUdpCloudJson();
std::string buildUdpCloudString();
void sendUdpCloudPacket();
#endif

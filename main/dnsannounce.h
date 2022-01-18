#pragma once

// system includes
#include <string>

extern std::string dns_lastIpAddress_v4;
extern std::string dns_lastIpAddress_v6;
extern std::string dns_lastIpAddress_v6_global;

void init_dns_announce();
void handle_dns_announce();

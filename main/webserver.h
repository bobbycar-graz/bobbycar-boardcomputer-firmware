#pragma once

// esp-idf includes
#include <esp_http_server.h>

extern httpd_handle_t httpdHandle;

void initWebserver();
void handleWebserver();
bool MenuDisplayChanged();

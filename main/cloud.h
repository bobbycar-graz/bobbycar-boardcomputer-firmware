#pragma once

// esp-idf includes
#ifdef FEATURE_CLOUD
#include <esp_websocket_client.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#endif

namespace {
#ifdef FEATURE_CLOUD
void cloudTask(void*)
{
    const esp_websocket_client_config_t config = {
        .uri = "ws://iot.wattpilot.io:8080/charger/bobbycar1",
    };
    esp_websocket_client_handle_t handle = esp_websocket_client_init(&config);

    if (handle)
    {
        //Serial.println("esp websocket init succeeded");

        if (const auto result = esp_websocket_client_start(handle); result == ESP_OK)
        {
            //Serial.println("esp websocket start succeeded");

            while (true)
            {
                if (esp_websocket_client_is_connected(handle))
                {
                    std::string msg = "{"
                        "\"type\": \"fullStatus\","
                        "\"partial\": false, "
                        "\"status\": {"
                            "\"millis\":" + std::to_string(std::chrono::milliseconds{espchrono::millis_clock::now().time_since_epoch()}.count()) + ","
                            "\"front.valid\":" + std::to_string(controllers.front.feedbackValid) + ","
                            "\"back.valid\":" + std::to_string(controllers.back.feedbackValid) + ","
                            "\"front.left.pwm\":" + std::to_string(controllers.front.command.left.pwm) + ","
                            "\"front.right.pwm\":" + std::to_string(controllers.front.command.right.pwm) + ","
                            "\"back.left.pwm\":" + std::to_string(controllers.back.command.left.pwm) + ","
                            "\"back.right.pwm\":" + std::to_string(controllers.back.command.right.pwm) + ","
                            "\"front.volt\":" + std::to_string(controllers.front.feedback.batVoltage) + ","
                            "\"back.volt\":" + std::to_string(controllers.back.feedback.batVoltage) + ","
                            "\"front.temp\":" + std::to_string(controllers.front.feedback.boardTemp) + ","
                            "\"back.temp\":" + std::to_string(controllers.back.feedback.boardTemp) + ","
                            "\"front.bad\":" + std::to_string(controllers.front.feedback.timeoutCntSerial) + ","
                            "\"back.bad\":" + std::to_string(controllers.back.feedback.timeoutCntSerial) + ","
                            "\"front.left.speed\":" + std::to_string(controllers.front.feedback.left.speed) + ","
                            "\"front.right.speed\":" + std::to_string(controllers.front.feedback.right.speed) + ","
                            "\"back.left.speed\":" + std::to_string(controllers.back.feedback.left.speed) + ","
                            "\"back.right.speed\":" + std::to_string(controllers.back.feedback.right.speed) + ","
                            "\"front.left.current\":" + std::to_string(controllers.front.feedback.left.dcLink) + ","
                            "\"front.right.current\":" + std::to_string(controllers.front.feedback.right.dcLink) + ","
                            "\"back.left.current\":" + std::to_string(controllers.back.feedback.left.dcLink) + ","
                            "\"back.right.current\":" + std::to_string(controllers.back.feedback.right.dcLink) + ","
                            "\"front.left.error\":" + std::to_string(controllers.front.feedback.left.error) + ","
                            "\"front.right.error\":" + std::to_string(controllers.front.feedback.right.error) + ","
                            "\"back.left.error\":" + std::to_string(controllers.back.feedback.left.error) + ","
                            "\"back.right.error\":" + std::to_string(controllers.back.feedback.right.error) +
                        "}"
                    "}";

                    const auto sent = esp_websocket_client_send_text(handle, msg.c_str(), msg.length(), 1000 / portTICK_PERIOD_MS);
                    if (sent == msg.length())
                    {
                        //Serial.println("Sent cloud message");
                    }
                    else
                    {
                        //Serial.printf("sent=%i, msgsize=%i\r\n", sent, msg.length());
                    }
                }
                else
                {
                    //Serial.println("Not sending cloud because not connected");
                }

                delay(100);
            }
        }
        else
        {
            //Serial.printf("esp websocket start failed with %s\r\n", esp_err_to_name(result));
        }
    }
    else
    {
        //Serial.println("esp websocket init failed");
    }

    vTaskDelete(NULL);
}

void startCloud()
{
    if (const auto result = xTaskCreatePinnedToCore(cloudTask, "cloudTask", 4096, nullptr, 10, nullptr, 1); result == pdTRUE)
    {
        //Serial.println("cloud task create succeeded");
    }
    else
    {
        //Serial.printf("cloud task create failed\r\n");
    }
}
#endif
} // namespace

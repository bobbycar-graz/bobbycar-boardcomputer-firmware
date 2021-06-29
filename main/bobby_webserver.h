#pragma once

// 3rdparty lib includes
#ifdef FEATURE_WEBSERVER
#include <ESPAsyncWebServer.h>
#endif

// local includes
#include "screens.h"
#include "textinterface.h"
#include "menudisplay.h"
#include "changevaluedisplay.h"
#include "displays/updatedisplay.h"
//#include "esputils.h"
#include "buttons.h"

namespace {
#ifdef FEATURE_WEBSERVER
AsyncWebServer webServer{80};

bool shouldReboot;

class HtmlTag {
public:
    HtmlTag(const char *tagName, AsyncResponseStream *response) :
        m_tagName{tagName},
        m_response{response}
    {
        m_response->printf("<%s>", m_tagName);
    }

    ~HtmlTag()
    {
        m_response->printf("</%s>", m_tagName);
    }

private:
    const char * const m_tagName;
    AsyncResponseStream * const m_response;
};

void initWebserver()
{
    shouldReboot = false;

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncResponseStream *response = request->beginResponseStream("text/html");

        {
            HtmlTag htmlTag{"html", response};

            {
                HtmlTag headTag{"head", response};

                {
                    HtmlTag titleTag{"title", response};
                    response->print("Bobbycar remote");
                }

                response->print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\" />");
            }

            {
                HtmlTag bodyTag{"body", response};

                {
                    HtmlTag h1Tag{"h1", response};
                    response->print("Bobbycar remote");
                }

                {
                    HtmlTag pTag{"p", response};
                    response->print("<a href=\"/up\">Up</a> "
                                    "<a href=\"/down\">Down</a> "
                                    "<a href=\"/confirm\">Confirm</a> "
                                    "<a href=\"/back\">Back</a>");
                }

                if (auto constCurrentDisplay = static_cast<const Display *>(currentDisplay.get()))
                {
                    if (const auto *textInterface = constCurrentDisplay->asTextInterface())
                    {
                        HtmlTag h2Tag{"h2", response};
                        response->print(textInterface->text().c_str());
                    }

                    if (const auto *menuDisplay = constCurrentDisplay->asMenuDisplay())
                    {
                        HtmlTag ulTag{"ul", response};

                        int i{0};
                        menuDisplay->runForEveryMenuItem([&,selectedIndex=menuDisplay->selectedIndex()](const MenuItem &menuItem){
                            response->print("<li");

                            if (i == selectedIndex)
                                response->print(" style=\"border: 1px solid black;\"");

                            response->print("><a href=\"/triggerItem?index=");
                            response->print(i);
                            response->print("\">");
                            response->print(menuItem.text().c_str());
                            response->print("</a></li>");

                            i++;
                        });
                    }
                    else if (const auto *changeValueDisplay = constCurrentDisplay->asChangeValueDisplayInterface())
                    {
                        response->print("<form action=\"/setValue\" method=\"GET\">");
                        response->print(("<input type=\"number\" name=\"value\" value=\"" + std::to_string(changeValueDisplay->shownValue()) + "\" />").c_str());
                        response->print("<button type=\"submit\">Update</button>");
                        response->print("</form>");
                    }
                    else
                    {
                        response->print("No web control implemented for current display.");
                    }
                }
                else
                {
                    response->print("Currently no screen instantiated.");
                }
            }
        }

        request->send(response);
    });

    webServer.on("/up", HTTP_GET, [](AsyncWebServerRequest *request){
        InputDispatcher::rotate(-1);

        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "ok");
        response->addHeader("Location", "/");
        request->send(response);
    });

    webServer.on("/down", HTTP_GET, [](AsyncWebServerRequest *request){
        InputDispatcher::rotate(1);

        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "ok");
        response->addHeader("Location", "/");
        request->send(response);
    });

    webServer.on("/confirm", HTTP_GET, [](AsyncWebServerRequest *request){
        InputDispatcher::confirmButton(true);
        InputDispatcher::confirmButton(false);

        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "ok");
        response->addHeader("Location", "/");
        request->send(response);
    });

    webServer.on("/back", HTTP_GET, [](AsyncWebServerRequest *request){
        InputDispatcher::backButton(true);
        InputDispatcher::backButton(false);

        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "ok");
        response->addHeader("Location", "/");
        request->send(response);
    });

    webServer.on("/triggerItem", HTTP_GET, [](AsyncWebServerRequest *request){
        if (!request->hasArg("index"))
        {
            request->send(400, "text/plain", "index parameter missing");
            return;
        }

        if (!currentDisplay)
        {
            request->send(400, "text/plain", "currentDisplay is null");
            return;
        }

        auto *menuDisplay = currentDisplay->asMenuDisplay();
        if (!menuDisplay)
        {
            request->send(400, "text/plain", "currentDisplay is not a menu display");
            return;
        }

        const auto indexStr = request->arg("index");

        char *ptr;
        const auto index = std::strtol(std::begin(indexStr), &ptr, 10);

        if (ptr != std::end(indexStr))
        {
            request->send(400, "text/plain", "index could not be parsed");
            return;
        }

        if (index < 0 || index >= menuDisplay->menuItemCount())
        {
            request->send(400, "text/plain", "index out of range");
            return;
        }

        menuDisplay->getMenuItem(index).triggered();

        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "ok");
        response->addHeader("Location", "/");
        request->send(response);
    });

    webServer.on("/setValue", HTTP_GET, [](AsyncWebServerRequest *request){
        if (!request->hasArg("value"))
        {
            request->send(400, "text/plain", "value parameter missing");
            return;
        }

        if (!currentDisplay)
        {
            request->send(400, "text/plain", "currentDisplay is null");
            return;
        }

        auto *changeValueDisplay = currentDisplay->asChangeValueDisplayInterface();
        if (!changeValueDisplay)
        {
            request->send(400, "text/plain", "currentDisplay is not a change value display");
            return;
        }

        const auto valueStr = request->arg("value");

        char *ptr;
        const auto value = std::strtol(std::begin(valueStr), &ptr, 10);

        if (ptr != std::end(valueStr))
        {
            request->send(400, "text/plain", "value could not be parsed");
            return;
        }

        changeValueDisplay->setShownValue(value);

        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "ok");
        response->addHeader("Location", "/");
        request->send(response);
    });


#ifdef FEATURE_WEBOTA
    webServer.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html",
                      "<form method=\"POST\" action=\"/updateCode\" enctype=\"multipart/form-data\">"
                      "<input type=\"file\" name=\"update\">"
                      "<input type=\"submit\" value=\"Update Code\">"
                      "</form>"
                      "<form method=\"POST\" action=\"/updateData\" enctype=\"multipart/form-data\">"
                      "<input type=\"file\" name=\"update\">"
                      "<input type=\"submit\" value=\"Update Data\">"
                      "</form>");
    });

    const auto handleUpdate = [](AsyncWebServerRequest *request){
        shouldReboot = !Update.hasError();

        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot ? "OK" : "FAIL");
        response->addHeader("Connection", "close");
        request->send(response);
    };

    const auto createHandleUpdtateUpload = [](size_t size, int command){
        return [size, command](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
            //ESP_UNUSED(request)

            //Serial.printf("callback %u %u\r\n", index, len);

            if (!index)
            {
                //Serial.printf("Update Start: %s\r\n", filename.c_str());
                //Update.runAsync(true);
                if (!Update.begin(size, command))
                    Update.printError(Serial);

                std::string type;
                if (ArduinoOTA.getCommand() == U_FLASH)
                    type = "sketch";
                else if (ArduinoOTA.getCommand() == U_SPIFFS) // U_SPIFFS
                    type = "filesystem";
                else
                    type = "unknown";

                switchScreenImpl<UpdateDisplay>("Updating " + type);
            }

            if (!Update.hasError())
            {
                if (Update.write(data, len) == len)
                {
                    ((UpdateDisplay*)currentDisplay.get())->m_progress = index;
                    ((UpdateDisplay*)currentDisplay.get())->m_total = size;
                    ((UpdateDisplay*)currentDisplay.get())->redraw();
                }
                else
                {
                    Update.printError(Serial);

                    ((UpdateDisplay*)currentDisplay.get())->m_error = {};
                    ((UpdateDisplay*)currentDisplay.get())->m_errorValid = true;
                    ((UpdateDisplay*)currentDisplay.get())->redraw();
                }
            }
            else
            {
                ((UpdateDisplay*)currentDisplay.get())->m_error = {};
                ((UpdateDisplay*)currentDisplay.get())->m_errorValid = true;
                ((UpdateDisplay*)currentDisplay.get())->redraw();
            }

            if (final)
            {
                if (Update.end(true))
                {
                    //Serial.printf("Update Success: %uB\r\n", index + len);

                    ((UpdateDisplay*)currentDisplay.get())->m_finished = true;
                    ((UpdateDisplay*)currentDisplay.get())->redraw();
                }
                else
                {
                    Update.printError(Serial);

                    ((UpdateDisplay*)currentDisplay.get())->m_error = {};
                    ((UpdateDisplay*)currentDisplay.get())->m_errorValid = true;
                    ((UpdateDisplay*)currentDisplay.get())->redraw();
                }
            }
        };
    };

    webServer.on("/updateCode", HTTP_POST, handleUpdate, createHandleUpdtateUpload((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000, U_FLASH));
    webServer.on("/updateData", HTTP_POST, handleUpdate, createHandleUpdtateUpload(UPDATE_SIZE_UNKNOWN, U_SPIFFS));
#endif

    webServer.begin();
}

void handleWebserver()
{
    if (shouldReboot)
    {
        shouldReboot = false;
        ESP.restart();
    }
}
#endif
}

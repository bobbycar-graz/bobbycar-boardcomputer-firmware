#pragma once

#include "htmltag.h"

namespace {
void breakLine(AsyncResponseStream &stream)
{
    stream.print("<br/>");
}

void label(AsyncResponseStream &stream, const char *name, const char *text)
{
    HtmlTag label(stream, "label", std::string(" for=\"") + name + "\"");
    stream.print(text);
}

template<typename T>
void input(AsyncResponseStream &stream, T value, const char *type, const char *id, const char *name, const char *additionalAttributes = nullptr)
{
    stream.print("<input type=\"");
    stream.print(type);
    if (id)
    {
        stream.print("\" id=\"");
        stream.print(id);
    }
    stream.print("\" name=\"");
    stream.print(name);
    stream.print("\" value=\"");
    stream.print(value);
    stream.print("\"");
    if (additionalAttributes)
        stream.print(additionalAttributes);
    stream.print("/>");
}

template<typename T>
void hiddenInput(AsyncResponseStream &stream, T value, const char *name)
{
    input(stream, value, "hidden", nullptr, name);
}

template<typename T>
void numberInput(AsyncResponseStream &stream, T value, const char *id, const char *name, const char *text)
{
    label(stream, id, text);

    breakLine(stream);

    input(stream, value, "number", id, name, " required");
}

template<typename T>
void numberInput(AsyncResponseStream &stream, T value, const char *name, const char *text)
{
    numberInput(stream, value, name, name, text);
}

void submitButton(AsyncResponseStream &stream)
{
    HtmlTag button(stream, "button", " type=\"submit\"");
    stream.print("Submit");
}

void checkboxInput(AsyncResponseStream &stream, bool value, const char *id, const char *name, const char *text)
{
    label(stream, id, text);

    breakLine(stream);

    input(stream, "on", "checkbox", id, name, value?" checked":"");
}

void checkboxInput(AsyncResponseStream &stream, bool value, const char *name, const char *text)
{
    checkboxInput(stream, value, name, name, text);
}

void selectOption(AsyncResponseStream &stream, const char *value, const char *text, bool selected)
{
    std::string str{" value=\""};
    str += value;
    str += "\"";

    if (selected)
        str += " selected";

    HtmlTag option(stream, "option", str);
    stream.print(text);
}
}

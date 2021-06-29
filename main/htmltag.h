#pragma once

#include <ESPAsyncWebServer.h>

namespace {
class HtmlTag {
public:
    HtmlTag(AsyncResponseStream &stream, const char *tag);

    template<typename T>
    HtmlTag(AsyncResponseStream &stream, const char *tag, const T &x);

    ~HtmlTag();

private:
    AsyncResponseStream &stream;
    const char * const tag;
};

HtmlTag::HtmlTag(AsyncResponseStream &stream, const char *tag) :
    stream{stream},
    tag{tag}
{
    stream.print("<");
    stream.print(tag);
    stream.print(">");
}

template<typename T>
HtmlTag::HtmlTag(AsyncResponseStream &stream, const char *tag, const T &x) :
    stream{stream},
    tag{tag}
{
    stream.print("<");
    stream.print(tag);
    stream.print(x);
    stream.print(">");
}

HtmlTag::~HtmlTag()
{
    stream.print("</");
    stream.print(tag);
    stream.print(">");
}
}

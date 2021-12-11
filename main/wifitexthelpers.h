#pragma once

// local includes
#include "textinterface.h"

class WifiStatusText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiScanStatusText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiHostnameText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiMacText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiSsidText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiBssidText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiRssiText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiEncryptionTypeText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiPairwiseCipherText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiGroupCipherText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiIpText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiNetmaskText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiGatewayText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiIpv6LinklocalText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiIpv6GlobalText : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiDns0Text : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiDns1Text : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

class WifiDns2Text : public virtual espgui::TextInterface
{
  public:
    std::string text() const override;
};

constexpr char TEXT_SOFTAPGETSTATIONNUM[] = "softAPgetStationNum: ";
using WifiSoftApGetStationNumText         = espgui::StaticText<TEXT_SOFTAPGETSTATIONNUM>; // StatusTextHelper<TEXT_SOFTAPGETSTATIONNUM, &WiFi.softAPgetStationNum>
constexpr char TEXT_SOFTAPIP[]            = "softAPIP: ";
using WifiSoftApIpText                    = espgui::StaticText<TEXT_SOFTAPIP>; // StatusTextHelper<TEXT_SOFTAPIP, &WiFi.softAPIP>
constexpr char TEXT_SOFTAPBROADCASTIP[]   = "softAPBroadcastIP: ";
using WifiSoftApBroadcastIpText           = espgui::StaticText<TEXT_SOFTAPBROADCASTIP>; // StatusTextHelper<TEXT_SOFTAPBROADCASTIP, &WiFi.softAPBroadcastIP>
constexpr char TEXT_SOFTAPNETWORKID[]     = "softAPNetworkID: ";
using WifiSoftApNetworkIdText             = espgui::StaticText<TEXT_SOFTAPNETWORKID>; // StatusTextHelper<TEXT_SOFTAPNETWORKID, &WiFi.softAPNetworkID>
constexpr char TEXT_SOFTAPSUBNETCIDR[]    = "softAPSubnetCIDR: ";
using WifiSoftApSubnetCidrText            = espgui::StaticText<TEXT_SOFTAPSUBNETCIDR>; // StatusTextHelper<TEXT_SOFTAPSUBNETCIDR, &WiFi.softAPSubnetCIDR>
constexpr char TEXT_SOFTAPIPV6[]          = "softAPIPv6: ";
using WifiSoftApIpV6Text                  = espgui::StaticText<TEXT_SOFTAPIPV6>; // StatusTextHelper<TEXT_SOFTAPIPV6, &WiFi.softAPIPv6>
constexpr char TEXT_SOFTAPGETHOSTNAME[]   = "softAPgetHostname: ";
using WifiSoftApHostnameText              = espgui::StaticText<TEXT_SOFTAPGETHOSTNAME>; // StatusTextHelper<TEXT_SOFTAPGETHOSTNAME, &WiFi.softAPgetHostname>
constexpr char TEXT_SOFTAPMACADDRESS[]    = "softAPmacAddress: ";
using WifiSoftApMacAddressText            = espgui::StaticText<TEXT_SOFTAPMACADDRESS>; // StatusTextHelper<TEXT_SOFTAPMACADDRESS, &WiFi.softAPmacAddress>

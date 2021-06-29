#pragma once

#include "textinterface.h"

namespace {
constexpr char TEXT_STATUSBITS[] = "statusBits: ";
using WifiStatusBitsText = StaticText<TEXT_STATUSBITS>; //StatusTextHelper<TEXT_STATUSBITS, &WiFi::getStatusBits>;
constexpr char TEXT_CHANNEL[] = "channel: ";
using WifiChannelText = StaticText<TEXT_CHANNEL>; //StatusTextHelper<TEXT_CHANNEL, &WiFi::channel>;

constexpr char TEXT_ISCONNECTED[] = "isConnected: ";
using WifiIsConnectedText = StaticText<TEXT_ISCONNECTED>; //StatusTextHelper<TEXT_ISCONNECTED, &WiFi.isConnected>
constexpr char TEXT_LOCALIP[] = "localIP: ";
using WifiLocalIpText = StaticText<TEXT_LOCALIP>; //StatusTextHelper<TEXT_LOCALIP, &WiFi.localIP>
constexpr char TEXT_MACADDRESS[] = "macAddress: ";
using WifiMacAddressText = StaticText<TEXT_MACADDRESS>; //StatusTextHelper<TEXT_MACADDRESS, &WiFi.macAddress>
constexpr char TEXT_SUBNETMASK[] = "subnetMask: ";
using WifiSubnetMaskText = StaticText<TEXT_SUBNETMASK>; //StatusTextHelper<TEXT_SUBNETMASK, &WiFi.subnetMask>
constexpr char TEXT_GATEWAYIP[] = "gatewayIP: ";
using WifiGatewayIpText = StaticText<TEXT_GATEWAYIP>; //StatusTextHelper<TEXT_GATEWAYIP, &WiFi.gatewayIP>
constexpr char TEXT_DNSIP[] = "dnsIP: ";
using WifiDnsIpText = StaticText<TEXT_DNSIP>; //StatusTextHelper<TEXT_DNSIP, &WiFi.dnsIP>
constexpr char TEXT_BROADCASTIP[] = "broadcastIP: ";
using WifiBroadcastIpText = StaticText<TEXT_BROADCASTIP>; //StatusTextHelper<TEXT_BROADCASTIP, &WiFi.broadcastIP>
constexpr char TEXT_NETWORKID[] = "networkID: ";
using WifiNetworkIdText = StaticText<TEXT_NETWORKID>; //StatusTextHelper<TEXT_NETWORKID, &WiFi.networkID>
constexpr char TEXT_SUBNETCIDR[] = "subnetCIDR: ";
using WifiSubnetCIDRText = StaticText<TEXT_SUBNETCIDR>; //StatusTextHelper<TEXT_SUBNETCIDR, &WiFi.subnetCIDR>
constexpr char TEXT_LOCALIPV6[] = "localIPv6: ";
using WifiLocalIpV6Text = StaticText<TEXT_LOCALIPV6>; //StatusTextHelper<TEXT_LOCALIPV6, &WiFi.localIPv6>
constexpr char TEXT_HOSTNAME[] = "hostname: ";
using WifiHostnameText = StaticText<TEXT_HOSTNAME>; //StatusTextHelper<TEXT_HOSTNAME, &WiFi.getHostname>
constexpr char TEXT_STATUS2[] = "status: ";
using WifiStatusText = StaticText<TEXT_STATUS2>; //StatusTextHelper<TEXT_STATUS2, &WiFi.status>
constexpr char TEXT_SSID[] = "SSID: ";
using WifiSsidText = StaticText<TEXT_SSID>; //StatusTextHelper<TEXT_SSID, &WiFi.SSID>
constexpr char TEXT_PSK[] = "psk: ";
using WifiPskText = StaticText<TEXT_PSK>; //StatusTextHelper<TEXT_PSK, &WiFi.psk>
constexpr char TEXT_BSSID[] = "BSSID: ";
using WifiBssidText = StaticText<TEXT_BSSID>; //StatusTextHelper<TEXT_BSSID, &WiFi.BSSIDstr>
constexpr char TEXT_RSSI[] = "RSSI: ";
using WifiRssiText = StaticText<TEXT_RSSI>; //StatusTextHelper<TEXT_RSSI, &WiFi.RSSI>

constexpr char TEXT_SOFTAPGETSTATIONNUM[] = "softAPgetStationNum: ";
using WifiSoftApGetStationNumText = StaticText<TEXT_SOFTAPGETSTATIONNUM>; //StatusTextHelper<TEXT_SOFTAPGETSTATIONNUM, &WiFi.softAPgetStationNum>
constexpr char TEXT_SOFTAPIP[] = "softAPIP: ";
using WifiSoftApIpText = StaticText<TEXT_SOFTAPIP>; //StatusTextHelper<TEXT_SOFTAPIP, &WiFi.softAPIP>
constexpr char TEXT_SOFTAPBROADCASTIP[] = "softAPBroadcastIP: ";
using WifiSoftApBroadcastIpText = StaticText<TEXT_SOFTAPBROADCASTIP>; //StatusTextHelper<TEXT_SOFTAPBROADCASTIP, &WiFi.softAPBroadcastIP>
constexpr char TEXT_SOFTAPNETWORKID[] = "softAPNetworkID: ";
using WifiSoftApNetworkIdText = StaticText<TEXT_SOFTAPNETWORKID>; //StatusTextHelper<TEXT_SOFTAPNETWORKID, &WiFi.softAPNetworkID>
constexpr char TEXT_SOFTAPSUBNETCIDR[] = "softAPSubnetCIDR: ";
using WifiSoftApSubnetCidrText = StaticText<TEXT_SOFTAPSUBNETCIDR>; //StatusTextHelper<TEXT_SOFTAPSUBNETCIDR, &WiFi.softAPSubnetCIDR>
constexpr char TEXT_SOFTAPIPV6[] = "softAPIPv6: ";
using WifiSoftApIpV6Text = StaticText<TEXT_SOFTAPIPV6>; //StatusTextHelper<TEXT_SOFTAPIPV6, &WiFi.softAPIPv6>
constexpr char TEXT_SOFTAPGETHOSTNAME[] = "softAPgetHostname: ";
using WifiSoftApHostnameText = StaticText<TEXT_SOFTAPGETHOSTNAME>; //StatusTextHelper<TEXT_SOFTAPGETHOSTNAME, &WiFi.softAPgetHostname>
constexpr char TEXT_SOFTAPMACADDRESS[] = "softAPmacAddress: ";
using WifiSoftApMacAddressText = StaticText<TEXT_SOFTAPMACADDRESS>; //StatusTextHelper<TEXT_SOFTAPMACADDRESS, &WiFi.softAPmacAddress>
}

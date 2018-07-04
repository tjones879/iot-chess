#pragma once

#include <string>
#include <string_view>
#include <initializer_list>
#include <optional>
#include "util.hpp"

namespace AT {

enum class Type : uint8_t
{
    TEST,
    QUERY,
    EXECUTE,
    SET
};

enum class WIFIMode : char
{
    Client = '1',
    Host = '2',
    Dual = '3'
};

enum class ECN : uint8_t
{
    OPEN = 0,
    WEP = 1,
    WPA_PSK = 2,
    WPA2_PSK = 3,
    WPA_WPA2_PSK = 4
};

class AccessPoint {
    ECN security;
    std::string ssid;
    int8_t rssi;
    std::string mac;
public:
    AccessPoint(ECN sec, std::string sid, int8_t signal, std::string mac);
};

enum class Code
{
    RST,
    GMR,
    GSLP,
    ATE,
    CWMODE,
    CWJAP,
    CWLAP,
    CWQAP,
    CWSAP,
    CWLIF,
    CWDHCP,
    CIPSTAMAC,
    CIPSTA,
    CIPAP,
    CIPSTATUS,
    CIPSTART,
    CIPSEND,
    CIPCLOSE,
    CIFSR,
    CIPMUX,
    CIPSERVER,
    CIPMODE,
    CIPSTO,
    CIUPDATE
};

constexpr std::initializer_list<std::pair<Code, const char*>> mapping = {
    {Code::RST,       "+RST"},
    {Code::GMR,       "+GMR"},
    {Code::GSLP,      "+GSLP"},
    {Code::ATE,       "E"},
    {Code::CWMODE,    "+CWMODE"},
    {Code::CWJAP,     "+CWJAP"},
    {Code::CWLAP,     "+CWLAP"},
    {Code::CWQAP,     "+CWQAP"},
    {Code::CWSAP,     "+CWSAP"},
    {Code::CWLIF,     "+CWLIF"},
    {Code::CWDHCP,    "+CWDHCP"},
    {Code::CIPSTAMAC, "+CIPSTAMAC"},
    {Code::CIPSTA,    "+CIPSTA"},
    {Code::CIPAP,     "+CIPAP"},
    {Code::CIPSTATUS, "+CIPSTATUS"},
    {Code::CIPSTART,  "+CIPSTART"},
    {Code::CIPSEND,   "+CIPSEND"},
    {Code::CIPCLOSE,  "+CIPCLOSE"},
    {Code::CIFSR,     "+CIFSR"},
    {Code::CIPMUX,    "+CIPMUX"},
    {Code::CIPSERVER, "+CIPSERVER"},
    {Code::CIPMODE,   "+CIPMODE"},
    {Code::CIPSTO,    "+CIPSTO"},
    {Code::CIUPDATE,  "+CIUPDATE"}
};

constexpr std::optional<const char *> toStr(Code c) {
    for (auto &p : mapping)
        if (c == p.first)
            return p.second;
    return {};
}

constexpr std::optional<Code> toEnum(std::string_view name) {
    for (auto &p : mapping)
        if (name == p.second)
            return p.first;
    return {};
}

template <size_t size>
std::array<AccessPoint, size> parseCWLAP(std::string message);
} /* namespace AT */

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
    CLIENT = '1',
    HOST = '2',
    DUAL = '3'
};

enum class ECN : uint8_t
{
    OPEN = 0,
    WEP = 1,
    WPA_PSK = 2,
    WPA2_PSK = 3,
    WPA_WPA2_PSK = 4
};

struct AccessPoint {
    ECN security;
    std::string ssid;
    int8_t rssi;
    std::string mac;
    std::string password;

    AccessPoint(ECN sec, std::string sid, int8_t signal, std::string mac);
};

enum class Code
{
    AT,
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
    {Code::AT,        ""},
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

/**
 * Attempt to convert a value in the Code enum to a string.
 *
 * This function should always complete during compile time.
 *
 * @param c Code value to convert to a string.
 * @return The corresponding string with the code enum
 *         Empty string on failure
 */
constexpr const char * toStr(Code c);

constexpr std::optional<Code> toEnum(std::string_view name);

/**
 * Attempt to find the appropriate Code token in the string.
 *
 * @param string  The string_view that the code will be searched in.
 * @param code    The Code enum value that will be found in the string
 * @return        The ending position of the Code on success,
 *                std::string_view::npos on failure.
 */
size_t parseCode(std::string_view string, Code code);

/**
 * Attempt to parse a string surrounded in quotation marks.
 *
 * @param message  The string_view that the string will be pulled from.
 * @return         Pair containing the extracted string if found
 *                 and the the new position in the string_view where
 *                 the parsed string ends.
 *
 */
std::pair<std::string, size_t> parseString(std::string_view message);

template <size_t size>
std::array<AccessPoint, size> parseCWLAP(std::string message);
} /* namespace AT */

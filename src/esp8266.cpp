#include "esp8266.hpp"
#include "atparser.hpp"

#include <libopencm3/stm32/usart.h>

void ESP8266Device::sendAT(AT::Code code, const std::string &extra)
{
    // All messages begin with AT followed by <command>
    usart_send_blocking(_usart, 'A');
    usart_send_blocking(_usart, 'T');

    auto cmdStr = std::string(AT::toStr(code));
    for (auto i : cmdStr)
        usart_send_blocking(_usart, i);

    std::string_view extraView{extra.c_str(), extra.size()};
    for (auto i : extraView)
        usart_send_blocking(_usart, i);

    usart_send_blocking(_usart, '\r');
    usart_send_blocking(_usart, '\n');
}

void ESP8266Device::send(std::string text)
{
    auto str = text.c_str();
    for (char c : text)
        usart_send_blocking(_usart, c);

    usart_send_blocking(_usart, '\r');
    usart_send_blocking(_usart, '\n');
}

int ESP8266Device::testAT()
{
    auto cmdHandle = command.getRAII();
    sendAT(AT::Code::AT, "");
    if (response.take())
        return 0;
    else
        return -1;
}

int ESP8266Device::restart()
{
    auto cmdHandle = command.getRAII();
    sendAT(AT::Code::RST, "");
    if (response.take())
        return 0;
    else
        return -1;
}

/**
 *
 */
int ESP8266Device::wifiMode(AT::Type type, AT::WIFIMode mode)
{
    auto cmdHandle = command.getRAII();
    std::string suffix;
    if (type == AT::Type::QUERY)
        suffix = "?";

    sendAT(AT::Code::CWMODE, suffix);
    if (response.take())
        return 0;
    else
        return -1;
}

bool ESP8266Device::connectAP(AT::AccessPoint ap)
{
    auto cmdHandle = command.getRAII();
    std::string suffix = "\"" + ap.ssid + "\",\"" + ap.password + "\"";

    sendAT(AT::Code::CWJAP, suffix);

    if (response.take())
        return true;
}

template <size_t numPoints>
std::array<AT::AccessPoint, numPoints> ESP8266Device::getAPList()
{
    return std::array<AT::AccessPoint, numPoints>();
}

bool ESP8266Device::disconnectAP()
{
    auto cmdHandle = command.getRAII();
    sendAT(AT::Code::CWQAP, "");
    if (response.take())
        return true;
}

int ESP8266Device::connStatus()
{
    auto cmdHandle = command.getRAII();
    sendAT(AT::Code::CIPSTATUS, "");
    if (response.take())
        return true;
}

int ESP8266Device::connStart(const Connection &conn)
{
    std::string suffix{};
    switch (conn.type) {
    case ConnectionType::UDP:
        suffix += "\"UDP\"";
    case ConnectionType::TCP:
        suffix += "\"TCP\"";
    }
    suffix += ",\"" + conn.address + "\"";
    suffix += "," + std::to_string(conn.port);

    auto cmdHandle = command.getRAII();
    sendAT(AT::Code::CIPSTART, suffix);
    if (response.take())
        return 0;
    else
        return -1;
}

int ESP8266Device::sendData(std::string data)
{
    auto cmdHandle = command.getRAII();
    std::string suffix = std::to_string(data.size());
    sendAT(AT::Code::CIPSEND, suffix);
    if (response.take()) {
        // Parse for '>'

    }
}

bool ESP8266Device::connClose()
{
    auto cmdHandle = command.getRAII();
    sendAT(AT::Code::CIPCLOSE, "");
    if (response.take())
        return true;
}

std::string ESP8266Device::getIP()
{
    auto cmdHandle = command.getRAII();
    sendAT(AT::Code::CIFSR, "");
    if (response.take())
        return "TRUE";
}

HttpResult ESP8266Interface::request(const Connection &conn,
                                     const HttpRequest &request)
{
    // Connect to the location
}

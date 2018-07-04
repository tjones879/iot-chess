#include "esp8266.hpp"
#include "atparser.hpp"

#include <libopencm3/stm32/usart.h>

void ESP8266Device::sendAT(AT::Code command, const std::string &extra)
{
    // All messages begin with AT followed by <command>
    usart_send_blocking(_usart, 'A');
    usart_send_blocking(_usart, 'T');

    auto cmdStr = AT::toStr(command);
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
    sendAT("");
    if (response.take())
        return 0;
    else
        return -1;
}

int ESP8266Device::restart()
{
    auto cmdHandle = command.getRAII();
    sendAT(AT::Code::RST);
    if (response.take())
        return 0;
    else
        return -1;
}

/**
 *
 */
int ESP8266Device::wifiMode(AT::Type type, WIFIMode mode)
{
    auto cmdHandle = command.getRAII();
    std::string suffix;
    if (type == AT::Type::Query)
        suffix = "?";

    sendAT(AT::Code::CWMODE, suffix);
    if (response.take())
        return 0;
    else
        return -1;
}

bool ESP8266Device::connectAP(AccessPoint ap)
{
    auto cmdHandle = command.getRAII();
    std::string suffix = "+CWJAP=\"" + ap.ssid + "\",\"" + ap.pwd + "\"";

    sendAT(suffix);

    if (response.take())
        return true;
}

template <size_t numPoints>
std::array<AccessPoint, numPoints> ESP8266Device::getAPList()
{
    return std::array<AccessPoint, numPoints>();
}

bool ESP8266Device::disconnectAP()
{
    auto cmdHandle = command.getRAII();
    std::string suffix = "+CWQAP";
    sendAT(suffix);
    if (response.take())
        return true;
}

int ESP8266Device::connStatus()
{
    auto cmdHandle = command.getRAII();
    std::string suffix = "+CIPSTATUS";
    sendAT(suffix);
    if (response.take())
        return true;
}

int ESP8266Device::connStart(const Connection &conn)
{
    std::string suffix = "+CIPSTART";
    switch (conn.type) {
    case ConnectionType::UDP:
        suffix += "\"UDP\"";
    case ConnectionType::TCP:
        suffix += "\"TCP\"";
    }
    suffix += ",\"" + conn.address + "\"";
    suffix += "," + std::to_string(conn.port);

    auto cmdHandle = command.getRAII();
    sendAT(suffix);
    if (response.take())
        return 0;
    else
        return -1;
}

int ESP8266Device::sendData(std::string data)
{
    auto cmdHandle = command.getRAII();
    std::string suffix = "+CIPSEND=" + std::to_string(data.size());
    sendAT(suffix);
    if (response.take()) {
        // Parse for '>'
        
    }
}

bool ESP8266Device::connClose()
{
    auto cmdHandle = command.getRAII();
    std::string suffix = "+CIPCLOSE";
    sendAT(suffix);
    if (response.take())
        return true;
}

std::string ESP8266Device::getIP()
{
    auto cmdHandle = command.getRAII();
    std::string suffix = "+CIFSR";
    sendAT(suffix);
    if (response.take())
        return "TRUE";
}

HttpResult ESP8266Interface::request(const Connection &conn,
                                     const HttpRequest &request)
{
    // Connect to the location
}

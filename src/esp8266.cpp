#include "esp8266.hpp"

#include <libopencm3/stm32/usart.h>

void ESP8266Device::sendAT(std::string suffix)
{
    // All commands begin with AT followed by +<SUFFIX>
    auto str = suffix.c_str();
    usart_send_blocking(_usart, 'A');
    usart_send_blocking(_usart, 'T');
    for (auto i = 0; i < suffix.size(); i++)
        usart_send_blocking(_usart, str[i]);
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
    sendAT("+RST");
    if (response.take())
        return 0;
    else
        return -1;
}

/**
 * 
 */
int ESP8266Device::wifiMode(ATType type, WIFIMode mode)
{
    auto cmdHandle = command.getRAII();
    std::string suffix = "+CWMODE";
    switch(type) {
    case ATType::QUERY:
        suffix += "?";
        break;
    case ATType::EXECUTE:
        break;
    default:
        return -1;
        break;
    }

    sendAT(suffix);
    if (response.take())
        return 0;
    else
        return -1;
}

int ESP8266Device::connectAP(ATType type)
{

}

int ESP8266Device::getAPList(ATType type)
{

}

int ESP8266Device::disconnectAP()
{

}

int ESP8266Device::softAPMode(ATType type)
{

}

int ESP8266Device::listClients()
{

}

int ESP8266Device::setDHCP()
{

}

int ESP8266Device::connStatus(ATType type)
{

}

int ESP8266Device::connStart(const Connection &conn)
{
    std::string suffix = "+CIPSTART";
    switch(conn.type) {
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

int ESP8266Device::sendData(ATType type)
{

}

int ESP8266Device::connClose(ATType type)
{

}

int ESP8266Device::getIP()
{

}

HttpResult ESP8266Interface::request(const Connection &conn, const HttpRequest &request)
{
    // Connect to the location

}
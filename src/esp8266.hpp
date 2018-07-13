#pragma once

#include "http.hpp"
#include "util.hpp"
#include <string>
#include <array>
#include "atparser.hpp"

/**
 * @brief A modem-level driver for the ESP 8266 wifi module.
 *
 * All communication with the ESP8266 will behave as synchronous
 * despite its asynchronous nature. The task must obtain a binary
 * semaphore from the UART interrupt routine before a new command
 * is issued and the response is returned.
 */
class ESP8266Device
{
private:
    uint32_t _usart;
    BinarySemaphore command, response;

public:
    ESP8266Device(uint32_t usart_module)
        : _usart(usart_module), command(), response() {}

    int testAT();
    int restart();
    int wifiMode(AT::Type type, AT::WIFIMode mode);
    bool connectAP(AT::AccessPoint ap);
    template <size_t numPoints>
    std::array<AT::AccessPoint, numPoints> getAPList();
    bool disconnectAP();
    int softAPMode(AT::Type type);
    int listClients();
    int setDHCP();
    int connStatus();
    int connStart(const Connection &conn);
    int sendData(AT::Type type);
    bool connClose();
    std::string getIP();

private:
    void sendAT(AT::Code command, const std::string &extra);
    void send(std::string text);
    int sendData(std::string data);
};

class ESP8266Interface : public HttpClient
{
private:
    ESP8266Device device;

public:
    ESP8266Interface();

    int connect();

    int connect(const std::string &ssid, const std::string &pass);
    int disconnect();
    std::string getIPAddress();
    std::string getMacAddress();
    /**
     * Scan for available networks.
     *
     * @param ap    Pointer to an allocated array to store access points.
     * @param count Size of the allocated array, max number of points
     * @return      Number of access points found if >= 0.
     *              Negative on error
     */
    int scan(AT::AccessPoint *ap, uint8_t count);
    HttpResult request(const Connection &conn,
                       const HttpRequest &request) override;
};

#pragma once

#include <string>
#include "http.hpp"
#include "util.hpp"

enum class ATType : uint8_t {
    TEST,
    QUERY,
    EXECUTE,
    SET
};

enum class WIFIMode : char {
    Client = '1',
    Host = '2',
    Dual = '3'
};

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
        : _usart(usart_module)
    {

    }

    int testAT();
    int restart();
    int wifiMode(ATType type, WIFIMode mode);
    int connectAP(ATType type);
    int getAPList(ATType type);
    int disconnectAP();
    int softAPMode(ATType type);
    int listClients();
    int setDHCP();
    int connStatus(ATType type);
    int connStart(const Connection &conn);
    int sendData(ATType type);
    int connClose(ATType type);
    int getIP();
private:
    void sendAT(std::string command);
};


class AccessPoint
{

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
    int scan(AccessPoint *ap, uint8_t count);
    HttpResult request(const Connection &conn, const HttpRequest &request) override;
};
#pragma once

#include <string>

enum class HttpCode : uint8_t
{
    HTTP_OK,
};

enum class RequestType : uint8_t
{
    GET,
    POST
};

enum class ConnectionType : uint8_t
{
    UDP,
    TCP
};

namespace {
    constexpr std::initializer_list<std::pair<ConnectionType, const char*>> mapping = {
        {ConnectionType::UDP,  "\"UDP\""},
        {ConnectionType::TCP,  "\"TCP\""}
    };
}

/**
 * Attempt to convert a value in the ConnectionType enum to a string.
 *
 * This function should always complete during compile time.
 *
 * @param c ConnectionType value to convert to a string.
 * @return  The corresponding string with the connection type
 *          Empty string on failure
 */
constexpr const char *toStr(ConnectionType c)
{
    for (auto &p : mapping)
        if (c == p.first)
            return p.second;
    return "";
}

class Connection
{
public:
    ConnectionType type;
    std::string address;
    uint16_t port;
};

class HttpResult
{
public:
    HttpCode code;
};

class HttpRequest
{
public:
    RequestType type;
    std::string baseUrl, path, body;

    std::string toString()
    {
        std::string request;

        switch (type) {
        case RequestType::GET:
            request += "GET";
            break;
        case RequestType::POST:
            request += "POST";
            break;
        }

        request += " " + path;
        request += " HTTP/1.1\r\n";
        request += body;
        request += "\r\n";

        return request;
    }
};

class HttpClient
{
public:
    virtual HttpResult request(const Connection &conn,
                               const HttpRequest &request);
    virtual ~HttpClient();
};

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
    std::string baseUrl;
    std::string path;
    std::string body;

    std::string toString()
    {
        std::string request;

        switch (type) {
        case RequestType::GET:
            request += "GET";
        case RequestType::POST:
            request += "POST";
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
};

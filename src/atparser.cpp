#include "atparser.hpp"
namespace AT {

constexpr const char * toStr(Code c) {
    for (auto &p : mapping)
        if (c == p.first)
            return p.second;
    return "";
}

constexpr std::optional<Code> toEnum(std::string_view name) {
    for (auto &p : mapping)
        if (name == p.second)
            return p.first;
    return {};
}

bool parseOK(std::string_view string) {
    // Check that the string view ends with OK\r\n
}

size_t parseCode(std::string_view string, Code code) {
    // Slice the string so that we don't look through the entire view
    std::string codeStr(toStr(code));
    auto pos = string.find(codeStr, 0, codeStr.size());
    if (pos != std::basic_string_view::npos)
        return pos + codeStr.size();
    else
        return 0;
}

size_t parseChar(std::string_view string, char c) {
    if (string[0] == c)
        return 1;
    else
        return 0;
}

std::pair<std::string, size_t> parseString(std::string_view message) {
    size_t pos = 0;
    // Strings are always surrounded as "<string>"
    // We simply throw away these quotation marks.
    if (parseChar(message, '"'))
        pos++;
    else
        return {"", std::string_view::npos};

    auto stop = message.find(pos, '"');
    if (stop != std::string_view::npos)
        pos = stop;
    else
        return {"", stop};

    return {message.substr(1, stop - 1), stop};
}

std::pair<int, size_t> parseInt(std::string_view message) {
    
}

template <size_t size>
std::array<AccessPoint, size> parseCWLAP(std::string message) {
    /* +CWLAP:
     * (
     * 3
     * ,
     * "Jammer1"
     * ,
     * -72
     *  ,
     *  "10:da:43:e0:21:f0"
     *  ,
     *  1
     *  ,
     *  -2
     *  ,
     *  0
     *  )
     */
    // Split on new lines
    auto lines = std::vector<std::string>();
    for (auto &line : lines) {
        parseCode(line, Code::CWLAP);
        parseChar(line, ":");
        parseChar(line, "(");
        parseInt(line);
        parseChar(line, ",");
        parseString(line);
        parseChar(line, ",");
        parseInt(line);
        parseChar(line, ",");
        parseString(line);
        parseChar(line, ",");
        parseInt(line);
        parseChar(line, ",");
        parseInt(line);
        parseChar(line, ",");
        parseInt(line);
        parseChar(line, ")");
    }

    // Ensure that string ends with OK\r\n
    return std::array<AccessPoint, size>{};
}

AccessPoint parseCWJAP(std::string message) {
    // +CWJAP:"Skynet","e4:95:6e:42:19:95",1,-52
    parseCode(message, Code::CWJAP);
    parseChar(message, ":");
    parseStr(message);
    parseChar(message, ",");
    parseStr(message);
    parseChar(message, ",");
    parseInt(message);
    parseChar(message, ",");
    parseInt(message);
    parseOK(message);
}

} /* namespace AT */

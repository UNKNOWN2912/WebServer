#pragma once
#include "macro.hpp"
#include <string>
#include <vector>

struct StartField
{
    std::string method;
    std::string uri;
    std::string protocol;
};

enum class ConnectionType
{
    None,
    KeepAlive,
    Close,
};
struct ConnectionField
{
    ConnectionType type = ConnectionType::None;
};

class HttpRequestParser
{
public:
    const StartField &GetStartLine();
    const ConnectionField &GetConnectionField();
    void Parse(const std::string &request);

private:
    [[nodiscard]] std::vector<std::string> ParseLine(const std::string &line);

    StartField ParseStartField(const std::vector<std::string> &words);
    ConnectionField ParseConnectionField(const std::vector<std::string> &words);
    StartField mStartField;
    ConnectionField mConnectionField;
};

#include "request.hpp"

const StartField &HttpRequestParser::GetStartLine()
{
    return mStartField;
}

const ConnectionField &HttpRequestParser::GetConnectionField()
{
    return mConnectionField;
}

void HttpRequestParser::Parse(const std::string &request)
{
    std::vector<std::string> lines;
    std::string line;
    for (char ch : request)
    {
        if (ch == '\n' && !line.empty())
        {
            lines.push_back(line);
            line.clear();
            continue;
        }

        line += ch;
    }

    lines.push_back(line);

    mStartField = ParseStartField(ParseLine(lines[0]));

    for (const auto &linel : lines)
    {
        const std::vector<std::string> &words = ParseLine(linel);
        if (words[0] == "Connection:")
        {
            mConnectionField = ParseConnectionField(words);
        }
    }
}

[[nodiscard]] std::vector<std::string> HttpRequestParser::ParseLine(const std::string &line)
{
    std::vector<std::string> words;
    std::string word;
    for (char ch : line)
    {
        if ((ch == ' ' || ch == '\r') && !word.empty())
        {
            words.push_back(word);
            word.clear();
            continue;
        }
        word += ch;
    }

    if (!word.empty())
    {
        words.push_back(word);
    }

    return words;
}

StartField HttpRequestParser::ParseStartField(const std::vector<std::string> &words)
{
    std::string httpMethods[] = {
        "GET",
        "PATCH",
        "PUT",
        "POST",
        "HEAD",
        "DELETE",
        "OPTIONS",
    };

    for (const auto &method : httpMethods)
    {
        if (words[0] == method)
        {
            StartField field;
            field.method = words[0];
            field.protocol = words[2];
            field.uri = words[1];

            return field;
        }
    }

    LOG("error: invalid http method");
    return {};
}

ConnectionField HttpRequestParser::ParseConnectionField(const std::vector<std::string> &words)
{
    ConnectionField field;
    if (words[1] == "keep-alive")
    {
        field.type = ConnectionType::KeepAlive;
    }
    else if (words[1] == "close")
    {
        field.type = ConnectionType::Close;
    }

    return field;
}

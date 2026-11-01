#pragma once
#include <format>
#include <string>

class HttpResponseBuilder
{
public:
    void SetContentType(const std::string &contentType);
    const std::string &GetContentType() const;
    void SetContentLength(size_t length);
    size_t GetContentLength() const;
    void SetData(const std::string &data);
    const std::string &GetData() const;
    std::string GetString() const;

private:
    size_t mContentLength = 0;
    std::string mContentType;
    std::string mData;
};
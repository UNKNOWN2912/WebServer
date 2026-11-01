#include "response.hpp"

void HttpResponseBuilder::SetContentType(const std::string &contentType)
{
    mContentType = contentType;
}

const std::string &HttpResponseBuilder::GetContentType() const
{
    return mContentType;
}

void HttpResponseBuilder::SetContentLength(size_t length)
{
    mContentLength = length;
}

size_t HttpResponseBuilder::GetContentLength() const
{
    return mContentLength;
}

void HttpResponseBuilder::SetData(const std::string &data)
{
    mData = data;
}

const std::string &HttpResponseBuilder::GetData() const
{
    return mData;
}

std::string HttpResponseBuilder::GetString() const
{
    std::string str;

    str += "HTTP/1.1 200 OK\n";
    if (!mContentType.empty())
        str += std::format("Content-Type: {}\n", mContentType);
    str += std::format("Content-Length: {}\n", mContentLength);
    if (!mData.empty())
    {
        str += "\r\n";
        str += std::format("{}", mData);
    }

    return str;
}

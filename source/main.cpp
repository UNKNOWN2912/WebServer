#include "request.hpp"
#include "response.hpp"
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <print>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

const int port = 8080;

std::string readFile(const std::string &filename)
{
    std::string content;
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == nullptr)
    {
        LOG("error: file not found {}", filename);
        return "";
    }

    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    content.resize(size);

    fread(content.data(), size, 1, fp);

    fclose(fp);

    return content;
}

void processClientRequest(int clientSocket)
{
    std::string request;
    request.resize(1024);

    while (recv(clientSocket, request.data(), request.size(), 0) > 0)
    {
        std::println("request: [{}]", request);
        HttpRequestParser httpRequest;
        httpRequest.Parse(request);
        if (httpRequest.GetConnectionField().type == ConnectionType::Close)
        {
            exit(0);
        }

        if (httpRequest.GetStartLine().method == "GET")
        {
            std::string url = (httpRequest.GetStartLine().uri == "/") ? "index.html" : &httpRequest.GetStartLine().uri[1];
            std::string htmlFileContent = readFile(url);

            HttpResponseBuilder builder;
            builder.SetContentLength(htmlFileContent.size());
            builder.SetContentType("text/html");
            builder.SetData(htmlFileContent);
            std::string extension = url.substr(url.find_last_of('.'));
            if (extension == ".png")
            {
                builder.SetContentType("image/png");
            }
            else if (extension == ".ico")
            {
                builder.SetContentType("image/x-icon");
            }

            std::string response = builder.GetString();
            LOG("response: {}", response);
            send(clientSocket, response.data(), response.size(), 0);
        }
    }
}

void terminalReader()
{
    while (true)
    {
        std::string str;
        std::cin >> str;

        if (str == "quit")
        {
            exit(0);
        }
    }
}

int main()
{
    sockaddr_in serverAddr = {};
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
    {
        LOG("Failed to set socket options");
    }

    if (bind(sockfd, (sockaddr *)&serverAddr, sizeof(sockaddr_in)) != 0)
    {
        LOG("Failed to bind server address to socket");
        close(sockfd);
        return -1;
    }

    listen(sockfd, 5);

    LOG("web server started");

    std::thread t(terminalReader);
    t.detach();

    while (true)
    {
        sockaddr_in clientAddress;
        socklen_t clientAddrSize;
        int clientSocket = accept(sockfd, (sockaddr *)&clientAddress, &clientAddrSize);

        char ipAddress[512];
        inet_ntop(AF_INET, &clientAddress.sin_addr.s_addr, ipAddress, 512);
        LOG("Accepted connection");
        LOG("IP Address: {}", ipAddress);

        std::thread thread(processClientRequest, clientSocket);
        thread.detach();
    }

    close(sockfd);
    return 0;
}

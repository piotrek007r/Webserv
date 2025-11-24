// Copyright [2025] <Antoni Gorski> <agorski@student.42warsaw.pl>
// Copyright [2025] <Piotr Ruszkiewicz> <pruszkie@student.42warsaw.pl>
// Copyright [2025] <Grzegorz Buczynski> <gbuczyns@student.42warsaw.pl>

#include "coreEngine/CoreEngine.hpp"
#include "configReader/config.hpp"
#include "http/HttpRequestParser.hpp"
#include <iostream>
#include <stdio.h>
// #include <sstream>

int main(int argc, char **argv)
{
    std::string filePath;
    std::string path = "configReader/conf/";
    if (argc == 1)
        filePath = "configReader/conf/default.conf";
    else if (argc == 2)
    filePath = path + argv[1];
    else
    {
        std::cerr << "Invalid number of arguments" << std::endl;
        return 1;
    }
    try
    {
        if (access(filePath.c_str(), F_OK) == -1)
            return perror("Can't open config file"), 1;
        std::string rawRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Connection: keep-alive\r\n"
            "\r\n";

        Config config(filePath);
        const std::vector<ServerConfig> &servers = config.getServers();

        HttpRequestParser parser;
        try
        {   bool keepAlive;
            HttpRequestParser::HttpRequest request = parser.parse(rawRequest);
            std::cout << "Method: " << request.method << "\n";
            std::cout << "Path: " << request.path << "\n";
            std::cout << "HTTP Version: " << request.httpVersion << "\n";
            for (std::map<std::string, std::string>::const_iterator it = request.headers.begin(); it != request.headers.end(); ++it)
            {
                if (it->second == "keep-alive")
                    keepAlive = true;
                
                std::cout << it->first << ": " << it->second << "\n";
            }
            if(keepAlive)
                std::cout << "Keeping connection alive due to keep-alive header\n" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
        // Potrzebujemy tego printa poniżej?
        
        config.printConfigs(); // Test print function

        CoreEngine CoreEngine(servers);
        CoreEngine.coreEngine();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Config error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

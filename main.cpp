#include "HttpRequestParser.hpp"
#include <iostream>

int main() {
    std::string rawRequest = 
        "GET /index.html HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: keep-alive\r\n"
        "\r\n";

    HttpRequestParser parser;
    try {
        HttpRequestParser::HttpRequest request = parser.parse(rawRequest);
        std::cout << "Method: " << request.method << "\n";
        std::cout << "Path: " << request.path << "\n";
        std::cout << "HTTP Version: " << request.httpVersion << "\n";
        for (const auto& header : request.headers) {
            std::cout << header.first << ": " << header.second << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}


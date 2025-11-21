#include "http/HttpRequestParser.hpp"
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

        // Zamiast range-based for używamy iteratorów
        for (std::map<std::string, std::string>::const_iterator it = request.headers.begin(); it != request.headers.end(); ++it) {
            std::cout << it->first << ": " << it->second << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}


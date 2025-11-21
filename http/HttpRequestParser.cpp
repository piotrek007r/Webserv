#include "HttpRequestParser.hpp"

void HttpRequestParser::parseRequestLine(const std::string& line, HttpRequest& request) {
    std::istringstream stream(line);
    if (!(stream >> request.method >> request.path >> request.httpVersion)) {
        throw std::invalid_argument("Invalid HTTP request line");
    }
}

void HttpRequestParser::parseHeaderLine(const std::string& line, HttpRequest& request) {
    size_t delimiterPos = line.find(": ");
    if (delimiterPos == std::string::npos) {
        throw std::invalid_argument("Invalid HTTP header line");
    }

    std::string key = line.substr(0, delimiterPos);
    std::string value = line.substr(delimiterPos + 2);
    request.headers[key] = value;
}

HttpRequestParser::HttpRequest HttpRequestParser::parse(const std::string& rawRequest) {
    HttpRequest request;
    std::istringstream stream(rawRequest);
    std::string line;

    // Parse the request line
    if (!std::getline(stream, line) || line.empty()) {
        throw std::invalid_argument("Empty HTTP request");
    }
    parseRequestLine(line, request);

    // Parse headers
    while (std::getline(stream, line) && !line.empty()) {
        if (line == "\r") {
            break; // End of headers
        }
        parseHeaderLine(line, request);
    }

    // Parse body (if any)
    std::string body;
    while (std::getline(stream, line)) {
        body += line + "\n";
    }
    request.body = body;

    return request;
}
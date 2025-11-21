#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include <string>
#include <map>
#include <sstream>
#include <stdexcept>

class HttpRequestParser {
public:
    struct HttpRequest {
        std::string method;
        std::string path;
        std::string httpVersion;
        std::map<std::string, std::string> headers;
        std::string body;
    };

    // Parses a raw HTTP request string and returns an HttpRequest object
    HttpRequest parse(const std::string& rawRequest);

private:
    // Helper functions
    void parseRequestLine(const std::string& line, HttpRequest& request);
    void parseHeaderLine(const std::string& line, HttpRequest& request);
};

#endif
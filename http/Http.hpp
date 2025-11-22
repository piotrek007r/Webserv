#ifndef HTTP_HPP
#define HTTP_HPP

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <stdexcept>

class Http {
public:
    struct HttpRequest {
        std::string method;
        std::string path;
        std::string httpVersion;
        std::map<std::string, std::string> headers;
        std::string body;
    };

    // Parses a raw HTTP request string and returns an HttpRequest object
    static HttpRequest parse(const std::string& rawRequest);

    // Constructor for creating HTTP responses
    Http(std::string& rawRequest);

    // Generates the HTTP response string
    std::string response();

private:
    // Response data
    HttpRequest request;
    std::string httpVersion;
    std::string statusCode;
    std::vector<std::string> headers;
    std::string body;

    // Helper functions for parsing
    static void parseRequestLine(const std::string& line, HttpRequest& request);
    static void parseHeaderLine(const std::string& line, HttpRequest& request);
    void getResponse();
    void postResponse();
    void putResponse();
    void deleteResponse();
};

#endif
#pragma once
#include <string>
#include <vector>
#include "HttpRequestParser.hpp"

class HttpResponse {
    public:
        HttpResponse(HttpRequestParser::HttpRequest &request, int statusCode, const std::string& body);
        std::string response(); // Generates the full HTTP response string
    private:
        std::string httpVersion;// = "HTTP/1.1";
        std::string statusCode;
        std::string statusText;
        std::vector<std::string> headers;
        std::string body;
        std::string getStatusText(int code); // Maps status codes to status text
};
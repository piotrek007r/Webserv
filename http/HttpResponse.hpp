#pragma once
#include <string>
#include <vector>
#include "HttpRequestParser.hpp"

class HttpResponse {
    public:
        HttpResponse(HttpRequestParser::HttpRequest &request, int statusCode, const std::string& body);
        std::string response();
    private:
        std::string httpVersion;// = "HTTP/1.1";
        std::string statusCode;
        std::vector<std::string> headers;
        std::string body;
        
};
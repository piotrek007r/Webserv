#include "HttpResponse.hpp"
#include <sstream>


HttpResponse::HttpResponse(HttpRequestParser::HttpRequest &request, int statusCode, const std::string& body) {
    std::stringstream ss;
    ss << statusCode;
    this->statusCode = ss.str();
    this->statusText = getStatusText(statusCode);
    
    this->httpVersion = request.httpVersion;
    this->body = body;
}
std::string HttpResponse::response() {
    std::string headersCombined;
    std::stringstream ss;
    ss << body.length();
    this->headers.push_back("Content-Length: " + ss.str());
    this->headers.push_back("Connection: close");
    this->headers.push_back("Content-Type: text/html");
    for (std::vector<std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
        headersCombined += *it + "\r\n";
    return httpVersion + " " + statusCode + " " + statusText + "\r\n" + headersCombined + "\r\n\r\n" + body;
}

std::string HttpResponse::getStatusText(int code) {
    switch (code) {
        case 200: return "OK";
        case 201: return "Created";
        case 204: return "No Content";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 304: return "Not Modified";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        default: return "Unknown";
    }
}
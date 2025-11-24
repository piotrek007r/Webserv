#include "HttpResponse.hpp"
#include <sstream>


HttpResponse::HttpResponse(HttpRequestParser::HttpRequest &request, int statusCode, const std::string& body) {
    std::stringstream ss;
    ss << statusCode;
    this->statusCode = ss.str();
    
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
    return httpVersion + " " + statusCode + "\r\n" + headersCombined + "\r\n\r\n" + body;
}
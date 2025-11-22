#include "Http.hpp"

void Http::parseRequestLine(const std::string& line, HttpRequest& request) {
    std::istringstream stream(line);
    if (!(stream >> request.method >> request.path >> request.httpVersion)) {
        throw std::invalid_argument("Invalid HTTP request line");
    }
}

void Http::parseHeaderLine(const std::string& line, HttpRequest& request) {
    size_t delimiterPos = line.find(": ");
    if (delimiterPos == std::string::npos) {
        throw std::invalid_argument("Invalid HTTP header line");
    }

    std::string key = line.substr(0, delimiterPos);
    std::string value = line.substr(delimiterPos + 2);
    request.headers[key] = value;
}

Http::HttpRequest Http::parse(const std::string& rawRequest) {
    HttpRequest request;
    if (rawRequest.empty()) {
        throw std::invalid_argument("Empty HTTP request");
    }
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

Http::Http(std::string& rawRequest) {
    this->request = parse(rawRequest);
}

void Http::getResponse() {
    this->httpVersion = "HTTP/1.1";
    this->statusCode = "200 OK";
    this->body = "<html><body><h1>GET request received</h1></body></html>";
}

void Http::postResponse() {
    this->httpVersion = "HTTP/1.1";
    this->statusCode = "200 OK";
    this->body = "<html><body><h1>POST request received</h1></body></html>";
}
void Http::putResponse() {
    this->httpVersion = "HTTP/1.1";
    this->statusCode = "200 OK";
    this->body = "<html><body><h1>PUT request received</h1></body></html>";
}


std::string Http::response() {
    if (this->request.method == "GET") {
        this->httpVersion = "HTTP/1.1";
        this->statusCode = "200 OK";
        this->body = "<html><body><h1>GET Request</h1></body></html>";
    } else if (this->request.method == "POST") {
        this->httpVersion = "HTTP/1.1";
        this->statusCode = "200 OK";
        this->body = "<html><body><h1>POST Request</h1></body></html>";
    } else if (this->request.method == "PUT") {
        this->httpVersion = "HTTP/1.1";
        this->statusCode = "200 OK";
        this->body = "<html><body><h1>PUT Request</h1></body></html>";
    } else if (this->request.method == "DELETE") {
        this->httpVersion = "HTTP/1.1";
        this->statusCode = "200 OK";
        this->body = "<html><body><h1>DELETE Request</h1></body></html>";
    } else {
        this->httpVersion = "HTTP/1.1";
        this->statusCode = "405 Method Not Allowed";
        this->body = "<html><body><h1>405 Method Not Allowed</h1></body></html>";
    }

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
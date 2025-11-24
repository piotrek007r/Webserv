void CoreEngine::recivNClose(size_t el)
{
    // ... parsuj HTTP request ...
    
    if (isCgiRequest)
    {
        std::map<std::string, std::string> envVars;
        
        // ═══════════════════════════════════════════════════════════
        // PRZYGOTUJ ZMIENNE ŚRODOWISKOWE
        // ═══════════════════════════════════════════════════════════
        
        // 1. REQUEST_METHOD
        envVars["REQUEST_METHOD"] = request.method; // "GET", "POST", etc.
        
        // 2. QUERY_STRING
        size_t qPos = request.path.find('?');
        if (qPos != std::string::npos)
            envVars["QUERY_STRING"] = request.path.substr(qPos + 1);
        else
            envVars["QUERY_STRING"] = "";
        
        // 3. CONTENT_LENGTH
        std::stringstream ss;
        ss << body.size();
        envVars["CONTENT_LENGTH"] = ss.str();
        
        // 4. CONTENT_TYPE
        if (request.headers.find("Content-Type") != request.headers.end())
            envVars["CONTENT_TYPE"] = request.headers["Content-Type"];
        
        // 5. SERVER_PROTOCOL
        envVars["SERVER_PROTOCOL"] = request.httpVersion; // "HTTP/1.1"
        
        // 6. PATH_INFO / SCRIPT_NAME
        envVars["PATH_INFO"] = request.path;
        envVars["SCRIPT_NAME"] = request.path;
        
        // 7. SERVER_NAME (z Host header)
        if (request.headers.find("Host") != request.headers.end())
        {
            std::string host = request.headers["Host"];
            size_t colonPos = host.find(':');
            
            if (colonPos != std::string::npos)
            {
                envVars["SERVER_NAME"] = host.substr(0, colonPos);
                envVars["SERVER_PORT"] = host.substr(colonPos + 1);
            }
            else
            {
                envVars["SERVER_NAME"] = host;
                envVars["SERVER_PORT"] = "80";
            }
        }
        
        // 8. REMOTE_ADDR (IP klienta)
        char clientIP[INET_ADDRSTRLEN];
        struct sockaddr_in* addr = (struct sockaddr_in*)&clientSockaddr;
        inet_ntop(AF_INET, &addr->sin_addr, clientIP, INET_ADDRSTRLEN);
        envVars["REMOTE_ADDR"] = clientIP;
        
        // 9. HTTP Headers
        for (std::map<std::string, std::string>::iterator it = request.headers.begin();
             it != request.headers.end(); ++it)
        {
            envVars[it->first] = it->second;
        }
        
        // ═══════════════════════════════════════════════════════════
        // WYWOŁAJ CGI
        // ═══════════════════════════════════════════════════════════
        
        std::string response = executeCgi(fullPath, 
                                         location->cgi_path, 
                                         body, 
                                         envVars);
        
        send(pollFDs[el].fd, response.c_str(), response.size(), 0);
    }
}
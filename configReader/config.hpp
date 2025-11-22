//Copyright [2025] <Antoni Gorski> <agorski@student.42warsaw.pl>

#pragma once

#include <string>
#include <vector>
#include <map>

struct LocationConfig {
    std::vector<std::string> allow_methods;
    bool autoindex;
    std::string index;
    std::string redirect;
    std::string upload_dir;
    std::string cgi_path;
    std::string cgi_ext;
    std::string root;
};

struct ServerConfig {
    std::string listen_port;
    std::string server_name;
    std::string host;
    std::map<int, std::string> error_pages;
    std::map<std::string, LocationConfig> locations;
};

class Config {
private:
    std::vector<ServerConfig> servers;

    void parseLines(std::ifstream& file);

public:
    Config(const std::string& filename);
    const std::vector<ServerConfig>& getServers() const;
    void printConfigs() const;
};

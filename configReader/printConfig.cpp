/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agorski <agorski@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 21:38:43 by agorski           #+#    #+#             */
/*   Updated: 2025/11/24 10:16:08 by agorski          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include <iostream>

void Config::printConfigs() const
{
    const std::vector<ServerConfig>& servers = getServers();
std::cout << "Number of Configured Servers: " << servers.size() << std::endl;
        for (size_t i = 0; i < servers.size(); ++i)
        {
            const ServerConfig &srv = servers[i];
            std::cout << "\nServer #" << i + 1 << std::endl;
            std::cout << "  listen_port: " << srv.listen_port << std::endl;
            std::cout << "  server_name: " << srv.server_name << std::endl;
            std::cout << "  host: " << srv.host << std::endl;
            std::cout << "  error_pages: ";
            for (std::map<int, std::string>::const_iterator it = srv.error_pages.begin(); it != srv.error_pages.end(); ++it)
                std::cout << it->first << "->" << it->second << " ";
            std::cout << std::endl;

            std::cout << "  Locations/routes: " << srv.locations.size() << std::endl;
            for (std::map<std::string, LocationConfig>::const_iterator loc = srv.locations.begin(); loc != srv.locations.end(); ++loc)
            {
                std::cout << "    route: " << loc->first << std::endl;
                const LocationConfig &lc = loc->second;
                std::cout << "      allow_methods: ";
                for (size_t j = 0; j < lc.allow_methods.size(); ++j)
                    std::cout << lc.allow_methods[j] << " ";
                std::cout << std::endl;
                std::cout << "      autoindex: " << (lc.autoindex ? "on" : "off") << std::endl;
                std::cout << "      index: " << lc.index << std::endl;
                std::cout << "      redirect: "<< lc.redirect << std::endl;
                std::cout << "      upload_dir: " << lc.upload_dir << std::endl;
                std::cout << "      root: " << lc.root << std::endl;
                std::cout << "      cgi_path: " << lc.cgi_path << std::endl;
                std::cout << "      cgi_ext: " << lc.cgi_ext << std::endl;
            }
        }
}
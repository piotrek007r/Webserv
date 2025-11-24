/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agorski <agorski@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 13:33:56 by agorski           #+#    #+#             */
/*   Updated: 2025/11/24 17:42:40 by agorski          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <map>
#include <stdexcept>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <iostream>
#include <cstdlib>

std::string executeCgi(const std::string& scriptPath,
                              const std::string& interpreterPath,
                              const std::string& body, 
                              std::map <std::string, std::string> envVars);
#endif